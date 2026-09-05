#if !defined( B33_VOXEL_GRID_HPP )
#    define B33_VOXEL_GRID_HPP

#    include "Primitives/ColoredCubes.hpp"
#    include "Raycaster/Voxel.hpp"
#    include "Vulkan/Memory/MemoryUploadTracker.hpp"

namespace B33::Rendering
{

enum EGridChanged
{
    NoChanges = 1,
    Position  = NoChanges << 1,
    Rotation  = Position << 1,
    HalfSize  = Rotation << 1,
};

class IWorldGrid : public MemoryUploadTracker
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;
    template <typename T>
    using Vector = ::std::vector<T>;

  protected:
    static constexpr usize DefaultVoxelGridDim = 64;

  public:
    explicit IWorldGrid( usize uGridWidth = DefaultVoxelGridDim )
      : m_uGridDim( uGridWidth )
      , m_VoxelGrid( uGridWidth * uGridWidth * uGridWidth )
      , m_uChanged( NoChanges )
    {
    }

  public:
    const void *GetGridPtr() const
    {
        return m_VoxelGrid.data();
    }

    Vector<Voxel> &GetGrid()
    {
        return m_VoxelGrid;
    }

    const Vector<Voxel> &GetGrid() const
    {
        return m_VoxelGrid;
    }

    usize GetVoxelsSizeInBytes() const
    {
        return m_VoxelGrid.size() * sizeof( Voxel );
    }

    usize GetGridWidth() const
    {
        return m_uGridDim;
    }

    usize GetVoxels() const
    {
        return m_VoxelGrid.size();
    }

    u32 GetChanged()
    {
        u32 r = m_uChanged;
        m_uChanged &= 0;
        return r;
    }

    virtual const ::B33::Math::WorldObjects &GetStoredObjects() const = 0;

  public:
    __B33_API void SetVoxel( const iVec &pos, u32 uColor );

  public:
    virtual bool CheckIfVoxelOccupied( const iVec &pos ) const = 0;

  protected:
    __B33_API usize CalcIndex( const iVec &pos ) const;

    __B33_API void PlaceOnGrid( const iVec &pos, const iVec &area, const usize uId );

    __B33_API void RemoveFromGrid( const iVec &pos, const iVec &area, const usize uId );

    void SetPositionChanged()
    {
        m_uChanged |= EGridChanged::Position;
    }

    void SetRotationChanged()
    {
        m_uChanged |= EGridChanged::Rotation;
    }

    void SetHalfSizeChanged()
    {
        m_uChanged |= EGridChanged::HalfSize;
    }

  private:
    usize         m_uGridDim = -1;
    Vector<Voxel> m_VoxelGrid;
    u32           m_uChanged;
};

template <class StoredObjectType>
class WorldGrid : public IWorldGrid
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;
    using Rot  = ::B33::Math::Rot3;
    template <typename T>
    using Vector = ::std::vector<T>;

    template <typename T>
    constexpr decltype( auto ) Forward( T &arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

    template <typename T>
    constexpr decltype( auto ) Forward( T &&arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

  public:
    explicit WorldGrid( usize uGridWidth = IWorldGrid::DefaultVoxelGridDim )
      : IWorldGrid( uGridWidth )
      , m_StoredObjects() // TODO: this->GetVoxelsSizeInBytes() / sizeof(Voxel))
      , m_uObjectsCount( 0 )
    {
    }

  public:
    virtual const StoredObjectType &GetStoredObjects() const override
    {
        return m_StoredObjects;
    }

  public:
    virtual bool CheckIfVoxelOccupied( const iVec &pos ) const override
    {
        const Vector<Voxel> &voxelsGrid = this->GetGrid();
        const usize          uIndex     = CalcIndex( pos );

        if ( voxelsGrid[ uIndex ].Type == 0 )
            return false;

        for ( u32 i = 0; i < voxelsGrid[ uIndex ].Type; ++i )
            if ( iVec::ToVec( m_StoredObjects.GetPosition( voxelsGrid[ uIndex ].Id[ i ] ) ) == pos )
                return true;

        return false;
    }

  public:
    template <class U>
    usize GenerateObjectAtVoxel( const iVec &pos, U &&sot )
    {
        usize uId = GenerateObject( pos, Forward<U>( sot ) );
        this->ForceUpload();
        this->SetPositionChanged();
        this->SetRotationChanged();
        this->SetHalfSizeChanged();
        return uId;
    }

    void RemoveObject( const usize uObjectId )
    {
        const iVec area = iVec::ToVec( m_StoredObjects.GetHalfSize( uObjectId ) + 1 );

        this->RemoveFromGrid( iVec::ToVec( m_StoredObjects.GetPosition( uObjectId ) ), area, uObjectId );

        this->ForceUpload();
        this->SetPositionChanged();
        this->SetRotationChanged();
        this->SetHalfSizeChanged();
        m_StoredObjects.RemoveObject( uObjectId );
    }

    void UpdatePos( const Vec &newPos, usize uObjectId )
    {
        if ( m_StoredObjects.GetPosition( uObjectId ) == newPos )
            return;

        const iVec area = iVec::ToVec( m_StoredObjects.GetHalfSize( uObjectId ) + 1 );
        this->RemoveFromGrid( iVec::ToVec( m_StoredObjects.GetPosition( uObjectId ) ), area, uObjectId );

        m_StoredObjects.SetPositon( newPos, uObjectId );
        this->PlaceOnGrid( iVec::ToVec( newPos ), area, uObjectId );

        this->SetPositionChanged();
    }

    void UpdateRot( const Rot &newRot, usize uId )
    {
        if ( m_StoredObjects.GetRotation( uId ) == newRot )
        {
            return;
        }

        m_StoredObjects.SetRotation( newRot, uId );

        this->SetRotationChanged();
    }

  private:
    template <class U>
    usize GenerateObject( iVec pos, U &&sot )
    {
        const usize uObjId = m_StoredObjects.AddObject();

        m_StoredObjects.SetPositon( Vec::ToVec( pos ), uObjId );
        m_StoredObjects.SetRotation( sot.GetRotation(), uObjId );
        m_StoredObjects.SetHalfSize( sot.GetHalfSize(), uObjId );

        this->PlaceOnGrid( iVec::ToVec( m_StoredObjects.GetPosition( uObjId ) ),
                           iVec::ToVec( m_StoredObjects.GetHalfSize( uObjId ) + 1 ),
                           uObjId );

        return uObjId;
    }

  private:
    StoredObjectType m_StoredObjects;
    usize            m_uObjectsCount = -1;
};

typedef WorldGrid<ColoredCubes> CubeWorld;

} // namespace B33::Rendering
#endif // !B33_VOXEL_GRID_HPP

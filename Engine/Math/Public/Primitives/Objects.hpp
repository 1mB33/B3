#if !defined( B33_OBJECTS_HPP )
#    define B33_OBJECTS_HPP

#    include <B33Core.h>

namespace B33::Math
{

/**
 * Holds positon of an object and rotation in radians.
 * */
class WorldObjects
{
    template <typename T>
    using Vector = ::std::vector<T>;

  public:
    explicit WorldObjects()
      : m_uRollingIndex( 0 )
      , m_vPositions( {} )
      , m_vRotations( {} )
    {
        m_vPositions.reserve( 64 * 64 * 64 );
        m_vRotations.reserve( 64 * 64 * 64 );
    }

    ~WorldObjects() noexcept = default;

  public:
    WorldObjects( WorldObjects && ) noexcept      = default;
    WorldObjects( const WorldObjects & ) noexcept = default;

    WorldObjects &operator=( const WorldObjects & ) noexcept = default;
    WorldObjects &operator=( WorldObjects && ) noexcept      = default;

  public:
    __B33_API void SetPositon( Vec3 pos, usize uIndex );

    __B33_API void SetRotation( Rot3 rot, usize uIndex );

    __B33_API void AddPositon( const Vec3 &pos, usize uIndex );

    __B33_API void AddRotation( const Rot3 &rot, usize uIndex );

  public:
    const Vector<Vec3> &GetPositions() const
    {
        return m_vPositions;
    }

    const Vector<Vec3> &GetRotations() const
    {
        return m_vRotations;
    }

    const Vec3 &GetPosition( usize uIndex ) const
    {
        return m_vPositions[ uIndex ];
    }

    const Rot3 &GetRotation( usize uIndex ) const
    {
        return m_vRotations[ uIndex ];
    }

  public:
    virtual usize AddObject()
    {
        usize i = m_uRollingIndex++;

        m_vPositions.push_back( Vec3() );
        m_vRotations.push_back( Vec3() );

        B33_ASSERT( i == m_vPositions.size() - 1 );
        B33_ASSERT( i == m_vRotations.size() - 1 );

        return i;
    }

    virtual void RemoveObject( usize ) {}

  private:
    usize        m_uRollingIndex;
    Vector<Vec3> m_vPositions;
    Vector<Rot3> m_vRotations;
};

} // namespace B33::Math
#endif // !B33_OBJECTS_HPP

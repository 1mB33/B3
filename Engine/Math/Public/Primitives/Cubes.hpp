#if !defined( B33_CUBES_HPP )
#    define B33_CUBES_HPP

#    include <B33Core.h>

#    include "Primitives/Objects.hpp"

namespace B33::Math
{

class Cubes : public WorldObjects
{
    template <typename T>
    using Vector = ::std::vector<T>;

  public:
    Cubes()
      : WorldObjects()
      , m_vHalfSizes( {} )
    {
        m_vHalfSizes.reserve( 64 * 64 * 64 );
    }

  public:
    const Vector<Vec3> &GetHalfSizes() const
    {
        return m_vHalfSizes;
    }

    Vec3 GetHalfSize( usize uIndex ) const
    {
        return m_vHalfSizes[ uIndex ];
    }

  public:
    void SetHalfSize( Vec3 halfSize, usize uIndex )
    {
        m_vHalfSizes[ uIndex ] = halfSize;
    }

  public:
    virtual usize AddObject() override
    {
        usize i = WorldObjects::AddObject();
        m_vHalfSizes.push_back( Vec3() );

        B33_ASSERT( i == m_vHalfSizes.size() - 1 );

        return i;
    }

    virtual void RemoveObject( usize uIndex ) override
    {
        WorldObjects::RemoveObject( uIndex );
    }

  private:
    Vector<Vec3> m_vHalfSizes;
};

} // namespace B33::Math
#endif // !B33_CUBES_HPP

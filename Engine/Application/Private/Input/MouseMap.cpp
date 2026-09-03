#include "B33App.h"

#include "MouseMap.hpp"

namespace B33::App
{

// Impl // ------------------------------------------------------------------------------------------------------------
void MouseMap::BindActionImpl( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const B33InputBind &ib,
                               void                                               *pThis,
                               __B33_ATTRIBUTE_MIGHT_BE_UNUSED B33Action           a,
                               B33MouseAction                                      ma )
{
    B33_ASSERT( ib.Type == EB33BindType::Mouse );
    B33_ASSERT( a == nullptr );
    B33_ASSERT( ma != nullptr );
    B33_ASSERT( pThis != nullptr );

    m_vMouseBinds.push_back( ActionReplayData { pThis, ma } );
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::UnbindActionImpl( __B33_ATTRIBUTE_MIGHT_BE_UNUSED const B33InputBind &ib, void *pThis )
{
    B33_ASSERT( ib.Type == EB33BindType::Mouse );
    B33_ASSERT( pThis != nullptr );

    auto it = m_vMouseBinds.begin();
    for ( ; it != m_vMouseBinds.end(); ++it )
    {
        if ( it->pThis == pThis )
        {
            m_vMouseBinds.erase( it );
            return;
        }
    }

    B33_ASSERT( it != m_vMouseBinds.end() );
}

// Public // ----------------------------------------------------------------------------------------------------------
void MouseMap::PlayAction( const float fDelta, int32_t fX, int32_t fY ) noexcept
{
    for ( const auto &bind : m_vMouseBinds )
    {
        B33_ASSERT( bind.pThis );

        bind.Action( fDelta, bind.pThis, fX, fY );
    }
}

} // namespace B33::App

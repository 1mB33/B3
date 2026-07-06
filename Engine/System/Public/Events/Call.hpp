#if !defined( B33_CALL_HPP )
#    define B33_CALL_HPP

#    include "ICall.hpp"

namespace B33::System
{

template <class OBJECT_CLASS>
class Call : public ICall
{
    using ObjectsMethod = void ( OBJECT_CLASS ::* )();

  public:
    Call( OBJECT_CLASS *pObj, ObjectsMethod pCall )
      : m_pObj( *pObj )
      , m_pCall( pCall )
    {
    }

    static Call *CreateCall( OBJECT_CLASS *pObj, ObjectsMethod pCall )
    {
        return new Call( pObj, pCall );
    }

  public:
    virtual inline void OnCall() override final
    {
        ( m_pObj.*m_pCall )();
    }

  private:
    OBJECT_CLASS &m_pObj  = nullptr;
    ObjectsMethod m_pCall = nullptr;
};

} // namespace B33::System
#endif

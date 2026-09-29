#if !defined( B33_ACTION_HPP )
#    define B33_ACTION_HPP

#    include "Bind.h"

namespace B33::App
{

struct Action
{
    EB33BindType   Type;
    B33Action      ButtonAction;
    B33MouseAction MouseAction;
};

class ActionFactory
{
  public:
    template <class CLASS, auto ACTION, auto... ARGS>
    static Action CreateKeyboardAction()
    {
        return {
            .Type = EB33BindType::Keyboard,
            .ButtonAction =
                +[]( const float fDelta, void *pThis )
                {
                    ( static_cast<CLASS *>( pThis )->GetObject().Get().*ACTION )( fDelta, ARGS... );
                    return B33ActionType();
                },
            .MouseAction = nullptr,
        };
    }

    template <class CLASS, auto ACTION, auto... ARGS>
    static Action CreateMouseAction()
    {
        return {
            .Type         = EB33BindType::Mouse,
            .ButtonAction = nullptr,
            .MouseAction =
                +[]( const float fDelta, void *pThis, int32_t fX, int32_t fY )
                {
                    ( static_cast<CLASS *>( pThis )->GetObject().Get().*ACTION )( fDelta, fX, fY, ARGS... );
                    return B33ActionType();
                },
        };
    }
};

} // namespace B33::App

#endif // !B33_ACTION_HPP

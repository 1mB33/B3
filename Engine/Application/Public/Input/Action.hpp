#if !defined(B33_ACTION_HPP)
#define B33_ACTION_HPP

#include "B33Core.h"

#include "Bind.h"
#include "Input/UserInput.hpp"

namespace B33::App
{

struct Action
{
    EAbBindType   Type;
    AbAction      ButtonAction;
    AbMouseAction MouseAction;
};

class ActionFactory
{
  public:
    template <class CLASS, auto ACTION, auto... ARGS>
    static Action CreateKeyboardAction()
    {
        return {
            .Type = EAbBindType::Keyboard,
            .ButtonAction =
                +[]( const float fDelta, void *pThis )
                {
                    ( static_cast<CLASS *>( pThis )->GetObject().Get().*ACTION )( fDelta, ARGS... );
                    return ::AbActionType();
                },
            .MouseAction = nullptr,
        };
    }

    template <class CLASS, auto ACTION, auto... ARGS>
    static Action CreateMouseAction()
    {
        return {
            .Type         = EAbBindType::Mouse,
            .ButtonAction = nullptr,
            .MouseAction =
                +[]( const float fDelta, void *pThis, int32_t fX, int32_t fY )
                {
                    ( static_cast<CLASS *>( pThis )->GetObject().Get().*ACTION )( fDelta, fX, fY, ARGS... );
                    return ::AbActionType();
                },
        };
    }
};

} // namespace B33::App

#endif // !B33_ACTION_HPP

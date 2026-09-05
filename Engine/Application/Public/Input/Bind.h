#if !defined( B33_KEY_BIND_H )
#    define B33_KEY_BIND_H

#    include <B33Core.h>

/**
 * Empty struct to force binding type safety. Like a rock for your shoe.
 * */
typedef struct B33ActionType
{
    char Reserved;
} B33ActionType;

typedef u8 B33KeyId;

typedef enum EState
{
    IsPressed  = 1,
    IsReleased = IsPressed << 1,
} EState;

typedef enum EB33OnState
{
    Release    = 1,
    Press      = Release << 1,
    Continuous = Press << 1,
} EB33OnState;

typedef enum EB33BindType
{
    Keyboard    = 1,
    Mouse       = Keyboard << 1,
    MouseButton = Mouse << 1,
} EB33BindType;

typedef struct B33KeyboardBind
{
    EB33OnState KeyState;
    B33KeyId    KeyCode;
} B33KeyboardBind;

typedef struct B33MouseButtonBind
{
    EB33OnState ButtonState;
    B33KeyId    ButtonCode;
} B33MouseButtonBind;

typedef struct B33InputBind
{
    EB33BindType Type;

    union
    {
        B33KeyboardBind    Keyboard;
        B33MouseButtonBind MouseButton;
    };
} B33InputBind;

typedef B33ActionType ( *B33Action )( const float fDelta, void *pThis );
typedef B33ActionType ( *B33MouseAction )( const float fDelta, void *pThis, i32 X, int32_t y );

#endif // !B33_KEY_BIND_H

#if !defined( B33_INPUT_EVENTS_H )
#    define B33_INPUT_EVENTS_H

#    include "Bind.h"

typedef enum EB33InputEvents
{
    B33KeyPress      = 1,
    B33KeyRelease    = B33KeyPress << 1,
    B33ButtonPress   = B33KeyRelease << 1,
    B33ButtonRelease = B33ButtonPress << 1,
    B33Motion        = B33ButtonRelease << 1,
} EB33InputEvents;

typedef struct B33InputStruct
{
    EB33InputEvents Event;

    union
    {
        struct
        {
            B33KeyId KeyId;
        } Keyboard;

        struct
        {
            i32 MouseX;
            i32 MouseY;
        } Mouse;

        struct
        {
            B33KeyId KeyId;
        } MouseButton;
    };
} B33InputStruct;

#endif // !B33_INPUT_EVENTS_H

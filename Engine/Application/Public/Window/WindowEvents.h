#if !defined( B33_WINDOW_EVENTS_H )
#    define B33_WINDOW_EVENTS_H

#    include "B33Core.h"

typedef u32 EB33WindowEventsFlags;

typedef enum EB33WindowEvents : EB33WindowEventsFlags
{
    NothingNew      = 1,
    Destroy         = NothingNew << 1,
    Resize          = Destroy << 1,
    Input           = Resize << 1,
    ChangedBehavior = Input << 1,
} EB33WindowEvents;

#endif // !B33_WINDOW_EVENTS_H

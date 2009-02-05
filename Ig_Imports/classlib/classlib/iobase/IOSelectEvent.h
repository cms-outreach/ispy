#ifndef CLASSLIB_IOSELECT_EVENT_H
# define CLASSLIB_IOSELECT_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Event to be sent to #IOSelector clients when a channel becomes
    ready for I/O activity.  */
struct IOSelectEvent
{
    IOChannel	*source; //< The channel that is ready for I/O.
    unsigned	events;  //< The kind of I/O #source is ready for.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IOSELECT_EVENT_H

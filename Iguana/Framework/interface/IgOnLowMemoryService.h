#ifndef IGUANA_FRAMEWORK_IG_ON_LOW_MEMORY_SERVICE_H
# define IGUANA_FRAMEWORK_IG_ON_LOW_MEMORY_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgOnLowMemoryService
{
public:
    // FIXME: these should be services app drivers can hook on to.
    static bool			init (const char *appname);
    static void			fatal (void);
    static void			quit (void);
    static void			danger (void);

private:
    /// Indicator for low memory hooks that we have allocated all pools
    static bool			s_allocated;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_ON_LOW_MEMORY_SERVICE_H

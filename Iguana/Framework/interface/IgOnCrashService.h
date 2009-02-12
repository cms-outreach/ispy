#ifndef IGUANA_FRAMEWORK_IG_ON_CRASH_SERVICE_H
# define IGUANA_FRAMEWORK_IG_ON_CRASH_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "classlib/utils/Signal.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgOnCrashService
{
public:
    // FIXME: must be able to unwind for GUI -- can't be simple hook!
    //        Use signal facilities directly?
    typedef bool (*FatalSignalHook) (int sig, siginfo_t *info, void *x);
    typedef void (*FatalExceptionHook) (const char *type, const char *what);

    static bool			init (const char *appname);
    static FatalSignalHook	fatalSignalHook (FatalSignalHook hook);
    static FatalExceptionHook	fatalExceptionHook (FatalExceptionHook hook);

    static const char *		fatalTitle (void);
    static const char *		fatalMessage (void);
    static void			printFatalMessage (const char *topic,
						   const char *title,
						   const char *reason);
    static void			printFatalException (const char *type,
						     const char *what);
    static void			fatalException (const char *type,
						const char *what);

private:
    IgOnCrashService (/* FIXME */);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static bool			fatalSignal (int sig, siginfo_t *info, void *x);
    static bool			defaultFatalSignal (int sig, siginfo_t *info, void *x);
    static void			defaultFatalException (const char *type,
						       const char *what);

    static void			explainException (const char *type,
						  const char *what,
						  std::string &topic,
						  std::string &title,
						  std::string &reason);

    static FatalSignalHook	s_signalHook;
    static FatalExceptionHook	s_exceptionHook;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_ON_CRASH_SERVICE_H



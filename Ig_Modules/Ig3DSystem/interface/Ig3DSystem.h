#ifndef IG_3D_SYSTEM_IG_3D_SYSTEM_H
# define IG_3D_SYSTEM_IG_3D_SYSTEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DSystem/interface/config.h"
# include <classlib/utils/Callback.h>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_SYSTEM_API Ig3DSystem
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static void			initAction (lat::Callback action);

    static void			init (IgState *state, QWidget *mainWindow);
    static QWidget *		init (IgState *state, int argc, char **argv,
			    	      const char *appname, const char *classname);

private:
    static void			doinit (IgState *state);

    static bool				s_initialised;
    static std::list<lat::Callback>	s_actions;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_SYSTEM_IG_3D_SYSTEM_H

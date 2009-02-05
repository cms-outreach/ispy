#ifndef IG_PYTHON_IG_PYTHON_STATE_H
# define IG_PYTHON_IG_PYTHON_STATE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

class IG_PYTHON_API IgPythonState
{
public:    
    IgPythonState (void);
    
    static void set (IgState *state);
    static IgState *get (void);
private:
    static IgState *s_pythonState;
};

//<<<<<< CLASS DECLARATIONS                                             >>>>>>



//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_PYTHON_STATE_H

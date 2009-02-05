#ifndef IG_PYTHON_IG_PYTHON_SERVICE_H
# define IG_PYTHON_IG_PYTHON_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgPythonInterpreter;
class IgPythonStream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_PYTHON_API IgPythonService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgPythonService);    
public:
    IgPythonService (IgState *state);
    ~IgPythonService (void);
    IgPythonInterpreter *interpreter (void);
    IgPythonStream *pythonStream (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    IgState *m_state;
    IgPythonStream *m_pythonStream;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_PYTHON_SERVICE_H

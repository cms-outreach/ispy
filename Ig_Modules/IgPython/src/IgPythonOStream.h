#ifndef IG_PYTHON_IG_PYTHON_O_STREAM_H
# define IG_PYTHON_IG_PYTHON_O_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"
# include <iostream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
class IG_PYTHON_API IgPythonOStreamOutput
{
private:
    std::ostream *m_ostream;
public:
    IgPythonOStreamOutput (void);
    IgPythonOStreamOutput (std::ostream &stream);    
    void write(const char *);
    std::ostream *&stream(void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

class IG_PYTHON_API IgPythonOStreamError
{
private:
    std::ostream *m_ostream;
public:
    IgPythonOStreamError (void);
    IgPythonOStreamError (std::ostream &stream);    
    void write(const char *);
    std::ostream *&stream(void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PYTHON_IG_PYTHON_O_STREAM_H

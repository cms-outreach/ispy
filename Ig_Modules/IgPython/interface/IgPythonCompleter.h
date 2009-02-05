#ifndef IG_PYTHON_IG_PYTHON_COMPLETER_H
# define IG_PYTHON_IG_PYTHON_COMPLETER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**This class is used as an interface to complete a python name.
 */
class IG_PYTHON_API IgPythonCompleter
{
public:
    /**Constructor
     */
    IgPythonCompleter (void);
    /**Check if name is a valid python word.
       \param name name to be checked against the dictionary.
    */
    virtual bool isValid(const char *name);
    /**Returns the first possible completion found for \c objectName.
       \param objectName the name to be completed
     */
    virtual std::string completeName(const char *objName);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PYTHON_IG_PYTHON_COMPLETER_H

#ifndef IG_QT_PYTHON_IG_QT_DECORATOR_H
# define IG_QT_PYTHON_IG_QT_DECORATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**\brief This is an interface class to decorate strings. 
   Usually this class can be extended in python so that the decoration is done within
   python, easing the string manipulation.
   The following example shows how the decorator was extended to provide output decoration 
   for the shell.
   \include "Ig_Modules/IgQtPython/python/ShellCellDecorator.py"
 */
class IG_QT_PYTHON_API IgQtDecorator
{
public:
    IgQtDecorator (void);
    /**Decorates the passed string contents with style subStyle. Default implementation just returns
       contents unaltered.
       \param contents the string to be decorated.
       \param subStyle the style of the decoration (ignored in default implementation).
       \return the decorated string (unaltered in default implementation).
     */
    virtual std::string decorate(const char *contents, int subStyle);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_QT_DECORATOR_H

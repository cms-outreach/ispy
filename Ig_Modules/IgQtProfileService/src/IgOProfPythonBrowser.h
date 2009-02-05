#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_PYTHON_BROWSER_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_PYTHON_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"

# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgOProfRepresentableInterface;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfPythonBrowser
{
private:
    IgOProfRepresentableInterface *m_representable;    
public:
    IgOProfPythonBrowser (IgOProfRepresentableInterface *representable = 0);
    std::string getResults(void);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_PYTHON_BROWSER_H

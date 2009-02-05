#ifndef IG_QT_PYTHON_IG_PYTHON_VIEW_H
# define IG_QT_PYTHON_IG_PYTHON_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgView.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgQtPythonShell;
class IgQtSite;
class IgQtPythonLoadMenu;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PYTHON_API IgPythonView : public IgView
{
public:
    IgPythonView (IgPage *page);

    static const int MENU_ID_SCRIPTS 		= 8000;
    static const int MENU_ID_SCRIPTS_LOAD 	= 8001;
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    // Only one instance...
    static IgQtSite		*m_site;
    static IgQtPythonShell 	*m_shell;    
    IgQtPythonLoadMenu          *m_loadMenu;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_PYTHON_VIEW_H

#ifndef IG_QT_PYTHON_LOAD_MENU_H
# define IG_QT_PYTHON_LOAD_MENU_H 

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include "Ig_Modules/IgQtPython/interface/IgQtPythonShell.h" 
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PYTHON_API IgQtPythonLoadMenu : public QObject
{
    Q_OBJECT
public:
    IgQtPythonLoadMenu (IgState *state);
public slots:
void slotPythonLoader (void);

private:
    IgState *m_state;
};

#endif // IG_QT_PYTHON_LOAD_MENU_H 

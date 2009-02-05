# ifndef IG_QT_PYTHON_IG_QT_PYTHON_SHELL_H
# define IG_QT_PYTHON_IG_QT_PYTHON_SHELL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include <qvbox.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPythonStream;
class IgQtPythonShellEditor;
class QWidget;
class QScrollView;
class QVBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PYTHON_API IgQtPythonShell : public QVBox
{
private:
    QWidget *m_parent;
    IgPythonStream *m_pythonStream;
    QScrollView *m_scrollView;
    QVBox *m_editorsLayout;
    IgQtPythonShellEditor *m_shellEditor;    
public:
    /**Embeds the shell in the provided widget*/
    IgQtPythonShell (QWidget *parent, IgPythonStream *pythonStream);
public:	
/**Pops up a QMessageBox with basic information about the shell*/
    void helpMessage( void );
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_QT_PYTHON_SHELL_H

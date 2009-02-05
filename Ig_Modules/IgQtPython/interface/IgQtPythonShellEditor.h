#ifndef IG_QT_PYTHON_IG_QT_PYTHON_SHELL_EDITOR_H
# define IG_QT_PYTHON_IG_QT_PYTHON_SHELL_EDITOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include <iostream>
# include <qtextedit.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
#define ERROR_STYLE 2
#define OUTPUT_STYLE 1
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class IgQtTextEditStreambuf;
class IgPythonStream;
class QFont;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**\brief This is the main python shell widget.  It's actually an
   overridden QTextEdit widget in which we modified the
   keyPressEvent() method in order to make it act more shell-like.
 */
class IG_QT_PYTHON_API IgQtPythonShellEditor : public QTextEdit
{
private:
    QWidget *m_parent;
    IgQtTextEditStreambuf *m_errorStreambuf;
    IgQtTextEditStreambuf *m_outputStreambuf;
    /** std::ostream to which the python environment streams the error
     * messages (things which python writes to sys.stderr).
     */
    std::ostream *m_errorStream;
    /** std::ostream to which the python environment streams the
     * output messages (things which python writes to sys.stdout).
     */
    std::ostream *m_outputStream;
    IgPythonStream *m_pythonStream;	
    std::vector<std::string> m_history;	
    int m_historyCounter;
    QFont *m_font;
    

    bool doIfSameParagraph(QTextEdit::CursorAction movement);    
public:
    IgQtPythonShellEditor (QWidget *parent, IgPythonStream *pythonStream);	
    /**	Overrides the standard keyPressEvent() to provide a terminal
	like behaviour in place of the word processor one.
     */
    virtual void keyPressEvent(QKeyEvent *ke);

    /**\return keybindings help
     */
    const char *helpMessage(void);    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PYTHON_IG_QT_PYTHON_SHELL_EDITOR_H

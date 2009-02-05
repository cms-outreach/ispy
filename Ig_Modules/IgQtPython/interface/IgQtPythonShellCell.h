#ifndef IG_QT_PYTHON_IG_QT_PYTHON_SHELL_CELL_H
# define IG_QT_PYTHON_IG_QT_PYTHON_SHELL_CELL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtPython/interface/config.h"
# include <qvbox.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class IgPythonStream;
class IgQtTextEditStream;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PYTHON_API IgQtPythonShellCell : public QVBox
{
private:
    QWidget *m_parent;
    IgPythonStream *m_pythonStream;
    IgQtTextEditStream *m_outputStream;
    IgQtTextEditStream *m_errorStream;
public:
    IgQtPythonShellCell (QWidget *parent, IgPythonStream *pythonStream);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_SCINTILLA_IG_QT_PYTHON_SHELL_CELL_H

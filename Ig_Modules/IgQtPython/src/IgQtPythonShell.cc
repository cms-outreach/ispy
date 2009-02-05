//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtPython/interface/IgQtPythonShell.h"
#include "Ig_Modules/IgQtPython/interface/IgQtPythonShellCell.h"
#include "Ig_Modules/IgQtPython/interface/IgQtPythonShellEditor.h"
#include "Ig_Modules/IgPython/interface/IgPythonStream.h"

#include <qlayout.h>
#include <qlistview.h>
#include <qvbox.h>
#include <iostream>
#include <qpushbutton.h>
#include <qmessagebox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtPythonShell::IgQtPythonShell (QWidget *parent, 
				  IgPythonStream *pythonStream)
    : QVBox (parent),
      m_parent (parent),
      m_pythonStream (pythonStream),
      m_shellEditor (0)
{
    ASSERT (m_parent);
    
    m_shellEditor = new IgQtPythonShellEditor (this, m_pythonStream);
    m_shellEditor->show ();
    this->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->show ();    
}

void IgQtPythonShell::helpMessage (void)
{
    QMessageBox::information (m_parent,
			      "Python Shell Help",
			      m_shellEditor->helpMessage());    
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtPython/interface/IgQtTextEditStreambuf.h"
#include "Ig_Modules/IgQtPython/interface/IgQtDecorator.h"
#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
#include "Ig_Modules/IgPython/interface/IgPythonFactory.h"
#include <classlib/utils/Log.h>
#include <qtextedit.h>
#include <qcolor.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtTextEditStreambuf::IgQtTextEditStreambuf (QTextEdit *widget, 
					      const int streamStyle, 
					      const bool waitToSync)
    :m_widget (widget),
     m_streamStyle (streamStyle),
     m_waitToSync (waitToSync),
     m_decorator (0),
     m_decoratorInterpreter (new IgPythonInterpreter())
{
    IgPythonFactory<IgQtDecorator> decoratorFactory(m_decoratorInterpreter);
    decoratorFactory.loadScript ("ShellCellDecorator");
    decoratorFactory.set ("ColorfulDecorator");
    m_decorator = &(decoratorFactory).create ();
}

IgQtTextEditStreambuf::~IgQtTextEditStreambuf (void) 
{
    sync ();
}

int 
IgQtTextEditStreambuf::sync (void) 
{
    if(m_waitToSync == false)
    {
	realSync ();	
    }    
    return 0;
}

void
IgQtTextEditStreambuf::realSync (void)
{
    if(str () != "\n")
    {
	if(str () != "")
	{
	    int para;
	    int pos;
	    m_widget->getCursorPosition (&para, &pos);
	    LOG (0, trace, lat::LFgeneral, 
		 "Current position:" << para 
		 << "\nNumber of paragraphs:" 
		 << m_widget->paragraphs() << "\n");
      
	    m_widget->append (m_decorator->decorate (str ().c_str (), 
						     m_streamStyle).c_str ());	  
	    m_widget->setCursorPosition (para+1, 0);
	}
	else
	{
	}
    }
    
    str (std::string (""));
}

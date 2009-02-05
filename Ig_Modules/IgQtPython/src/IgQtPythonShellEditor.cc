//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtPython/interface/IgQtPythonShellEditor.h"
#include "Ig_Modules/IgQtPython/interface/IgQtTextEditStreambuf.h"
#include "Ig_Modules/IgPython/interface/IgPythonStream.h"

#include <iostream>
#include <qregexp.h>
#include <qfont.h>
#include <qsizepolicy.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtPythonShellEditor::IgQtPythonShellEditor (QWidget *parent,
					      IgPythonStream *pythonStream)
    : QTextEdit (parent),
      m_parent (parent),
      m_errorStreambuf (new IgQtTextEditStreambuf(this,ERROR_STYLE, true )),
      m_outputStreambuf (new IgQtTextEditStreambuf(this,OUTPUT_STYLE, true )),
      m_errorStream (new std::ostream(m_errorStreambuf)),
      m_outputStream (new std::ostream(m_outputStreambuf)),
      m_pythonStream (pythonStream),
      m_history (),
      m_historyCounter (0),
      m_font (new QFont ("clean",10,QFont::Light))
{
    m_history.push_back ("");
    
    m_pythonStream->redirectOutput (*m_outputStream);    
    m_pythonStream->redirectError (*m_errorStream);    
    setTextFormat (Qt::RichText);
    m_font->setStyleHint (QFont::Courier);
    setCurrentFont (*m_font);
    setWordWrap (QTextEdit::NoWrap);
    setTabStopWidth (100);
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    QString startMessage = 
	"<TABLE BORDER=0 WIDTH=100% CELLSPACING=0"
	" CELLPADDING=0 BGCOLOR=#A0A0A0>" 
	"<TR><TD><PRE>"
	+ QString (helpMessage ()) +
	"</PRE></TD></TR></TABLE>";    
    append (startMessage);
    append ("");
}

bool
IgQtPythonShellEditor::doIfSameParagraph (QTextEdit::CursorAction movement)
{
    int para, pos, newPara, newPos;
    
    getCursorPosition (&para, &pos);
    moveCursor (movement, false);
    getCursorPosition (&newPara, &newPos);
    if(newPara != para) 
    {
	setCursorPosition (para, pos);
	return false;
    }
    return true;    
}

void
IgQtPythonShellEditor::keyPressEvent (QKeyEvent *ke)
{
    // FIXME: there should be a keybinding service which 
    int para;
    int pos;

    getCursorPosition (&para, &pos);
    
    int key = ke->key ();
    int modifier = ke->state ();
    
    /*When not typing in last paragraph, move to last paragraph!*/
    if(para != paragraphs()-1 )
    {
	setCursorPosition( paragraphs() - 1, 
			   paragraphLength (paragraphs () - 1));
    }
    
    if(key == Qt::Key_Return 
       && modifier == ControlButton)
    {
	getCursorPosition (&para, &pos);
	QString executionString (text (para));	
	if(executionString != "" )
	{
	    removeParagraph (para);
	    QString executedCommand = 
		"<TABLE BORDER=0 WIDTH=100% CELLSPACING=0"
		"CELLPADDING=0 BGCOLOR=#A0A0A0><TR><TD><PRE>"
		+ executionString
		+ "</PRE></TD></TR></TABLE>";	    
	    
	    append (executedCommand);

	    executionString = executionString.replace(QRegExp("<br />"),"\n");
	    executionString = executionString.replace(QRegExp("&quot;"),"\"");
	    executionString = executionString.replace(QRegExp("&amp;"),"&");
	    executionString = executionString.replace(QRegExp("&lt;"),"<");
	    executionString = executionString.replace(QRegExp("&gt;"),">");
	    
	    m_history.push_back (std::string (executionString.ascii()));
	    m_historyCounter = 0;
	
	    *m_pythonStream << executionString << std::endl;
	
	    m_outputStreambuf->realSync ();
	    m_errorStreambuf->realSync ();
	
	    if (paragraphs() > 200) 
		removeParagraph (0);
	
	    append ("-");
	    setCursorPosition (paragraphs () - 1, 0);
	    setSelection (paragraphs () - 1, 
			  0, 
			  paragraphs () -1, 
			  1);
	    insert ("");
	}
    }
    else if(key==Key_Return && modifier != ControlButton)
    {
	QKeyEvent *t_ke = new QKeyEvent(QEvent::KeyPress,
					key,
					ke->ascii(),
					ControlButton);
	t_ke->ignore();
	QTextEdit::keyPressEvent (t_ke);	
    }    
    else if( (key == Qt::Key_Up || key == Qt::Key_Down) 
	     && modifier == ControlButton)
    {
	int historyCounter = m_historyCounter;
	
	if(key == Qt::Key_Up) 
	    m_historyCounter--;
	else 
	    m_historyCounter++;

	getCursorPosition (&para, &pos);
	setSelection(para, 0, para, paragraphLength (para));
	QString historyEntry (m_history[historyCounter % m_history.size ()].c_str ());

	QStringList t_stringList = QStringList::split("\n", historyEntry);

	// If the history entry is empty, just insert "" otherwise
	// insert, one by one, all the lines in the historyEntry
	if (historyEntry == "")
	{
	    insert ("");	    
	}
	else
	{
	    for (QStringList::iterator i = t_stringList.begin() ; 
		 i != t_stringList.end() ; 
		 i++)
	    {		    
		insert((*i).ascii());
		moveCursor(MoveLineEnd,false);
		
		QKeyEvent *t_ke = new QKeyEvent(QEvent::KeyPress,
						Key_Return,
						10,
						ControlButton);
		t_ke->ignore();
		QTextEdit::keyPressEvent (t_ke);
	    }
	}
	setCursorPosition (para,
			   paragraphLength (para));
    }
    else if (key == Qt::Key_Down)
    {
	doIfSameParagraph (MoveDown);
    }    
    else if (key == Qt::Key_Up)
    {
	doIfSameParagraph (MoveUp);
    }
    else if (key == Qt::Key_Left)
    {
	doIfSameParagraph (MoveBackward);
    }    
    else if (key == Qt::Key_Right)
    {
	doIfSameParagraph (MoveForward);
    }
    else if (key == Key_Delete)
    {
	getCursorPosition (&para,&pos);
	if (pos < paragraphLength (para))
	{
	    ke->ignore ();
	    QTextEdit::keyPressEvent (ke);	    
	}	
    }
    else if (key == Key_Backspace)
    {
	getCursorPosition (&para,&pos);
	if(pos > 0)
	{
	    QRegExp isTabRE ("(^|\n)\\s\\s\\s*$");
	    QString trunc = text (para);
	    trunc.truncate (pos);
	    
	    if( trunc.find (isTabRE,0) != -1 )
	    {
		ke->ignore ();
		QTextEdit::keyPressEvent (ke);
		ke->ignore ();
		QTextEdit::keyPressEvent (ke);
	    }else
	    {
		ke->ignore ();
		QTextEdit::keyPressEvent (ke);
	    }	    
	}
    }
    else if (key == Key_PageUp)
    {
	if( modifier == ControlButton)
	{	    
	    scrollBy (0, -50);
	}
	else
	{
	    doIfSameParagraph (MovePgDown);
	}	
    }
    else if (key == Key_PageDown)
    {
	if(modifier == ControlButton)
	{	    
	    scrollBy (0, 50);
	}else
	{
	    doIfSameParagraph (MovePgDown);
	}	
    }
    else if (key == Qt::Key_Space 
	     && modifier == ControlButton)
    {
	getCursorPosition (&para, &pos);
	QString str = text (para);
	QString tempStr = str;
	tempStr.truncate (pos);
	QRegExp startStr ("[\\w._]*$");
	QRegExp endStr ("[\\w._]*");

	unsigned int s = startStr.match (tempStr, 0);
	endStr.match (str, pos);
	unsigned int e = pos + endStr.matchedLength ();
	
	setSelection (para, s, para, e);
	insert (m_pythonStream->completeName (selectedText ().ascii ()).c_str ());	
    }
    else if (key == Qt::Key_Tab)
    {
	getCursorPosition (&para, &pos);
	moveCursor (MoveLineStart, false);
	insert ("  ");
	setCursorPosition (para, pos+2);	
    }
    else    
    {
	ke->ignore ();
	QTextEdit::keyPressEvent (ke);
    }
}

const char *
IgQtPythonShellEditor::helpMessage (void)
{
    static std::string HelpMessage = 
	"Welcome to IGUANA python shell!\n"
	"----------------------------------\n"
	"Ctrl-Return to execute typed text\n"
	"Ctrl-Space on a word to complete it"
	"according to python dictionary\n"
	"Ctrl-Up for previous history item\n"
	"Ctrl-Down for next history item\n";

    return HelpMessage.c_str();    
}

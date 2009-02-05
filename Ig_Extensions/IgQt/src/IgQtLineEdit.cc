//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtLineEdit::IgQtLineEdit (QWidget *parent, const char *name)
    : QLineEdit (parent, name),
      m_textChanged (false)
{
    connect(this, SIGNAL(textChanged(const QString&)),
	    this, SLOT(changed(const QString&)));

    connect(this, SIGNAL(returnPressed()),
	    this, SLOT(changed()));
}

IgQtLineEdit::IgQtLineEdit (const QString &contents,
			    QWidget *parent,
			    const char *name)
    : QLineEdit (contents, parent, name),
      m_textChanged (false)
{
    connect(this, SIGNAL(textChanged(const QString&)),
	    this, SLOT(changed(const QString&)));
    connect(this, SIGNAL(returnPressed()),
	    this, SLOT(changed()));
}

void IgQtLineEdit::changed (const QString & /* str */)
{ m_textChanged = true; }

void IgQtLineEdit::changed (void)
{
    if (m_textChanged)
    {
	emit valueChanged (text ());
	m_textChanged = false;
    }
}

void IgQtLineEdit::focusOutEvent (QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    changed ();
}

void IgQtLineEdit::setChanged (bool value)
{ m_textChanged = value; }

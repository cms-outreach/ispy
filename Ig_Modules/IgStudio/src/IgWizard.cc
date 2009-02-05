//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgWizard.h"
#include "Ig_Modules/IgStudio/interface/IgWizardPage.h"
#include <qwidgetstack.h>
#include <qpushbutton.h>
#include <qaccel.h>
#include <qlabel.h>

#undef emit
#include <classlib/utils/DebugAids.h>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgWizard::IgWizard (QWidget *parent, const char *name, bool modal, WFlags f)
    : IgWizardBase (parent, name, modal, f),
      m_stack (new QWidgetStack (m_content)),
      m_current (0),
      m_accels (new QAccel (this)),
      m_backAccel (-1),
      m_nextAccel (-1)
{
    m_backAccel = m_accels->insertItem (Qt::ALT + Qt::Key_Left);
    m_accels->connectItem (m_backAccel, this, SLOT(back()));
    m_nextAccel = m_accels->insertItem (Qt::ALT + Qt::Key_Right);
    m_accels->connectItem (m_nextAccel, this, SLOT(next()));

    m_stack->installEventFilter (this);
    m_helpButton->hide ();
}

//////////////////////////////////////////////////////////////////////
void
IgWizard::show (void)
{
    if (currentPage ())
	showPage (currentPage ());
    else if (pageCount () > 0)
    {
	// Find first appropriate page; there may not be any, so be
	// prepared to fall back on showing a blank face
	int i = 0, n = pageCount ();
	while (i < n && !page (i)->isAppropriate ())
	    ++i;
	showPage (i < n ? page (i) : 0);
    }
    else
	showPage (0);

    QDialog::show ();
}

bool
IgWizard::eventFilter (QObject *o, QEvent *e)
{
    if (o == m_stack && e && e->type () == QEvent::ChildRemoved)
    {
        QChildEvent *c = (QChildEvent *) e;
        if (c->child () && c->child ()->isWidgetType ())
	{
	    ASSERT (dynamic_cast<IgWizardPage *> (c->child ()));
            removePage (static_cast<IgWizardPage *> (c->child()));
	}
    }
    return IgWizardBase::eventFilter (o, e);
}

//////////////////////////////////////////////////////////////////////
QString
IgWizard::title (void) const
{ return m_title->text (); }

void
IgWizard::setTitle (QString title)
{ m_title->setText (title); }

//////////////////////////////////////////////////////////////////////
void
IgWizard::addPage (IgWizardPage *page)
{
    ASSERT (page);
    ASSERT (indexOf (page) == -1);
    // FIXME: tell previous page that next page is available?
    // FIXME: tell page if it has a previous one?
    m_pages.push_back (page);
    m_stack->addWidget (page);
}

void
IgWizard::insertPage (IgWizardPage *page, int index)
{
    ASSERT (page);
    ASSERT (indexOf (page) == -1);
    // FIXME: tell previous page that next page is available
    // FIXME: tell page if it has a previous one?
    if (index < 0 || index > pageCount ())
	index = pageCount ();

    m_pages.insert (m_pages.begin () + index, page);
    m_stack->addWidget (page, index);
}

void
IgWizard::removePage (IgWizardPage *page)
{
    ASSERT (page);

    int			i = indexOf (page);
    IgWizardPage	*cp = currentPage ();

    ASSERT (i >= 0 && i < pageCount ());
    m_pages.erase (m_pages.begin () + i);
    m_stack->removeWidget (page);

    if (cp == page) 
    {
	if (--i < 0)
	    i = 0;
	if (pageCount () > 0)
	    showPage (this->page (i));
	else
	    showPage (0);
    }
}

//////////////////////////////////////////////////////////////////////
void
IgWizard::showPage (IgWizardPage *page)
{
    if (m_current && page != m_current)
	m_current->leave ();

    if (page && page != m_current)
    {
	// FIXME: We are not resizing the page correctly.  What is
	// supposed to happen is that m_content automatically resizes
	// to the maximum size available, m_stack follows resizing the
	// page.  Somewhere in the maze of layout management this is
	// not happening, and I don't know if it because of wrong size
	// hints, the fact that pages aren't the children of the stack
	// or what -- no setting seems to help.
	//
	// The calls to adjustSize()/resize() are band-aid to try to
	// get a decent work-around, but they still don't do the right
	// thing.  Calling just resize() gets the first size page
	// wrong (it gets resized to default m_contents size); calling
	// adjustSize() on the first time sorts this out.  The first
	// page still has wrong size if the user goes back.
	m_stack->raiseWidget (page);
	if (! m_current)
	    m_stack->adjustSize ();
	else
	    m_stack->resize (m_content->size ());
	m_category->setText (page->title ());
	page->enter ();
    }

    m_current = page;
    updateButtons ();

    selected (page);
}

IgWizardPage *
IgWizard::currentPage (void) const
{
    return m_current;
}

IgWizardPage *
IgWizard::page (int index) const
{
    ASSERT (index >= 0 && index < pageCount ());
    return m_pages [index];
}

int
IgWizard::pageCount (void) const
{
    return m_pages.size ();
}

int
IgWizard::indexOf (IgWizardPage *page) const
{
    std::vector<IgWizardPage *>::const_iterator i
	= std::find (m_pages.begin (), m_pages.end (), page);
    return i == m_pages.end () ? -1 : i - m_pages.begin ();
}

//////////////////////////////////////////////////////////////////////
void
IgWizard::back (void)
{
    int i = indexOf (m_current);
    ASSERT (i >= 0 && i < pageCount ());
    
    for (--i; i >= 0 && !page (i)->isAppropriate (); --i)
	;

    ASSERT (i >= 0);
    ASSERT (page (i));
    showPage (page (i));
}

void
IgWizard::next (void)
{
    int i = indexOf (m_current);
    ASSERT (i >= 0 && i < pageCount ());
    
    for (++i; i < pageCount () && !page (i)->isAppropriate (); ++i)
	;

    ASSERT (i < pageCount ());
    ASSERT (page (i));
    showPage (page (i));
}

void
IgWizard::help (void)
{
    QWidget		*w = m_stack->visibleWidget ();
    IgWizardPage	*page = dynamic_cast<IgWizardPage *> (w);
    ASSERT (page);
    page->help ();
}

//////////////////////////////////////////////////////////////////////
void
IgWizard::updateButtons (void)
{
    updateBack ();
    updateNext ();
    updateFinish ();
    updateHelp ();
}

void
IgWizard::updateBack (void)
{
    IgWizardPage	*page = currentPage ();
    int			i = page ? indexOf (page) : -1;
    bool		enable = false;

    // Check if this is the first page
    while (!enable && --i >= 0)
	enable = this->page (i)->isAppropriate ();

    enable = enable && page && page->isBackAvailable ();
    m_backButton->setEnabled (enable);
    m_accels->setItemEnabled (m_backAccel, enable);
    if (enable && ! m_backButton->isVisible ())
	m_backButton->show ();
}

void
IgWizard::updateNext (void)
{
    IgWizardPage	*page = currentPage ();
    int			i = page ? indexOf (page) : pageCount ();
    bool		enable = false;
 
    // Check if this is the last page
    while (!enable && ++i < pageCount ())
	enable = this->page (i)->isAppropriate ();

    enable = enable && page && page->isNextAvailable ();
    m_nextButton->setEnabled (enable);
    m_accels->setItemEnabled (m_nextAccel, enable);
    if (enable && ! m_nextButton->isVisible ())
	m_nextButton->show ();
    if (enable)
	m_nextButton->setDefault (true);
}

void
IgWizard::updateFinish (void)
{
    IgWizardPage	*page = currentPage ();
    bool		enable = page && page->isFinishAvailable ();

    m_finishButton->setEnabled (enable);
    if (enable && ! m_finishButton->isVisible ())
	m_finishButton->show ();
    if (enable)
	m_finishButton->setDefault (true);
}

void
IgWizard::updateHelp (void)
{
    IgWizardPage	*page = currentPage ();
    bool		enable = page && page->isHelpAvailable ();

    m_helpButton->setEnabled (enable);
    if (enable && ! m_helpButton->isVisible ())
	m_helpButton->show ();
}

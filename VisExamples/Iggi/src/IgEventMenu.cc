//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/Iggi/interface/IgEventMenu.h"
#include "Iguana/Framework/interface/IgEnvsElement.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/GLModels/interface/IgSoTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Studio/interface/IgFileFetcher.h"
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoGroup.h>
#include <qbuttongroup.h>
#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qstring.h>
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/InputStream.h"
#undef emit
#include "classlib/utils/Log.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/zip/ZipMember.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

lat::logflag LFiggi = { 0, "iggi", true, -1 };

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgEventMenu, "Services/Iggi/GUI/EventMenu");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace lat;

IgEventMenu::IgEventMenu (IgState *state)
    : m_state (state)
{
    ASSERT (m_state);
    m_state->put (s_key, this);

    ASSERT (IgQtAppMenuService::get (m_state));

    QMenuBar *menu = IgQtAppMenuService::get (m_state)->menuBar ();

    ASSERT (menu);

    QPopupMenu	*event = new QPopupMenu (menu);
    event->setCaption ("Iggi");
    event->insertTearOffHandle ();

    ASSERT (menu);

    event->insertItem ("&Open Event File...",
		       this,
		       SLOT(openEventFile ()), Qt::CTRL+Qt::Key_O,
		       MENU_RE_INIT_APP, IgQtAppMenuService::index
		       (event, MENU_RE_INIT_APP));    

    event->insertItem ("&Next Event",
		       this,
		       SLOT(nextEvent ()), Qt::CTRL+Qt::Key_N,
		       MENU_EVENT_NEXT, IgQtAppMenuService::index
		       (event, MENU_EVENT_NEXT));    

    event->insertItem ("&Previous Event",
		       this,
		       SLOT(previousEvent ()), Qt::CTRL+Qt::Key_P,
		       MENU_EVENT_PREVIOUS, IgQtAppMenuService::index
		       (event, MENU_EVENT_PREVIOUS));    

    event->insertItem ("&Goto Event...",
		       this,
		       SLOT(processEventDialog ()), Qt::CTRL+Qt::Key_G,
		       MENU_EVENT_NUM, IgQtAppMenuService::index
		       (event, MENU_EVENT_NUM));

    event->insertItem ("&Rewind",
		       this,
		       SLOT(rewind ()), Qt::CTRL+Qt::Key_R,
		       MENU_EVENT_REWIND, IgQtAppMenuService::index
		       (event, MENU_EVENT_REWIND));

    event->insertItem ("&Skip...",
		       this,
		       SLOT(skipEventDialog ()), 0,
		       MENU_EVENT_SKIP, IgQtAppMenuService::index
		       (event, MENU_EVENT_SKIP));

    menu->insertItem ("&Iggi", event, MENU_ID_EVENT,
		      IgQtAppMenuService::index (menu, MENU_ID_EVENT));
				       
    menu->setItemEnabled (MENU_RE_INIT_APP, true);
    menu->setItemEnabled (MENU_EVENT_NEXT, false);
    menu->setItemEnabled (MENU_EVENT_PREVIOUS, false);
    menu->setItemEnabled (MENU_EVENT_NUM, false);
    menu->setItemEnabled (MENU_EVENT_REWIND, false);	
    menu->setItemEnabled (MENU_EVENT_SKIP, false);
}

IgEventMenu::~IgEventMenu (void)
{
    m_archive->close ();
    delete m_archive;
    m_archive = 0;

    ASSERT (m_state);
    m_state->detach (s_key);
}
   
void
IgEventMenu::openEventFile (void) 
{   	
    QString filename = QFileDialog::getOpenFileName ("./",	
						     "",
						     0,
						     QString::null,
						     "Open Event File");

    Filename inputFilename (Filename(filename.latin1 ()).substitute (ShellEnvironment ()));
    m_archive = new ZipArchive (inputFilename, IOFlags::OpenRead);
    m_it = m_archive->begin ();

    QMenuBar *menu = IgQtAppMenuService::get (m_state)->menuBar ();
    ASSERT (menu);
    menu->setItemEnabled (MENU_EVENT_NEXT, true);
    menu->setItemEnabled (MENU_EVENT_PREVIOUS, true);
    //menu->setItemEnabled (MENU_EVENT_NUM, true);
    menu->setItemEnabled (MENU_EVENT_REWIND, true);	
    //menu->setItemEnabled (MENU_EVENT_SKIP, true);

    IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state);
    ASSERT (sbar);
    sbar->setMessage (QString ((*m_it)->name ()));

    nextEvent ();
}

void
IgEventMenu::nextEvent (void)
{
    if (m_it != m_archive->end ())
    {	
	++m_it;
    
	IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state);
	ASSERT (sbar);
	sbar->setMessage (QString ((*m_it)->name ()));

	//readEventFile ((*m_it)->name ().name ());
	readZipMember (m_it);
    }
}

void
IgEventMenu::previousEvent (void)
{    
    if (m_it != m_archive->begin ())
    {
	--m_it;
    
	IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state);
	ASSERT (sbar);
	sbar->setMessage (QString ((*m_it)->name ()));

	//readEventFile ((*m_it)->name ().name ());
	readZipMember (m_it);
    }
}

void
IgEventMenu::rewind (void)
{    
    m_it = m_archive->begin ();
    
    IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state);
    ASSERT (sbar);
    sbar->setMessage (QString ((*m_it)->name ()));

    nextEvent ();
}

void
IgEventMenu::processEvent (unsigned int runNum, unsigned int eventNum)
{}

void
IgEventMenu::processEventDialog (void) 
{}

void
IgEventMenu::skipEvent (long num)
{}

void
IgEventMenu::skipEventDialog (void)
{}

void
IgEventMenu::readEventFile (const std::string &filename)
{
    IgFileFetcher *fetcher = IgFileFetcher::get (m_state);
    if (! fetcher)
	fetcher = new IgFileFetcher (m_state);

    std::string local = fetcher->fetch (filename);
    if (local.empty ())
    {
	LOG (0, trace, LFiggi, "Iggi: " << filename
	     << ": ERROR Failed to Access File.\n");
	QMessageBox::warning (0,
                              "File Open Error", 
			      "\""+ filename + "\" file not found.", "Ok");
	return;
    }

    SoNode *node = 0;
    SoInput file;
    if (! file.openFile (local.c_str ()) 
	|| ! SoDB::read (&file, node) 
	|| ! node)
    {
	LOG (0, trace, LFiggi, "Iggi: " << filename
	     << ": ERROR Failed to Read File.\n");
	QMessageBox::warning (0,
			      "File Access Error", 
			      "Can not open file \""+ filename + "\" for reading.", "Ok");
	return;
    }

    IgTwig *rootTwig = IgDocumentData::get (m_state)->root ();
    if (rootTwig->children ())
	rootTwig->destroy ();

    IgSoTwig *twigs = IgSoTwig::buildFrom (node);
    rootTwig->add (twigs);

    IgRepSet::invalidate (rootTwig, IgTwig::STRUCTURE_MASK | IgTwig::SELF_MASK);
}

void
IgEventMenu::readZipMember (lat::ZipArchive::Iterator i)
{    
    InputStream *is = m_archive->input (*i);
    
    IOSize length = (*i)->size (ZipMember::UNCOMPRESSED);
    std::vector<char> buffer(length+1, 0);
    is->xread(&buffer[0], length);
    is->close ();
    delete is;

    SoInput input;
    SoNode *node = 0;
    input.setBuffer (&buffer[0], length);
    if (! SoDB::read (&input, node) 
	|| ! node)
    {
	LOG (0, trace, LFiggi, "Iggi: " << (*i)->name ().name ()
	     << ": ERROR Failed to Read File.\n");
	QMessageBox::warning (0,
			      "ZipMember Access Error", 
			      QString ("Can not open ZipMember \"%1\" for reading.").arg ((*i)->name ().name ()), "Ok");
	return;
    }
    
    IgTwig *rootTwig = IgDocumentData::get (m_state)->root ();
    if (rootTwig->children ())
	rootTwig->destroy ();

    IgSoTwig *twigs = IgSoTwig::buildFrom (node);
    rootTwig->add (twigs);

    IgRepSet::invalidate (rootTwig, IgTwig::STRUCTURE_MASK | IgTwig::SELF_MASK);
}

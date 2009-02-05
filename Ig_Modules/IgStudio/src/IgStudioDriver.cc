//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"
#include "Ig_Modules/IgStudio/interface/IgStudioSetup.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppExtensions.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppLoopService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppActiveService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppImageService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQMainWindowSite.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgEnvsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgPluginLoader.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionService.h"
#include "Ig_Modules/IgStudio/interface/IgModelManager.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"
#include "Ig_Modules/IgConfig/interface/IgConfigElement.h"
#include "Ig_Extensions/IgIcons/interface/IgImageIDs.h"
#include "Ig_Extensions/IgIcons/interface/autoprint.xpm"
#include "Ig_Extensions/IgIcons/interface/home.xpm"
#include "Ig_Extensions/IgIcons/interface/set_home.xpm"
#include "Ig_Extensions/IgIcons/interface/view.xpm"
#include "Ig_Extensions/IgIcons/interface/view_all.xpm"
#include "Ig_Extensions/IgIcons/interface/perspective.xpm"
#include "Ig_Extensions/IgIcons/interface/ortho.xpm"
#include "Ig_Extensions/IgIcons/interface/yz_small.xpm"
#include "Ig_Extensions/IgIcons/interface/xz_small.xpm"
#include "Ig_Extensions/IgIcons/interface/yx_small.xpm"
#include "Ig_Extensions/IgIcons/interface/yx_small_col.xpm"
#include "Ig_Extensions/IgIcons/interface/inverse.xpm"
#include "Ig_Extensions/IgIcons/interface/grid.xpm"
#include "Ig_Extensions/IgIcons/interface/zoom_minus.xpm"
#include "Ig_Extensions/IgIcons/interface/zoom_plus.xpm"
#include "Ig_Extensions/IgIcons/interface/pick.xpm"
#include "Ig_Extensions/IgIcons/interface/seek.xpm"
#include "Ig_Extensions/IgIcons/interface/fileprint.xpm"
#include "Ig_Extensions/IgIcons/interface/filesave.xpm"
#include "Ig_Extensions/IgIcons/interface/print_views.xpm"
#include "Ig_Extensions/IgIcons/interface/control_centre.xpm"
#include "Ig_Extensions/IgIcons/interface/IGUANA_SPLASH.xpm"
#include <qapplication.h>
#include <qwindowsstyle.h>
#include <qmessagebox.h>
#include <qsplashscreen.h>
#include <qpixmap.h>
#include <qlistbox.h>
#include <qstring.h>
#include <qregexp.h>
#include <qwhatsthis.h>
#include <qtoolbutton.h>
#include <map>

#undef emit
#include <classlib/utils/DebugAids.h>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgStudioDriver::catalogLabel (void)
{ return "IGUANA"; }

IgStudioDriver::IgStudioDriver (IgState *state)
    : m_state (state),
      m_setup (0),
      m_mainSite (0),
      m_mainWindow (0)
{
    ASSERT (m_state);
    IgExtensionDB::get ()->create ("Services/Global/Config", m_state);
}

IgSite *
IgStudioDriver::mainSite (void) const
{ return m_mainSite; }

QWidget *
IgStudioDriver::mainWindow (void) const
{ return m_mainWindow; }

IgState *
IgStudioDriver::state (void) const
{ return m_state; }

int
IgStudioDriver::run (void)
{
    // Qt scales screen fonts by point size (1/72 inches), not pixel
    // size; the default is 12pt Helvetica.  To get pixel size font,
    // the point size is scaled by the real monitor resolution if it
    // is known.  Thus, on a 100 DPI screen (when correctly reported
    // by X) Qt will use 16-pixel fonts.  I don't know about you but
    // on my screen that just looks plain ugly, so we just tell Qt to
    // ignore to X11 on monitor resolution.
    //
    // You should also make sure you do not have ISO 10646 (Unicode)
    // fonts installed on your X11 server, at least not before any of
    // the normal fonts (at least as of Qt 3.0.1).  They tend to have
    // much larger vertical size (e.g. 20 vs. 15 pixels), presumably
    // to accommodate complex scripts like ideographs.  We don't use
    // any such characters in the GUI, so things look quite odd.
    // (FIXME: Alternatively tell Qt to not use the 10646 variants of
    // the fonts -- can we?)
#if !defined WIN32 && !defined QT_MACOSX_VERSION
    if (! ::getenv ("IGUANA_KEEP_X11_RESOLUTION"))
    {
	QPaintDevice::x11SetAppDpiX (72);
	QPaintDevice::x11SetAppDpiY (72);
    }
#endif //WIN32

    // Initialise Qt
    IgArgsElement	*args = IgArgsElement::get (m_state);
    IgEnvsElement	*envs = IgEnvsElement::get (m_state);
    IgConfigElement	*conf = IgConfigElement::get (m_state);
    QApplication	qapp (args->args (), args->argv ());
    QApplication::setOverrideCursor (Qt::waitCursor);
    
    // Show splash screen
    QSplashScreen *splash = new QSplashScreen(QPixmap (IGUANA_SPLASH));
    splash->show();

    // Load environment
    // and update message on the splash screen
    loadSettings ();
    splash->message ("Loaded settings");
    qApp->processEvents ();    

    // Setup main window
    // and update message on the splash screen
    setupMainWindow ();
    splash->message ("Set up main window");
    qApp->processEvents ();    

    // Delete splash screen and restore cursor   
    splash->clear ();
    QApplication::restoreOverrideCursor ();
    delete splash;

    QString sessionType ("");
    std::string ext ("");
    
    std::vector<std::string> arg;
    arg.push_back ("--iguana-session");
    arg.push_back ("-is");
    
    if (args->find (arg, ext)
        || conf->getValue("/Iguana/Setup/Session", ext)
	|| envs->getEnv ("IGUANA_SETUP", ext))
        sessionType = ext.c_str ();
    
    // Pick a session driver once we run  
    if (! setupSession (sessionType))
	return EXIT_FAILURE;

    // Run the application.  (FIXME: always in separate thread?)
    IgQtAppLoopService::get (m_state)->run ();
    delete m_setup;
    return EXIT_SUCCESS;
}

void
IgStudioDriver::loadSettings (void)
{
    // FIXME: load parameters and environment settings, preferences
}

void
IgStudioDriver::setupMainWindow (void)
{
    // Set main window; set its window attributes early, it may pop up
    // at some point even though we haven't asked for it yet.  (FIXME:
    // set icon) (FIXME: set style only if no -style arg given!) (FIXME:
    // it's braindead to use sitedb to create the site, do it directly)
    m_mainSite = new IgQMainWindowSite (m_state, 0);
    m_mainWindow = IgQtSite::selfFrom (m_mainSite);
    qApp->setMainWidget (m_mainWindow);
#ifndef QT_MACOSX_VERSION // not on mac
    qApp->setStyle (new QWindowsStyle);
#endif

    IgQtAppExtensions::install (m_state, m_mainWindow);
    ASSERT (IgQtAppLoopService::get (m_state));

    // FIXME: Need a better way to setup state
    new IgSelectionService (m_state);
    new IgQtObjectMenuService (m_state);
    new IgQtAppActiveService (m_state);
    new IgDocumentDataManager (m_state);
    new IgModelManager (m_state);
    IgQtAppImageService *is = new IgQtAppImageService (m_state);
    is->registerImage (autoprint_xpm, IgImageIDs::IMAGE_ID_AUTO_PRINT);
    is->registerImage (fileprint, IgImageIDs::IMAGE_ID_PRINT);
    is->registerImage (filesave, IgImageIDs::IMAGE_ID_SAVE_AS);
    is->registerImage (control_centre_xpm, IgImageIDs::IMAGE_ID_CONTROL_CENTRE);
    is->registerImage (yz_small_xpm, IgImageIDs::IMAGE_ID_YZ_SMALL);    
    is->registerImage (yx_small_xpm, IgImageIDs::IMAGE_ID_YX_SMALL);    
    is->registerImage (xz_small_xpm, IgImageIDs::IMAGE_ID_XZ_SMALL);    
    is->registerImage (yx_small_col_xpm, IgImageIDs::IMAGE_ID_YX_SMALL_COL);    
    is->registerImage (inverse_xpm, IgImageIDs::IMAGE_ID_INVERSE);
    is->registerImage (zoom_plus_xpm, IgImageIDs::IMAGE_ID_ZOOM_PLUS);
    is->registerImage (zoom_minus_xpm, IgImageIDs::IMAGE_ID_ZOOM_MINUS);
    is->registerImage (grid_xpm, IgImageIDs::IMAGE_ID_GRID);
    is->registerImage (home_xpm, IgImageIDs::IMAGE_ID_HOME);
    is->registerImage (set_home_xpm, IgImageIDs::IMAGE_ID_SET_HOME);
    is->registerImage (ortho_xpm, IgImageIDs::IMAGE_ID_ORTHO);
    is->registerImage (perspective_xpm, IgImageIDs::IMAGE_ID_PERSPECTIVE);
    is->registerImage (view_all_xpm, IgImageIDs::IMAGE_ID_VIEW_ALL);
    is->registerImage (perspective_xpm, IgImageIDs::IMAGE_ID_PERSPECTIVE);
    is->registerImage (pick_xpm, IgImageIDs::IMAGE_ID_PICK);
    is->registerImage (view_xpm, IgImageIDs::IMAGE_ID_VIEW);
    is->registerImage (seek_xpm, IgImageIDs::IMAGE_ID_SEEK);
    is->registerImage (print_views_xpm, IgImageIDs::IMAGE_ID_PRINT_DEFAULT_VIEWS);
    
    IgConfigElement	*conf = IgConfigElement::get (m_state);
    conf->setPrefix ("/Iguana/Setup");
    std::string caption("IGUANA");
    conf->getValue("Caption", caption);
    m_mainWindow->setCaption (caption.c_str());
    
    unsigned int x = 1000, y=760;
    conf->getValue("Size/X", x);
    conf->getValue("Size/Y", y);
    m_mainWindow->resize (x, y);

    // FIXME: add standard menu items like quit, ...
    // FIXME: add standard tool bar buttons

    // Now auto-load all other GUI extensions.
    ASSERT (IgPluginLoader::get (m_state));
    IgPluginLoader::get (m_state)->load (m_state, "Extensions/Qt GUI/", true);

    // Make it visible
    m_mainWindow->show ();
}

bool
IgStudioDriver::setupSession (QString sessionType)
{
    // FIXME: Use own plug-in category
    ASSERT (! m_setup);

    std::string	prefix ("Services/Studio/Session Types/");
    std::string	preferred;
    
    // Build list of available session drivers (FIXME: remember last?)
    IgExtensionDB::Iterator ext = IgExtensionDB::get ()->begin ();
    IgExtensionDB::Iterator last = IgExtensionDB::get ()->end ();

    if (! sessionType.isEmpty ()) 
    {
	bool found = false;
	for ( ; (ext != last) && (!found); ++ext)
	{
	  std::string fullname = (*ext)->name();
	  std::vector<std::string> aliases = getAliases (fullname, prefix);
	  
	  for (unsigned int i = 0; i < aliases.size (); i++)
	  {
	    if (aliases [i] == sessionType.latin1() )
	    {
	      preferred = fullname;
	      found = true;
	      break;
	    }
	  }
	}
	  
	if (!found)
	{
	    QMessageBox::information (0, "IGUANA Setup", QString ("<p>IGUANA_SETUP environment variable"
								  " is set to <b>\"%1\"</b>. This session type"
								  " is not available.</p>"
								  "<p>Please select one from the next setup"
								  " page.").arg (sessionType));
	    sessionType = "";
	}
    }

    if (sessionType.isEmpty ()) 
    {
	// Create the setup window
	IgStudioSetup *setup = new IgStudioSetup (qApp->mainWidget (), 0, true);
	QListBox	  *configs = setup->m_configs;

	ext = IgExtensionDB::get ()->begin ();
	int select = 0;

        std::map <std::string , std::string> aliasesMap;
	for ( ; ext != last; ++ext)
	{
	  std::string fullname = (*ext)->name();
	  std::vector<std::string> aliases = getAliases (fullname, prefix);
	  
	  for(unsigned int i = 0; i < aliases.size (); i++)
	    aliasesMap [aliases [i]] = fullname;
	  
	  if (aliases.size ())
	  {
	    configs->insertItem (aliases [0].c_str ());
	    if (! preferred.empty () && fullname == preferred)
	      select = configs->count ()-1;
	  }
	}

	if (! configs->count ())
	{
	    QMessageBox::critical (0, "IGUANA", "<p>You do not seem to have any"
				   " IGUANA session configurations available in"
				   " your system at the moment (cf. ``iguana"
				   " --list'', $IGUANA_PLUGINS).  I cannot"
				   " continue without one.</p>"
				   "<p>Please correct the above problem and try"
				   " again.");
	    delete setup;
	    return false;
	}

	configs->setCurrentItem (select);
	preferred = "";

	// Let the user select the driver
	if (setup->exec () != QDialog::Accepted || configs->currentItem () == -1)
	{
	    delete setup;
	    return false;
	}

	preferred = aliasesMap[ configs->currentText ().latin1 () ];
	delete setup;
    }
    
    // Instantiate the selected session driver
    ASSERT (! preferred.empty ());
    IgStudioSetupExtension	*sext = 0;
    IgExtension			*newext = 0;

    try
    {
	// Try to instantiate it
	QApplication::setOverrideCursor (Qt::waitCursor);
	newext = IgExtensionDB::get ()->create (preferred, m_state);

	// Make sure we got what we expect
	ASSERT (newext);
	ASSERT (dynamic_cast<IgStudioSetupExtension *> (newext));
	sext = static_cast<IgStudioSetupExtension *> (newext);
	QApplication::restoreOverrideCursor ();

	// Try to set it up; if fails, take short cut out
	if (! sext->setup (this))
	    throw 1;
    }
    // FIXME: too broad...
    catch (...)
    {
	QApplication::restoreOverrideCursor ();
	delete newext;
	return false;
    }

    m_setup = sext;
    return true;
}

std::vector<std::string>
IgStudioDriver::getAliases (const std::string& name, const std::string& type)
{
  std::vector<std::string> aliases;
  if (name.substr (0, type.length ()) != type){ return aliases; }
  QString substr (&name[type.length ()]);
  QRegExp search (";\\s+");
  while (!substr.isEmpty ())
  {
    int index = substr.find(search);
    if (index == 0)
    { substr = substr.mid (index+search.matchedLength ()); }
    else if (index > 0)
    {
      aliases.push_back (substr.left (index).latin1());
      substr = substr.mid (index+search.matchedLength ());
    }
    else
    {
      aliases.push_back (substr.latin1());
      substr="";
    }
  }
  return aliases;
}

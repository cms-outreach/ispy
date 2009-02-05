//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgIVView/interface/IgIVView.h"
#include "Ig_Modules/IgIVView/interface/IgSoTwig.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetStackSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppWindowService.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/IgFileProxy/interface/IgFileFetcher.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgModelManager.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"

#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSelection.h>

#include <qdir.h>
#include <qfileinfo.h>
#include <qinputdialog.h>
#include <qmainwindow.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <iostream>

#ifdef emit
# undef emit
#endif
#include <classlib/utils/Log.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgIVView::IgIVView (IgPage *page)
    :IgView (page),
     m_state (0),
     m_3dBrowser (0),
     m_twigBrowser (0)
{
    QString filename;
    QString workingDirectory ("./");
    QWidget *mainWindow = dynamic_cast<QMainWindow *> (IgQtAppWindowService::get (page->state ())->mainWindow ());
    
    IgArgsElement *args = IgArgsElement::get (page->state ());
    std::string fname;
    std::vector<std::string> arg;
    arg.push_back ("--ivfile");
    arg.push_back ("-iv");
    
    if (args->find (arg, fname, true)) filename = fname.c_str ();

    if (filename.isEmpty ())
        filename = QFileDialog::getOpenFileName (workingDirectory,
						 "Open Inventor Scenegraph (*.iv)",
						 mainWindow,
						 QString::null,
						 "Select Scenegraph to View");

    if (! filename.isEmpty ())
    {
        IgDocumentDataManager *dm = IgDocumentDataManager::get (page->state ());
        IgState *s =  dm->lookup (0, filename.latin1 ());
    
        if (s)
        {
	    // Do not open the same file gain 
            IgDocumentDataRoot::currentRoot (filename.latin1 ());
	    return;
        }

	// Read in the file
	IgFileFetcher *fetcher = IgFileFetcher::get (page->state ());
	if (! fetcher)
	    fetcher = new IgFileFetcher (page->state ());

	std::string local = fetcher->fetch (filename.ascii ());
	if (local.empty ())
	{
	    LOG (0, trace, lat::LFgeneral, "SoReader: " << filename.ascii ()
		 << ": error: failed to access file.\n");
	    QMessageBox::warning (mainWindow,
				  "File open error", 
				  "\""+ filename + "\" file not found.", "Ok");
	    return;
	}
	
	SoNode      *node = 0;
	SoInput     file;
	if (! file.openFile (local.c_str ()) 
	    || ! SoDB::read (&file, node) 
	    || ! node)
	{
	    LOG (0, trace, lat::LFgeneral, "SoReader: " << filename.ascii ()
		 << ": error: failed to read\n");
	    QMessageBox::warning (mainWindow,
				  "File Access Error", 
				  "Can not open file \""+ filename + "\" for reading.", "Ok");
	    return;
	}
	IgDocumentDataRoot::currentRoot (filename.latin1 ());

	// Construct twigs from the node tree.  This always produces a
	// root that contains magic stuff to go into the browser's root
	// node, i.e. global magic clip planes etc.
	IgSoTwig    *twigs = IgSoTwig::buildFrom (node);
	dump (0, twigs);
	std::cout << std::flush;

        IgDocumentData::get (page->state ())->root (dynamic_cast<IgSoTwig*>(twigs->child(0)));
    
	m_state = new IgState (page->state ());
	new IgSelectionService (m_state);

        Ig3DModel *model = 0;
	IgModelManager::get (m_state)->lookup (m_state, &model);
	
        model->attachPoint ()->magic (twigs->magic ());
        model->attachPoint ()->node (twigs->node ());

        m_3dBrowser = dynamic_cast<IgBrowser*> (IgViewDB::get ()->create ("3D Window", page ));
	
	IgQWidgetStackSite *stackArea = dynamic_cast<IgQWidgetStackSite *> (page->stackArea ());
        m_twigBrowser = IgBrowserDB::get()->create ("Twig", m_state,
					   stackArea->hostWithName (filename.latin1 ()));
        m_twigBrowser->browse (twigs->child (0));
    } 
    else 
    {
	QMessageBox::warning (mainWindow,
			      "No file selected", 
			      "No OpenInventor file has been selected", "Ok");
    }
}

IgIVView::~IgIVView ()
{
  if (m_3dBrowser)
      delete m_3dBrowser;
  if (m_twigBrowser)
      delete m_twigBrowser;
  if (m_state)
      delete m_state;
}

void
IgIVView::dump (unsigned level, IgTwig *twig)
{
    ASSERT (twig);
    LOG (0, trace, lat::LFgeneral, std::string(level, ' ') << "Twig " << twig->name () 
	 << (twig->selfVisible () ? " visible" : " invisible")
	 << (twig->cascade () ? " cascade; " : " no cascade; ")
	 << ((IgSoTwig *) twig)->node ()->getNumChildren () 
	 << " OIV children\n" );
    for (unsigned n = 0; n < twig->children (); ++n)
	dump (level + 2, twig->child (n));
}

IgState*
IgIVView::state (void)
{ return m_state; }

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppMain.h"
#include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppSetup.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepresentable.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQToolBoxSite.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgDocument.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgMDIView.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include <classlib/utils/DebugAids.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgNTupleAppMain, "Services/Generic/NTuple");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgNTupleAppMain::IgNTupleAppMain (IgState *state)
    : m_state (state),
      m_studio (0),
      m_document (0)
{ ASSERT (m_state); m_state->put (s_key, this);}

IgNTupleAppMain::~IgNTupleAppMain (void)
{ ASSERT (m_state); m_state->detach (s_key);}

bool
IgNTupleAppMain::setup (IgNTupleAppSetup *from)
{
    m_studio = from->studio ();
    
    // Create document
    ASSERT (! m_document);
    m_document = new IgDocument (m_studio);
    IgPage *page = new IgPage (m_document);
    
    //Create a toolbox area where the stack is.
    IgQToolBoxSite *toolbox = new IgQToolBoxSite (page->state (), 
						  page->stackArea ());
    
    // Change window caption.
    const char* mName = "DQM/Sources";
    IgDocumentDataRoot::currentRoot (mName);
    
    IgBrowser *fullTreeBrowser 
	= IgBrowserDB::get()->create ("Tree", 
				      page->state (),
				      toolbox->hostWithName ("Available monitorables"));

    ASSERT (fullTreeBrowser);
        
    IgState *groupBrowserState = new IgState (page->state ());    
    new IgSelectionService (groupBrowserState);
    
    IgBrowser *groupsTreeBrowser 
	= IgBrowserDB::get()->create ("Twig", 
				      groupBrowserState,
				      toolbox->hostWithName ("Subscribed monitorables"));
    
    ASSERT (groupsTreeBrowser);

    IgTwig *root = IgDocumentData::get (m_document->state ())->root ("", true);
    ASSERT (root);
    IgSimpleTwig *availableTwigsBranch = new IgSimpleTwig (root, "Available");
    IgSimpleTwig *subscribedTwigsBranch = new IgSimpleTwig (root, "Subscribed");

    m_document->addContentProxy (mName);
    m_document->load ("Services/NTuple/GUI/", true);
    
    fullTreeBrowser->browse (availableTwigsBranch);
    groupsTreeBrowser->browse (subscribedTwigsBranch);
    

    IgRepSet::invalidate (m_document->data ()->root (mName),
		          IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
    
    return true;    
}

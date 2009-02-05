//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgDocument.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQHSplitterSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppStatusBarService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgPluginLoader.h"
#include <qwidget.h>
#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDocument::IgDocument (IgStudioDriver *parent)
    : m_owner (parent),
      m_state (new IgState (parent->state ())),
      // FIXME: We don't really need or want a splitter here, but
      //  I haven't yet figured out how to otherwise make the window
      //  expand to maximum possible space...  Setting size policy
      //  [Expanding,Expanding] doesn't seem to be helping.
      m_window (new IgQHSplitterSite (m_state, parent->mainSite ())),
      m_data (new IgDocumentData (m_state, this)),
      m_currentPage (0)
{
    // FIXME: record IgDocumentElement in state to point to myself?
    // FIXME: construct window layout: tree browser, page area,
    //   docking areas, controller control panels, ...
    // FIXME: maximise, show window?
    setName ("IGUANA Visualization");
    IgQtSite::selfFrom (m_window)->show ();
}

IgDocument::~IgDocument (void)
{
    // FIXME: detach from m_owner
    delete m_window;
    delete m_state;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgStudioDriver *
IgDocument::owner (void) const
{ return m_owner; }

IgState *
IgDocument::state (void) const
{ return m_state; }

IgDocumentData *
IgDocument::data (void) const
{ return m_data; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtSite *
IgDocument::window (void) const
{ return m_window; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgDocument::setName (const std::string &name)
{
    // FIXME: Set (document) window title
    // FIXME: Main window, document window title merging?
    // IgQtSite::selfFrom (m_window)->setName (name.c_str ());
    m_owner->mainWindow ()->setName (name.c_str ());
    m_owner->mainWindow ()->setCaption (name.c_str ());
    // FIXME: file name?
}

void
IgDocument::addContentProxy (const std::string &name)
{
    // FIXME: load certain data prefix
    // FIXME: remember in construction -> write out -> reload on read
    IgQtAppStatusBarService::get (m_state)
	->setMessage ("Loading " + name + "...");
    IgPluginLoader::get (m_state)->load (m_state, "Data/" + name);
}

void
IgDocument::load (const std::string &name, bool prefix /* = false */)
{
    // FIXME: IgPluginLoader::load()
    // FIXME: remember in construction -> write out -> reload on read
    IgQtAppStatusBarService::get (m_state)
	->setMessage ("Loading " + name + (prefix ? "*..." : "..."));
    IgPluginLoader::get (m_state)->load (m_state, name, prefix);
}

void
IgDocument::addPage (IgPage *page)
{
    ASSERT (page);    
    m_pageMap.push_back (page);
    m_currentPage = page;    
}

IgPage *
IgDocument::currentPage (void)
{
    return m_currentPage;    
}

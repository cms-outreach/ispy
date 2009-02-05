//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQWorkspaceSite.h"
#include "Ig_Extensions/IgQt/interface/IgQtWorkspace.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQWorkspaceSite::IgQWorkspaceSite (IgState *, IgSite *parent)
    : m_widget (new IgQtWorkspace (hostFrom (parent))),
      m_currentName (IgDocumentDataRoot::getCurrentRoot())
{    
    IgQtSite::host (parent, m_widget);
}

const char *
IgQWorkspaceSite::catalogLabel (void)
{ 
    return "QWorkspace"; 
}

void
IgQWorkspaceSite::tile (void)
{
    m_widget->tile ();
}

void
IgQWorkspaceSite::cascade (void)
{
    m_widget->cascade ();
}

QWidget *
IgQWorkspaceSite::self (void)
{
    return m_widget;
}

QWidget *
IgQWorkspaceSite::host (void)
{
    return m_widget;
}

void
IgQWorkspaceSite::host (QWidget *child)
{
    // FIXME: add requester on closing window.
    // connect (child, SIGNAL (close ()), this, SLOT (closeWindow ()))
    
    // FIXME: this is hackish... We should make sure that the this
    // is already shown when this is called so that its size cannot be
    // 0.
    QSize defaultSize (300, 300);
    child->resize (defaultSize.expandedTo (m_widget->size () / 2));
    child->parentWidget ()->setCaption (m_currentName);

    m_currentName = IgDocumentDataRoot::getCurrentRoot();    
}

IgQtSite *
IgQWorkspaceSite::hostWithName (const std::string &name)
{
    m_currentName = name;    
    return this;    
}

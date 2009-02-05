//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
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

IgQtSiteStack::IgQtSiteStack (void)
    :m_currentName (IgDocumentDataRoot::getCurrentRoot())
{
}

IgQtSiteStack *
IgQtSiteStack::hostWithName (const QString &name)
{
    m_currentName = name;    
    return this;    
}

QString
IgQtSiteStack::currentName (void)
{
    return m_currentName;    
}

void
IgQtSiteStack::tile (void)
{
}

void
IgQtSiteStack::cascade (void)
{
}

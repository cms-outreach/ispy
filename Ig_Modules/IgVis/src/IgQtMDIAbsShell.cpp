// IgQtMDIAbsShell.cpp: implementation of the IgQtMDIAbsShell class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShell.h"
#include "Ig_Modules/IgVis/interface/IgQtMDI.h"
#include "qworkspace.h"
#include "qdom.h"
#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"
#include "Inventor/Qt/viewers/SoQtFullViewer.h"

int IgQtMDIAbsShell::contextcnt=0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// Note that the widget parent is the workspace of the IgQtMDI, but that
// we must keep track of the MDI itself to access global resources
//////////////////////////////////////////////////////////////////////
const QString IgQtMDIAbsShell::theShellName = QString("IgQtMDIAbsShell");
int IgQtMDIAbsShell::serialNo = 0;

IgQtMDIAbsShell::IgQtMDIAbsShell(IgQtMDI *parent, SoQtFullViewer *aViewer) :
QMainWindow(parent->workspace(),theShellName,0), theViewer(aViewer), parentMDI(parent) 
{
	constructor( true );
}
IgQtMDIAbsShell::IgQtMDIAbsShell(IgQtMDI *parent, SoQtFullViewer *aViewer, bool build_now):
QMainWindow(parent->workspace(),theShellName,0), theViewer(aViewer), parentMDI(parent) 
{
	constructor( build_now );
}
IgQtMDIAbsShell::~IgQtMDIAbsShell()
{
    parentMDI->removeBrowser( this );
}
const QString IgQtMDIAbsShell::shellName()
{
	return theShellName;
}
void IgQtMDIAbsShell::eventsSlot()
{
}
void IgQtMDIAbsShell::autoEventsSlot(bool)
{
}
void IgQtMDIAbsShell::autoPrintViewsSlot(bool)
{
}
void IgQtMDIAbsShell::viewAll()
{
    theViewer->viewAll();
}
void IgQtMDIAbsShell::setUniqueName(const QString &sname)
{
	// make this particular shell widget have a unique (recalculable) name
	QString sn;
	sn.setNum(mySerialNo);
	uniqueName = sname+'_'+sn;
	setName(uniqueName);
}
void IgQtMDIAbsShell::constructor(bool /*build*/)
{
	mySerialNo = serialNo++;
//	setUniqueName(theShellName); //blows up under gcc3_4_3? don't need it anyway
	parentMDI->addBrowser( this );
	if (theViewer)
	{
#if 0 // for now, only bump the cache when cloning... look in derived classes where cloning is available
#ifndef __COIN_
  // IMPORTANT: make sure each GL context has a unique cache context
  // id.  this is needed for TGS/SGI Inventor. Coin handles this
  // automatically.
    static uint32_t contextcnt = 0;
    theViewer->getGLRenderAction()->setCacheContext(contextcnt++);
#endif
#endif
	}

}
QDomElement IgQtMDIAbsShell::shellToXML( void ) const
{
    QDomElement de;
    IgQtResourceMgr& theMgr = IgQtResourceMgr::getInstance();
    QDomDocument &theDoc = theMgr.getDoc(true);

    de = theDoc.createElement( "browser" );
    de.setAttribute( "resourceID", getUniqueName() );

    // only have widget params to store
    de.appendChild( theMgr.widgetToXML( this ) );

    return de;
}
bool IgQtMDIAbsShell::xmlToShell( QDomElement& de )
{
    if ( de.tagName() != "browser" ) return false;
    IgQtResourceMgr& theMgr = IgQtResourceMgr::getInstance();
    return theMgr.xmlToWidget( this, de.namedItem( "widget" ).toElement() );

}


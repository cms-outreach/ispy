// IgQtMDIScanShell.cpp: implementation of the IgQtMDIScanShell class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtMDIScanShell.h"

#include "Ig_Modules/IgVis/interface/IgQtMDIShellBuilder.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIShellFactory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const QString IgQtMDIScanShell::theShellName = QString("IgQtMDIScanShell");
/* uncomment to make visible
namespace {
    IgQtShellRegisterEntry e( QString("IgQtMDIScanShell"), IgQtMDIShellBuilder<IgQtMDIBaseShell>::getInstance(), QString( "Scan Shell" ) );
}
*/
IgQtMDIScanShell::IgQtMDIScanShell(IgQtMDI *parent, SoQtFullViewer *aViewer) :
	IgQtMDIBaseShell(parent, aViewer)
{
	setUniqueName(theShellName);
}
IgQtMDIScanShell::~IgQtMDIScanShell()
{

}

void IgQtMDIScanShell::eventsSlot()
{

}
const QString IgQtMDIScanShell::shellName()
{
	return theShellName;
}


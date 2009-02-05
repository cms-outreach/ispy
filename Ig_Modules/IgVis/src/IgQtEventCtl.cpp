// IgQtEventCtl.cpp: implementation of the IgQtEventCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtEventCtl.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtEventCtl::IgQtEventCtl(IgQtMDIAbsShell *shell) : QPopupMenu(shell)
{
  insertTearOffHandle();
  setCaption("Event Control");
  insertItem("New &Event", shell, SLOT(eventsSlot()));
  autoEventsId   = insertItem("Auto Events",this, SLOT(autoEventsSlot()));
  autoPrintId   = insertItem("Auto Print",this,SLOT(autoPrintSlot()));
  connect(this,SIGNAL(autoPrintOn(bool)),shell,SLOT(autoPrintViewsSlot(bool)));
  connect(this,SIGNAL(autoEventsOn(bool)),shell,SLOT(autoEventsSlot(bool)));

  setItemChecked(autoEventsId,false);
  setItemEnabled(autoEventsId,true);
  setItemChecked(autoPrintId,false);
  setItemEnabled(autoPrintId,true);
}

IgQtEventCtl::~IgQtEventCtl()
{

}
void IgQtEventCtl::autoPrintSlot()
{
	bool itemOn = !isItemChecked(autoPrintId);
	setItemChecked(autoPrintId,itemOn);
	// emit
	autoPrintOn(itemOn);
}
void IgQtEventCtl::autoEventsSlot()
{
	bool itemOn = !isItemChecked(autoEventsId);
	setItemChecked(autoEventsId,itemOn);
	// emit
	autoEventsOn(itemOn);
}

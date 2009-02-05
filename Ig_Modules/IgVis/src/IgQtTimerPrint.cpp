// IgQtTimerPrint.cpp: implementation of the IgQtTimerPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtTimerPrint.h"
#include "Ig_Modules/IgVis/interface/IgQtBrowser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// the static variable
IgQtTimerPrint* IgQtTimerPrint::theTimerPrint = NULL;
// the code for the class
IgQtTimerPrint* IgQtTimerPrint::getTheTimerPrint()
{
	if (!theTimerPrint)
	{
		theTimerPrint = new IgQtTimerPrint;
	}
	return theTimerPrint;
}
void IgQtTimerPrint::setBrowser(IgQtBrowser *browser)
{
	//TODO: do we need to unschedule if there's already a browser?
	theBrowser = browser;
}
IgQtTimerPrint::IgQtTimerPrint() : printOn(false), theBrowser(NULL) 
{
	idleQ.unschedule(); // default is off
	idleQ.setFunction(IgQtTimerPrint::theIgQtTimerPrintCallback);
	idleQ.setData(static_cast<void*>(this));
	waitQ.unschedule();
	waitQ.setFunction(IgQtTimerPrint::theIgQtTimerPrintWaitCallback);
	waitQ.setData(static_cast<void*>(this));
	
}
void IgQtTimerPrint::theIgQtTimerPrintCallback(void *data, SoSensor * /*sensor*/)
{
	IgQtTimerPrint *cc = static_cast<IgQtTimerPrint*>(data);
	cc->doCallback();
}
void IgQtTimerPrint::theIgQtTimerPrintWaitCallback(void *data, SoSensor * /*sensor*/)
{
	IgQtTimerPrint *cc = static_cast<IgQtTimerPrint*>(data);
	cc->doWaitCallback();
}
void IgQtTimerPrint::doWaitCallback()
{
	// schedule the print callback
	idleQ.schedule();
	// emit
	newEventRequest();
}

void IgQtTimerPrint::doCallback()
{
	// don't need to have  a separate callback as there is only one IgQtTimerPrint,
	// but we did it anyway
	if (theBrowser) // should emit an error if undefined
	{
		theBrowser->xClicked();
		theBrowser->printDefSlot(0);
		theBrowser->yClicked();
		theBrowser->printDefSlot(1);
		theBrowser->zClicked();
		theBrowser->printDefSlot(2);
//		idleQ.schedule();
		// emit
//		newEventRequest();
		waitQ.setTimeFromNow(10.0); //time in seconds; needs to be a double as of OIV 3.0
		waitQ.schedule();
	}
}
void IgQtTimerPrint::setTimerPrintOn(bool onoff)
{
	if (onoff == printOn)
	{
		return;
	}
	else if (printOn)
	{
		idleQ.unschedule();
	}
	else
	{
		idleQ.schedule();
	}
	printOn = onoff;
}
IgQtTimerPrint::~IgQtTimerPrint()
{

}

// IgQtTimerPrint.h: interface for the IgQtTimerPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IgQtTimerPrint_H__F5672B73_CE1C_4C7F_9B0C_71A549CE2B50__INCLUDED_)
#define AFX_IgQtTimerPrint_H__F5672B73_CE1C_4C7F_9B0C_71A549CE2B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!
 *  \class IgQtTimerPrint 
 *  \brief A class to encapsulate the idle queue handling for auto printing
 *
 *   Alternates callbacks between the idle queue (to ensure that the rendering is complete from
 *   the last request) and a timer queue (to guarantee a minimum amount of time for interaction, 
 *   such as stopping the auto print).
 *
 */

class IgQtBrowser;

#include "Ig_Modules/IgVis/interface/config.h"
#include "qobject.h"
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

class IG_VIS_API IgQtTimerPrint : public QObject
{
	Q_OBJECT
public:
	//! we only want one instance requesting new events
	static IgQtTimerPrint* getTheTimerPrint();
	//! enable the print on timer function
	void setTimerPrintOn(bool);
	//! the callback routine for the timer
	static void theIgQtTimerPrintCallback(void *data, SoSensor *sensor);
	//! the callback routine for the timer to allow interaction
	static void theIgQtTimerPrintWaitCallback(void *data, SoSensor *sensor);
	//! set the pointer to the browser to take a snapshot of
	void setBrowser(IgQtBrowser *browser);
	virtual ~IgQtTimerPrint();
signals:
	//! We need a new event signal
	void newEventRequest(); //!< ready for another event
private:
	//! private constructor
	IgQtTimerPrint();
	bool printOn;
	SoAlarmSensor waitQ;
	SoIdleSensor idleQ;
	//! the real callback
	void doCallback();
	//! the real wait callback
	void doWaitCallback();
	IgQtBrowser *theBrowser;
	static IgQtTimerPrint *theTimerPrint;
};

#endif // !defined(AFX_IgQtTimerPrint_H__F5672B73_CE1C_4C7F_9B0C_71A549CE2B50__INCLUDED_)

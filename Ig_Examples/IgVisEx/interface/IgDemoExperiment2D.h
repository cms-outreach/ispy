#if !defined IG_DEMO_EXPERIMENT2D_H
#define IG_DEMO_EXPERIMENT2D_H

#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

/*! /class IgDemoExperiment2D
 *  /brief Basic demo/how-to using the classic architecture
 *
 *  /date 20-Jul-2002
 */

class IgQtMDI;

class IgDemoExperiment2D : public IgQtMDIBaseShell
{
	Q_OBJECT //need this for interaction with Qt widgets
public:
	//! base constructor doesn't really need the parent & viewer args
	explicit IgDemoExperiment2D(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgDemoExperiment2D();
	//! returns the className
	virtual const QString shellName();
signals:
	//! signal to indicate a new event is required (local)
	void triggerSignal();
	//! signal to pass on the trigger
	void needTriggerSignal();
public slots: 
	// slots receive signals... this class will need to be run
	// through the moc preprocessor          
	//! slot to cause the experiment to be built and displayed
	virtual void buildSlot();
	//! to notify this viewer a new event is available
	virtual void eventNotifySlot();
	//! processing routine for the "Next Event" button on the viewer
	virtual void eventsSlot();
protected:
private:
	//! need a unique name for this class
	static const QString theShellName;
};

#endif //IG_DEMO_EXERIMENT2D_H

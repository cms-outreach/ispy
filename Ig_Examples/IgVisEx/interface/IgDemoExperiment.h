#if !defined IG_DEMO_EXPERIMENT
#define IG_DEMO_EXPERIMENT

#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

/*! /class IgDemoExperiment
 *  /brief Basic demo/how-to using the classic architecture
 *
 *  /date 20-Jul-2002
 */

class IgQtMDI;

class IgDemoExperiment : public IgQtMDIBaseShell
{
	Q_OBJECT //need this for interaction with Qt widgets
public:
	//! base constructor doesn't really need the parent & viewer args
	explicit IgDemoExperiment(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgDemoExperiment();
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

#endif //IG_DEMO_EXERIMENT

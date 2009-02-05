// IgQtMDITestShell.h: interface for the IgQtMDITestShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDITESTSHELL_H__9D240E12_70FF_44DB_924B_5FFF2FC04CE2__INCLUDED_)
#define AFX_IGQTMDITESTSHELL_H__9D240E12_70FF_44DB_924B_5FFF2FC04CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

/*! /class IgQtMDITestShell 
 *
 *  /brief A test (demo) derived from IgQtMDIBaseShell but with an actual scene
 */

class IgQtMDI;

class IgQtMDITestShell : public IgQtMDIBaseShell  
{
	Q_OBJECT
	typedef IgQtMDITestShell inherited;
public:
	IgQtMDITestShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgQtMDITestShell();
	//! returns the className
	virtual const QString shellName();
public slots:
	//! slot to call build detector, etc., method
	virtual void buildSlot();
	//! slot to get a new event. if adorned, create twigWidget & infoWidget. Overrides parent.
	void eventsSlot();
	//! slot to notify that a new event is ready
	void eventNotifySlot();
private:
	//! a dummy event number
	int event_number;
	//! holds a pointer to a test file
    SoInput *eventInput;
	//! the name
	static const QString theShellName;
	//! for the status of the dummy events file
	bool fileAvailable;

};

#endif // !defined(AFX_IGQTMDITESTSHELL_H__9D240E12_70FF_44DB_924B_5FFF2FC04CE2__INCLUDED_)

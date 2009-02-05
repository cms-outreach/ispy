// IgQtPickTestShell.h: interface for the IgQtPickTestShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IgQtPickTestSHELL_H__9D240E12_70FF_44DB_924B_5FFF2FC04CE2__INCLUDED_)
#define AFX_IgQtPickTestSHELL_H__9D240E12_70FF_44DB_924B_5FFF2FC04CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

/*! /class IgQtPickTestShell 
 *
 *  /brief A test (demo) derived from IgQtMDIBaseShell but with an actual scene
 */

class IgQtMDI;

class IgQtPickTestShell : public IgQtMDIBaseShell  
{
	Q_OBJECT
	typedef IgQtPickTestShell inherited;
public:
	IgQtPickTestShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgQtPickTestShell();
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

};

#endif // !defined(AFX_IgQtPickTestSHELL_H__9D240E12_70FF_44DB_924B_5FFF2FC04CE2__INCLUDED_)

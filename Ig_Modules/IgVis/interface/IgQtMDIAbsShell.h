// IgQtMDIAbsShell.h: interface for the IgQtMDIAbsShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDIABSSHELL_H__46418418_F2BB_47C7_B2CF_DE23E669571F__INCLUDED_)
#define AFX_IGQTMDIABSSHELL_H__46418418_F2BB_47C7_B2CF_DE23E669571F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "qmainwindow.h"
#include "qstring.h"

class SoQtFullViewer;
class IgQtPhysTabs;
class IgQtMDI;
class QDomElement;
class IgQtMDIAbsShell;

/*!
 * \class IgQtMDIAbsShell IgQtMDIAbsShell.h Ig_Modules/IgVis/interface/IgQtMDIAbsShell.h
 *
 * \brief Abstract class for all IgQt MDI widgets
 *
 *  \author G. Alverson, Northeastern University Computing Group
 *  \date 16-Feb-2001
 */

class IG_VIS_API IgQtMDIAbsShell : public QMainWindow  
{
	Q_OBJECT
public:
	explicit IgQtMDIAbsShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgQtMDIAbsShell() = 0;
	//! returns the className
	virtual const QString shellName();
	//! store parameters into XML
	virtual QDomElement shellToXML( void ) const;
	//! restore parameters from XML
	virtual bool xmlToShell( QDomElement& de );
	//! the unique name of this browser
	virtual const QString getUniqueName() const;
public slots:
	//! slot to get a new event. if adorned, create twigWidget & infoWidget
	virtual void eventsSlot();
	//! slot to continuously get new events
	virtual void autoEventsSlot(bool);
	//! slot to continuously get new events and print
	virtual void autoPrintViewsSlot(bool);
	//! slot to view entire scene
	virtual void viewAll();
protected:
	IgQtMDIAbsShell(IgQtMDI *parent, SoQtFullViewer *aViewer, bool build_now);
	//! the SoQt viewer (if any) controlled by this class
	SoQtFullViewer *theViewer;
	//! generate a unique (hidden) name
	virtual void setUniqueName(const QString &sname);
	//! currently use a tab widget to control the physics/our browser preferences; SoQt has its own.
	IgQtPhysTabs *prefs;
	//! the IgQtMDI parent
	IgQtMDI *parentMDI;
	//! in order to share a view, each view must have a unique cache context
	static int contextcnt;
private:
	static const QString theShellName;
	//! to keep track of all MDI shells
	static int serialNo;
	//! version of serialNo for this shell and descendents
	int mySerialNo;
	QString uniqueName;
	//! routine for actual build
	void constructor(bool build);

};
inline const QString IgQtMDIAbsShell::getUniqueName() const {return uniqueName;}
#endif // !defined(AFX_IGQTMDIABSSHELL_H__46418418_F2BB_47C7_B2CF_DE23E669571F__INCLUDED_)

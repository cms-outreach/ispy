// IgQtMDIScanShell.h: interface for the IgQtMDIScanShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDISCANSHELL_H__F29679CB_5389_4A4A_884D_2171DA377AB5__INCLUDED_)
#define AFX_IGQTMDISCANSHELL_H__F29679CB_5389_4A4A_884D_2171DA377AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

/*! /class IgQtMDIScanShell 
 *
 *  /brief A more complete demo than IgQtMDITestShell
 */

class IG_VIS_API IgQtMDIScanShell : public IgQtMDIBaseShell  
{
	Q_OBJECT
public:
	IgQtMDIScanShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgQtMDIScanShell();
	//! returns the className
	virtual const QString shellName();
public slots:
	//! slot to get a new event. if adorned, create twigWidget & infoWidget. Overrides parent.
	void eventsSlot();
protected:
private:
	//! the name
	static const QString theShellName;
};

#endif // !defined(AFX_IGQTMDISCANSHELL_H__F29679CB_5389_4A4A_884D_2171DA377AB5__INCLUDED_)

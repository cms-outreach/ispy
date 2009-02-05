// IgQtMDITest2DShell.h: interface for the IgQtMDITest2DShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDITEST2DSHELL_H__C5636F83_E3DA_4153_855E_49EA55363A9B__INCLUDED_)
#define AFX_IGQTMDITEST2DSHELL_H__C5636F83_E3DA_4153_855E_49EA55363A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

/*! /class IgQtMDITest2DShell 
 *
 *  /brief A demo for a 2D viewer
 */

class IgQtMDI;

class IgQtMDITest2DShell: public IgQtMDIBaseShell  
{
	Q_OBJECT
public:
	explicit IgQtMDITest2DShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	virtual ~IgQtMDITest2DShell();
	//! returns the className
	virtual const QString shellName();
public slots:
	virtual void buildSlot();
protected:
private:
	//! the name
	static const QString theShellName;
};

#endif // !defined(AFX_IGQTMDITEST2DSHELL_H__C5636F83_E3DA_4153_855E_49EA55363A9B__INCLUDED_)

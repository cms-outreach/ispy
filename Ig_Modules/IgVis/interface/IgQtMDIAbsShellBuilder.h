// IgQtMDIAbsShellBuilder.h: interface for the IgQtMDIAbsShellBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDIABSSHELLBUILDER_H__D75FC4A9_5053_4F5A_B33A_C32E24EC6731__INCLUDED_)
#define AFX_IGQTMDIABSSHELLBUILDER_H__D75FC4A9_5053_4F5A_B33A_C32E24EC6731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "qstring.h"

class IgQtMDIAbsShell;
class SoQtFullViewer;
class QWidget;

class IgQtMDI;


/*!
 * \class IgQtMDIAbsShellBuilder IgQtMDIAbsShellBuilder.h Ig_Modules/IgVis/interface/IgQtMDIAbsShellBuilder.h
 *
 * \brief A singleton class to generate MDI browser shells.
 *
 *  \author G. Alverson, Northeastern University Computing Group
 *  
 */

class IG_VIS_API IgQtMDIAbsShellBuilder  
{
public:
	virtual IgQtMDIAbsShell *getShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	static  IgQtMDIAbsShellBuilder *getInstance();
	virtual const QString shellName() const;
protected:
	IgQtMDIAbsShellBuilder();
	virtual ~IgQtMDIAbsShellBuilder() = 0;
	static IgQtMDIAbsShellBuilder *theBuilder;

};

#endif // !defined(AFX_IGQTMDIABSSHELLBUILDER_H__D75FC4A9_5053_4F5A_B33A_C32E24EC6731__INCLUDED_)

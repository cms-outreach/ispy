// IgQtPhysTabsImpl.h: interface for the IgQtPhysTabsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTPHYSTABSIMPL_H__72327933_CA0F_47D5_A6CE_B3856F66CFE7__INCLUDED_)
#define AFX_IGQTPHYSTABSIMPL_H__72327933_CA0F_47D5_A6CE_B3856F66CFE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/IgQtPhysTabs.h"
#include <qtabdialog.h>

class IG_VIS_API IgQtPhysTabsImpl : public IgQtPhysTabs  
{
	Q_OBJECT
public:
	IgQtPhysTabsImpl(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	virtual ~IgQtPhysTabsImpl();

};

#endif // !defined(AFX_IGQTPHYSTABSIMPL_H__72327933_CA0F_47D5_A6CE_B3856F66CFE7__INCLUDED_)

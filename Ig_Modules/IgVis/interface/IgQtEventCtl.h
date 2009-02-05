#if !defined(AFX_IGQTEVENTCTL_H__D81897F2_64B8_436C_A28D_6B6F66100E1A__INCLUDED_)
#define AFX_IGQTEVENTCTL_H__D81897F2_64B8_436C_A28D_6B6F66100E1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "qpopupmenu.h"
#include <map>

class IgQtMDIAbsShell;

/*!   \class IgQtEventCtl IgQtEventCtl.h  Ig_Modules/IgVis/interface/IgQtEventCtl.h
 *
 *     \brief Generates controls for next event
 * 
 *     \author G. Alverson
 *     \date 8-Sep-2000
 */


class IG_VIS_API IgQtEventCtl : public QPopupMenu  
{
	Q_OBJECT
public:
	//! c-tor
	explicit IgQtEventCtl(IgQtMDIAbsShell *shell);
	//! d-tor
	virtual ~IgQtEventCtl();
signals:
	void autoPrintOn(bool);
	void autoEventsOn(bool);
protected slots:
	virtual void autoPrintSlot();
	virtual void autoEventsSlot();
private:
	int autoPrintId;
	int autoEventsId;
};

#endif // !defined(AFX_IGQTEVENTCTL_H__D81897F2_64B8_436C_A28D_6B6F66100E1A__INCLUDED_)

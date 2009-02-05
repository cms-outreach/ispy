#if !defined(AFX_IGQTMISCCTL_H__297FA1B5_8CDB_4F1B_839C_0E3EC455A78B__INCLUDED_)
#define AFX_IGQTMISCCTL_H__297FA1B5_8CDB_4F1B_839C_0E3EC455A78B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!  \class IgQtMiscCtl IgQtMiscCtl.h IgQtMiscCtl.h
 *
 *   \brief Controller for Miscellaneous Items
 *
 *   \author G. Alverson, Northeastern University
 *   \date 22 November 2000
 *
 *   Mods:
 *
 *   16 Oct. 2001 - Extended to provide background color
 *   and color scale panels.
 *
 *   I. Osborne
 *
 *
 */

#include "Ig_Modules/IgVis/interface/config.h"

#include <qpopupmenu.h>

class SoGroup;
class IgQtSlicerImpl;
class SoQtFullViewer;
class IgQtColorScale;
class IgQtXyzScaleImpl;


class IG_VIS_API IgQtMiscCtl : public QPopupMenu 
{
    Q_OBJECT

public:

    //! c-tor requires a parent Widget and a node to attach a Z slicer
    IgQtMiscCtl(QWidget *parent, SoGroup *topNode, SoQtFullViewer *viewer);
    virtual ~IgQtMiscCtl();

signals:

	void wireframeSignal(bool); //! wireframe setting has changed
	void cutawaySignal(bool); //! cutaway setting has changed
	void optimiseSignal(bool); //! try and optimise the static objects...only works first time

private:

    SoGroup *root;
    IgQtSlicerImpl *zDialog;
    SoQtFullViewer *theBrowser;
    IgQtColorScale *colorScaleDialog;
	IgQtXyzScaleImpl *scaleDialog;
    int infoId; //!< id for info slot menu item
	int wireframeId; //!< id for wireframe menu item
	int cutawayId; //!< id for cutaway menu item

private slots:

    void zSliceSlot(); //!< slot to popup the slicer
	void scaleSlot(); //!< slot to popup the scaler
    void infoCtlSlot(); //!< slot to control info
    void bkgColorSlot(); //!< slot to popup background color dialog box choices, and change the background color
    void colorScaleSlot();//!< slot to popup the color scale dialog to change color of hits according to property
	void wireframeSlot(); //!< slot to set non-volatile twigs to wireframe
	void cutawaySlot(); //!< slot to set non-volatile twigs to cutaway mode
	void optimiseSlot(); //!< slot to set non-volatile twigs to cutaway mode

};

#endif // !defined(AFX_IGQTMISCCTL_H__297FA1B5_8CDB_4F1B_839C_0E3EC455A78B__INCLUDED_)


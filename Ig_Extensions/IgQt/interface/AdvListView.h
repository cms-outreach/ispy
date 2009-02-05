// (c) 1999 by Syntext, Inc.
//
// Authors: Timofey Fouriaev timof@syntext.com
// Please see COPYING file for details.

/*! \file
 *  $Id: AdvListView.h,v 1.6 2003/09/24 10:27:24 lat Exp $
 */

#ifndef ADV_LIST_VIEW_H
#define ADV_LIST_VIEW_H

#include "Ig_Extensions/IgQt/interface/config.h"
#include <qlistview.h>
#include <qpixmap.h>

class AdvListViewItem;

/*! \brief AdvListView provides extra flexibility to QListView.

  Features:
  <ul>
    <li>Ability to set italic/bold font for drawing item in the list.
    <li>Check box column is provided so that items in the list can be checked 
        with mouse click.
    <li>Ability to disable opening branches on double click on
        its text. In this case it is possible to open/close branch using root 
        pictogram at the left side. It is useful if additional action is 
        associated with double click on the item.
  </ul>
  It can display and control a hierarchy of multi-column items, and provides    
  the ability to add new items at run-time, lets the user select one or many 
  items, sort the list in increasing or decreasing order by any column, and 
  so on. 

  AdvListView can also contain QListViewItems.

  <img src="../AdvListView.gif" alt="AdvListView pic" align=center border=0>

  The simplest mode of usage is to create a AdvListView, add some column 
  headers using setColumn(), create one or more QListViewItem or AdvListViewItem
  objects with the AdvListView as parent, set up the list view's geometry(), 
  and show() it. 

  The main setup functions are 
  <UL>
  <LI>addColumn() - adds a column, with text and perhaps width. 
  <LI>setColumnWidthMode() - sets the column to be resized automatically or not. 
  <LI>setMultiSelection() - decides whether one can select one or many objects 
  in this list view. The default is FALSE (selecting one item unselects any other
  selected item). 
  <LI>setAllColumnsShowFocus() - decides whether items should show keyboard focus 
  using all columns, or just column 0. The default is to show focus using
  just column 0. 
  <LI>setRootIsDecorated() - decides whether root items can be opened and closed 
  by the user, and have open/close decoration to their left. The default is FALSE. 
  <LI>setTreeStepSize() - decides the how many pixels an item's children are 
  indented relative to their parent. The default is 20. This is mostly a matter of
  taste. 
  <LI>setSorting() - decides whether the items should be sorted, whether it should 
  be in ascending or descending order, and by what column it should be sorted.

  The other points are described in QListView documentation. 
*/

class IG_QT_API AdvListView : public QListView {
    Q_OBJECT
public:
    //! Creates list view witdh a parent widget
    AdvListView( QWidget * parent = 0, const char *name = 0 );
    //! Reimplemented from QListView
    void    resizeEvent(QResizeEvent* e);
    //! Initiates an inserted item
    void    initItem(QListViewItem* item);
    //! Gets the column used to for checking box
    uint    getCheckPosition()              { return checkAtPosition_; }
    //! Sets the column used to for checking box
    void    setCheckPosition(uint pos )     { checkAtPosition_ = pos; }
    //! Set the item to be checked
    void    setChecked(AdvListViewItem* item, bool is_checked);
protected:
    //! Initiates the AdvListView
    void    init();
    //! Reimplemented from QListView
    void    contentsMouseDoubleClickEvent(QMouseEvent* e);
    //! Reimplemented from QListView
    void    contentsMousePressEvent(QMouseEvent* e);
    //! Reimplemented from QListView
    void    contentsMouseReleaseEvent(QMouseEvent* e);
    //! Returns true if mouse is inside checker rectangle
    bool    isInsideChecker(int x);
private:    
    uint    checkAtPosition_;
    int     oldWidth_;
    bool    disableMouseRelease_;
	QPixmap checkedPixmap_;
	QPixmap uncheckedPixmap_;
};

#endif // ADV_LIST_VIEW_H

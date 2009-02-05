// (c) 1999 by Syntext, Inc.
//
// Authors: Timofey Fouriaev timof@syntext.com
// Please see COPYING file for details.

/*! \file
 *  $Id: AdvListView.cc,v 1.4 2001/02/09 14:52:58 yana Exp $
 */

#include <qpixmap.h>
#include <qheader.h>
#include "Ig_Extensions/IgQt/interface/AdvListView.h"
#include "Ig_Extensions/IgQt/interface/AdvListViewItem.h"

static const char* unchecked_[] = {
    "    16    13        2            1",
    "  c none",
    "x c #000000",
    "x x x x x x x   ",
    "                ",
    "x           x   ",
    "                ",
    "x           x   ",
    "                ",
    "x           x   ",
    "                ",
    "x           x   ",
    "                ",
    "x           x   ",
    "                ",
    "x x x x x x x   "
};
static const char* checked_[] = {
    "    16    13        2            1",
    "  c none",
    "x c #000000",
    "x x x x x x x   ",
    "               x",
    "x            xx ",
    "            x   ",
    "x xx      xx    ",
    " xxxx    xx     ",
    "x xxxx  xx  x   ",
    "   xxxxxx       ",
    "x   xxxxx   x   ",
    "     xxx        ",
    "x     xx    x   ",
    "                ",
    "x x x x x x x   "
};

AdvListView::AdvListView(QWidget * parent, const char *name)
    : QListView(parent, name), checkAtPosition_(1), 
    oldWidth_(-1), disableMouseRelease_(false), checkedPixmap_(QPixmap(checked_)), uncheckedPixmap_(QPixmap(unchecked_))
{
    header()->setResizeEnabled(false);
    header()->setMovingEnabled(false);
}

void AdvListView::init()
{
    QListViewItem* child = firstChild();
    while (child) {
        initItem(child);
        child = child->nextSibling();
    }
}

void AdvListView::resizeEvent(QResizeEvent* e)
{
    QListView::resizeEvent(e);

    int h_width = 0;
    int c = 0;
    while (columnWidth(c)) {
        h_width +=columnWidth(c);
        c++;
    }
    int columns = c;
    if (!h_width)
        return;
    
    float coeff = 1.0*viewport()->width()/h_width;
    int new_width = 0;
    for (c = 0; c<columns-1; c++) {
        int w = int(coeff*columnWidth(c));
        setColumnWidth(c, w);
        new_width += w;
    }
    setColumnWidth(columns-1, viewport()->width()-new_width);
    viewport()->repaint();
}

void AdvListView::initItem(QListViewItem* item)
{
    // Check the item itself
    AdvListViewItem* a_item = static_cast<AdvListViewItem*>(item);
    if (a_item && a_item->isCheckable()) 
        setChecked(a_item, a_item->isChecked());
    // Check all its children
    QListViewItem* child = item->firstChild();
    while (child) {
        initItem(child);
        child = child->nextSibling();
    }
}

void AdvListView::setChecked(AdvListViewItem* item, bool is_checked)
{
    if (item && item->isCheckable()) {
        if (item->isChecked() != is_checked)
            item->setChecked(is_checked);
        if (is_checked) 
            item->setPixmap(checkAtPosition_, checkedPixmap_);
        else 
            item->setPixmap(checkAtPosition_, uncheckedPixmap_);
    }    
}

void AdvListView::contentsMouseDoubleClickEvent(QMouseEvent * e)
{
    QPoint vp = contentsToViewport(e->pos());

    AdvListViewItem*  item  = static_cast<AdvListViewItem*>(itemAt(vp));
    if (!item || !item->isMagic())
        QListView::contentsMouseDoubleClickEvent(e);
    
    QListViewItem* qitem = itemAt(vp);
    if (qitem)
        emit doubleClicked( qitem );
}

void AdvListView::contentsMousePressEvent(QMouseEvent * e)
{
    QPoint vp = contentsToViewport(e->pos());
    AdvListViewItem*  item  = static_cast<AdvListViewItem*>(itemAt( vp ));
    if (item && item->isCheckable() && isInsideChecker(e->x())) {
        if (item->isChecked()) 
            setChecked(item, false);
        else 
            setChecked(item, true);
        disableMouseRelease_ = true;
    }    
    else {
        disableMouseRelease_ = false;
        QListView::contentsMousePressEvent(e);
    }
}

void AdvListView::contentsMouseReleaseEvent(QMouseEvent * e)
{
    if (disableMouseRelease_)
        disableMouseRelease_ = false;
    else
        QListView::contentsMouseReleaseEvent(e);
}

bool AdvListView::isInsideChecker(int x)
{
    for (uint c = 0; c<checkAtPosition_; c++)
        x -= columnWidth(c);
    return  (x >= 0 && x <= columnWidth(checkAtPosition_));
}


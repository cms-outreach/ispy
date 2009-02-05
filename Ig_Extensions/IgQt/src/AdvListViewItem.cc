// (c) 1999 by Syntext, Inc.
//
// Authors: Timofey Fouriaev timof@syntext.com
// Please see COPYING file for details.

/*! \file
 *  $Id: AdvListViewItem.cc,v 1.2 2004/12/01 12:39:19 muzaffar Exp $
 */

#include "Ig_Extensions/IgQt/interface/AdvListView.h"
#include "Ig_Extensions/IgQt/interface/AdvListViewItem.h"
#include <qpainter.h>

AdvListViewItem::AdvListViewItem( QListView * parent )
    :QListViewItem(parent)
{
    init();    
}

/*!  Creates a new list view item which is a child of \a parent and first
  in the parent's list of children. */

AdvListViewItem::AdvListViewItem( QListViewItem * parent )
    :QListViewItem(parent)
{
    init();    
}

/*!  Constructs an empty list view item which is a child of \a parent
  and is after \a after in the parent's list of children */

AdvListViewItem::AdvListViewItem( QListView * parent, QListViewItem * after )
    :QListViewItem(parent, after)
{
    init();    
}

/*!  Constructs an empty list view item which is a child of \a parent
  and is after \a after in the parent's list of children */

AdvListViewItem::AdvListViewItem( QListViewItem * parent, QListViewItem * after )
    :QListViewItem(parent, after)
{
    init();    
}

/*!  Creates a new list view item in the QListView \a parent,
  parent, with at most 8 constant strings as contents.
  (void)new QListViewItem( lv, "/", "Root directory" );
  \sa setText()
*/

AdvListViewItem::AdvListViewItem( QListView * parent,
                              QString label1,
                              QString label2,
                              QString label3,
                              QString label4,
                              QString label5,
                              QString label6,
                              QString label7,
                              QString label8 )
    :QListViewItem(parent, label1, label2, label3, label4, 
                   label5, label6, label7, label8)
{
    init();    
}

/*!  Creates a new list view item that's a child of the QListViewItem
  parent, with at most 8 constant strings as contents.  Possible
  example in a threaded news or e-mail reader:

  (void)new QListViewItem( parentMessage, author, subject );
  
  \sa setText()
*/

AdvListViewItem::AdvListViewItem( QListViewItem * parent,
                              QString label1,
                              QString label2,
                              QString label3,
                              QString label4,
                              QString label5,
                              QString label6,
                              QString label7,
                              QString label8 )
    :QListViewItem(parent, label1, label2, label3, label4,
                   label5, label6, label7, label8)
{
    init();    
}

/*!  Creates a new list view item in the QListView \a parent,
  after item \a after, with at most 8 constant strings as contents.

  Note that the order is changed according to QListViewItem::key()
  unless the list view's sorting is disabled using
  QListView::setSorting( -1 ).

  \sa setText()
*/

AdvListViewItem::AdvListViewItem( QListView * parent, QListViewItem * after,
                              QString label1,
                              QString label2,
                              QString label3,
                              QString label4,
                              QString label5,
                              QString label6,
                              QString label7,
                              QString label8 )
    :QListViewItem(parent, after, label1, label2, label3, label4, 
                   label5, label6, label7, label8)
{
    init();    
}


/*!  Creates a new list view item that's a child of the QListViewItem
  \a parent, after item \a after, with at most 8 constant strings as
  contents.

  Note that the order is changed according to QListViewItem::key()
  unless the list view's sorting is disabled using
  QListView::setSorting( -1 ).

  \sa setText()
*/

AdvListViewItem::AdvListViewItem( QListViewItem * parent, QListViewItem * after,
                              QString label1,
                              QString label2,
                              QString label3,
                              QString label4,
                              QString label5,
                              QString label6,
                              QString label7,
                              QString label8 )
    :QListViewItem(parent, after, label1, label2, label3, label4,
                   label5, label6, label7, label8)
{
    init();    
}

void AdvListViewItem::init()
{
    isBold_ = isItalic_ = isMagic_ = isCheckable_ = false;
}

void AdvListViewItem::paintCell( QPainter * p, const QColorGroup & cg,
                               int column, int width, int align )
{
    QFont old_font( p->font());
    
    QFont new_font( p->font());
    new_font.setBold(isBold_);
    new_font.setItalic(isItalic_);   
    
    p->setFont(new_font);
    QListViewItem::paintCell(p, cg, column, width, align);
    p->setFont(old_font);
}

int AdvListViewItem::width( const QFontMetrics&,
                          const QListView* lv, int c ) const
{  
    QFont new_font(listView()->font());
    new_font.setBold(isBold_);
    new_font.setItalic(isItalic_);   
    QFontMetrics fm_new(new_font);
  
    return QListViewItem::width(fm_new, lv, c);
}

void AdvListViewItem::setChecked(bool is_checked)  
{ 
    isChecked_ = is_checked; 
    AdvListView* list = static_cast<AdvListView*>(listView());
    if (list) 
        list->setChecked(this, is_checked);
}

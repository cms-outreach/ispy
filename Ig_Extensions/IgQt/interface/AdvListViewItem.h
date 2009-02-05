// (c) 1999 by Syntext, Inc.
//
// Authors: Timofey Fouriaev timof@syntext.com
// Please see COPYING file for details.

/*! \file
 *  $Id: AdvListViewItem.h,v 1.5 2003/09/24 10:27:24 lat Exp $
 */

#ifndef ADV_LIST_VIEW_ITEM_H
#define ADV_LIST_VIEW_ITEM_H

#include "Ig_Extensions/IgQt/interface/config.h"
#include <qlistview.h>

/*! \brief The AdvListViewItem class implements advanced list view item to 
    be used with AdvListView. 

    It is is a multi-column object capable of displaying itself. Its design 
    has the following main goals: 

    <OL>
    <LI>Work like QListViewItem.
    <LI>Privide an extra drawing capability.
    <LI>Allow item checking.
    <LI>Easy application in simple case.
    </OL>

    <img src="../AdvListView.gif" alt="AdvListView pic" align=center border=0>

    The simplest way to use AdvListViewItem is to construct one with a few 
    constant strings. This creates an item which is a child of parent with 
    two fixed-content strings, and discards the pointer to it: 

    (void) new AdvListViewItem( parent, "first column", "second column" );

    This object will be deleted when parent is deleted, as for QObjects. 

    The parent is either another AdvListViewItem or AdvListView. If the 
    parent is an AdvListView, this item is a top-level item within that 
    AdvListView. If the parent is another AdvListViewItem, this item becomes 
    a child of the parent item. 

    If you keep the pointer, you can set or change the texts using setText(), 
    add pixmaps using setPixmap(), change its mode using setSelectable(), 
    setSelected(), setOpen() and setExpandable(), change its height using 
    setHeight(), and do much tree traversal. The set* functions in AdvListView 
    also affect AdvListViewItem, of course. 

    Other functions derived from QListViewItem are described in QListViewItem
    documentation.
 */

class IG_QT_API AdvListViewItem : public QListViewItem {
public:
    //! Creates the top level item in the list "parent"
    AdvListViewItem( QListView * parent );
    //! Creates the branch item with the parent "parent"
    AdvListViewItem( QListViewItem * parent );
    //! Creates the branch item and places it after "after" item
    AdvListViewItem( QListView * parent, QListViewItem * after );
    AdvListViewItem( QListViewItem * parent, QListViewItem * after );
    AdvListViewItem( QListView * parent,
                   QString,     QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null );
    AdvListViewItem( QListViewItem * parent,
                   QString,     QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null );

    AdvListViewItem( QListView * parent, QListViewItem * after,
                   QString,     QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null );
    AdvListViewItem( QListViewItem * parent, QListViewItem * after,
                   QString,     QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null,
                   QString = QString::null, QString = QString::null );
    //! Reimplemented from QListViewItem
    virtual void paintCell( QPainter *, const QColorGroup & cg,
                            int column, int width, int alignment );
    //! Returns the text width of the item in given column
    virtual int  width( const QFontMetrics& fm,
                          const QListView* lv, int c ) const;
    //! Returns true if this item is bold faced
    bool    isBold()   { return isBold_;   }
    //! Returns true if this item is italic
    bool    isItalic() { return isItalic_; }
    //! Returns true if this item can`t be opened using mouse dblclick
    //! on the item`s text.
    bool    isMagic()  { return isMagic_;  }
    //! Returns true if this item can be checked
    bool    isCheckable()   {return isCheckable_; }
    //! Returns true if this item is checked
    bool    isChecked()     {return isChecked_; }
public:
    //! Sets the item`s font to be bold faced
    void    setBold(bool is_bold = true)        { isBold_   = is_bold;   }
    //! Sets the item`s font to be italic
    void    setItalic(bool is_italic = true)    { isItalic_ = is_italic; }
    //! Disables item opening using mouse dblclick
    void    setMagic(bool is_magic = true)      { isMagic_  = is_magic;  }
    //! Sets item to be checkable
    void    setCheckable(bool is_checkable = true)  { isCheckable_ = is_checkable; }
    //! Sets item to be checked
    void    setChecked(bool is_checked = true);
private:
    //! Initiates the item
    void    init();
private:
    bool    isBold_;
    bool    isItalic_;
    bool    isMagic_;
    bool    isCheckable_;
    bool    isChecked_;
};

#endif // ADV_LIST_VIEW_ITEM_H

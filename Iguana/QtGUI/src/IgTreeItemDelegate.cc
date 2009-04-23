//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgTreeItemDelegate.h"
#include <QtGui>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTreeItemDelegate::IgTreeItemDelegate (QObject *parent)
    : QItemDelegate (parent)
{
}

void 
IgTreeItemDelegate::paint (QPainter *painter,
			   const QStyleOptionViewItem &option,
			   const QModelIndex &index) const
{
    if (index.column () == 3) 
    {
	QVariant value = index.model ()->data (index, Qt::UserRole);
	//if (!isSupportedType(value.type ())) 
	{
	    QStyleOptionViewItem myOption = option;
	    myOption.state &= ~QStyle::State_Enabled;
	    QItemDelegate::paint (painter, myOption, index);
	    return;
	}
    }

    QItemDelegate::paint (painter, option, index);
}

QWidget *
IgTreeItemDelegate::createEditor (QWidget *parent, const QStyleOptionViewItem &option,
				  const QModelIndex &index) const
{}

void 
IgTreeItemDelegate::setEditorData (QWidget *editor, const QModelIndex &index) const
{}

void 
IgTreeItemDelegate::setModelData (QWidget *editor, QAbstractItemModel *model,
				  const QModelIndex &index) const
{}


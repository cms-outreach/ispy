#ifndef VIEW_IG_TREE_ITEM_DELEGATE_H
# define VIEW_IG_TREE_ITEM_DELEGATE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QItemDelegate>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgTreeItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    IgTreeItemDelegate (QObject *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void paint (QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option,
			   const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model,
		       const QModelIndex &index) const;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_TREE_ITEM_DELEGATE_H

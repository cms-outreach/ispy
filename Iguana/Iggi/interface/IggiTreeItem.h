#ifndef IGGI_IGGI_TREE_ITEM_H
# define IGGI_IGGI_TREE_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QList>
# include <QVariant>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiTreeItem
{
public:
    IggiTreeItem (const QList<QVariant> &data, IggiTreeItem *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IggiTreeItem (void);

    void 		appendChild (IggiTreeItem *child);
    IggiTreeItem *	child (int row);
    int 		childCount (void) const;
    int 		columnCount (void) const;
    QVariant 		data (int column) const;
    int 		row (void) const;
    IggiTreeItem *     	parent (void) const;
    void                destroy (void);
    void		clear (void);

private:
    QList<IggiTreeItem *> 	m_childItems;
    QList<QVariant> 		m_itemData;
    IggiTreeItem       	       *m_parentItem;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_TREE_ITEM_H

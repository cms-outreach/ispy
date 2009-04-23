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

class ISpyTreeItem
{
public:
    ISpyTreeItem (const QList<QVariant> &data, ISpyTreeItem *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~ISpyTreeItem (void);

    void 		appendChild (ISpyTreeItem *child);
    ISpyTreeItem *	child (int row);
    int 		childCount (void) const;
    int 		columnCount (void) const;
    QVariant 		data (int column) const;
    int 		row (void) const;
    ISpyTreeItem *     	parent (void) const;
    void                destroy (void);
    void		clear (void);

private:
    QList<ISpyTreeItem *> 	m_childItems;
    QList<QVariant> 		m_itemData;
    ISpyTreeItem              *m_parentItem;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_TREE_ITEM_H

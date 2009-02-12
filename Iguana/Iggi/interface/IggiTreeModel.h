#ifndef IGGI_IGGI_TREE_MODEL_H
# define IGGI_IGGI_TREE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QAbstractItemModel>
# include <QModelIndex>
# include <QVariant>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IggiTreeItem;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiTreeModel : public QAbstractItemModel
{    
    Q_OBJECT

public:
    IggiTreeModel (const QString &data, QObject *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IggiTreeModel (void);

    QVariant 		data (const QModelIndex &index, int role) const;
    Qt::ItemFlags 	flags (const QModelIndex &index) const;
    QVariant 		headerData (int section, Qt::Orientation orientation,
				    int role = Qt::DisplayRole) const;
    QModelIndex 	index (int row, int column,
			       const QModelIndex &parent = QModelIndex ()) const;
    QModelIndex 	parent (const QModelIndex &index) const;
    int 		rowCount (const QModelIndex &parent = QModelIndex ()) const;
    int 		columnCount (const QModelIndex &parent = QModelIndex ()) const;
    bool 		setData (const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole);
    IggiTreeItem *	rootItem (void);
    bool 		insertRows (int position, int rows, const QModelIndex &index = QModelIndex ());
    bool 		removeRows (int position, int rows, const QModelIndex &index = QModelIndex ());
    void 		clear (void);

private:
    void 		setupModelData (const QStringList &lines, IggiTreeItem *parent);
    IggiTreeItem       *m_rootItem;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_TREE_MODEL_H

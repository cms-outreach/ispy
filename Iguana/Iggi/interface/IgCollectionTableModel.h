#ifndef IGGI_IG_COLLECTION_TABLE_MODEL_H
# define IGGI_IG_COLLECTION_TABLE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QAbstractTableModel>
# include <QModelIndex>
# include <QVariant>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgCollection;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgCollectionTableModel : public QAbstractTableModel
{    
    Q_OBJECT
public:
    IgCollectionTableModel(IgCollection *collection, QObject *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgCollectionTableModel(void);

    QVariant 		data(const QModelIndex &index, int role) const;
    QVariant 		headerData(int section, Qt::Orientation orientation,
				   int role = Qt::DisplayRole) const;
    int 		rowCount(const QModelIndex &parent = QModelIndex ()) const;
    int 		columnCount(const QModelIndex &parent = QModelIndex ()) const;
    void                setCollection(IgCollection *collection);
private:
    IgCollection *m_collection;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_COLLECTION_TABLE_MODEL_H

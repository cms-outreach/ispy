#ifndef IGGI_IG_MULTI_STORAGE_TREE_MODEL_H
# define IGGI_IG_MULTI_STORAGE_TREE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QAbstractTableModel>
# include <QModelIndex>
# include <QVariant>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDataStorageTableModel;
class IgDataStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgMultiStorageTreeModel: public QAbstractItemModel
{
  Q_OBJECT
public:
  IgMultiStorageTreeModel(QObject *parent = 0);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor
  ~IgMultiStorageTreeModel(void);

  void          addStorage(IgDataStorage *storage, const char *label);
  void                clear(void);

  QVariant              data(const QModelIndex &index, int role) const;
  QVariant              headerData(int section, Qt::Orientation orientation,
                                   int role = Qt::DisplayRole) const;
  int           rowCount(const QModelIndex &parent = QModelIndex()) const;
  int           columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex   index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
  QModelIndex   parent(const QModelIndex & index ) const;

private:
  std::vector<IgDataStorageTableModel *> m_storages;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_MULTI_STORAGE_TREE_MODEL_H

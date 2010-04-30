#ifndef IGGI_IG_DATA_STORAGE_TABLE_MODEL_H
# define IGGI_IG_DATA_STORAGE_TABLE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QAbstractTableModel>
# include <QModelIndex>
# include <QVariant>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDataStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgDataStorageTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  IgDataStorageTableModel(IgDataStorage *collection, const char *label,
                          QObject *parent = 0);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor
  ~IgDataStorageTableModel(void);

  const char *  label(void);
  QVariant      data(const QModelIndex &index, int role) const;
  QVariant      headerData(int section, Qt::Orientation orientation,
                           int role = Qt::DisplayRole) const;
  int           rowCount(const QModelIndex &parent = QModelIndex()) const;
  int           columnCount(const QModelIndex &parent = QModelIndex()) const;
private:
  IgDataStorage    *m_storage;
  std::string       m_label;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_COLLECTION_TABLE_MODEL_H

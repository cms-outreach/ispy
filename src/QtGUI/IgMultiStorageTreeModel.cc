//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/IgMultiStorageTreeModel.h"
#include "QtGUI/IgDataStorageTableModel.h"
#include "Framework/IgCollection.h"
#include <QtGui>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

struct UniqueId
{
public:
  UniqueId(void)
    : DATA_STORAGE_ID(-1),
      COLLECTION_ID(-1),
      m_isRoot(true)
    {}

  UniqueId(int id)
    : DATA_STORAGE_ID((id-1) & DATA_STORAGE_MASK),
      COLLECTION_ID(((id-1) >> DATA_STORAGE_ID_SIZE) & COLLECTION_ID_MASK),
      m_isRoot(id ? false : true)
    {}

  UniqueId(int dataStorage, int collection)
    : DATA_STORAGE_ID(dataStorage),
      COLLECTION_ID(collection),
      m_isRoot(false)
    {
      if (dataStorage == -1 && collection == -1)
      {
        m_isRoot = true;
      }
    }

  int toInt()
    {
      if (m_isRoot) return 0;

      int res = DATA_STORAGE_ID;
      res |= COLLECTION_ID << DATA_STORAGE_ID_SIZE;
      return res + 1;
    }

  bool isRoot(void)
    {
      return m_isRoot;
    }

  bool isDataStorage(void)
    {
      return DATA_STORAGE_ID >= 0 && COLLECTION_ID == -1;
    }

  bool isCollection(void)
    {
      return DATA_STORAGE_ID >= 0 && COLLECTION_ID >= 0;
    }

  UniqueId parent(void)
    {
      UniqueId result;
      // If it's a collection its parent is the datastorage.
      if (isCollection())
      {
        UniqueId id(DATA_STORAGE_ID, -1);
        return id;
      }
      // If it's a datastorage, its parent is the toplevel one
      if (isDataStorage())
      {
        return UniqueId();
      }

      return UniqueId();
    }

  int dataStorage()
    {
      return DATA_STORAGE_ID;
    }

  int collection()
    {
      return COLLECTION_ID;
    }
private:
  int           DATA_STORAGE_ID : 8;
  int           COLLECTION_ID : 24;
  bool          m_isRoot;
  const static int      DATA_STORAGE_ID_SIZE = 8;
  const static int      COLLECTION_ID_SIZE = 24;
  const static int      DATA_STORAGE_MASK = (1 << DATA_STORAGE_ID_SIZE) - 1;
  const static int      COLLECTION_ID_MASK = (1 << COLLECTION_ID_SIZE) - 1;
};

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMultiStorageTreeModel::IgMultiStorageTreeModel(QObject *parent)
  : QAbstractItemModel(parent)
{}

IgMultiStorageTreeModel::~IgMultiStorageTreeModel(void)
{}

QVariant
IgMultiStorageTreeModel::data(const QModelIndex &index, int role) const
{
  UniqueId id(index.internalId());

  if (role != Qt::DisplayRole)
    return QVariant();
  if (id.isDataStorage())
  {
    return QString(m_storages [id.dataStorage()]->label());
  }
  if (id.isCollection())
  {
    IgDataStorageTableModel *model = m_storages[id.dataStorage()];
    return model->data(model->index(id.collection(), 0), role);
  }
  return QVariant();
}

QVariant
IgMultiStorageTreeModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (section)
  {
  case 0:
    return QString("Object");
    break;
  case 1:
    return QString("Visibility");
    break;
  }
  return QVariant();
}

int
IgMultiStorageTreeModel::rowCount(const QModelIndex &parent) const
{
  UniqueId pid(parent.internalId());

  if (pid.isRoot())
  {
    return m_storages.size();
  }

  if (pid.isDataStorage())
  {
    return m_storages [parent.row()]->rowCount();
  }

  if (pid.isCollection())
  {
    return 0;
  }

  return m_storages.size();
}

int
IgMultiStorageTreeModel::columnCount(const QModelIndex &parent) const
{
  UniqueId pid(parent.internalId());

  if (! parent.isValid())
  {
    return 1;
  }
  if (pid.isDataStorage())
  {
    return 1;
  }
  if (pid.isCollection())
  {
    return 2;
  }
  return 0;
}

void
IgMultiStorageTreeModel::addStorage(IgDataStorage *storage, const char * label)
{
  IgDataStorageTableModel *model = new IgDataStorageTableModel(storage, label);
  m_storages.push_back(model);
  emit reset();
}

void
IgMultiStorageTreeModel::clear(void)
{
  beginRemoveRows(QModelIndex(), 0, m_storages.size());
  m_storages.clear();
  endRemoveRows();
  emit reset();
}

QModelIndex
IgMultiStorageTreeModel::index(int row, int column, const QModelIndex & parent) const
{
  UniqueId pid(parent.internalId());

  // Check if this is the first child.
  // If it is, return the IgDataStorage label(only for column 0);
  if (pid.isRoot())
  {
    UniqueId id(row, -1);
    return createIndex(row, column, id.toInt());
  }
  if (pid.isDataStorage())
  {
    UniqueId id(pid.dataStorage(), row);
    return createIndex(row, column, id.toInt());
  }
  return createIndex(row, column, 0);
}

QModelIndex
IgMultiStorageTreeModel::parent(const QModelIndex & index) const
{
  UniqueId id(index.internalId());

  if (id.isDataStorage())
  {
    return QModelIndex();
  }
  if (id.isCollection())
  {
    return createIndex(id.dataStorage(), 0, id.parent().toInt());
  }
  return QModelIndex();
}

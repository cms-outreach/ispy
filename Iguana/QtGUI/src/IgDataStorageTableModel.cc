//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgDataStorageTableModel.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <QtGui>
#include "classlib/utils/DebugAids.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDataStorageTableModel::IgDataStorageTableModel(IgDataStorage *storage, const char *label, QObject *parent)
  : QAbstractTableModel(parent),
    m_storage(storage),
    m_label(label)
{
}

IgDataStorageTableModel::~IgDataStorageTableModel(void)
{
}

const char *
IgDataStorageTableModel::label(void)
{
  return m_label.c_str();
}

QVariant
IgDataStorageTableModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid() && role == Qt::DisplayRole)
  {
    if (index.column() == 0)
      return  QString(m_storage->collectionNames()[index.row()].c_str());
    else if (index.column() == 1)
      return m_storage->getCollectionByIndex(index.row()).size();
  }
  return QVariant();
}

QVariant
IgDataStorageTableModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (section)
  {
  case 0:
    return QString("Collection name");
    break;
  case 1:
    return QString("Size");
    break;
  default:
    return QVariant();
  }
}

int
IgDataStorageTableModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  ASSERT(m_storage);
  return m_storage->collectionNames().size();
}

int
IgDataStorageTableModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 2;
}

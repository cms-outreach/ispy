//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/IgCollectionTableModel.h"
#include "QtGUI/IgCollectionTableModel.h"
#include "Framework/IgCollection.h"
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

IgCollectionTableModel::IgCollectionTableModel(IgCollection *collection, QObject *parent)
  : QAbstractTableModel(parent),
    m_collection(collection)
{
}

IgCollectionTableModel::~IgCollectionTableModel(void)
{
}

void
IgCollectionTableModel::setCollection(IgCollection *collection)
{
  m_collection = collection;
  emit reset();
}

QVariant
IgCollectionTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  if (role != Qt::DisplayRole)
    return QVariant();
  if (!m_collection)
    return QVariant();

  if ((index.column() - 1) < 0 && role == Qt::DisplayRole)
    return index.row();

  IgProperty p = m_collection->properties()[index.column() - 1];

  IgV2d p2;
  IgV3d p3;
  IgV4d p4;
  QStringList l;

  switch (p.handle().type())
  {
  case INT_COLUMN:
    return p.handle().get<int>(index.row());
    break;
  case DOUBLE_COLUMN:
    return p.handle().get<double>(index.row());
    break;
  case STRING_COLUMN:
    return QString(p.handle().get<std::string>(index.row()).c_str());
    break;
  case VECTOR_2D:
    p2 = p.handle().get<IgV2d>(index.row());
    l.append(QString::number(p2.x()));
    l.append(QString::number(p2.y()));
    return l.join(", ");
    break;
  case VECTOR_3D:
    p3 = p.handle().get<IgV3d>(index.row());
    l.append(QString::number(p3.x()));
    l.append(QString::number(p3.y()));
    l.append(QString::number(p3.z()));
    return l.join(", ");
    break;
  case VECTOR_4D:
    p4 = p.handle().get<IgV4d>(index.row());
    l.append(QString::number(p4.x()));
    l.append(QString::number(p4.y()));
    l.append(QString::number(p4.z()));
    l.append(QString::number(p4.w()));
    return l.join(", ");
    break;
  default:
    assert(false);
  }
  return QVariant();
}

QVariant
IgCollectionTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
  if (!m_collection)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Vertical)
    return QVariant(section);

  if (section == 0)
    return "id";

  if (section < 0)
    return QVariant();

  if ((unsigned int) (section - 1) >= m_collection->columnLabels().size())
    return QVariant();

  return QString(m_collection->columnLabels()[section - 1].label.c_str());
}

int
IgCollectionTableModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  if (! m_collection)
    return 0;
  return m_collection->size();
}

int
IgCollectionTableModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  if (! m_collection)
    return 0;

  return m_collection->properties().size() + 1;
}

void
IgCollectionTableModel::clear(void)
{
  beginRemoveRows(QModelIndex(), 0, rowCount());
  m_collection = 0;
  endRemoveRows();
  reset();
}

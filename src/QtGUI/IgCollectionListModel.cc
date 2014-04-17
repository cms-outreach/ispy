#include "QtGUI/IgCollectionListModel.h"
#include "Framework/IgCollection.h"
#include <QtGui>
#include <cassert>
#include <iostream>

IgCollectionListModel::IgCollectionListModel (IgCollection *collection, QObject *parent)
  : QAbstractListModel(parent),
    m_collection(collection)
{}

QVariant
IgCollectionListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  if (role != Qt::DisplayRole)
    return QVariant();
  if (!m_collection)
    return QVariant();
  IgProperty p = m_collection->properties()[index.column()];

  IgV2d p2;
  IgV3d p3;
  IgV4d p4;
  QStringList l;

  switch (p.handle().type())
  {
  case INT_COLUMN:
    return p.handle().get<int>(index.row());
    break;
  case LONG_COLUMN:
    return p.handle().get<qlonglong>(index.row());   
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
IgCollectionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role != Qt::DisplayRole)
    return QVariant();

  if(orientation == Qt::Horizontal)
  {
    switch(section)
    {
    case 0:
      return tr("Name");
    case 1:
      return tr("Property");      
    default:
      return QVariant();
    }
  }
  return QVariant();
}

int
IgCollectionListModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  if (! m_collection)
    return 0;
  return m_collection->size();
}

void
IgCollectionListModel::setCollection(IgCollection *collection)
{
  assert(collection);
  m_collection = collection;
  //std::cout << " with collection name " << m_collection->name() << std::endl;
  //emit reset();
  emit endResetModel();
}

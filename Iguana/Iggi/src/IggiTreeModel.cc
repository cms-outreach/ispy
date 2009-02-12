//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IggiTreeModel.h"
#include "Iguana/Iggi/interface/IggiTreeItem.h"
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

IggiTreeModel::IggiTreeModel (const QString &data, QObject *parent)
     : QAbstractItemModel (parent)
{
    QList<QVariant> rootData;
    rootData << "Object" << "Visibility";
    m_rootItem = new IggiTreeItem (rootData);
    setupModelData (data.split (QString ("\n")), m_rootItem);
}

IggiTreeModel::~IggiTreeModel (void)
{
    delete m_rootItem;
}

int 
IggiTreeModel::columnCount (const QModelIndex &parent) const
{
    if (parent.isValid ())
	return static_cast<IggiTreeItem*> (parent.internalPointer ())->columnCount ();
    else
	return m_rootItem->columnCount ();
}

QVariant 
IggiTreeModel::data (const QModelIndex &index, int role) const
{
    if (!index.isValid ())
        return QVariant ();
    if (role != Qt::DisplayRole)
        return QVariant ();
    IggiTreeItem *item = static_cast<IggiTreeItem *> (index.internalPointer ());

    return item->data (index.column ());
}

Qt::ItemFlags 
IggiTreeModel::flags (const QModelIndex &index) const
{
    if (!index.isValid())
	return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant 
IggiTreeModel::headerData (int section, Qt::Orientation orientation,
			   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	return m_rootItem->data (section);

    return QVariant ();
}

QModelIndex 
IggiTreeModel::index (int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex (row, column, parent))
	return QModelIndex ();

    IggiTreeItem *parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
	parentItem = static_cast<IggiTreeItem *>(parent.internalPointer ());
    IggiTreeItem *childItem = parentItem->child (row);
    if (childItem)
	return createIndex (row, column, childItem);
    else
	return QModelIndex ();
}

QModelIndex 
IggiTreeModel::parent (const QModelIndex &index) const
{
    if (!index.isValid ())
	return QModelIndex ();

    IggiTreeItem *childItem = static_cast<IggiTreeItem *> (index.internalPointer ());
    IggiTreeItem *parentItem = childItem->parent ();

    if (parentItem == m_rootItem)
        return QModelIndex ();
    return createIndex (parentItem->row (), 0, parentItem);
}

int 
IggiTreeModel::rowCount (const QModelIndex &parent) const
{
    IggiTreeItem *parentItem;
    if (parent.column () > 0)
	return 0;

    if (!parent.isValid ())
	parentItem = m_rootItem;
    else
	parentItem = static_cast<IggiTreeItem *> (parent.internalPointer ());

    return parentItem->childCount ();
}

void 
IggiTreeModel::setupModelData (const QStringList &lines, IggiTreeItem *parent)
{
    QList<IggiTreeItem *> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;
    while (number < lines.count ()) 
    {
        int position = 0;
	while (position < lines[number].length ()) 
	{
	    if (lines[number].mid(position, 1) != " ")
		break;
            position++;
	}

	QString lineData = lines [number].mid (position).trimmed ();
	std::cout << lineData.toStdString () << std::endl;	
	
	if (!lineData.isEmpty ()) {
	    // Read the column data from the rest of the line.
	    QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
	    for (int column = 0; column < columnStrings.count (); ++column)
		columnData << columnStrings[column];
	    if (position > indentations.last()) {
		// The last child of the current parent is now the new parent
                // unless the current parent has no children.

		if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
		    indentations << position;
		}
            } else {
		while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
	    }
	    // Append a new item to the current parent's list of children.
            parents.last()->appendChild (new IggiTreeItem (columnData, parents.last ()));
	}

	number++;
    }
}

IggiTreeItem *
IggiTreeModel::rootItem (void) 
{
    return m_rootItem;
}

bool 
IggiTreeModel::setData (const QModelIndex &index,
			const QVariant &value, int role)
{
//      if (index.isValid() && role == Qt::EditRole) 
//      {
	// stringList.replace (index.row (), value.toString ());
	QList <QVariant> name;
	name << value;
	//	std:: cout << "Setting data " << name.first ().toString ().toStdString () << std::endl;
// 	IggiTreeItem *coll = new IggiTreeItem (name, rootItem ());
// 	rootItem ()->appendChild (coll);
	QModelIndex parentIndex = parent (index);
	
	IggiTreeItem *parentItem = static_cast<IggiTreeItem *> (parentIndex.internalPointer ());
	if (! parentItem) 
	    parentItem = rootItem ();
	
	IggiTreeItem *coll = new IggiTreeItem (name, parentItem);
	parentItem->appendChild (coll);
	
	//emit dataChanged (index, index);
	reset ();
        return true;
	//     }
     return false;
}

bool
IggiTreeModel::insertRows (int position, int rows, const QModelIndex &index)
{
    beginInsertRows (QModelIndex (), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) 
    {
	// FIXME: stringList.insert (position, "");
    }

    endInsertRows ();
    return true;
}

bool
IggiTreeModel::removeRows (int position, int rows, const QModelIndex &index)
{
    beginRemoveRows (QModelIndex (), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) 
    {
	// FIXME: stringList.removeAt (position);
	ASSERT (m_rootItem->child (row));
    }

    endRemoveRows ();
    return true;
}

void
IggiTreeModel::clear (void)
{
    m_rootItem->clear ();
    reset ();
    ASSERT (m_rootItem);
}

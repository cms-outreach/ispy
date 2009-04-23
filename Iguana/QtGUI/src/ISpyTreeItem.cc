//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyTreeItem.h"
#include <QStringList>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyTreeItem::ISpyTreeItem (const QList<QVariant> &data, ISpyTreeItem *parent) 
    : m_itemData (data), 
      m_parentItem (parent)
{}

ISpyTreeItem::~ISpyTreeItem (void)
{
    qDeleteAll (m_childItems);
}

void 
ISpyTreeItem::appendChild (ISpyTreeItem * item)
{
    m_childItems.append (item);
}

ISpyTreeItem *
ISpyTreeItem::child (int row)
{
    return m_childItems.value (row);
}

int 
ISpyTreeItem::childCount (void) const
{
    return m_childItems.count ();
}

int 
ISpyTreeItem::columnCount (void) const
{
    return m_itemData.count ();
}

QVariant 
ISpyTreeItem::data (int column) const
{
    return m_itemData.value (column);
}

ISpyTreeItem *
ISpyTreeItem::parent (void) const
{
    return m_parentItem;
}

void
ISpyTreeItem::destroy (void)
{
    qDeleteAll (m_childItems.constBegin (), m_childItems.constEnd ());
}

void
ISpyTreeItem::clear (void)
{
    for (QList<ISpyTreeItem *>::iterator it = m_childItems.begin (), end = m_childItems.end (); it != end; ++it) 
    {
	(*it)->clear ();
	delete (*it);
    }
    m_childItems.clear ();
}

int 
ISpyTreeItem::row (void) const
{
    if (m_parentItem)
	return m_parentItem->m_childItems.indexOf (const_cast<ISpyTreeItem *>(this));
    
    return 0;
}

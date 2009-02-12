//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IggiTreeItem.h"
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

IggiTreeItem::IggiTreeItem (const QList<QVariant> &data, IggiTreeItem *parent) 
    : m_itemData (data), 
      m_parentItem (parent)
{}

IggiTreeItem::~IggiTreeItem (void)
{
    qDeleteAll (m_childItems);
}

void 
IggiTreeItem::appendChild (IggiTreeItem * item)
{
    m_childItems.append (item);
}

IggiTreeItem *
IggiTreeItem::child (int row)
{
    return m_childItems.value (row);
}

int 
IggiTreeItem::childCount (void) const
{
    return m_childItems.count ();
}

int 
IggiTreeItem::columnCount (void) const
{
    return m_itemData.count ();
}

QVariant 
IggiTreeItem::data (int column) const
{
    return m_itemData.value (column);
}

IggiTreeItem *
IggiTreeItem::parent (void) const
{
    return m_parentItem;
}

void
IggiTreeItem::destroy (void)
{
    qDeleteAll (m_childItems.constBegin (), m_childItems.constEnd ());
}

void
IggiTreeItem::clear (void)
{
    for (QList<IggiTreeItem *>::iterator it = m_childItems.begin (), end = m_childItems.end (); it != end; ++it) 
    {
	(*it)->clear ();
	delete (*it);
    }
    m_childItems.clear ();
}

int 
IggiTreeItem::row (void) const
{
    if (m_parentItem)
	return m_parentItem->m_childItems.indexOf (const_cast<IggiTreeItem *>(this));
    
    return 0;
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/Ig3DRep.h"
#include "Iguana/QtGUI/interface/Ig3DModel.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include <QGraphicsItem>
#include <classlib/utils/DebugAids.h>
#include <algorithm>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DRep::Ig3DRep (Ig3DModel *model,
		  Ig3DRep *parent,
		  Ig3DRep *prev,
		  QGraphicsItemGroup *node)
    : m_context (0),
      m_model (model)
{
    ASSERT (m_model);
    ASSERT (node);
}

Ig3DRep *
Ig3DRep::asRep (QGraphicsItem *node)
{
    return (node
	    && ! node->isWidget ()
	    && ! node->isWindow ()
	    ? static_cast<Ig3DRep *> (node) : 0);
}

Ig3DRep::~Ig3DRep (void)
{}

void
Ig3DRep::zap (bool search /* = false */, bool kill /* = true */)
{
    // Kill rep children.
    ASSERT (childItems ().size () >= 2);
    for (int i = 0; i < childItems ().size(); ++i) 
    {

	//for (int i = getNumChildren () - 1; i >= 2; --i)

    }
}

void
Ig3DRep::context (IgRepContext *context)
{
    ASSERT (! m_context);
    ASSERT (context);
    m_context = context;
}

//////////////////////////////////////////////////////////////////////
IgRepContext *
Ig3DRep::context (void) const
{ return m_context; }

IgModel *
Ig3DRep::model (void) const
{ return m_model; }

//////////////////////////////////////////////////////////////////////
int
Ig3DRep::children (void) const
{
    ASSERT (childItems ().size () >= 2);
    return childItems ().size () - 2;
}

Ig3DRep *
Ig3DRep::child (int i) const
{
    ASSERT (i >= 0);
    ASSERT (childItems ().size () >= 2);
    ASSERT (i + 2 < childItems ().size ());
//     ASSERT (childItems ().at (i+2)
// 	    && childItems ().at (i+2)->isOfType (Ig3DRep::getClassTypeId ()));

    return static_cast<Ig3DRep *> (childItems ().at (i+2));
}

Ig3DRep *
Ig3DRep::child (QString name) const
{
    for (int i = 0; i < children (); ++i)
    {
	Ig3DRep *c = child (i);
// 	if (c->node ()->name () == name.toStdString())
// 	    return c;
    }

    return 0;
}

std::string &
Ig3DRep::name (void) 
{
    return m_name;
}

void 
Ig3DRep::name (std::string &name)
{
    m_name = name;
}

//////////////////////////////////////////////////////////////////////
QGraphicsItemGroup *
Ig3DRep::magic (void) const
{
    ASSERT (childItems ().size () >= 2);
    //ASSERT (childItems ().at (0) && getChild (0)->isOfType(QGraphicsItemGroup::getClassTypeId()));
    return static_cast<QGraphicsItemGroup *> (childItems ().at (0));
}

QGraphicsItemGroup *
Ig3DRep::magic (QGraphicsItemGroup *node)
{
//     ASSERT (getNumChildren () >= 2);
//     ASSERT (getChild (0) && getChild (0)->isOfType(QGraphicsItemGroup::getClassTypeId()));
//     ASSERT (node);
//     replaceChild (0, node);
//     m_model->change (this);
    return node;
}

QGraphicsItem *
Ig3DRep::findMagic (QString name) const
{
//     for (int j = 0; j < magic ()->getNumChildren (); j++)
//     {
// 	SoNode *node = magic ()->getChild (j);
// 	if (node->getName () == name)
// 	    return node;	
//     }	
    return 0;
}

QGraphicsItemGroup *
Ig3DRep::node (void) const
{
//     ASSERT (getNumChildren () >= 2);
//     ASSERT (getChild (1) && getChild (1)->isOfType(SoGroup::getClassTypeId()));
    return static_cast<QGraphicsItemGroup *> (childItems ().at (1));
}

QGraphicsItemGroup *
Ig3DRep::node (QGraphicsItemGroup *node)
{
//     ASSERT (getNumChildren () >= 2);
//     ASSERT (getChild (1) && getChild (1)->isOfType(QGraphicsItemGroup::getClassTypeId()));
//     ASSERT (node);
//     replaceChild (1, node);
//     m_model->change (this);
    return node;
}

void
Ig3DRep::clear (void)
{
//     ASSERT (node ());
//     node ()->removeAllChildren ();
}

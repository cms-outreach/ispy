/*
 *  ISpyScene.cc
 *  
 *
 *  Created by Ianna Osborne on 5/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Iguana/QtGUI/interface/ISpyScene.h"
#include <QGraphicsItemGroup>

ISpyScene::ISpyScene (QObject *parent /*= 0 */) 
    : QGraphicsScene (parent),
      m_eventGroup (new QGraphicsItemGroup)
{}

ISpyScene::ISpyScene (const QRectF & sceneRect, QObject * parent /*= 0 */) 
    : QGraphicsScene (sceneRect, parent) 
{}

ISpyScene::ISpyScene (qreal x, qreal y, qreal width, qreal height, QObject * parent /*= 0 */) 
    : QGraphicsScene (x, y, width, height, parent) 
{}

void
ISpyScene::setEventGroup (QGraphicsItemGroup* group)
{
    m_eventGroup = group;
}

QGraphicsItemGroup*
ISpyScene::eventGroup (void) 
{
    return m_eventGroup;
}

void 
ISpyScene::mousePressEvent (QGraphicsSceneMouseEvent *event)
{}

void 
ISpyScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{}

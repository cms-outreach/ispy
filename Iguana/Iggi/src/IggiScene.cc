/*
 *  IggiScene.cc
 *  
 *
 *  Created by Ianna Osborne on 5/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Iguana/Iggi/interface/IggiScene.h"
#include <QGraphicsItemGroup>

IggiScene::IggiScene (QObject *parent /*= 0 */) 
    : QGraphicsScene (parent),
      m_eventGroup (new QGraphicsItemGroup)
{}

IggiScene::IggiScene (const QRectF & sceneRect, QObject * parent /*= 0 */) 
    : QGraphicsScene (sceneRect, parent) 
{}

IggiScene::IggiScene (qreal x, qreal y, qreal width, qreal height, QObject * parent /*= 0 */) 
    : QGraphicsScene (x, y, width, height, parent) 
{}

void
IggiScene::setEventGroup (QGraphicsItemGroup* group)
{
    m_eventGroup = group;
}

QGraphicsItemGroup*
IggiScene::eventGroup (void) 
{
    return m_eventGroup;
}

void 
IggiScene::mousePressEvent (QGraphicsSceneMouseEvent *event)
{}

void 
IggiScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{}

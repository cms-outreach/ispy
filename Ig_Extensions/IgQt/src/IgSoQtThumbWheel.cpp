/**************************************************************************
 *
 *  This file is part of the Coin SoQt GUI binding library.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoQt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if SOQT_DEBUG
static const char rcsid[] =
  "$Id: IgSoQtThumbWheel.cpp,v 1.1 2002/08/16 01:52:41 cmscan Exp $";
#endif // SOQT_DEBUG

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include <qpainter.h>
#include <qdrawutil.h>
#include <qimage.h>
#include <qpixmap.h>

// #include <Inventor/SbBasic.h>

// #include <soqtdefs.h>
#include "Ig_Extensions/IgQt/interface/IgSoAnyThumbWheel.h"
#include "Ig_Extensions/IgQt/interface/IgSoQtThumbWheel.h"

// *************************************************************************

static const int SHADEBORDERWIDTH = 0;

IgSoQtThumbWheel::IgSoQtThumbWheel(
  QWidget * parent,
  const char * name )
: QWidget( parent, name )
{
  this->constructor( IgSoQtThumbWheel::Vertical );
} // IgSoQtThumbWheel()

IgSoQtThumbWheel::IgSoQtThumbWheel(
  Orientation orientation,
  QWidget * parent,
  const char * name )
: QWidget( parent, name )
{
  this->constructor( orientation );
} // IgSoQtThumbWheel()

void
IgSoQtThumbWheel::constructor( // private
  Orientation orientation )
{
  this->orient = orientation;
  this->state = IgSoQtThumbWheel::Idle;
  this->wheelValue = this->tempWheelValue = 0.0f;
  this->wheel = new IgSoAnyThumbWheel;
  this->wheel->setMovement( IgSoAnyThumbWheel::UNIFORM );
  this->wheel->setGraphicsByteOrder( IgSoAnyThumbWheel::ARGB );
  this->wheel->setBoundaryHandling( IgSoAnyThumbWheel::MODULATE );
  this->pixmaps = NULL;
  this->numPixmaps = 0;
  this->currentPixmap = -1;
} // constructor()

IgSoQtThumbWheel::~IgSoQtThumbWheel(
  void )
{
  delete this->wheel;
  if ( this->pixmaps ) {
    for ( int i = 0; i < this->numPixmaps; i++ )
      delete this->pixmaps[i];
    delete [] this->pixmaps;
  }
} // ~IgSoQtThumbWheel()

void
IgSoQtThumbWheel::setOrientation(
  Orientation orientation )
{
  this->orient = orientation;
  this->repaint( FALSE );
} // setOrientation()

void
IgSoQtThumbWheel::paintEvent(
  QPaintEvent * event )
{
  QPainter p( this );
  QRect paintRect = event->rect();
  p.setClipRect(paintRect);
  QColorGroup g = this->colorGroup();

  int w, d;
  if ( this->orient == IgSoQtThumbWheel::Vertical ) {
    w = this->width() - 12;
    d = this->height() - 6;
  } else {
    w = this->height() - 12;
    d = this->width() - 6;
  }

  // Handle resizing to too small dimensions gracefully.
  if ( ( d <= 0 ) || ( w <= 0 ) ) return;

  this->initWheel( d, w );

  int pixmap = this->wheel->getBitmapForValue( this->tempWheelValue,
     (this->state == IgSoQtThumbWheel::Disabled) ?
        IgSoAnyThumbWheel::DISABLED : IgSoAnyThumbWheel::ENABLED );

  QRect widgetrect( 0, 0, this->width(), this->height() );
  QRect wheelrect( widgetrect );

  if ( this->orient == Vertical ) {
    wheelrect.setTop(    wheelrect.top() + 2 );
    wheelrect.setBottom( wheelrect.bottom() - 2 );
    wheelrect.setLeft(   wheelrect.left() + 5 );
    wheelrect.setRight(  wheelrect.right() - 5 );
  } else {
    wheelrect.setTop(    wheelrect.top() + 5 );
    wheelrect.setBottom( wheelrect.bottom() - 5 );
    wheelrect.setLeft(   wheelrect.left() + 2 );
    wheelrect.setRight(  wheelrect.right() - 2 );
  }
  
  qDrawPlainRect( &p, wheelrect.left(), wheelrect.top(), wheelrect.width(),
                  wheelrect.height(), QColor( 0, 0, 0 ), 1 );
  
  wheelrect.setTop(    wheelrect.top() + 1 );
  wheelrect.setBottom( wheelrect.bottom() - 1 );
  wheelrect.setLeft(   wheelrect.left() + 1 );
  wheelrect.setRight(  wheelrect.right() - 1 );
  // wheelrect is now wheel-only

  if ( this->orient == Vertical )
    bitBlt( this, wheelrect.left(), wheelrect.top(), this->pixmaps[pixmap],
            0, 0, w, d, CopyROP );
  else
    bitBlt( this, wheelrect.left(), wheelrect.top(), this->pixmaps[pixmap],
            0, 0, d, w, CopyROP );
  this->currentPixmap = pixmap;
} // paintEvent()

/*!
  \internal
*/

void
IgSoQtThumbWheel::mousePressEvent(
  QMouseEvent * event )
{
  if ( this->state != IgSoQtThumbWheel::Idle )
    return;

  if ( event->button() != LeftButton )
    return;

  QRect wheel;
  if ( this->orient == Vertical ) {
    wheel.setLeft( SHADEBORDERWIDTH + 3 );
    wheel.setTop( SHADEBORDERWIDTH + 6 );
    wheel.setRight( this->width() - SHADEBORDERWIDTH - 3 );
    wheel.setBottom( this->height() - SHADEBORDERWIDTH - 6 );
  } else {
    wheel.setLeft( SHADEBORDERWIDTH + 6 );
    wheel.setTop( SHADEBORDERWIDTH + 3 );
    wheel.setRight( this->width() - SHADEBORDERWIDTH - 6 );
    wheel.setBottom( this->height() - SHADEBORDERWIDTH - 3 );
  }

  if ( ! wheel.contains( event->pos() ) )
    return;

  this->state = IgSoQtThumbWheel::Dragging;

  if ( this->orient == IgSoQtThumbWheel::Vertical )
    this->mouseDownPos = event->pos().y() - SHADEBORDERWIDTH - 6;
  else
    this->mouseDownPos = event->pos().x() - SHADEBORDERWIDTH - 6;

  this->mouseLastPos = this->mouseDownPos;

//emit 
  wheelPressed();
} // mousePressEvent()

/*!
  \internal
*/

void
IgSoQtThumbWheel::mouseMoveEvent(
  QMouseEvent * event )
{
  if ( this->state != IgSoQtThumbWheel::Dragging )
    return;

  if ( this->orient == IgSoQtThumbWheel::Vertical )
    this->mouseLastPos = event->pos().y() - SHADEBORDERWIDTH - 6;
  else
    this->mouseLastPos = event->pos().x() - SHADEBORDERWIDTH - 6;
 

  this->tempWheelValue = this->wheel->calculateValue( this->wheelValue,
      this->mouseDownPos, this->mouseLastPos - this->mouseDownPos );

//emit 
  wheelMoved( this->tempWheelValue );

  this->repaint( FALSE );
} // mouseMoveEvent()

/*!
  \internal
*/

void
IgSoQtThumbWheel::mouseReleaseEvent(
  QMouseEvent * event )
{
  if ( this->state != IgSoQtThumbWheel::Dragging )
    return;

  if ( event->button() != LeftButton )
    return;

  this->wheelValue = this->tempWheelValue;
  this->mouseLastPos = this->mouseDownPos;
  this->state = IgSoQtThumbWheel::Idle;
//emit 
  wheelReleased();
} // mouseReleaseEvent()

/*
float
IgSoQtThumbWheel::getNormalizedValue(int pos) const
{
  int relativepos = pos - this->mouseDownPos;
  return (float) relativepos / (float)this->getWheelLength() * 2.0f;
} // getNormalizedValue()
*/

/*
int
IgSoQtThumbWheel::getWheelLength(void) const
{
  return this->orient == IgSoQtThumbWheel::Vertical ?
    this->height() : this->width();
} // getWheelLength()
*/

/*
int
IgSoQtThumbWheel::orientedCoord(const QPoint &p) const
{
  return (this->orient == IgSoQtThumbWheel::Horizontal) ?  p.x() : p.y();
} // orientedCoord()
*/

QSize
IgSoQtThumbWheel::sizeHint(void) const
{
  const int length = 122;
  int thick = 24;

  if (this->orient == IgSoQtThumbWheel::Horizontal)
    return QSize(length, thick);
  else
    return QSize(thick, length);
} // sizeHint()

IgSoQtThumbWheel::Orientation
IgSoQtThumbWheel::orientation(void) const
{
  return this->orient;
} // orientation()

float
IgSoQtThumbWheel::value(void) const
{
  return this->wheelValue;
} // value()

// *************************************************************************

void
IgSoQtThumbWheel::initWheel(
  int diameter,
  int width )
{
  int d, w;
  this->wheel->getSize( d, w );
  if ( d == diameter && w == width ) return;

  this->wheel->setSize( diameter, width );

  int pwidth = width;
  int pheight = diameter;
  if ( this->orient == Horizontal ) {
    pwidth = diameter;
    pheight = width;
  }

  if ( this->pixmaps != NULL ) {
    for ( int i = 0; i < this->numPixmaps; i++ )
      delete this->pixmaps[i];
    delete [] this->pixmaps;
  }

  this->numPixmaps = this->wheel->getNumBitmaps();
  this->pixmaps = new QPixmap * [this->numPixmaps];
  QImage image( pwidth, pheight, 32, 0 );
  for ( int i = 0; i < this->numPixmaps; i++ ) {
    this->wheel->drawBitmap( i, image.bits(), (this->orient == Vertical) ?
      IgSoAnyThumbWheel::VERTICAL : IgSoAnyThumbWheel::HORIZONTAL );
    this->pixmaps[i] = new QPixmap( QSize( pwidth, pheight) );
    this->pixmaps[i]->convertFromImage( image );
  }
} // initWheel()

// *************************************************************************

void
IgSoQtThumbWheel::setEnabled(
  bool enable )
{
  if ( enable )
    this->state = IgSoQtThumbWheel::Idle;
  else
    this->state = IgSoQtThumbWheel::Disabled;
  this->repaint( FALSE );
} // setEnabled()

bool
IgSoQtThumbWheel::isEnabled(
  void ) const
{
  return ( this->state != IgSoQtThumbWheel::Disabled );
} // isEnabled()

void
IgSoQtThumbWheel::setValue(
  float value )
{
  this->wheelValue = this->tempWheelValue = value;
  this->mouseDownPos = this->mouseLastPos;
  this->repaint( FALSE );
} // setValue()

// *************************************************************************

void
IgSoQtThumbWheel::setRangeBoundaryHandling(
  boundaryHandling handling )
{
  switch ( handling ) {
  case CLAMP:
    this->wheel->setBoundaryHandling( IgSoAnyThumbWheel::CLAMP );
    break;
  case MODULATE:
    this->wheel->setBoundaryHandling( IgSoAnyThumbWheel::MODULATE );
    break;
  case ACCUMULATE:
    this->wheel->setBoundaryHandling( IgSoAnyThumbWheel::ACCUMULATE );
    break;
  default:
    assert( 0 && "impossible" );
  }
} // setRangeBoundaryHandling()

// *************************************************************************

IgSoQtThumbWheel::boundaryHandling
IgSoQtThumbWheel::getRangeBoundaryHandling(
  void ) const
{
  switch ( this->wheel->getBoundaryHandling() ) {
  case IgSoAnyThumbWheel::CLAMP:
    return CLAMP;
  case IgSoAnyThumbWheel::MODULATE:
    return MODULATE;
  case IgSoAnyThumbWheel::ACCUMULATE:
    return ACCUMULATE;
  default:
    assert( 0 && "impossible" );
  }
  return CLAMP; // never reached
} // getRangeBoundaryHandling()

// *************************************************************************

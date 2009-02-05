/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// @configure_input@
// $Id: IgSoAnyThumbWheel.h,v 1.1 2002/08/16 01:51:10 cmscan Exp $

#ifndef IGSOANY_THUMBWHEEL_H
#define IGSOANY_THUMBWHEEL_H

// ************************************************************************

/*!	\class IgSoAnyThumbWheel
 *  \brief An exposed version of the Thumbwheel widget from Coin's SoQt classes.
 *  Used to produce IgSoQtThumbWheel
 *
 */
class IgSoAnyThumbWheel {
public:
  enum State              { DISABLED, ENABLED };
  enum Alignment          { VERTICAL, HORIZONTAL };
  enum BoundaryHandling   { MODULATE, ACCUMULATE, CLAMP };
  enum Movement           { UNIFORM, AUTHENTIC };
  enum GraphicsByteOrder  { ABGR, RGBA, ARGB, BGRA };

  IgSoAnyThumbWheel(void);
  ~IgSoAnyThumbWheel(void);

  void setSize( const int diameter, const int width );
  void getSize( int & diameter, int & width ) const;

  void setColor( const float red, const float green, const float blue );
  void getColor( float & red, float & green, float & blue ) const;
  void setColorFactors( const float light, const float front, const float normal, const float shade );
  void getColorFactors( float & light, float & front, float & normal, float & shade ) const;

  int getNumBitmaps(void) const;
  void drawBitmap( const int number, void * bitmap, Alignment alignment ) const;
  float calculateValue( const float origValue, const int origPosition, const int deltaPosition ) const;
  int getBitmapForValue( const float value, const State state ) const;

  void setGraphicsByteOrder( const GraphicsByteOrder byteorder );
  GraphicsByteOrder getGraphicsByteOrder(void) const;

  void setMovement( const Movement movement );
  Movement getMovement(void) const;

  void setBoundaryHandling( const BoundaryHandling handling );
  BoundaryHandling getBoundaryHandling(void) const;

private:
  unsigned int swapWord( unsigned int ) const;

  int diameter, width;
  // float disabledred, disabledgreen, disabledblue; // not implemented
  float red, green, blue;
  float light, front, normal, shade;

  GraphicsByteOrder  byteorder;
  BoundaryHandling   boundaryhandling;
  Movement           movement;

  enum Tables { SIN, COS, RAD, NUMTABLES };

  mutable float * tables [ NUMTABLES ];
  mutable int dirtyTables;
  mutable int dirtyVariables;
  mutable float squarelength, squarespacing, shadelength, unistep, numsquares;

  void drawDisabledWheel( const int number, void * bitmap, Alignment alignment ) const;
  void drawEnabledWheel( const int number, void * bitmap, Alignment alignment ) const;

  void validate(void) const;

}; // class IgSoAnyThumbWheel

// ************************************************************************

#endif // ! IGSOANY_THUMBWHEEL_H

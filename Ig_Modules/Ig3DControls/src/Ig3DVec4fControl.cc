//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVec4fControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DVec4fControl::Ig3DVec4fControl (IgControlCategory *pane,
				    QString		label,
				    QString		labels /*= "X:Y:Z:W"*/)
    : Ig3DVectorBaseControl (pane, label, PositionWOAStyle, 4, labels)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtRangeControlFloat *
Ig3DVec4fControl::control (void) const
{ return control (0); }

IgQtRangeControlFloat *
Ig3DVec4fControl::xControl (void) const
{ return control (0); }

IgQtRangeControlFloat *
Ig3DVec4fControl::yControl (void) const
{ return control (1); }

IgQtRangeControlFloat *
Ig3DVec4fControl::zControl (void) const
{ return control (2); }

IgQtRangeControlFloat *
Ig3DVec4fControl::wControl (void) const
{ return control (3); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float
Ig3DVec4fControl::xValue (void) const
{ return xControl ()->value (); }

float
Ig3DVec4fControl::yValue (void) const
{ return yControl ()->value (); }

float
Ig3DVec4fControl::zValue (void) const
{ return zControl ()->value (); }

float
Ig3DVec4fControl::wValue (void) const
{ return wControl ()->value (); }

SbVec4f
Ig3DVec4fControl::value (void) const
{ return SbVec4f (xValue (), yValue (), zValue (), wValue ()); }

void
Ig3DVec4fControl::setValue (const SbVec4f &newval)
{ setValues (newval.getValue (), 4); }

void
Ig3DVec4fControl::setValue (float newx, float newy, float newz, float neww)
{ setValue (SbVec4f (newx, newy, newz, neww)); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float
Ig3DVec4fControl::xChange (void) const
{ return xControl ()->valueChange (); }

float
Ig3DVec4fControl::yChange (void) const
{ return yControl ()->valueChange (); }

float
Ig3DVec4fControl::zChange (void) const
{ return zControl ()->valueChange (); }

float
Ig3DVec4fControl::wChange (void) const
{ return wControl ()->valueChange (); }

SbVec4f
Ig3DVec4fControl::change (void) const
{ return SbVec4f (xChange (), yChange (), zChange (), wChange ()); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
SoSFVec4f *
Ig3DVec4fControl::field (void) const
{ return static_cast<SoSFVec4f *> (Ig3DFieldControl::field ()); }

void
Ig3DVec4fControl::setField (SoSFVec4f *field)
{ Ig3DFieldControl::setField (field); }

SbVec4f
Ig3DVec4fControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DVec4fControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DVec4fControl::applyValue (void)
{ field ()->setValue (value ()); }

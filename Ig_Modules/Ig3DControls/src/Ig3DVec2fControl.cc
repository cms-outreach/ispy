//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVec2fControl.h"
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

Ig3DVec2fControl::Ig3DVec2fControl (IgControlCategory *pane,
				      QString	      label,
				      QString         labels /*= "X:Y" */)
    : Ig3DVectorBaseControl (pane, label, Vector, 2, labels)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtRangeControlFloat *
Ig3DVec2fControl::control (void) const
{ return control (0); }

IgQtRangeControlFloat *
Ig3DVec2fControl::xControl (void) const
{ return control (0); }

IgQtRangeControlFloat *
Ig3DVec2fControl::yControl (void) const
{ return control (1); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float
Ig3DVec2fControl::xValue (void) const
{ return xControl ()->value (); }

float
Ig3DVec2fControl::yValue (void) const
{ return yControl ()->value (); }

SbVec2f
Ig3DVec2fControl::value (void) const
{ return SbVec2f (xValue (), yValue ()); }

void
Ig3DVec2fControl::setValue (const SbVec2f &newval)
{ setValues (newval.getValue (), 2); }

void
Ig3DVec2fControl::setValue (float newx, float newy)
{ setValue (SbVec2f (newx, newy)); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float
Ig3DVec2fControl::xChange (void) const
{ return xControl ()->valueChange (); }

float
Ig3DVec2fControl::yChange (void) const
{ return yControl ()->valueChange (); }

SbVec2f
Ig3DVec2fControl::change (void) const
{ return SbVec2f (xChange (), yChange ()); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
SoSFVec2f *
Ig3DVec2fControl::field (void) const
{ return static_cast<SoSFVec2f *> (Ig3DFieldControl::field ()); }

void
Ig3DVec2fControl::setField (SoSFVec2f *field)
{ Ig3DFieldControl::setField (field); }

SbVec2f
Ig3DVec2fControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DVec2fControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DVec2fControl::applyValue (void)
{ field ()->setValue (value ()); }

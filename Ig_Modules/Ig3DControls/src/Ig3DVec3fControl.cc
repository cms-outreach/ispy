//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVec3fControl.h"
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

Ig3DVec3fControl::Ig3DVec3fControl (IgControlCategory *pane,
				    QString 	      label,
				    Style	      style /*= PositionStyle*/)
    : Ig3DVectorControl (pane, label, style)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
SoSFVec3f *
Ig3DVec3fControl::field (void) const
{ return static_cast<SoSFVec3f *> (Ig3DFieldControl::field ()); }

void
Ig3DVec3fControl::setField (SoSFVec3f *field)
{ Ig3DFieldControl::setField (field); }

SbVec3f
Ig3DVec3fControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DVec3fControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DVec3fControl::applyValue (void)
{ field ()->setValue (value ()); }

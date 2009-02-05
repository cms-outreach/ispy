//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorControl.h"
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

Ig3DVectorControl::Ig3DVectorControl (IgControlCategory *pane,
				      QString		label,
				      Style		style)
    : Ig3DVectorBaseControl (pane, label, style, 3, "")
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtRangeControlFloat *
Ig3DVectorControl::control (void) const
{ return control (0); }

IgQtRangeControlFloat *
Ig3DVectorControl::xControl (void) const
{ return control (0); }

IgQtRangeControlFloat *
Ig3DVectorControl::yControl (void) const
{ return control (1); }

IgQtRangeControlFloat *
Ig3DVectorControl::zControl (void) const
{ return control (2); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get the control's current X angle value in degrees.  Note that
    currently the control does not actually track the rotation matrix'
    angles as such, only the changes made since last apply or update.

    @sa #yValue(), #zValue(), #value(), #change(), #fieldValue().  */
float
Ig3DVectorControl::xValue (void) const
{ return xControl ()->value (); }

/** Get the control's current Y angle value in degrees.  Note that
    currently the control does not actually track the rotation matrix'
    angles as such, only the changes made since last apply or update.

    @sa #xValue(), #zValue(), #value(), #change(), #fieldValue().  */
float
Ig3DVectorControl::yValue (void) const
{ return yControl ()->value (); }

/** Get the control's current Z angle value in degrees.  Note that
    currently the control does not actually track the rotation matrix'
    angles as such, only the changes made since last apply or update.

    @sa #xValue(), #yValue(), #value(), #change(), #fieldValue().  */
float
Ig3DVectorControl::zValue (void) const
{ return zControl ()->value (); }

/** Get the control's current angle values in degrees.  Note that
    currently the control does not actually track the rotation matrix'
    angles as such, only the changes made since last apply or update.
    Note also the that this may not be the same as the value of the
    @em field this control is attached to.

    @sa #xValue(), #yValue(), #zValue(), #change(), #fieldValue().  */
SbVec3f
Ig3DVectorControl::value (void) const
{ return SbVec3f (xValue (), yValue (), zValue ()); }

/** Set the control's current angles to @a newval in degrees.  Note
    that currently the control does not actually track the rotation
    matrix' angles as such, only the changes made since last apply or
    update.  Note also the that this may not change the value of the
    @em field this control is attached to.

    @sa #value(), #change(), #fieldValue().  */
void
Ig3DVectorControl::setValue (const SbVec3f &newval)
{ setValues (newval.getValue (), 3); }

void
Ig3DVectorControl::setValue (float newx, float newy, float newz)
{ setValue (SbVec3f (newx, newy, newz)); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get the angle by which the control's X value has changed since the
    last #apply() or #update().

    @sa #yChange(), #zChange(), #change(), #value(),
    #fieldValue().  */
float
Ig3DVectorControl::xChange (void) const
{ return xControl ()->valueChange (); }

/** Get the angle by which the control's Y value has changed since the
    last #apply() or #update().

    @sa #xChange(), #zChange(), #change(), #value(),
    #fieldValue().  */
float
Ig3DVectorControl::yChange (void) const
{ return yControl ()->valueChange (); }

/** Get the angle by which the control's Z value has changed since the
    last #apply() or #update().

    @sa #xChange(), #yChange(), #change(), #value(),
    #fieldValue().  */
float
Ig3DVectorControl::zChange (void) const
{ return zControl ()->valueChange (); }

/** Get the angles by which the control's value has changed since the
    last #apply() or #update().

    @sa #xChange(), #yChange(), #zChange(), #value(),
    #fieldValue().  */

SbVec3f
Ig3DVectorControl::change (void) const
{ return SbVec3f (xChange (), yChange (), zChange ()); }

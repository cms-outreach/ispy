//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DRotationControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include <Inventor/fields/SoSFRotation.h>
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

/** Convert @a value from degrees to radians. */
float
Ig3DRotationControl::deg2rad (float value)
{ return value * M_PI / 180; }

/** Convert @a value from radians to degrees. */
float
Ig3DRotationControl::rad2deg (float value)
{ return value * 180 / M_PI; }

/** Rotate @a rot by @a angles (where @a angles is in degrees). */
SbRotation
Ig3DRotationControl::rotate (const SbRotation &rot, const SbVec3f &angles)
{
    return SbRotation (SbVec3f (-1.F,0.F,0.F), deg2rad (angles [0]))
	* SbRotation (SbVec3f (0.F,-1.F,0.F), deg2rad (angles [1]))
	* SbRotation (SbVec3f (0.F,0.F,-1.F), deg2rad (angles [2]))
	* rot;
}

/** Calculate Euler rotation angles in degrees from rotation matrix @a
    rot into a sequence of rotations around X, Y, and Z axes, in that
    order.  The resulting angles are in the range [-180, 180].

    If @a yOver90 is set, rotation around Y is known to be within
    [-180,90) or (90,180] and the resulting angles will reflect that.
    Otherwise (= by default) it is assumed that Y is within [-90,90].
    The range of Y cannot be determined automatically so passing this
    flag is useful when smooth results are needed and for example
    incremental rotations are applied.

    If the rotation around the Y axis is exactly 90 degrees such that
    X maps to Z or vice versa, only the sum or difference of rotations
    around X and Z can be determined, but not each individually.  If
    the client knows a good initial guess for Z rotation, for example
    when small incremental rotations are applied, it can pass it in
    the @a zForY90 argument to disambiguate the sum and to guarantee
    smooth results; by default Z is set to zero and X is given the
    complete rotation.

    Pass @a epsilon to indicate how close values to 90-degree rotation
    around Y should be considered as 90 degrees.  If you pass zero,
    only rotations of exactly 90 degrees will match.  The default is a
    small but noticable error -- zero error makes it difficult to lock
    on a unit vector due to numerical errors.  (FIXME: Test this with
    the new code.)  */
SbVec3f
Ig3DRotationControl::eulerXYZAngles (const SbRotation &rot,
				     bool yOver90 /* = false */,
				     float zForY90 /* = 0 */,
				     float epsilon /* = .0001 */)
{
    float	x;
    float	y;
    float	z;
    SbMatrix	m;

    rot.getValue (m);

    // In the notation below ca == cos(a), sa == sin(a)
    //
    //          0            1                   2
    //   m =  cy*cz   cz*sx*sy + cx*sz   -cx*cz*sy + sx*sz    0
    //       -cy*sz   cx*cz - sx*sy*sz    cz*sz + cx*sy*sz    1
    //         sy         -cy*sx               cx*cy          2
    //
    // If sin(y) is +-1 (y is +- 90 deg) the solution is ambiguous and
    // we can only determine z+x (+90) or z-x (-90).  Allow client to
    // lock on to +- 90 when we get close to avoid numerical errors.
    if (m [2][0] < 1-epsilon)
    {
	if (m [2][0] > -1+epsilon)
	{
	    float yrad = asin (m [2][0]);
	    if (yOver90) yrad = yrad > 0 ? M_PI - yrad : -M_PI - yrad;
	    float cy = cos (yrad);
	    y = rad2deg (yrad);
	    z = rad2deg (atan2 (-m[1][0]/cy, m[0][0]/cy));
	    x = rad2deg (atan2 (-m[2][1]/cy, m[2][2]/cy));
	}
	else
	{
	    // sin(y) =~ -1, i.e. -90 degrees
	    // m[0][1] == -cz*sx + cx*sz == sin(z-x)
	    // m[1][1] ==  cx*cz + sx*sz == cos(z-x)
	    //  ==> z-x = atan2(sin(z-x),cos(z-x))
	    y = -90;
	    z = zForY90;
	    x = z - rad2deg (atan2 (m[0][1], m[1][1]));
	}
    }
    else
    {
	// sin(y) =~ 1, i.e. 90 degrees
	// m[0][1] == cz*sx + cx*sz == sin(z+x)
	// m[1][1] == cx*cz - sx*sz == cos(z+x)
	//  ==> z+x = atan2(sin(z+x),cos(z+x))
	y = 90;
	z = zForY90;
	x = rad2deg (atan2(m[0][1], m[1][1])) - z;
    }

    // FIXME? return SbVec3f (x ? -x : 0, y ? -y : 0, z ? -z : 0);
    return SbVec3f (x, y, z);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Create a new rotation controller.  */
Ig3DRotationControl::Ig3DRotationControl (IgControlCategory *pane,
					  QString label)
    : Ig3DVectorControl (pane, label, RotationStyle),
      m_yOver90 (false),
      m_zForY90 (0)
{
    xControl ()->resetChangedValue (true);
    yControl ()->resetChangedValue (true);
    zControl ()->resetChangedValue (true);

    disconnect(pane,		SIGNAL (tracking (bool)),
               xControl (), 	SLOT   (resetChangedValue (bool)));  
    disconnect(pane,		SIGNAL (tracking (bool)),
               yControl (), 	SLOT   (resetChangedValue (bool)));  
    disconnect(pane,		SIGNAL (tracking (bool)),
               zControl (), 	SLOT   (resetChangedValue (bool)));  
}

/** Set Z rotation angle to @a degrees for the situation when it cannot
    be determined from the rotation matrix (rotation around Y axis is
    90 degrees).  The default value is zero.  */
void
Ig3DRotationControl::zForY90 (float degrees)
{ ASSERT (degrees >= -90 && degrees <= 90); m_zForY90 = degrees; }

/** Indicate whether current rotation around Y is over 90 degrees.  Use
    this to avoid jumps when the differences are known to be minor and
    current Y rotation is known.  */
void
Ig3DRotationControl::yOver90 (bool value)
{ m_yOver90 = value; }

//////////////////////////////////////////////////////////////////////
/** Get the field this control is attached to, or null if the control
    is currently not attached to a rotation field.

    @sa #setField(), #fieldValue(). */
SoSFRotation *
Ig3DRotationControl::field (void) const
{ return static_cast<SoSFRotation *> (Ig3DFieldControl::field ()); }

/** Attach this control to @a field.  Detaches the control from the
    field it was previously attached to if there is one, and attaches
    it to @a field if @a field is non-null.

    @sa #field(), #fieldValue().  */
void
Ig3DRotationControl::setField (SoSFRotation* field)
{ Ig3DFieldControl::setField (field); }

/** Return the current value of the field this control is attached to.
    The control must currently by attached to a non-null field.  Note
    that this is not necessarily the same as the current value that
    the @em control holds.

    @sa #field(), #setField(), #value().  */
SbRotation
Ig3DRotationControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Update the control's current value from the field.  The control
    must currently be attached to a field; this is normally assured by
    base class #update() before invoking this method.  Currently the
    control tracks only the changes made to the rotation matrix, not
    the actual angle values it has, so this does nothing.  */
void
Ig3DRotationControl::updateValue (void)
{
    // Since we never show the actual angle, there is nothing to do
    // here; when we get marked clean, the controls will also be made
    // clean, and will lose their change.  (FIXME: Get the current
    // angle values and display them.)

    //   // Use old values as best guess?
    //   setValue (eulerXYZAngles (fieldValue (),
    //                             fabs (yValue ()) > 90),
    //				   -zValue ()));
}

/** Apply the control's current value to the field.  The control must
    currently be attached to a field; this is normally assured by base
    class #apply() before invoking this method.  Currently the control
    tracks only the changes made to the rotation matrix, not the
    actual angle values it has.  */
void
Ig3DRotationControl::applyValue (void)
{    
    if (isChanged ())
	// We currently work only on changed (= delta) values, not the
	// absolute angles.  To switch to using actual angle values use:
	//   field ()->setValue (rotate (SbRotation (), value ()));
	field ()->setValue (rotate (fieldValue (), change ()));
}

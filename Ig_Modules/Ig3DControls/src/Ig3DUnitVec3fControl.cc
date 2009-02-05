//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DUnitVec3fControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include <classlib/utils/DebugAids.h>

#ifdef WIN32
#include <float.h>
#define copysign _copysign
#endif // WIN32

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DUnitVec3fControl::Ig3DUnitVec3fControl (IgControlCategory *pane,
					    QString 	      label,
					    Style style /*= DirectionStyle*/ )
    : Ig3DVec3fControl (pane, label, style),
      m_internalUpdate (false)
{
    control ()->setSmallStep (.01); 
    control ()->setLongStep (.1); 
    control ()->setRange (-1.,1.);
    
    xControl ()->rangeEditable (false);
    yControl ()->rangeEditable (false);
    zControl ()->rangeEditable (false);
    
    m_internalUpdate = true;
    setValue(1., 0., 0.);
    m_internalUpdate = false;
}

void
Ig3DUnitVec3fControl::setValues (const float *values, unsigned int size)
{
    ASSERT (size == 3);
    SbVec3f vec (values[0], values[1], values[2]);
    // If this is coming from inside, don't touch it
    if (! m_internalUpdate)
	vec.normalize ();

    // Save current precision setting across the update.  Increase the
    // precision while setting the value to avoid jittered values.
    float savedPrecision = control (0)->precision ();
    bool oldSetting = control (0)->isEmitSettingsChanged ();
    control (0)->setEmitSettingsChanged (false);
    for(unsigned int i = 0; i < 3; i++)
      control (i)->setPrecision (.00001);

    // Set the value.  Prevent further internal normalisation.  Then
    // mark all the controls clean.
    bool savedInternal = m_internalUpdate;
    m_internalUpdate = true;
    Ig3DVectorBaseControl::setValues (vec.getValue (), (unsigned int)(3));

    for(unsigned int i = 0; i < 3; i++)
      control (i)->setClean ();

    m_internalUpdate = savedInternal;

    // Restore the precision back to whatever it was.
    
    for(unsigned int i = 0; i < 3; i++)
      control (i)->setPrecision (savedPrecision);
    
    control (0)->setEmitSettingsChanged (oldSetting);
}

void
Ig3DUnitVec3fControl::setDirty (bool dirty)
{
    // Normalise the vector if the value is not being forced on us
    // from outside and we are not recursing (from setValue() below).
    //
    // If the change occurs due to GUI changes and there is just one
    // changed component, fix the changed one and normalise the other
    // two components relative to it.  This makes the user interface
    // more intuitive and avoids numerical errors when one of the
    // values is stepped to boundaries.
    if (m_internalUpdate || ! dirty)
	Ig3DVectorControl::setDirty (dirty);

    else
    {    
	SbVec3f vec (value ());
	bool xdirty = xControl ()->isDirty ();
	bool ydirty = yControl ()->isDirty ();
	bool zdirty = zControl ()->isDirty ();
	
	if (xdirty && !ydirty && !zdirty)
	    // Fix X and normalise the others.
	    normalize (xChange (), vec[0], vec[1], vec[2],
	               0.00001);
	else if (!xdirty && ydirty && !zdirty)
	    // Fix Y and normalise the others.
	    normalize (yChange (), vec[1], vec[0], vec[2],
	               0.00001);
	else if (!xdirty && !ydirty && zdirty)
	    // Fix Z and normalise the others.
	    normalize (zChange (), vec[2], vec[0], vec[1],
	               0.00001);
	else
	    // Normalise all.
	    vec.normalize ();

	// Prevent recursive normalisation and set it.
	m_internalUpdate = true;
	setValue (vec);
	m_internalUpdate = false;
    }
}

void
Ig3DUnitVec3fControl::normalize (float delta, float &v, float &a1, float &a2,
                                 float precision /* = 0.001 */)
{
    // If the other two components are zero, give v everything;
    // determine the sign from v's previous value, or if zero, from
    // the direction we are going to (increase -> 1, decrease -> -1).
    
    if (fabs(a1) <= precision && fabs(a2) <= precision)
    {
	a1 = a2 = 0.;
	v = copysign (1, v ? v : delta);
    }
    // Otherwise, if v is nearly one, make it one.
    else if ((fabs (v) + precision) >= 1.0)
    {
	v = copysign (1, v);
	a1 = a2 = 0.;
    }

    // Otherwise, normalise by fixing v and giving the rest of the
    // magnitude to one or two other components.
    else
    {
	float newMag = v * v;
	float oldMag = (v-delta) * (v-delta);
	float diff   = newMag - oldMag;

	ASSERT (newMag <= 1);
	// If no difference, nothing to do
	if (diff == 0.)
	    ;

	// If one component is zero, give the rest of magnitude to the other
	else if (fabs(a1) <= precision)
	{
	    a1 = 0.;
	    a2 = copysign (sqrt (1-newMag), a2);
	}
	else if (fabs(a2) <= precision)
	{
	    a2 = 0.;
	    a1 = copysign (sqrt (1-newMag), a1);
	}

	// Otherwise, distribute the magnitude growth to the other two
	// proportional to their current magnitudes.
	else
	{
	    float a11 = a1*a1;
	    float a22 = a2*a2;
	    a11 = fabs (a11 * (1. - diff / (a11 + a22)));
	    a22 = fabs (a22 * (1. - diff / (a11 + a22)));
	    a1 = copysign (sqrt (a11), a1);
	    a2 = copysign (sqrt (a22), a2);
	}
    }
}

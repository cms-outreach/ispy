//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DCameraUtils.h"
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
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

Ig3DCameraUtils::Ig3DCameraUtils (void)
    : m_orientation (0),
      m_position (0),
      m_focalDistance (0)
{}

//////////////////////////////////////////////////////////////////////
void
Ig3DCameraUtils::setCamera (SoSFRotation *orientation,
			    SoSFVec3f *position,
			    SoSFFloat *focalDistance)
{
    m_orientation = orientation;
    m_position = position;
    m_focalDistance = focalDistance;
}

//////////////////////////////////////////////////////////////////////
SbVec3f
Ig3DCameraUtils::getFocalPoint (void) const
{
    ASSERT (m_orientation);
    ASSERT (m_position);
    ASSERT (m_focalDistance);

    SbVec3f dir;
    m_orientation->getValue ().multVec (SbVec3f (0, 0, -1), dir);
    return m_position->getValue () + m_focalDistance->getValue () * dir;
}

void
Ig3DCameraUtils::setFocalPoint (const SbVec3f &fp)
{
    ASSERT (m_orientation);
    ASSERT (m_position);
    ASSERT (m_focalDistance);

    SbVec3f dir;
    m_orientation->getValue ().multVec (SbVec3f (0, 0, -1), dir);
    *m_position = fp - m_focalDistance->getValue () * dir;
}

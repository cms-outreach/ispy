//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoJet.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoTransform.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoJet);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoJet::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoJet, IgSoShapeKit, "IgSoShapeKit"); }

IgSoJet::IgSoJet (void)
{
    SO_KIT_CONSTRUCTOR (IgSoJet);
    SO_KIT_ADD_FIELD (energy,    (0.0));
    SO_KIT_ADD_FIELD (theta,     (0.0));
    SO_KIT_ADD_FIELD (phi,       (0.0));
    SO_KIT_ADD_FIELD (thrust,    (1.0));
    SO_KIT_ADD_FIELD (maxZ,      (4.0)) ;               // set these to something more sensible ...
    SO_KIT_ADD_FIELD (maxR,      (2.0)) ;
    SO_KIT_ADD_FIELD (maxEnergy, (100.0)) ;
    SO_KIT_ADD_CATALOG_ENTRY (body,      SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (bodyTrans, SoTransform, FALSE, body,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (bodyCone,  SoCone,      FALSE, body,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (hat,       SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (hatTrans,  SoTransform, FALSE, hat,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (hatCone,   SoCone,      FALSE, hat,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoJet::refresh (void)
{
    SoTransform	*bodyTrans = new SoTransform;
    SoCone	*bodyCone = new SoCone;
    SoTransform	*hatTrans = new SoTransform;
    SoCone	*hatCone = new SoCone;

    // private data members
    float	theta = this->theta.getValue ();
    float	phi = this->phi.getValue ();
    double	ct = cos (theta);
    double	st = sin (theta);
    double	cp = cos (phi);
    double	sp = sin (phi);

    // Define cone rotations and translations; rotVec is the normal of
    // the thrust-y plane and alpha the angle between the y and thrust
    // axes.
    SbVec3f	rotVec (ct, 0.0, -st * cp);
    float	alpha = acos (st * sp);

    // The body cone of the jet (FIXME: Set radius to something
    // meaningful; first attempt to make bodyRadius somehow related to
    // thrust.)  Move cone on its head (M_PI rotation) and vertex at
    // the origin
    float	length1 = ct ? maxZ.getValue () / fabs (ct) : maxZ.getValue ();
    float	length2 = st ? maxR.getValue () / fabs (st) : maxR.getValue ();
    float	bodyHeight = length1 < length2 ? length1 : length2;
    float	bodyRadius = 0.3 * (1.0 / (thrust.getValue () + 0.001));

    bodyCone->bottomRadius = bodyRadius;
    bodyCone->height = bodyHeight;
    bodyTrans->rotation.setValue (rotVec, alpha + M_PI);
    bodyTrans->translation = SbVec3f (bodyHeight * st * cp / 2.0,
				      bodyHeight * st * sp / 2.0,
				      bodyHeight * ct / 2.0);

    // The pointy "hat" on top of the jet.  (FIXME: Set height to
    // something meaningful; first attempt to relate hat height to
    // energy.  For now, hat height varies from 0 up to maxR,
    // depending on what max energy is set to.)
    float	hatRadius = 1.4 * bodyRadius;
    float	hatHeight = maxR.getValue () * energy.getValue ()
			    / maxEnergy.getValue ();

    hatCone->bottomRadius = hatRadius;
    hatCone->height = hatHeight;
    hatTrans->rotation.setValue (rotVec, alpha);
    hatTrans->translation = SbVec3f ((bodyHeight + hatHeight / 2.0) * st * cp,
				     (bodyHeight + hatHeight / 2.0) * st * sp,
				     (bodyHeight + hatHeight / 2.0) * ct);

    setPart ("bodyTrans", bodyTrans);
    setPart ("bodyCone",  bodyCone);
    setPart ("hatTrans",  hatTrans);
    setPart ("hatCone",   hatCone);
}

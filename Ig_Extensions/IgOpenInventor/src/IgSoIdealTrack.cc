//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgSoIdealTrack.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgParticleChar.h"
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <cfloat>
#include <vector>
#include <cassert>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#ifdef WIN32
# define copysign _copysign
#else
# if (defined (__linux) && !defined (__KCC)) || (defined (MIPS) && defined (__KCC))
extern "C" inline double copysign (double x, double y) throw ()
{
    return (fabs (x) * y /fabs (y));
}
# endif
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

#ifdef TGS_VERSION
static const float maxNurbAngle =  M_PI / 2.; // was M_PI/2
#else
static const float maxNurbAngle = M_PI / 6.;
#endif
static const float maxAngle = 4 * M_PI; // truncate curve after two loops

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
float	IgSoIdealTrack::bfield = 4.0F;		//< constant bfield in tesla
float	IgSoIdealTrack::rmax = 1.4F;		//< max radius in central tracker
float	IgSoIdealTrack::zmax = 3.2F;		//< max z extent

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

static const int     NORDER = 3;
static const float   SPEEDOLIGHT = 0.00299792458F; // in (m/nsec) *10^-2

SO_KIT_SOURCE (IgSoIdealTrack);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// An OpenInventor object to represent an ideal track and ideal beam track

void
IgSoIdealTrack::initClass (void)
{
    SO_KIT_INIT_CLASS (IgSoIdealTrack, IgSoShapeKit, "IgSoShapeKit");
    IgParticleChar::initParticles ();
}

IgSoIdealTrack::IgSoIdealTrack (void)
    : m_particleChar (0),
      m_ptot (0),
      m_pt (0),
      m_charge (0)
{
    SO_KIT_CONSTRUCTOR (IgSoIdealTrack);
    SO_KIT_ADD_FIELD (phi,		(0.0F));
    SO_KIT_ADD_FIELD (radius,		(1.0F));
    SO_KIT_ADD_FIELD (zeta,		(0.01F));
    SO_KIT_ADD_FIELD (vertex,		(0.0F, 0.0F, 0.0F));
    SO_KIT_ADD_FIELD (t0,		(0.0F));
    SO_KIT_ADD_FIELD (dt,		(1.0F));
    SO_KIT_ADD_FIELD (tstart,		(0.0F));
    SO_KIT_ADD_FIELD (tend,		(500.F));
    SO_KIT_ADD_FIELD (particleType,	("unknown"));
    SO_KIT_ADD_CATALOG_ENTRY (material, SoMaterial, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (style, SoDrawStyle, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (complexity, SoComplexity, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (points, SoCoordinate4, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (curve, SoNurbsCurve, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (debugStyle, SoDrawStyle, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (debugPoints, SoPointSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoIdealTrack::refresh (void)
{
    // Initialise local variables (and members for the converters)
    float	radius	= this->radius.getValue ();
    float	zeta	= this->zeta.getValue ();
    SbVec3f	vertex	= this->vertex.getValue ();
    float	phi	= this->phi.getValue ();
    float	tstart	= this->tstart.getValue ();
    float	tend	= this->tend.getValue ();

    float	t0	= this->t0.getValue ();
    float	dt	= this->dt.getValue ();

    m_particleChar = IgParticleChar::getByName (particleType.getValue ().getString ());
	if (!m_particleChar) m_particleChar = IgParticleChar::getByName ("unknown");

    m_charge = m_particleChar->getCharge ();
    m_pt =  SPEEDOLIGHT * 100.F * bfield * m_charge * radius;
    float pz = SPEEDOLIGHT * 100.F * bfield * m_charge * zeta;
    m_ptot = sqrt (m_pt * m_pt + pz * pz);

    if (t0 < tstart)
	t0 = tstart;
    if (t0 > tend)
	t0 = tend;

    float t1 = dt+t0;
    if (t1 > tend)
	t1 = tend;
    if (t1 < tstart)
	t1 = tstart;


    if (!fabs (tend - tstart) || !fabs (t1 - t0))
    {
	setPart ("material",	m_particleChar->getMaterial ());
	setPart ("style",	m_particleChar->getStyle ());
	setPart ("complexity",	0);
	setPart ("points",	0);
	setPart ("curve",	0);
	setPart ("debugStyle",	0);
	setPart ("debugPoints",	0);
	return;
    }

    SoComplexity	*complexity = new SoComplexity;
    SoCoordinate4	*points = new SoCoordinate4;
    SoNurbsCurve	*curve = new SoNurbsCurve;
    // SoDrawStyle	*debugStyle = new SoDrawStyle;
    // SoPointSet	*debugPoints = new SoPointSet;
    // SoVertexProperty	*debugVtx = new SoVertexProperty;

    // complexity->type = SoComplexity::SCREEN_SPACE;
    complexity->value = 0.6; // 0.7?

    // Calculate the start & end points
    float x0, x1, y0, y1, z0, z1;
    float phi0 = timeToAngle (t0);
    float phi1 = timeToAngle (t1);
    float delPhi = phi1 - phi0;

    // Until we implement adjustable vector for control points,
    // truncate curve at 2 loops
    if (fabs (delPhi) > maxAngle)
    {
	delPhi = static_cast<float>(copysign (maxAngle, delPhi));
	phi1 = delPhi + phi0;
	t1 = angleToTime (phi1);
    }

    if (delPhi == 0)
    {
	z0 = timeToZ (t0);
	z1 = timeToZ (t1);
    }
    else
    {
	z0 = vertex [2] - (phi0 - phi) * zeta;
	z1 = vertex [2] - (phi1 - phi) * zeta;
    }

    float xc = vertex [0] + radius * sin (phi);
    float yc = vertex [1] - radius * cos (phi);
    x0 = xc - radius * sin (phi0);
    y0 = yc + radius * cos (phi0);
    x1 = xc - radius * sin (phi1);
    y1 = yc + radius * cos (phi1);

    // Calculate the control points & knot vector
    std::vector<SbVec4f> ctlPoints(static_cast<int>(2*(maxAngle/maxNurbAngle +2))); // actually 2*(maxAngle/maxNurbAngle + 1), but avoid rounding prob
    std::vector<float>	knots (static_cast<int>(2*(maxAngle/maxNurbAngle +2)+NORDER)); // #ctlPoints + NORDER
    int		npoints = 0;
    int		nknots = 0;
    int		nknot = 0;

    // the first knot value has multiplicity NORDER (=3), put one in
    // now, and let the normal routine put in the additional NORDER-1.
    knots [nknots++] = static_cast<float>(nknot);

    if (fabs (delPhi) < maxNurbAngle)
    {
	// if less than the max arc covered in one nurb segment, just use the points.
	ctlPoints [npoints++] = SbVec4f (x0, y0, z0, 1.0);

	// knot values for this point: nknots for NORDER-1 terms
	knots [nknots++] = static_cast<float>(nknot);
	knots [nknots++] = static_cast<float>(nknot++);

	ctlPoints [npoints++] =
	    SbVec4f (xc - sin (delPhi/2.F + phi0) * radius / fabs (cos (delPhi/2.F)),
		     yc + cos (delPhi/2.F + phi0) * radius / fabs (cos (delPhi/2.F)),
		     (z0 + z1) / 2.F,
		     1.F)
	    * fabs (cos (delPhi / 2.F));
    }
    else
    {
	// Must generate some filler points
	//
	//   x--------------------------------x
	//               ||
	//               \/
	//   x-----o-----x------o----x---o----x
	//
	//   (one segment becomes three in this example.
	//    nfill=1 => nfill=3, w/ 2 x points (normal weight)
	//    and  3 o points to insert

	int nfill = 1 + (int) (fabs (delPhi) / maxNurbAngle ); // ensure at least one fill division
	if (nfill == 1)
	    // ensure at least 5 points
	nfill = 2;
	double delAngle = delPhi / nfill;
	double delZ = (z1 - z0) / delPhi; // change in z wrt angle

	for (int i = 0; i < nfill; ++i)
	{
	    double angle = phi0 + i * delAngle;

	    if (i == 0)
		// put the first point at exactly the right point
		ctlPoints [npoints++] = SbVec4f (x0, y0, z0, 1.0F);
	    else
		ctlPoints [npoints++] = SbVec4f (xc - radius * sin (angle),
						 yc + radius * cos (angle),
						 z0 + delZ * (angle - phi0),
						 1.0F);

	    // put in the knot values for this point: nknots for NORDER-1 terms
	    knots [nknots++] = static_cast<float>(nknot);
	    knots [nknots++] = static_cast<float>(nknot++);

	    ctlPoints [npoints++] =
		SbVec4f (xc - radius * sin (angle + delAngle/2.F) / fabs (cos (delAngle/2.F)),
			   yc + radius * cos (angle + delAngle/2.F) / fabs (cos (delAngle/2.F)),
			   z0 + delZ * (angle + delAngle/2.F - phi0),
			   1.0F)
		* fabs (cos (delAngle/2.F));
	}
    }

    // The end point
    ctlPoints [npoints++] = SbVec4f (x1, y1, z1, 1.0F);
    knots [nknots++] = static_cast<float>(nknot);
    knots [nknots++] = static_cast<float>(nknot);
    knots [nknots++] = static_cast<float>(nknot);

    // Set the coordinates
    points->point.setValues (0, npoints, &ctlPoints[0]);

    // Rebuild the curve
    curve->numControlPoints = npoints;
    curve->knotVector.setValues (0, nknots, &knots[0]);

    // Add debugging polymarkers for control points
    // for (int i = 0; i < npoints; ++i)
    // {
    //     SbVec3f pt;
    // 	   ctlPoints [i].getReal (pt);
    // 	   debugVtx->vertex.set1Value (i, pt);
    // }
    // 
    // debugStyle->pointSize = 4;
    // debugStyle->style = SoDrawStyle::LINES;
    // debugPoints->vertexProperty = debugVtx;

    setPart ("material",	m_particleChar->getMaterial ());
    setPart ("style",		m_particleChar->getStyle ());
    setPart ("complexity",	complexity);
    setPart ("points",		points);
    setPart ("curve",		curve);
    // setPart ("debugStyle",	debugStyle);
    // setPart ("debugPoints",	debugPoints);
#if 0 //DEBUG
	std::cout << "IgSoIdealTrack particle name: " << particleType.getValue ().getString ()
		<< " material: " << m_particleChar->getMaterial()
		<< std::endl;
#endif //DEBUG
}

float
IgSoIdealTrack::timeToAngle (float time)
{
    float phi = this->phi.getValue ();
    float radius = this->radius.getValue ();
    float tstart = this->tstart.getValue ();
    float mass = m_particleChar->getMass ();
    float betat = m_pt / sqrt (mass * mass + m_ptot * m_ptot);
    if (fabs (radius))
	return phi - betat * SPEEDOLIGHT * (time - tstart) / radius;
    else
	return phi;
}

float
IgSoIdealTrack::angleToTime (float angle)
{
    float phi = this->phi.getValue ();
    float radius = this->radius.getValue ();
    float tstart = this->tstart.getValue ();
    float mass = m_particleChar->getMass ();
    float betat = m_pt / sqrt (mass * mass + m_ptot * m_ptot);
    return tstart - (radius * (angle - phi)) / (betat * SPEEDOLIGHT);
}

float
IgSoIdealTrack::zToTime (float z)
{
    SbVec3f vertex = this->vertex.getValue ();
    float zeta = this->zeta.getValue ();
    float tstart = this->tstart.getValue ();
    float mass = m_particleChar->getMass ();
    float betaz = (zeta * SPEEDOLIGHT * 100 * bfield * m_charge)
		  / sqrt (mass * mass + m_ptot * m_ptot);

    if (fabs (betaz))
	return tstart + (z - vertex [2]) / (betaz * SPEEDOLIGHT);
    else
	return tstart;
}

float
IgSoIdealTrack::timeToZ (float time)
{
    SbVec3f vertex = this->vertex.getValue ();
    float zeta = this->zeta.getValue ();
    float tstart = this->tstart.getValue ();
    float mass = m_particleChar->getMass ();
    float betaz = (zeta * SPEEDOLIGHT * 100 * bfield * m_charge)
		  / sqrt (mass * mass + m_ptot * m_ptot);
    return (time - tstart) * betaz * SPEEDOLIGHT + vertex [2];
}

SbVec2f
IgSoIdealTrack::timeToXY (float time)
{
    float angle = timeToAngle (time);
    SbVec3f vertex = this->vertex.getValue ();
    double radius = this->radius.getValue ();
    double phi = this->phi.getValue ();
    double xc = vertex [0] + radius * sin (phi);
    double yc = vertex [1] - radius * cos (phi);
    return SbVec2f (xc - radius * sin (angle), yc + radius * cos (angle));
}

/** initialize the track using one of the initTrack methods
    @a vx vertex location in the x dimension
    @a vy vertex location in the y dimension
    @a vz vertex location in the z dimension
    @a px momentum (at the vertex) in the x direction
    @a py momentum (at the vertex) in the y direction
    @a pz momentum (at the vertex) in the z direction
    @a t0 time of the creation of the vertex
    @a pc the type of particle */
void
IgSoIdealTrack::initialise (double *vx, double *vy, double *vz, 
			       double *px, double *py, double *pz, 
			       float *t0, IgParticleChar *pc)
{ initialise (*vx, *vy, *vz, *px, *py, *pz, *t0, pc); }

void
IgSoIdealTrack::initialise (double vx, double vy, double vz, 
			       double px, double py, double pz, 
			       float t0, int pid)
{ initialise (vx, vy, vz, px, py, pz, t0, IgParticleChar::getByGeantID (pid)); }

void
IgSoIdealTrack::initialise (double vx, double vy, double vz,
			       double px, double py, double pz,
			       float t0, IgParticleChar *pc)
{
    m_particleChar = pc;
    m_charge = pc->getCharge ();
    particleType = pc->getName ();

    SbVec3f vec3 = SbVec3f (px, py, pz);
    SbVec2f vec2 = SbVec2f (px, py);
    m_ptot = vec3.length ();
    m_pt = vec2.length ();

    double radius = m_pt / (SPEEDOLIGHT * 100 * bfield * m_charge);
    double zeta = pz / (SPEEDOLIGHT * 100 * bfield * m_charge);
    double phi;

    if (py == 0.0 && px == 0.0)
	phi = 0.0;
    else
	phi = atan2 (py, px);

    this->vertex = SbVec3f (vx, vy, vz);
    this->radius = static_cast<float>(radius);
    this->zeta = static_cast<float>(zeta);
    this->phi = static_cast<float>(phi);
    this->t0 = t0;
    this->tstart = t0;

    initEndPts ();
}

void
IgSoIdealTrack::initEndPts (void)
{
    float	radius = this->radius.getValue ();
    float	zeta = this->zeta.getValue ();
    SbVec3f	vertex = this->vertex.getValue ();
    float	phi = this->phi.getValue ();
    double	xc = vertex [0] + radius * sin (phi);
    double	yc = vertex [1] - radius * cos (phi);
    double	delPhi;  // end pt of track & total arc length in x-y plane
    double	x1;
    double	y1;
    double	z1;

    // See if there is an intersection between our helix and the
    // visible edge boundary cylinder
    if (xc == 0 && yc == 0)
    {
	// concentric with z-axis
	if (fabs (radius) >= rmax)
	    // outside max radius
	    return;
	
	x1 = vertex [0];
	y1 = vertex [1];
	delPhi = - 2. * copysign (1.0, m_charge * bfield) * M_PI * 2;

	if (m_pt == 0.0)
	{
	    // z1 = pz
	    z1 = copysign (zmax, zeta * m_charge * bfield);
	    delPhi = 0.0;
	}
	else
	{
	    z1 = vertex [2] - delPhi * zeta;
	    if (fabs (z1) > zmax)
		delPhi = -(copysign (zmax, z1) - vertex [2]) / zeta;
	}
    }
    else
    {
	double x2, y2, delPhi1, delPhi2;
	double gamma = yc ? -xc / yc : FLT_MAX;
	double delta = (rmax * rmax - radius * radius - xc * xc - yc * yc) / (2 * yc);
	double det = 4 * gamma * gamma * delta * delta
		     - 4 * (1 + gamma * gamma) * (delta * delta - radius * radius);

	if (det >= 0)
	{
	    // have intersection
	    det = sqrt (det);
	    x1 = (-2 * gamma * delta + det) / (2 * (1 + gamma * gamma));
	    x2 = (-2 * gamma * delta - det) / (2 * (1 + gamma * gamma));
	    y1 = gamma * x1 + delta;
	    y2 = gamma * x2 + delta;
	    x1 = xc + x1;
	    x2 = xc + x2;
	    y1 = yc + y1;
	    y2 = yc + y2;

	    // Determine which intersection it is... : pos for neg charge.
	    delPhi1 = atan2 (-(x1 - xc) / radius, (y1 - yc) / radius) - phi;
	    delPhi2 = atan2 (-(x2 - xc) / radius, (y2 - yc) / radius) - phi;

	    if (m_charge * bfield < 0)
	    {
		// arc increases wrt time for negative charge
		if (delPhi1 < 0)
		    delPhi1 += M_PI*2;
		if (delPhi2 < 0)
		    delPhi2 += M_PI*2;
	    }
	    else
	    {
		// arc decreases wrt time for positive charge
		if (delPhi1 > 0)
		    delPhi1 -= M_PI*2;
		if (delPhi2 > 0)
		    delPhi2 -= M_PI*2;
	    }

	    if ((m_charge * bfield < 0 && delPhi1 < delPhi2)
		|| (m_charge * bfield > 0 && delPhi1 > delPhi2))
		delPhi = delPhi1;
	    else
	    {
		x1 = x2;
		y1 = y2;
		delPhi = delPhi2;
	    }
	}
	else
	{
	    // just put in two loops or z intersection
	    delPhi = -2.0 * copysign (1.0, m_charge * bfield) * M_PI*2;
	    x1 = vertex [0];
	    y1 = vertex [1];
	    z1 = timeToZ (angleToTime (delPhi + phi));

	    if (fabs (z1) > zmax)
	    {
		z1 = copysign (zmax, z1);
		delPhi = timeToAngle (zToTime (z1 - vertex [2])) - phi;
	    }
	}
    }

    if (delPhi)
	tend = angleToTime (delPhi + phi);
    else
	tend = zToTime (z1 - vertex [2]);

    dt = tend.getValue () - tstart.getValue ();

    // Reset member variables, render code resets them; only needed
    // for the various conversion routines.
    m_particleChar = 0;
    m_pt = 0;
    m_ptot = 0;
    m_charge = 0;
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Inventor/interface/IgSoFieldPlaneMap.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSbField.h"
#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SbLinear.h>
//#include <SealBase/DebugAids.h>
#include <math.h>
#include <assert.h>

#ifdef WIN32
# include <windows.h>
#endif
#ifdef __APPLE__
# include <OpenGL/gl.h>                                                                        
#else
# include <GL/gl.h>
#endif

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoFieldPlane);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoFieldPlane::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoFieldPlane, SoSwitch, "Switch");
}

IgSoFieldPlane::IgSoFieldPlane (void)
    : m_cmap (IgSbColorMap::getColorMap (IgSbColorMap::Jet)),
      m_field (0),
      m_manip (0),
      m_fieldPlane (0),
      m_segments (0),
      m_gradientScale (0),
      m_showSensor (0),
      m_planeSensor (0),
      m_mapDensityXSensor (0),
      m_mapDensityZSensor (0),
      m_segRatioXSensor (0),
      m_segRatioZSensor (0),
      m_winSizeXSensor (0),
      m_winSizeZSensor (0),
      m_winDensityXSensor (0),
      m_winDensityZSensor (0),
      m_winOriginXSensor (0),
      m_winOriginZSensor (0),
      m_componentSensor (0),
      m_maxvalueSensor (0),
      m_invisibleSensor (0),
      m_alphaSensor (0),
      m_mplaneSensor (0),
      m_gradientScaleSensor (0)
{
	SO_NODE_CONSTRUCTOR (IgSoFieldPlane);
	SO_NODE_ADD_FIELD (plane, (SbPlane (SbVec3f (0.0f, 1.0f, 0.0f), 00.f)));
	SO_NODE_ADD_FIELD (mapDensityX, (50));
	SO_NODE_ADD_FIELD (mapDensityZ, (50));
	SO_NODE_ADD_FIELD (segRatioX, (2));
	SO_NODE_ADD_FIELD (segRatioZ, (2));
	
	SO_NODE_ADD_FIELD (winSizeX, (10.0f));
	SO_NODE_ADD_FIELD (winSizeZ, (10.0f));
	SO_NODE_ADD_FIELD (winDensityX, (2));
	SO_NODE_ADD_FIELD (winDensityZ, (2));
	SO_NODE_ADD_FIELD (winOriginX, (0.0f));
	SO_NODE_ADD_FIELD (winOriginZ, (0.0f));
	
	SO_NODE_ADD_FIELD (component, (XYZ_ALL));
	SO_NODE_DEFINE_ENUM_VALUE (Component, XYZ_ALL);
	SO_NODE_DEFINE_ENUM_VALUE (Component, XYZ_X);
	SO_NODE_DEFINE_ENUM_VALUE (Component, XYZ_Y);
	SO_NODE_DEFINE_ENUM_VALUE (Component, XYZ_Z);
	SO_NODE_DEFINE_ENUM_VALUE (Component, XYZ_R);
	SO_NODE_DEFINE_ENUM_VALUE (Component, XYZ_PHI);
	SO_NODE_SET_SF_ENUM_TYPE (component, Component);	
	SO_NODE_ADD_FIELD (maxvalue, (4.75f));
	SO_NODE_ADD_FIELD (invisible, (0));
	SO_NODE_ADD_FIELD (alpha, (255));
	SO_NODE_ADD_FIELD (show, (FALSE));
	SO_NODE_ADD_FIELD (manip, (FALSE));
	SO_NODE_ADD_FIELD (showMap, (TRUE));
	SO_NODE_ADD_FIELD (showSegments, (TRUE));
	SO_NODE_ADD_FIELD (gradientScale, (TRUE));

	// SO_NODE_ADD_FIELD (showContours, (TRUE));
	// SO_NODE_ADD_FIELD (showSurfaces, (TRUE));
	
	// Add callbacks to determine field changes
	m_showSensor          = new SoFieldSensor (&showChanged, this);
	m_showMapSensor       = new SoFieldSensor (&showMapChanged, this);
	m_showSegmentsSensor  = new SoFieldSensor (&showSegmentsChanged, this);
	m_gradientScaleSensor = new SoFieldSensor (&gradientScaleChanged, this);
	
	m_showSensor->attach (&show);
	m_showMapSensor->attach (&showMap);
	m_showSegmentsSensor->attach (&showSegments);
	m_gradientScaleSensor->attach (&gradientScale);
}

IgSoFieldPlane::~IgSoFieldPlane (void)
{
	show = FALSE;
	showMap = FALSE;
	showSegments = FALSE;
	gradientScale = FALSE;
	
	if (m_fieldPlane)
	{
		m_fieldPlane->unref ();
		m_segments->unref ();
		m_gradientScale->unref ();
	}
	
	if (m_planeSensor)
	{
		delete m_planeSensor;
		delete m_mplaneSensor;
		delete m_mapDensityXSensor;
		delete m_mapDensityZSensor;
		delete m_segRatioXSensor;
		delete m_segRatioZSensor;
		
		delete m_winSizeXSensor;
		delete m_winSizeZSensor;
		delete m_winDensityXSensor;
		delete m_winDensityZSensor;
		delete m_winOriginXSensor;
		delete m_winOriginZSensor;
		
		delete m_componentSensor;
		delete m_maxvalueSensor;
		delete m_invisibleSensor;
		delete m_alphaSensor;
	}
	
	delete m_showSensor;
	delete m_showMapSensor;
	delete m_showSegmentsSensor;
	delete m_gradientScaleSensor;
}

void
IgSoFieldPlane::colorMap (const IgSbColorMap *cmap)
{
	assert (cmap);
	m_cmap = cmap;
	if (show.getValue () && m_field)
	{
		refreshColors ();
	}
}

void
IgSoFieldPlane::attachSensors (void)
{
	m_planeSensor->attach (&plane);
	m_mapDensityXSensor->attach (&mapDensityX);
	m_mapDensityZSensor->attach (&mapDensityZ);
	m_segRatioXSensor->attach (&segRatioX);
	m_segRatioZSensor->attach (&segRatioZ);
	
	m_winSizeXSensor->attach (&winSizeX);
	m_winSizeZSensor->attach (&winSizeZ);
	m_winDensityXSensor->attach (&winDensityX);
	m_winDensityZSensor->attach (&winDensityZ);
	m_winOriginXSensor->attach (&winOriginX);
	m_winOriginZSensor->attach (&winOriginZ);
	
	m_componentSensor->attach (&component);
	m_maxvalueSensor->attach (&maxvalue);
	m_invisibleSensor->attach (&invisible);
	m_alphaSensor->attach (&alpha);
	m_mplaneSensor->attach (&m_manip->plane);
}

void
IgSoFieldPlane::detachSensors (void)
{
	m_planeSensor->detach ();
	m_mapDensityXSensor->detach ();
	m_mapDensityZSensor->detach ();
	m_segRatioXSensor->detach ();
	m_segRatioZSensor->detach ();
	
	m_winSizeXSensor->detach ();
	m_winSizeZSensor->detach ();
	m_winDensityXSensor->detach ();
	m_winDensityZSensor->detach ();
	m_winOriginXSensor->detach ();
	m_winOriginZSensor->detach ();
	
	m_componentSensor->detach ();
	m_maxvalueSensor->detach ();
	m_invisibleSensor->detach ();
	m_alphaSensor->detach ();
	m_mplaneSensor->detach ();
}


void
IgSoFieldPlane::field (const IgSbField *field, SbBox3f world)
{
	// FIXME: Replace world by a SbBox. for example viewingbox?
	m_world = world;

	if (field)
	{
		buildStructure ();
	
		if (!m_planeSensor)
		{
			// Add callbacks to determine field plane changes
			m_planeSensor  = new SoFieldSensor (&planeChanged, this);
			m_mplaneSensor = new SoFieldSensor (&mplaneChanged, this);
		
			//Update segments and colour map when these field are changed
			m_mapDensityXSensor =
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_mapDensityZSensor = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_componentSensor   = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_maxvalueSensor    = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_invisibleSensor   = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_alphaSensor       = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_segRatioXSensor   = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_segRatioZSensor   = 
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_winSizeXSensor    =
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_winSizeZSensor    =
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_winDensityXSensor =
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_winDensityZSensor =
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_winOriginXSensor  =
				new SoFieldSensor (&resampleSegmentsCB, this);
			m_winOriginZSensor  =
				new SoFieldSensor (&resampleSegmentsCB, this);
		}
		else if (m_field)
		{
			detachSensors ();
		}

		if (!m_field)
		{
			m_manip->manip.connectFrom (&manip);
			//m_fieldPlane->xdivs.connectFrom (&mapDensityX);
			//m_fieldPlane->zdivs.connectFrom (&mapDensityZ);
		}	
		m_field = field;
		update ();
		attachSensors ();
		m_showSensor->attach (&show);
		m_showMapSensor->attach (&showMap);
		m_showSegmentsSensor->attach (&showSegments);
		m_gradientScaleSensor->attach (&gradientScale);
		show = TRUE;
	}
	else if (m_field)
	{
		m_field = field;
		manip = FALSE;
		show = FALSE;
		showMap = FALSE;
		showSegments = FALSE;
		gradientScale = FALSE;
		m_manip->manip.disconnect ();
		//m_fieldPlane->xdivs.disconnect ();
		//m_fieldPlane->zdivs.disconnect ();
		m_showSensor->detach ();
		m_showMapSensor->detach ();
		m_showSegmentsSensor->detach ();
		m_gradientScaleSensor->detach ();
		detachSensors ();
	}
}

void
IgSoFieldPlane::buildStructure (void)
{
	if (m_manip) {return;}
	
	assert ((getNumChildren () == 0) || (getNumChildren () >= 2));
	
	if (getNumChildren () == 0)
	{
		whichChild = SO_SWITCH_NONE;
	
		// Kill the transform, all the rest is in world coordinates
		addChild (new SoResetTransform);
	
		// Add the plane manipulator
		m_manip = new IgSoPlaneManip;
		addChild (m_manip);
	}
	else
	{
		m_manip = dynamic_cast<IgSoPlaneManip*>(getChild(1));
        
		assert (dynamic_cast<SoResetTransform*>(getChild(0)));
		assert (m_manip);
	
		if (m_manip->manip.isConnectedFromField ())
		{
			m_manip->manip.disconnect ();
		}
		m_manip->manip = FALSE;

		for(int i = 2; i < getNumChildren (); i++)
		{
			SoNode *child = getChild(i);
			if (!m_fieldPlane && dynamic_cast<IgSoFieldPlaneMap*>(child)
				&& child->getName() == SbName ("FieldMap"))
			{
				m_fieldPlane = dynamic_cast<IgSoFieldPlaneMap*>(child);
			}
			else if (!m_segments && dynamic_cast<SoIndexedLineSet*>(child))
			{
				m_segments = dynamic_cast<SoIndexedLineSet*>(child);
			}
			else if (!m_gradientScale && dynamic_cast<SoGroup*>(child)
						&& child->getName() == SbName ("GradientScale"))
			{
				m_gradientScale = dynamic_cast<SoGroup*>(child);
				assert (m_gradientScale->getNumChildren () == 12);
			}
			if (m_fieldPlane && m_segments && m_gradientScale)
			{
				break;
			}
		}
	}	
    
	if (!m_segments)
	{
		m_segments = new SoIndexedLineSet;
		SoVertexProperty *vtx = new SoVertexProperty;
		vtx->orderedRGBA = 0xff0000ff;
		vtx->materialBinding = SoVertexProperty::OVERALL;
		m_segments->vertexProperty = vtx;
		
		if (showSegments.getValue ())
		{
			addChild (m_segments);
		}
	}

	if (!m_fieldPlane)
	{
		m_fieldPlane = new IgSoFieldPlaneMap;
		m_fieldPlane->setName ("FieldMap");
		if (showMap.getValue ())
		{
			addChild (m_fieldPlane);
		}
	}
	else
	{
		//m_fieldPlane->xdivs.disconnect ();
		//m_fieldPlane->zdivs.disconnect ();
	}
    
	if (!m_gradientScale)
	{
		m_gradientScale = new SoGroup;
		m_gradientScale->setName ("GradientScale");
		IgSoFieldPlaneMap *gs = new IgSoFieldPlaneMap;
// 		gs->xdivs = 10;
// 		gs->zdivs = 1;
		m_gradientScale->addChild (gs);
		
		SoIndexedLineSet *grid = new SoIndexedLineSet;
		SoVertexProperty *vtx = new SoVertexProperty;
		vtx->orderedRGBA = 0xffffffff;
		vtx->materialBinding = SoVertexProperty::OVERALL;
		grid->vertexProperty = vtx;
		m_gradientScale->addChild (grid);
			
		m_gradientScale->addChild (new SoTranslation);
		m_gradientScale->addChild (new SoText2);
		m_gradientScale->addChild (new SoTranslation);
		m_gradientScale->addChild (new SoText2);
		m_gradientScale->addChild (new SoTranslation);
		m_gradientScale->addChild (new SoText2);
		m_gradientScale->addChild (new SoTranslation);
		m_gradientScale->addChild (new SoText2);
		m_gradientScale->addChild (new SoTranslation);
		m_gradientScale->addChild (new SoText2);
		if (gradientScale.getValue ())
		{	
			addChild (m_gradientScale);
		}
	}
	
	m_segments->ref ();
	m_fieldPlane->ref ();
	m_gradientScale->ref ();
}

void
IgSoFieldPlane::update (void)
{
	// update the interscetion 
	updateCorners ();
	
	//resample the segments and colors
	resampleSegments ();
}

void
IgSoFieldPlane::updateCorners (void)
{
    // Determine the minimum rectangular intersection of the field's
    // volume bounding box and our slice plane.  Sample the field
    // within this minimal rectangle.  The plane is thought to be in
    // in X-Z space with Y axis as the plane normal, but all this is
    // transformed by `matrix'.

    SbMatrix matrix;
    m_manip->getMotionMatrix (matrix);

    SbVec3f		xdir   (matrix[0][0],matrix[0][1],matrix[0][2]);
    SbVec3f		zdir   (matrix[2][0],matrix[2][1],matrix[2][2]);
    SbVec3f		origin (matrix[3][0],matrix[3][1],matrix[3][2]);
    SbPlane		slicePlane (plane.getValue ());

    // Calculate the minimal rectangular intersection of the slice
    // plane and the sliced volume bounding box as follows.  Check
    // which bounding box edges intersect the plane.  For the edges
    // that do, track the minimum and maximum coordinates for the
    // plane intersection point (X-Z points on the plane, Y being the
    // normal).  The minimal rectangle is determined by the minimum
    // and maximum X-Z values of such points.  If no edge intersects
    // the plane or the rectangle would otherwise be "too small", give
    // the rectangle an artificial smallish size.

    int			nedges = 0;	// # of edges intersecting plane
    float		xmin = 0.f;	// min x of intersecting edges
    float		xmax = 0.f;	// max x of intersecting edges
    float		zmin = 0.f;	// min z of intersecting edges
    float		zmax = 0.f;	// max z of intersecting edges
    SbVec3f		wcorners [8] = {// world bounding box corners
	SbVec3f (m_world.getMin()[0], m_world.getMin()[1], m_world.getMin()[2]),
	SbVec3f (m_world.getMin()[0], m_world.getMin()[1], m_world.getMax()[2]),
	SbVec3f (m_world.getMin()[0], m_world.getMax()[1], m_world.getMin()[2]),
	SbVec3f (m_world.getMin()[0], m_world.getMax()[1], m_world.getMax()[2]),
	SbVec3f (m_world.getMax()[0], m_world.getMin()[1], m_world.getMin()[2]),
	SbVec3f (m_world.getMax()[0], m_world.getMin()[1], m_world.getMax()[2]),
	SbVec3f (m_world.getMax()[0], m_world.getMax()[1], m_world.getMin()[2]),
	SbVec3f (m_world.getMax()[0], m_world.getMax()[1], m_world.getMax()[2])
    };
    static const int	edgeidx [] = {	// edges between vertices in `wcorners'
	0, 1,		// (xmin,        ymin,        zmin - zmax)
	0, 2,		// (xmin,        ymin - ymax, zmin       )
	2, 3,		// (xmin,        ymin,        zmin - zmax)
	1, 3,		// (xmin,        ymin - ymax, zmin       )
	0, 4,		// (xmin - xmax, ymin,        zmin       )
	1, 5,		// (xmin - xmax, ymin,        zmax       )
	3, 7,           // (xmin - xmax, ymax,        zmax       )
	2, 6,           // (xmin - xmax, ymax,        zmin       )
	4, 5,		// (xmax,        ymin,        zmin - zmax)
	4, 6,		// (xmax,        ymin - ymax, zmin       )
	6, 7,		// (xmax,        ymin,        zmin - zmax)
	5, 7,		// (xmax,        ymin - ymax, zmin       )
	-1, -1,
    };

    for (int i = 0; edgeidx [i] != -1; i += 2)
    {
	// Define a line along the edge.
	SbVec3f	pt0 (wcorners [edgeidx [i]]);
	SbVec3f pt1 (wcorners [edgeidx [i+1]]);
	SbLine	edge (pt0, pt1);
	SbVec3f	point;
	float	product = 0.f;

	// Check if the edge line intersects the plane, and if so, if
	// the intersection point is within the edge end points.  If
	// so, record min and max plane X-Z points.
	if (slicePlane.intersect (edge, point)
	    && (product = (point - pt0).dot (edge.getDirection ())) >= 0.f
	    && product <= (pt1 - pt0).length ())
	{
	    float x = xdir.dot (point - origin);
	    float z = zdir.dot (point - origin);
	    if (++nedges == 1)
	    {
		xmin = xmax = x;
		zmin = zmax = z;
	    }
	    else
	    {
		xmin = MINIMUM (xmin, x);
		xmax = MAXIMUM (xmax, x);
		zmin = MINIMUM (zmin, z);
		zmax = MAXIMUM (zmax, z);
	    }

	}
    }

    if (xmax - xmin < 1.f)
    {
	float xcenter = (xmax + xmin) / 2;
	xmin = xcenter - .5f;
	xmax = xcenter + .5f;
    }

    if (zmax - zmin < 1.f)
    {
	float zcenter = (zmax + zmin) / 2;
	zmin = zcenter - .5f;
	zmax = zcenter + .5f;
    }

    // OK, now define the plane.  (FIXME: Optimise code here and above
    // such that in-plane rotation around the normal is ignored and we
    // always sample orthogonal to the bounding box volume; now x/zdir
    // may include rotation around plane normal, and it would be best
    // to ignore it.  For this to work we need ensure the result is
    // still a rectangle, which (and reasonable size thereof) may end
    // up being more trouble to figure out than it is worth.)
    SbVec3f corners [4];
    corners [0] = origin + xmin * xdir + zmin * zdir;
    corners [1] = origin + xmax * xdir + zmin * zdir;
    corners [2] = origin + xmin * xdir + zmax * zdir;
    corners [3] = origin + xmax * xdir + zmax * zdir;
    
    m_fieldPlane->corners.setValues (0, 4, corners);
    
    // Gradient Scale map corners
    zmax += 0.5f;
    corners [0] = corners [2];
    corners [1] = corners [3];
    corners [2] = origin + xmin * xdir + zmax * zdir;
    corners [3] = origin + xmax * xdir + zmax * zdir;

    assert (dynamic_cast<IgSoFieldPlaneMap*>(m_gradientScale->getChild(0)));
    assert (dynamic_cast<SoIndexedLineSet*>(m_gradientScale->getChild(1)));
    
    static_cast<IgSoFieldPlaneMap*>(m_gradientScale->getChild(0))->
        corners.setValues (0, 4, corners);

    // Draw the out line and ticks
    // First draw the out line of the gradient map
    float lineOffset = 0.01;
    corners[0][0] -= lineOffset;
    corners[0][2] -= lineOffset;
    corners[1][0] += lineOffset;
    corners[1][2] -= lineOffset;
    corners[2][0] -= lineOffset;
    corners[2][2] += lineOffset;
    corners[3][0] += lineOffset;
    corners[3][2] += lineOffset;
    
    std::vector<int>	coords;
    std::vector<SbVec3f> vtxs;
    vtxs.push_back (corners [0]);
    vtxs.push_back (corners [1]);
    vtxs.push_back (corners [3]);
    vtxs.push_back (corners [2]);
    for(int i = 0; i <= 4; i++)
      coords.push_back (i%4);
    coords.push_back (SO_END_LINE_INDEX);

    // Now draw the ticks
    // Add first tick here
    assert (dynamic_cast<SoTranslation*>(m_gradientScale->getChild(2)));
    
    SoTranslation *t = static_cast<SoTranslation*>(m_gradientScale->
                           getChild(2));
    t->translation = corners [2];
    SbVec3f step = (corners [3]-corners [2])/4;
    SbVec3f pcorner = corners [2];
    SbVec3f tmpcorner = corners [2];
    
    tmpcorner[2] += 0.5f;
    vtxs.push_back (tmpcorner);
    unsigned vsize = vtxs.size ();
    coords.push_back (vsize - 2);
    coords.push_back (vsize - 1);
    coords.push_back (SO_END_LINE_INDEX);
    
    // Add the rest of 4 ticks here
    for (int i = 1; i <= 4; i++)
    {
		assert (dynamic_cast<SoTranslation*> (m_gradientScale->getChild ((i * 2) + 2)));
		t = static_cast<SoTranslation*> (m_gradientScale->getChild ((i * 2) + 2));
		t->translation = step;
		pcorner = pcorner + step;
		vtxs.push_back (pcorner);
		tmpcorner = pcorner;
		tmpcorner[2] += 0.5f;
		vtxs.push_back (tmpcorner);
		vsize = vtxs.size ();
		coords.push_back (vsize - 2);
		coords.push_back (vsize - 1);
		coords.push_back (SO_END_LINE_INDEX);
    }
    
    SoIndexedLineSet *grid = static_cast<SoIndexedLineSet*>(m_gradientScale->getChild(1));
    SoVertexProperty *vtx = (SoVertexProperty *)grid->vertexProperty.getValue ();
    vtx->vertex.setValues (0, vtxs.size (), &vtxs [0]);
    vtx->vertex.deleteValues (vtxs.size ());
    grid->coordIndex.setValues (0, coords.size (), &coords [0]);
    grid->coordIndex.deleteValues (coords.size ());
}

void
IgSoFieldPlane::convertMagFieldColors (double &minMag, double &maxMag)
{
	float    rgb [3];
	double   mag;
	unsigned r,g,b,a;
	unsigned invisibleAlpha	= invisible.getValue ();
	unsigned defaultAlpha	= alpha.getValue ();
	
	unsigned startColors = 0;
	
	for (unsigned regionIndex = 0; regionIndex < 5; regionIndex++)
	{
		unsigned size = m_colorsMags[regionIndex].size ();
		
		if (size > 0)
		{
			std::vector<unsigned> colors (size, 0u);
			// determine the color values for all magnitude values and put these into a 
			// color array. Also determine the smallest and the greatest magnitude value
			for (unsigned i = 0; i < size; i++)
			{
				mag = m_colorsMags[regionIndex] [i];
				
				// looking for the smallest value greater zero
				if ((mag > 0.f) && (mag < minMag))
				{
					minMag = mag;
				}
				
				// looking for the greatest value
				if (mag > maxMag)
				{
					maxMag = mag;
				}
				
				// get a rgb color accoring to the value of mag (static method call)
				m_cmap->color (mag, rgb);
				
				r = int (rgb [0] * 255 + .5);
				g = int (rgb [1] * 255 + .5);
				b = int (rgb [2] * 255 + .5);
				a = mag < 1./256 ? invisibleAlpha : defaultAlpha;
				
				// yet another conversion: color vector to an unsigned integer
				colors [i] = r << 24 | g << 16 | b << 8 | a;
			}
			
			// set the 'size' values of the color array from index 0 on
			m_fieldPlane->orderedRGBA.setValues (startColors, size, &colors [0]);
			startColors += size;
			// deletes values from index 'size' upwards
			m_fieldPlane->orderedRGBA.deleteValues (startColors);
		}
	}
}

void
IgSoFieldPlane::convertGradientScaleColors (double &minMag, double &maxMag)
{
	float		rgb [3];
	double		mag;
	unsigned 	r,g,b,a;
	unsigned 	size = 0;
	
	for (unsigned i = 0; i < 5; i++)
	{
		size += m_colorsMags[i].size ();
	}
	std::vector<unsigned>	colors (size, 0u);
	
	assert (dynamic_cast<IgSoFieldPlaneMap*> (m_gradientScale->getChild (0)));
	IgSoFieldPlaneMap *gs =
		static_cast<IgSoFieldPlaneMap*> (m_gradientScale->getChild (0));
		
	// the gradient scale's fix division
	size = 10;
	colors.resize ((size + 1) * 2);
	
	// check if there are any wrong values
	if (minMag < 0.f) { minMag = 0.f; }
	if (maxMag > 1.f) { maxMag = 1.f; }

	double delta = (maxMag - minMag)/(size);
	mag = minMag;
		
	// determine the color values for the gradient scale
	// note that the loop has size + 1 iterations
	for (unsigned i = 0; i <= size; i++)
	{			
		m_cmap->color (mag, rgb);
		
		r = int (rgb [0] * 255 + .5);
		g = int (rgb [1] * 255 + .5);
		b = int (rgb [2] * 255 + .5);
		a = 255;
		
		// first row of the stripe
		colors [i] = r << 24 | g << 16 | b << 8 | a;
		// second row of the stripe
		colors [i+size+1] = colors [i];
		mag += delta;
	}
	
	size = colors.size ();
	// set the color values for the gradient scale
	gs->orderedRGBA.setValues (0, size, &colors [0]);
	gs->orderedRGBA.deleteValues (size);
	
	// divide the scale into 10 by 1 patches
	gs->xdivs.set1Value (0, 10);
	gs->zdivs.set1Value (0, 1);
	
	// set the rest of the regions zero
	for (unsigned i = 1; i < 5; i++)
	{
		gs->xdivs.set1Value (i, 0);
		gs->zdivs.set1Value (i, 0);
	}
	
	gs->lowerLeftCorners.set1Value (0, gs->corners[0]);
	gs->upperRightCorners.set1Value (0, gs->corners[3]);
	gs->lowerRightCorners.set1Value (0, gs->corners[1]);	
}

void
IgSoFieldPlane::refreshColors (void)
{
	double minMag = 1.f;
	double maxMag = 0.f;

	convertMagFieldColors (minMag, maxMag);
	
	convertGradientScaleColors (minMag, maxMag);
		
	// Now set the gradient scale's display values
	char    textLabel [20];
	double  clamp = maxvalue.getValue ();
	SoText2 *text;
	double  step = (maxMag - minMag)/4;
	double  mag = minMag;
	
	for (int i = 0; i < 5; i++)
	{
		assert (dynamic_cast<SoText2*> (m_gradientScale->getChild (3 + (i * 2))));
		text = static_cast<SoText2*> (m_gradientScale->getChild (3 + (i * 2)));
		sprintf (textLabel, "%.4f", mag * clamp);
		text->string = textLabel;
		mag += step;
	}
}

void
IgSoFieldPlane::resampleSegments (void)
{
	// Basically we have a plane that is cut into 5 regions (0 to 4), 
	// the region 2 always represents the detailed window.
	m_xBaseDir = (m_fieldPlane->corners[1] - m_fieldPlane->corners[0]) / mapDensityX.getValue ();
	m_zBaseDir = (m_fieldPlane->corners[2] - m_fieldPlane->corners[0]) / mapDensityZ.getValue ();
	
	// clear all regions
	m_colorsMags.clear ();
	
	unsigned borderBitmask;
	borderBitmask = initializeParameters ();

	determineCorners (borderBitmask);

	transferValuesToMap ();

	calcColorMags ();
	
	// calculate magnetic field lines
	std::vector<SbVec3f>  segments;
	std::vector<int>      segidx;
	std::vector<unsigned> colors;
	unsigned              invisibleAlpha = invisible.getValue ();
	unsigned              defaultAlpha   = alpha.getValue ();
	Component             comp = (Component) component.getValue ();
	unsigned              division [2] = { mapDensityX.getValue (), mapDensityZ.getValue () };
    unsigned              subdiv   [2] = { segRatioX.getValue (), segRatioZ.getValue () };
	float                 clamp = maxvalue.getValue ();

	SbVec3f xdir = m_xBaseDir;
	SbVec3f zdir = m_zBaseDir; 

	float segscale = MINIMUM (xdir.length()*subdiv[0],zdir.length()*subdiv[1])
						/ clamp;	

	for (unsigned i = 0; i <= division [1]; ++i)
	{
		for (unsigned j = 0; j <= division [0]; ++j)
		{			
			// Prepare field position and get value.
			SbVec3f	from (m_fieldPlane->corners[0] + zdir * i + xdir * j);
			double	pt [3] = { from [0], from [1], from [2] };
			double	val [3];
		
			m_field->evaluate (pt, val);
		
			// Compute vertex colour and transparency from field
			// value.  This depends on what components the client
			// asked to see.
			SbVec3f	dir (val [0], val [1], val [2]);			
			double	mag;
		
			switch (comp)
			{
				case XYZ_ALL:
					mag = MINIMUM (dir.length (), clamp) / clamp;
					break;
			
				case XYZ_X:
					mag = MINIMUM (fabs (dir [0]), clamp) / clamp;
					break;
			
				case XYZ_Y:
					mag = MINIMUM (fabs (dir [1]), clamp) / clamp;
					break;
			
				case XYZ_Z:
					mag = MINIMUM (fabs (dir [2]), clamp) / clamp;
					break;
			
				case XYZ_PHI:
				{
					SbVec3f radius (from [0], from [1], 0);
					radius.normalize ();
					SbVec3f angular (SbVec3f (0, 0, 1).cross (radius));
					mag = MINIMUM (fabs (angular.dot (dir)), clamp) / clamp;
				}
					break;
			
				case XYZ_R:
				{
					SbVec3f radius (from [0], from [1], 0);
					radius.normalize ();
					mag = MINIMUM (fabs (radius.dot (dir)), clamp) / clamp;
				}
					break;
			
				default:
					assert (false);
					mag = 0.;
					break;
			}
							
			unsigned a = mag < 1./256 ? invisibleAlpha : defaultAlpha;
			
			// why/what is being done here? next time, at least write a hint please
			if (i % subdiv [1] == 0 && j % subdiv [0] == 0 && a)
			{
				unsigned index = segments.size ();
				segments.push_back (from);
				segments.push_back (from + dir * segscale);
				segidx.push_back (index);
				segidx.push_back (index + 1);
				segidx.push_back (SO_END_LINE_INDEX);
			}
		}
	}
	
	refreshColors ();
	
	SoVertexProperty *vtx;
	vtx = (SoVertexProperty *) m_segments->vertexProperty.getValue ();
	vtx->vertex.setValues (0, segments.size (), &segments [0]);
	vtx->vertex.deleteValues (segments.size ());
	m_segments->coordIndex.setValues (0, segidx.size (), &segidx [0]);
	m_segments->coordIndex.deleteValues (segidx.size ());
}

void
IgSoFieldPlane::determineCorners (unsigned bitmask)
{
	// This method determines the three corners of each region. lower left,
	// lower right and upper right corner. After that, the densities are 
	// accordingly calculated. Note that if the window is touching any border,
	// at least one region will have no volume, with the division/density 
	// being zero.
	//
	// below you see a schematic view of the regions 0-4 and corners marked 
	// by "o". All regions always keep their region index, i.e. the detailed 
	// window has the index 2.
	//
	// z
	// ^
	// |
	// +-----------o
	// |     4     |
	// o---o---o---o
	// | 1 | 2 | 3 |
	// o---o---o---o
	// |     0     |
	// o-----------o ---> x
	
	// if we dont have a detailed window, we just have one non-zero region
	// and the rest has zero density
	if ( m_sizeIsZero || (m_densityXTooLow && m_densityZTooLow) )
	{
		m_regions[0].density[X_AXIS] = mapDensityX.getValue ();
		m_regions[0].density[Z_AXIS] = mapDensityZ.getValue ();
		m_regions[0].lowerLeft = m_fieldPlane->corners[0];
		m_regions[0].upperRight = m_fieldPlane->corners[2] - m_fieldPlane->corners[0] + m_fieldPlane->corners[1];
		m_regions[0].lowerRight = m_fieldPlane->corners[1];
		
		for (int i = 1; i < 5; i++)
		{
			// corners of regions with 0 density are ignored, so set them to 0
			m_regions[i].density[X_AXIS] = 0;
			m_regions[i].density[Z_AXIS] = 0;
		}
	}
	else
	{
		m_regions[0].lowerLeft = m_fieldPlane->corners[0];
		m_regions[0].lowerRight = m_fieldPlane->corners[1];
		
		SbVec3f xdir = m_fieldPlane->corners[1] - m_fieldPlane->corners[0];
		SbVec3f zdir = m_fieldPlane->corners[2] - m_fieldPlane->corners[0];
		
		m_regions[4].upperRight = m_fieldPlane->corners[2] - m_fieldPlane->corners[0] + m_fieldPlane->corners[1];
		
		SbLine leftEdge (m_fieldPlane->corners[0], m_fieldPlane->corners[2]);
		SbLine rightEdge (m_fieldPlane->corners[1], m_regions[4].upperRight);
		
		// calculating the lower left, upper left, lower right and upper 
		// right corner(s) of the window
		m_regions[2].lowerLeft  = m_fieldPlane->corners[0] + (xdir * (m_window.origin[X_AXIS] + 1.0f - m_window.size[X_AXIS])  
									+ zdir * (m_window.origin[Z_AXIS] + 1.0f - m_window.size[Z_AXIS])) / 2.0f;
		m_regions[1].lowerRight = m_regions[2].lowerLeft;
		m_regions[1].upperRight = m_regions[2].lowerLeft + m_zBaseDir * m_window.baseDiv[Z_AXIS];
		m_regions[3].lowerLeft  = m_regions[2].lowerLeft + m_xBaseDir * m_window.baseDiv[X_AXIS];
		m_regions[2].lowerRight = m_regions[3].lowerLeft;
		m_regions[2].upperRight = m_regions[3].lowerLeft + m_zBaseDir * m_window.baseDiv[Z_AXIS];
		
		// projecting two corners of the window to the edges: lower left,
		// lower right, upper left and upper right
		m_regions[1].lowerLeft  = leftEdge.getClosestPoint (m_regions[2].lowerLeft);
		m_regions[0].upperRight = rightEdge.getClosestPoint (m_regions[2].lowerLeft);
		m_regions[3].lowerRight = m_regions[0].upperRight;
		m_regions[4].lowerLeft  = leftEdge.getClosestPoint (m_regions[1].upperRight);
		m_regions[3].upperRight = rightEdge.getClosestPoint (m_regions[1].upperRight);
		m_regions[4].lowerRight = m_regions[3].upperRight;

		unsigned mapDensX = mapDensityX.getValue ();
		unsigned mapDensZ = mapDensityZ.getValue ();
		
		// now check the bitmask and calculate the according densities for each region
		if ((bitmask & 2u) == 2u)
		{
			m_regions[0].density[X_AXIS] = 0;
			m_regions[0].density[Z_AXIS] = 0;
		}
		else
		{
			m_regions[0].density[X_AXIS] = mapDensX;
			m_regions[0].density[Z_AXIS] = (unsigned) round ((m_regions[1].lowerLeft - m_regions[0].lowerLeft).length () / m_zBaseDir.length ());
		}
		
		if ((bitmask & 8u) == 8u)
		{
			m_regions[4].density[X_AXIS] = 0;
			m_regions[4].density[Z_AXIS] = 0;
		}
		else
		{
			m_regions[4].density[X_AXIS] = mapDensX;
			m_regions[4].density[Z_AXIS] = mapDensZ - m_regions[0].density[Z_AXIS] - m_window.baseDiv[Z_AXIS];
		}
		
		if ((bitmask & 1u) == 1u)
		{
			m_regions[1].density[X_AXIS] = 0;
			m_regions[1].density[Z_AXIS] = 0;
		}
		else
		{
			m_regions[1].density[X_AXIS] = (unsigned) round ((m_regions[2].lowerLeft - m_regions[1].lowerLeft).length () / m_xBaseDir.length ());
			m_regions[1].density[Z_AXIS] = m_window.baseDiv[Z_AXIS];
		}
		
		if ((bitmask & 4u) == 4u)
		{
			m_regions[3].density[X_AXIS] = 0;
			m_regions[3].density[Z_AXIS] = 0;
		}
		else
		{
			m_regions[3].density[X_AXIS] = mapDensX - m_regions[1].density[X_AXIS] - m_window.baseDiv[X_AXIS];
			m_regions[3].density[Z_AXIS] = m_window.baseDiv[Z_AXIS];
		}
		
		// if all borders are touched by the window
		if ((bitmask & 15u) == 15u)
		{
			m_regions[2].density[X_AXIS] = m_window.density[X_AXIS] * mapDensX;
			m_regions[2].density[Z_AXIS] = m_window.density[Z_AXIS] * mapDensZ;
		}
		else
		{
			m_regions[2].density[X_AXIS] = (unsigned) round ((float) mapDensX * (float) m_window.density[X_AXIS] * m_window.size[X_AXIS]);
			m_regions[2].density[Z_AXIS] = (unsigned) round ((float) mapDensZ * (float) m_window.density[Z_AXIS] * m_window.size[Z_AXIS]);
		}
	}
}

void
IgSoFieldPlane::correctDetailedSize ()
{
	// in order to align the outer grids we need to calculate the apropriate
	// size of the inner window. the window density is always a integer 
	// multiple of the base density.
	unsigned mapDens[2];
	mapDens[X_AXIS] = mapDensityX.getValue ();
	mapDens[Z_AXIS] = mapDensityZ.getValue ();
	
	float smallest[2];
		
	for (unsigned axis = X_AXIS; axis <= Z_AXIS; axis++)
	{
		// smallest possible unit in the 'axis' dimension
		smallest[axis] = 1.0f / (float) mapDens[axis];
		
		// find out how many _whole_ units are covered by the detailed window
		m_window.baseDiv[axis] = (unsigned) round (m_window.size[axis] * mapDens[axis]);

		// calculate that back to the size
		m_window.size[axis] = ((float) m_window.baseDiv[axis] / (float) mapDens[axis]);
		
		// respect the case, in which the size becomes zero after the rounding, but we dont want that because we got a non-zero size from the user
		if (m_window.size[axis] <= 0.0f)
		{
			m_window.size[axis] = smallest[axis];
			m_window.baseDiv[axis] = 1;
		}
	}
}

void
IgSoFieldPlane::transferValuesToMap ()
{
	// set the values on the map	
	m_fieldPlane->xdivs.set1Value (0, m_regions[0].density[X_AXIS]);
	m_fieldPlane->zdivs.set1Value (0, m_regions[0].density[Z_AXIS]);
	
	m_fieldPlane->lowerLeftCorners.set1Value (0, m_regions[0].lowerLeft);
	m_fieldPlane->upperRightCorners.set1Value (0, m_regions[0].upperRight);
	m_fieldPlane->lowerRightCorners.set1Value (0, m_regions[0].lowerRight);
	
	// in case we actually have a window to display
	if (! (m_sizeIsZero || (m_densityXTooLow && m_densityZTooLow) ))
	{
		for (unsigned i = 1; i < 5; i++)
		{
			m_fieldPlane->xdivs.set1Value (i, m_regions[i].density[X_AXIS]);
			m_fieldPlane->zdivs.set1Value (i, m_regions[i].density[Z_AXIS]);
			
			m_fieldPlane->lowerLeftCorners.set1Value (i, m_regions[i].lowerLeft);
			m_fieldPlane->upperRightCorners.set1Value (i, m_regions[i].upperRight);
			m_fieldPlane->lowerRightCorners.set1Value (i, m_regions[i].lowerRight);
		}
	}
	else
	{
		// set the fields zero
		for (unsigned i = 1; i < 5; i++)
		{
			m_fieldPlane->xdivs.set1Value (i, 0);
			m_fieldPlane->zdivs.set1Value (i, 0);
		}
	}	
}

unsigned
IgSoFieldPlane::initializeParameters ()
{
	// Get the parameters for the detailed window, note that the window's
	// size is given in percent in the application, but for convienence 
	// of calculations later on we now divide it by 100. Throughout the 
	// code the words "density" and "division" are (used and mixed) and 
	// they are supposed to be the sampling density for a given region.
	
	const unsigned nbrOfRegions = 5u;
	unsigned borderBitmap = 0u;
	
	m_window.size[X_AXIS]    = winSizeX.getValue () / 100.0f;
	m_window.size[Z_AXIS]    = winSizeZ.getValue () / 100.0f;
	m_window.density[X_AXIS] = winDensityX.getValue ();
	m_window.density[Z_AXIS] = winDensityZ.getValue ();
	m_window.origin[X_AXIS]  = winOriginX.getValue ();
	m_window.origin[Z_AXIS]  = winOriginZ.getValue ();

	m_sizeIsZero = (m_window.size[X_AXIS] <= 0.0f || m_window.size[Z_AXIS] <= 0.0f);
	
	// it doesnt make sense to have a window with less than 2 times as 
	// dense grid
	m_densityXTooLow = m_window.density[X_AXIS] < 2;
	m_densityZTooLow = m_window.density[Z_AXIS] < 2;
	
	// skip the calculations if the area of the window is zero or the 
	// density of it is too low, i.e. we dont have a detailed window
	if ( !(m_sizeIsZero || (m_densityXTooLow && m_densityZTooLow)) )
	{
		correctDetailedSize ();
		
		// The bitmap tells us which borders are touched by the window
		borderBitmap = correctOriginAndGetMask ();
	}
	
	// create the empty color magnitude vectors for each region
	for (unsigned i = 0; i < nbrOfRegions; i++)
	{
		m_colorsMags.push_back (std::vector<double> ());
	}
	return borderBitmap;
}

unsigned
IgSoFieldPlane::correctOriginAndGetMask ()
{
	// note: the window's coordinates are local coordinates in the plane
	// reaching from -1 to 1 in x and z direction. 
	
	// bitmask: lsb 1st bit = left border, 2nd bit = lower, 3rd bit = right, 4th bit = upper
	unsigned crossingBorder = 0u;
	
	unsigned mapDens[2];
	mapDens[X_AXIS] = mapDensityX.getValue ();
	mapDens[Z_AXIS] = mapDensityX.getValue ();
	
	// map the origin into the right place in order to fit the window into
	// the cells, i.e. there are no positions between the cells
	for (unsigned axis = X_AXIS; axis <= Z_AXIS; axis++)
	{
		float position = mapDens[axis] * (m_window.origin[axis] + 1.0f) / 2.0f;
		float roundedPos = round (position);
		
		if ((m_window.baseDiv[axis] % 2) == 1)
		{
			position = ((position > roundedPos) ? (roundedPos + 0.5) : (roundedPos - 0.5));
		}
		else
		{
			position = roundedPos;
		}
		
		m_window.origin[axis] = (position * 2.0f / (float) mapDens[axis]) - 1.0f;
	}

	// The detailed window is not allowed to get out of the base plane's 
	// borders, therefore we have to correct the position of the origin 
	// in order to fit it into the base, since the window must have a 
	// non-zero size
	for (unsigned axis = X_AXIS; axis <= Z_AXIS; axis++)
	{
		// left resp. lower border check
		if ( m_window.origin[axis] <= (-1.0f + m_window.size[axis]) )
		{
			m_window.origin[axis] = -1.0f + m_window.size[axis];
			crossingBorder = crossingBorder | (1u << axis);
		}
	
		// right resp. upper border check
		if ( (1.0f - m_window.size[axis]) <= m_window.origin[axis] )
		{
			m_window.origin[axis] = 1.0f - m_window.size[axis];
			crossingBorder = crossingBorder | (4u << axis);
		}
	}
	return crossingBorder;
}

void 
IgSoFieldPlane::calcColorMags ()
{

	// Now loop over the regions. Precalculate the step vectors into
	// xdir and zdir: their lengths will match the sampling ratio in
	// that direction.
	//    
	// Note that we calculate a colour for every vertex, not just per
	// face.  In both directions there will be one more vertex than
	// there are faces.  The loop is traversed in i/z-major, j/x-minor
	// order as expected by IgSoFieldPlaneMap.
	
	Component comp = (Component) component.getValue ();
	float clamp = maxvalue.getValue ();
	SbVec3f xdir;
	SbVec3f zdir;
	
	// loop over 5 regions and calculate their color values and put them 
	// into the 5-dim vector, i.e. to eachregion belongs a dimension
	for (unsigned index = 0; index < 5; index++)
	{
		// skip the calculation if the region has no volume
		if (m_regions[index].density[X_AXIS] == 0 
			|| m_regions[index].density[Z_AXIS] == 0)
		{
			continue;
		}

		// take each regions length and density to calculate the unit step
		// that has to be made to go across the area
		xdir = (m_regions[index].lowerRight - m_regions[index].lowerLeft) / (float) m_regions[index].density[X_AXIS];
		zdir = (m_regions[index].upperRight - m_regions[index].lowerRight) / (float) m_regions[index].density[Z_AXIS];
		
		for (unsigned i = 0; i <= m_regions[index].density [Z_AXIS]; ++i)
		{
			for (unsigned j = 0; j <= m_regions[index].density [X_AXIS]; ++j)
			{
				// Prepare field position and get value.
				SbVec3f	from (m_regions[index].lowerLeft + zdir * i + xdir * j);
				double	pt [3] = { from [0], from [1], from [2] };
				double	val [3];
				
				m_field->evaluate (pt, val);
				
				// Compute vertex colour and transparency from field
				// value. This depends on what components the client
				// asked to see.
				SbVec3f	dir (val [0], val [1], val [2]);
				double	mag;
			
				switch (comp)
				{
					case XYZ_ALL:
						mag = MINIMUM (dir.length (), clamp) / clamp;
						break;
				
					case XYZ_X:
						mag = MINIMUM (fabs (dir [0]), clamp) / clamp;
						break;
				
					case XYZ_Y:
						mag = MINIMUM (fabs (dir [1]), clamp) / clamp;
						break;
				
					case XYZ_Z:
						mag = MINIMUM (fabs (dir [2]), clamp) / clamp;
						break;
				
					case XYZ_PHI:
					{
						SbVec3f radius (from [0], from [1], 0);
						radius.normalize ();
						SbVec3f angular (SbVec3f (0, 0, 1).cross (radius));
						mag = MINIMUM (fabs (angular.dot (dir)), clamp) / clamp;
					}
						break;
				
					case XYZ_R:
					{
						SbVec3f radius (from [0], from [1], 0);
						radius.normalize ();
						mag = MINIMUM (fabs (radius.dot (dir)), clamp) / clamp;
					}
						break;
				
					default:
						assert (false);
						mag = 0.;
						break;
				}
					
				m_colorsMags[index].push_back (mag);
			}
		}
	}	
}

void
IgSoFieldPlane::planeChanged (void *me, SoSensor *)
{
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->m_mplaneSensor->detach ();
    self->m_manip->plane = self->plane;
    self->update ();
    self->m_mplaneSensor->attach (&self->m_manip->plane);
}

void
IgSoFieldPlane::mplaneChanged (void *me, SoSensor *)
{
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->m_planeSensor->detach ();
    self->plane = self->m_manip->plane;
    self->update ();
    self->m_planeSensor->attach (&self->plane);
}

void
IgSoFieldPlane::showChanged (void *me, SoSensor *)
{
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->buildStructure ();
    if (! self->show.getValue ())
        self->whichChild = SO_SWITCH_NONE;
    else
        self->whichChild = SO_SWITCH_ALL;
}

void
IgSoFieldPlane::resampleSegmentsCB (void *me, SoSensor *)
{
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->resampleSegments ();
}

void
IgSoFieldPlane::showMapChanged (void *me, SoSensor *)
{
    // field map should come before Gradient scale
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->buildStructure ();
    if (self->showMap.getValue ())
    {
        if (self->m_fieldPlane->getRefCount () == 1)
	{
            if (self->m_gradientScale->getRefCount () == 1)
	        self->addChild (self->m_fieldPlane);
	    else
	        self->insertChild (self->m_fieldPlane,
		                   self->getNumChildren()-1);
	}
    }
    else if (self->m_fieldPlane->getRefCount () > 1)
	self->removeChild (self->m_fieldPlane);
}

void
IgSoFieldPlane::gradientScaleChanged (void *me, SoSensor *)
{
    // Gradient scale should always be the last child
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->buildStructure ();
    if (self->gradientScale.getValue ())
    {
        if (self->m_gradientScale->getRefCount () == 1)
            self->addChild (self->m_gradientScale);
    }
    else if (self->m_gradientScale->getRefCount () > 1)
	self->removeChild (self->m_gradientScale);
}

void
IgSoFieldPlane::showSegmentsChanged (void *me, SoSensor *)
{
    // Segments should be the 2nd child
    IgSoFieldPlane *self = static_cast<IgSoFieldPlane *> (me);
    self->buildStructure ();
    if (self->showSegments.getValue ())
    {
        if (self->m_segments->getRefCount () == 1)
	    self->insertChild (self->m_segments, 2);
    }
    else if (self->m_segments->getRefCount () > 1)
	self->removeChild (self->m_segments);
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoHits.h"
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoHits);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoHits::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoHits, IgSoShapeKit, "IgSoShapeKit"); }

IgSoHits::IgSoHits (void)
{
    SO_KIT_CONSTRUCTOR (IgSoHits);
    SO_KIT_ADD_FIELD (points, (0, 0, 0));
    SO_KIT_ADD_CATALOG_ENTRY (pointset, SoPointSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoHits::refresh (void)
{
    SoPointSet		*pointset = new SoPointSet;
    SoVertexProperty	*vtx = new SoVertexProperty;

    vtx->vertex = points;
    pointset->vertexProperty = vtx;

    setPart ("pointset", pointset);
}

void
IgSoHits::convertRZPhiToXYZ (const float *rzphi, float *xyz)
{
    xyz [0] = rzphi [0] * cos (rzphi [2]);
    xyz [1] = rzphi [0] * sin (rzphi [2]);
    xyz [2] = rzphi [1];
}

void
IgSoHits::initXYZ (int n, const float (*pts) [3])
{
    points.deleteValues (0);
    std::vector<SbVec3f> p;
    p.reserve (n);
    for (int i = 0; i < n; ++i)
    	p.push_back (pts [n]);
    points.setValues (0, n, &p [0]);
}

void
IgSoHits::initXYZ (int n, const SbVec3f *pts)
{
    points.deleteValues (0);
    points.setValues (0, n, pts);
}

void
IgSoHits::initRZPhi (int n, const float (*pts) [3])
{
    std::vector<SbVec3f> xyz;
    xyz.reserve (n);
    for (int i = 0; i < n; ++i)
    {
	SbVec3f pt;
	convertRZPhiToXYZ (pts [n], &pt[0]);
	xyz.push_back (pt);
    }
    initXYZ (n, &xyz[0]);
}

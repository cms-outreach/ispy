//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoQuad.h"
#include "Inventor/nodes/SoVertexProperty.h"
#include "Inventor/nodes/SoFaceSet.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoQuad);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoQuad::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoQuad, IgSoShapeKit, "IgSoShapeKit"); }

IgSoQuad::IgSoQuad (void)
{
    SO_KIT_CONSTRUCTOR (IgSoQuad);
    SO_KIT_ADD_FIELD (llCornerOffset, (-0.5f, -0.5f));
    SO_KIT_ADD_FIELD (lrCornerOffset, (0.5f, -0.5f));
    SO_KIT_ADD_FIELD (urCornerOffset, (0.5f, 0.5f));
    SO_KIT_ADD_FIELD (ulCornerOffset, (-0.5f, 0.5f));
    SO_KIT_ADD_CATALOG_ENTRY (face, SoFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoQuad::refresh (void)
{
    SoFaceSet		*face = new SoFaceSet;
    SoVertexProperty	*vtx = new SoVertexProperty;
    SbVec2f		ll = llCornerOffset.getValue ();
    SbVec2f		lr = lrCornerOffset.getValue ();
    SbVec2f		ul = ulCornerOffset.getValue ();
    SbVec2f		ur = urCornerOffset.getValue ();

    vtx->vertex.set1Value (0, SbVec3f (ll [0], ll [1], 0));
    vtx->vertex.set1Value (1, SbVec3f (ul [0], ul [1], 0));
    vtx->vertex.set1Value (2, SbVec3f (ur [0], ur [1], 0));
    vtx->vertex.set1Value (3, SbVec3f (lr [0], lr [1], 0));
    vtx->normal = SbVec3f (0, 0, 1);
    vtx->normalBinding = SoVertexProperty::OVERALL;

    face->numVertices = 4;
    face->vertexProperty = vtx;

    setPart ("face", face);
}

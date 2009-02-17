//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCalHit.h"
#include "Inventor/nodes/SoVertexProperty.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include "Inventor/nodes/SoMaterialBinding.h"
#include "Inventor/SbColor.h"
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoCalHit);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoCalHit::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoCalHit, IgSoShapeKit, "IgSoShapeKit"); }

IgSoCalHit::IgSoCalHit (void)
{
    SO_KIT_CONSTRUCTOR (IgSoCalHit);

    SO_KIT_ADD_FIELD (vertices,		(0, 0, 0));
    SO_KIT_ADD_FIELD (energy,		(0.1f));
    SO_KIT_ADD_FIELD (emFraction,	(0.5f));
    SO_KIT_ADD_FIELD (scaleFactor,	(1.0f));
    SO_KIT_ADD_FIELD (minimumEnergy,	(0.001f));
    SO_KIT_ADD_FIELD (relativeWidth,	(.85f));
    SO_KIT_ADD_FIELD (drawShape,	(TRUE));
    SO_KIT_ADD_FIELD (drawHit,		(TRUE));
    SO_KIT_ADD_FIELD (twoColor,		(TRUE));
    SO_KIT_ADD_FIELD (orderedRGBA,	(0));

    orderedRGBA.set1Value (0, SbColor (0.5,0.5,0.5).getPackedValue ());
    orderedRGBA.set1Value (1, SbColor (0.5,1.0,0.5).getPackedValue ());
    orderedRGBA.set1Value (2, SbColor (0.5,0.5,1.0).getPackedValue ());

    SO_KIT_ADD_CATALOG_ENTRY (faces, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoCalHit::refresh (void)
{
    /*    Vertex layout  
     *       |             |            |        |         |         |             
     *       |     np      |  np        |    np  |    np   |    np   |   : total 5*np vertices
     *       |             |            |        |         |         |             
     *            bottom     shape top    xition    EM top   had top	           
     *
     *   co-ordinate index layout (np = nper) 
     *       |             |             |        |        |        |       | 
     *       | (np + 1)    | (np + 1)    |  5*np  |  5*np  |  5*np  |  5*np |   : total 22*np+2 coord entries
     *       |             |             |        |        |        |       | 
     *            bottom        top        shape    xition    EM       had
     *     
     *   face layout
     *       |             |             |        |        |        |       |
     *       |    bottom   |    top      |  np    |  np    |   np   |   np  |  : total 4*np+2  
     *            bottom        top        shape    xition    EM       had
     */

    if (vertices.getNum () % 2)
	return;

    SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
    SoVertexProperty	*vp = new SoVertexProperty;
    int			nper = vertices.getNum () / 2;
    int			nfaces = 2 + 3*nper; // top & bottom + two extruded sections
    int			nverts = 4*nper; // as above

    if (twoColor.getValue ())
    {
	nfaces += nper; // get extra extruded section
	nverts += nper;  
    }

    // follow IgSoCrystalHit... assumes symmetry & even number of sides
    std::vector<SbVec3f>	tmpv (2*nper);
    std::vector<SbVec3f>	verts (nverts);
    const float			eps = 0.0001F; // offset between hit & shape

    std::copy (vertices.getValues (0),
	       vertices.getValues (0) + vertices.getNum (),
	       &verts[0]);

    // generate a scaled shape
    for (int i = 0; i < 2*nper; i += nper)
	for (int j = 0, k = i; j < nper; j++, k++)
	    tmpv[k] = verts[k] * (1 + relativeWidth.getValue ())/2
		      + verts[i + (k+nper/2)%nper] * (1 - relativeWidth.getValue ())/2;

    // front (bottom) face of hit
    for (int i = 0; i < nper; i++)
	verts[2*nper+i] = tmpv[nper+i] + (tmpv[nper+i]-tmpv[i])*eps;

    // mid face of hit
    int backPt = 3 * nper;
    if (twoColor.getValue ())
    {
	backPt = 4 * nper;
	for (int i = 0; i < nper; i++)
	    verts[3*nper+i] = verts[2*nper+i]
			      + ((tmpv[nper+i]-tmpv[i])
				 * scaleFactor.getValue ()
				 * energy.getValue ()
				 * emFraction.getValue ());
    }

    // back (top) face of hit
    for (int i = 0; i < nper; i++)
	verts [backPt+i] = verts[2*nper+i]
			   + ((tmpv[nper+i]-tmpv[i])
			      * scaleFactor.getValue ()
			      * energy.getValue ());

    // generate required items for each face
    int			numIndices = 5*(nfaces-2) + 2*(nper+1); // all but top/bottom faces have 4 vertexes plus stop
    std::vector<int>	indices (numIndices); // indices for vertices/face
    std::vector<int>	mindices (nfaces,1); // indices for material/face
    std::vector<int>	nindices (nfaces); // indices for the normals
    std::vector<SbVec3f> normals (nfaces); // inormals/face

    // bottom (for outward normal, walk backwards)
    for (int i = 0; i < nper; i++)
	indices[nper - 1 -i] = i;
    indices[nper] = SO_END_FACE_INDEX;
    mindices[0] = 0;

    // top
    for (int i = 0; i < nper; i++)
	indices[nper + 1 + i] = nverts - nper + i;
    indices[2*nper+1] = SO_END_FACE_INDEX;

    // all others
    int ip = 2*nper+2;
    int iv = 0;
    int iface = 2; 
    while (iv < nverts - nper)
    {
	for (int i = 0; i < nper -1; i++)
	{
	    indices[ip++] = iv;
	    indices[ip++] = iv + 1;
	    indices[ip++] = iv + 1 + nper;
	    indices[ip++] = iv + nper;
	    indices[ip++] = SO_END_FACE_INDEX;	
	    normals[iface++] = (verts[iv+1] - verts[iv])
			       .cross (verts[iv+nper]-verts[iv]);
	    iv++;
	}
	// last face wraps vertices back to start
	indices[ip++] = iv;
	indices[ip++] = iv + 1 - nper;
	indices[ip++] = iv + 1;
	indices[ip++] = iv + nper;
	indices[ip++] = SO_END_FACE_INDEX;
	normals[iface++] = (verts[iv+1-nper] - verts[iv])
			   .cross (verts[iv+nper]-verts[iv]);
	iv++;
    }
    normals[0] = (verts[nper-1] - verts[0])
		 .cross (verts[1]-verts[0]); // reverse the order
    normals[1] = (verts[nverts - nper + 1] - verts[nverts - nper])
		 .cross (verts[nverts-1]-verts[nverts - nper]);

    for (int i = 2; i < 2*nper+2; i++)
	mindices[i] = 0;

    if (twoColor.getValue ())
    {
	mindices[1] = 2; 
	std::fill (mindices.begin ()+3*nper+2,mindices.end (),2);
    }

    for (int i = 0; i < nfaces; i++)
	nindices[i] = i;

    // adjust for options
    if (! drawHit.getValue ())
    {
	// readjust down to just the cal cell
	nverts = 2*nper;
	nfaces = 2+nper;
	numIndices = 5* (nfaces-2) + 2* (nper+1); // all but top/bottom faces have 4 vertexes plus stop
	verts.resize (nverts);
	for (int i = 0; i < nper; i++)
	    indices[nper + 1 + i] = nverts - nper + i;
	indices[2*nper+1] = SO_END_FACE_INDEX;
	normals[1] = (verts[nverts - nper + 1] - verts[nverts - nper])
		     .cross (verts[nverts-1]-verts[nverts - nper]);
	mindices[1] = 0;
    }

    int findexStart = 0;
    int indexStart = 0;
    if (!drawShape.getValue ())
    {
	// cut off/transfer first section
	std::copy (indices.begin ()+ (nper+1),
		   indices.begin ()+2* (nper+1),
		   indices.begin ()+11*nper+1); // back face

	// front (bottom) face
	for (int i = 2*nper; i < 3*nper; i++)
	    indices[13*nper -i -1] = i;

	indices[11*nper] = SO_END_FACE_INDEX;
	mindices[2*nper] = 1;
	mindices[2*nper+1] = 2;
	normals[2*nper+1] = normals[1];
	normals[2*nper] = (verts[3*nper-1] - verts[2*nper])
			  .cross (verts[2*nper+1]-verts[2*nper]); // reverse the order
	findexStart = 2*nper;
	indexStart = 10*nper;
	nfaces -= 2*nper;
	numIndices -= 10*nper;
    }
    else if (energy.getValue () <= 0)
    {
	// remove the extrusion
	//   over-write the top hit face with the original shape top face
	for (int i = 0; i < nper; i++)
	    indices[nper + 1 + i] = nper + i;

	mindices[1] = 0;
	normals[1] = (verts[nper + 1] - verts[nper])
		     .cross (verts[2*nper-1]-verts[nper]);
	nfaces = nper+2;
	numIndices = 7 * nper +2;
    }

    vp->materialBinding = SoMaterialBinding::PER_FACE_INDEXED;
    vp->normalBinding = SoMaterialBinding::PER_FACE_INDEXED;
    vp->orderedRGBA = orderedRGBA;
    vp->vertex.setValues (0,verts.size (),&verts[0]);
    vp->normal.setValues (0,nfaces,&normals[findexStart]);
    faces->coordIndex.setValues (0,numIndices,&indices[indexStart]);
    faces->materialIndex.setValues (0,nfaces,&mindices[findexStart]);
    faces->normalIndex.setValues (0,nfaces,&nindices[0]);
    faces->vertexProperty = vp;

    setPart ("faces", faces);
}


//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlaneMap.h"
#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#ifdef WIN32
# include <windows.h>
#endif
#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define DO_MATERIAL(i)							\
   if (colours [i] != prevcolour)					\
   {									\
	float mat [4] = { ((colours [i] >> 24) & 0xff) * 1.0/255,	\
			  ((colours [i] >> 16) & 0xff) * 1.0/255,	\
			  ((colours [i] >> 8 ) & 0xff) * 1.0/255,	\
			  ((colours [i]      ) & 0xff) * 1.0/255 };	\
	glColor4fv (mat);						\
	prevcolour = colours [i];					\
   }

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoFieldPlaneMap);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/* FIXME: Most of the hard-coded values (sampling density, plane
   location and direction, alpha value assigned to visible cells,
   colour scale, yes/no for line segments, etc.) should be given
   through fields.  */

IgSoFieldPlaneMap::IgSoFieldPlaneMap (void)
{
    SO_NODE_CONSTRUCTOR (IgSoFieldPlaneMap);
    SO_NODE_ADD_FIELD (corners,		(0, 0, 0));
    SO_NODE_ADD_FIELD (xdivs,		(1));
    SO_NODE_ADD_FIELD (zdivs,		(1));
    SO_NODE_ADD_FIELD (orderedRGBA,	(0));
}

void
IgSoFieldPlaneMap::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoFieldPlaneMap, SoShape, "Shape");
    SO_ENABLE (SoCallbackAction, SoLazyElement);
    SO_ENABLE (SoPickAction,     SoLazyElement);
    // SO_ENABLE (SoGLRenderAction, SoGLLazyElement); 
}

IgSoFieldPlaneMap::~IgSoFieldPlaneMap (void)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgSoFieldPlaneMap::computeBBox (SoAction *, SbBox3f &box, SbVec3f &center)
{
    if (corners.getNum () == 4)
    {
	box.extendBy (corners [0]);
	box.extendBy (corners [1]);
	box.extendBy (corners [2]);
	box.extendBy (corners [3]);
	center = (corners [0] + corners [3]) / 2;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgSoFieldPlaneMap::GLRender (SoGLRenderAction *action)
{
    // NB: This is pretty much like generatePrimitives().  If you
    // change one, please change both!

    if (corners.getNum () != 4
        || ! shouldGLRender (action)
	|| action->handleTransparency (true))
	return;

    // Push a new state since we modify shape hints.  Then setup and
    // transmit styles.  Colours we do ourselves.  Then we have to tell
    // Inventor which bits of state we modified.  Note that we don't
    // need to manage blending as handleTransparency() took care of that.
    SoState *state = action->getState ();
    state->push ();

    SoShapeHintsElement::set (state, SoShapeHintsElement::COUNTERCLOCKWISE,
			      SoShapeHintsElement::UNKNOWN_SHAPE_TYPE,
			      SoShapeHintsElement::CONVEX);
    SoLazyElement::setColorMaterial (state, true);
    SoGLLazyElement::sendNoMaterial (state);

    // Precalculate some coordinates
    SbVec3f		corner = corners [0];
    SbVec3f		normal = SbPlane (corners[0], corners[1], corners[2])
				 .getNormal ();

    unsigned		xdiv = xdivs.getValue ();
    SbVec3f		xstep = (corners [1] - corners [0]) / xdiv;

    unsigned		zdiv = zdivs.getValue ();
    SbVec3f		zstep = (corners [2] - corners [0]) / zdiv;

    unsigned		prevcolour = 0;

    // Send first the one and only normal for this plane and then all
    // non-empty rectangles.  We process the plane Z-major (i),
    // X-minor (j).  Y remains constant zero.  In both directions
    // there are one more vertex than there are divisions.  Each
    // vertex is assigned a colour and transparency from packed colour
    // array `rgba'.  The location in the array is counted by `index'.
    // Faces with all four corners with a = 0, i.e. invisible, are not
    // drawn at all.
    glBegin (GL_QUADS);
    glNormal3f (normal [0], normal [1], normal [2]);
    for (unsigned i = 0; i < zdiv; ++i)
	for (unsigned j = 0; j < xdiv; ++j)
	{
	    unsigned colours [4] = {
		orderedRGBA [i * (xdiv+1) + j],
		orderedRGBA [i * (xdiv+1) + j+1],
		orderedRGBA [(i+1) * (xdiv+1) + j+1],
		orderedRGBA [(i+1) * (xdiv+1) + j]
	    };

	    if ((colours [0] & 0xff)
		|| (colours [1] & 0xff)
		|| (colours [2] & 0xff)
		|| (colours [3] & 0xff))
	    {
		// Draw the quad.  Avoid material switch where we can.
		DO_MATERIAL (0);
		SbVec3f vertex = corner + j * xstep + i * zstep;
		glVertex3f (vertex [0], vertex [1], vertex [2]);

		DO_MATERIAL (1);
		vertex = corner + (j+1) * xstep + i * zstep;
		glVertex3f (vertex [0], vertex [1], vertex [2]);

		DO_MATERIAL (2);
		vertex = corner + (j+1) * xstep + (i+1) * zstep;
		glVertex3f (vertex [0], vertex [1], vertex [2]);

		DO_MATERIAL (3);
		vertex = corner + j * xstep + (i+1) * zstep;
		glVertex3f (vertex [0], vertex [1], vertex [2]);
	    }
	}
    glEnd ();

    // Restore old state.
    ((SoGLLazyElement *) SoLazyElement::getInstance (state))
	->reset (state, SoLazyElement::DIFFUSE_MASK);
    state->pop ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgSoFieldPlaneMap::generatePrimitives (SoAction *action)
{
    // NB: This is pretty much like GLRender ().  If you change one,
    // please change both!
    if (corners.getNum () != 4)
	return;

    // Push a new state since we modify shape hints.  Then setup and
    // transmit styles and the one and only normal for this plane.
    SoState		*state = action->getState ();
    state->push ();

    SoShapeHintsElement::set (state, SoShapeHintsElement::COUNTERCLOCKWISE,
			      SoShapeHintsElement::UNKNOWN_SHAPE_TYPE,
			      SoShapeHintsElement::CONVEX);

    // Precalculate some coordinates
    SbVec3f		corner = corners [0];
    SbVec3f		normal = SbPlane (corners[0], corners[1], corners[2])
				 .getNormal ();

    unsigned		xdiv = xdivs.getValue ();
    SbVec3f		xstep = (corners [1] - corners [0]) / xdiv;

    unsigned		zdiv = zdivs.getValue ();
    SbVec3f		zstep = (corners [2] - corners [0]) / zdiv;

    // Send all non-empty rectangles as two triangles each.  See
    // GLRender() for details.
    SoPrimitiveVertex	pv;
    SoFaceDetail	fd;
    SoPointDetail	pd;

    pv.setNormal (normal);
    pv.setDetail (&pd);
    pd.setNormalIndex (0);
    pd.setTextureCoordIndex (0);

    for (unsigned i = 0, index = 0; i < zdiv; ++i)
    {
	fd.setPartIndex (int (i));
	for (unsigned j = 0; j < xdiv; ++j, ++index)
	{
	    unsigned colours [4] = {
		orderedRGBA [i * (xdiv+1) + j],
		orderedRGBA [i * (xdiv+1) + j+1],
		orderedRGBA [(i+1) * (xdiv+1) + j+1],
		orderedRGBA [(i+1) * (xdiv+1) + j]
	    };

	    if ((colours [0] & 0xff)
		|| (colours [1] & 0xff)
		|| (colours [2] & 0xff)
		|| (colours [3] & 0xff))
	    {
		// Send the quad as a triangle strip.  We installed
		// above point detail into `pv', and pass face detail
		// below to beginShape().  Thus the face has full info
		// about all its points.
		//
		// We provide material indices into our orderedRGBA
		// array and vertex indices as if this was a quad
		// mesh/strip.

		fd.setFaceIndex (int (index));
		beginShape (action, TRIANGLE_STRIP, &fd);

		pv.setPoint (corner + j * xstep + i * zstep);
		pv.setMaterialIndex (int (index));
		pd.setCoordinateIndex (int (i * xdiv + j));
		pd.setMaterialIndex (int (index));
		shapeVertex (&pv);

		pv.setPoint (corner + (j+1) * xstep + i * zstep);
		pv.setMaterialIndex (int (index));
		pd.setCoordinateIndex (int (i * xdiv + j+1));
		pd.setMaterialIndex (int (index));
		shapeVertex (&pv);
		
		pv.setPoint (corner + (j+1) * xstep + (i+1) * zstep);
		pv.setMaterialIndex (int (index));
		pd.setCoordinateIndex (int ((i+1) * xdiv + j+1));
		pd.setMaterialIndex (int (index));
		shapeVertex (&pv);
		
		pv.setPoint (corner + j * xstep + (i+1) * zstep);
		pv.setMaterialIndex (int (index));
		pd.setCoordinateIndex (int (i * xdiv + j+1));
		pd.setMaterialIndex (int (index));
		shapeVertex (&pv);
		endShape ();
	    }
	}
    }

    // Restore old state.
    state->pop ();
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgSoGL2PSAction.h"
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include "Iguana/QtGUI/interface/gl2ps.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_ACTION_SOURCE (IgSoGL2PSAction)

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void IgSoGL2PSAction::initClass (void)
{
    SO_ACTION_INIT_CLASS (IgSoGL2PSAction,  SoGLRenderAction);
    SO_ACTION_ADD_METHOD (SoIndexedLineSet, indexedLineSet);
    SO_ACTION_ADD_METHOD (SoLineSet,        lineSet);
    SO_ACTION_ADD_METHOD (SoPointSet,       pointSet);
    SO_ACTION_ADD_METHOD (SoText2,          text2);
    SO_ACTION_ADD_METHOD (SoMarkerSet,      markerSet);
    SO_ACTION_ADD_METHOD (SoSeparator,      separatorSet);
}

IgSoGL2PSAction::IgSoGL2PSAction (const SbViewportRegion& region)
    :SoGLRenderAction (region)
{
    SO_ACTION_CONSTRUCTOR (IgSoGL2PSAction);
}

void
IgSoGL2PSAction::indexedLineSet (SoAction* self, SoNode* node)
{
    GLfloat value;
    glGetFloatv (GL_LINE_WIDTH, &value);
    gl2psLineWidth (value);
    GLint pat;
    glGetIntegerv (GL_LINE_STIPPLE_PATTERN, &pat);
    if (pat == 0xffff)
	gl2psDisable (GL2PS_LINE_STIPPLE);
    else
	gl2psEnable (GL2PS_LINE_STIPPLE);
    node->GLRender ((IgSoGL2PSAction *)self);
}

void 
IgSoGL2PSAction::lineSet (SoAction* self, SoNode* node)
{
    GLfloat value;
    glGetFloatv (GL_LINE_WIDTH, &value);
    gl2psLineWidth (value);
    node->GLRender ((IgSoGL2PSAction *)self);
}

void
IgSoGL2PSAction::pointSet (SoAction* self, SoNode* node)
{
    GLfloat value;
    glGetFloatv (GL_POINT_SIZE, &value);
    gl2psPointSize (value);
    node->GLRender ((IgSoGL2PSAction *)self);
}

void
IgSoGL2PSAction::text2 (SoAction* self, SoNode* node)
{
    SoText2 *text = (SoText2 *) node;
    SoState * state = self->getState ();
    state->push ();
      
    SbName fontName = SoFontNameElement::get(state);
    if (fontName == SoFontNameElement::getDefault ())
	fontName = "Courier";

    int fontSize = static_cast<short>(SoFontSizeElement::get (state));
    int alignment = GL2PS_TEXT_C;
    switch (text->justification.getValue ())
    {
    case 1:
        alignment = GL2PS_TEXT_CL;
	break;
    case 2:
	alignment = GL2PS_TEXT_CR;
	break;
    case 3:
	alignment = GL2PS_TEXT_C;
    default:
	break;
    }
 
    SbVec3f nilpoint (0.0f, 0.0f, 0.0f);
    SbColor color = SoLazyElement::getDiffuse (state, 0);
	
    const SbMatrix & mat = SoModelMatrixElement::get (state);
    const SbMatrix & projmatrix = (mat * SoViewingMatrixElement::get (state) *
                                   SoProjectionMatrixElement::get (state));
    const SbViewportRegion & vp = SoViewportRegionElement::get (state);
    SbVec2s vpsize = vp.getViewportSizePixels ();

    projmatrix.multVecMatrix (nilpoint, nilpoint);
    nilpoint [0] = (nilpoint [0] + 1.0f) * 0.5f * vpsize [0];
    nilpoint [1] = (nilpoint [1] + 1.0f) * 0.5f * vpsize [1];      
 
    // Set new state.
    glPushAttrib ((GLbitfield)(GL_CURRENT_BIT | GL_ENABLE_BIT));
    glDisable (GL_LIGHTING);
    glColor3fv (color.getValue ());
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();
    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glOrtho (0, vpsize [0], 0, vpsize [1], -1.0f, 1.0f);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    const int nrlines = text->string.getNum ();
    for (int i = 0; i < nrlines; ++i)
    {
	SbString str = text->string [i];
	glRasterPos3f (nilpoint [0], nilpoint [1], -nilpoint [2]);
	
	gl2psTextOpt (str.getString (),
		     fontName.getString (), fontSize,
		     alignment, 0.F);
	nilpoint [1] -= (fontSize * text->spacing.getValue ());
    }
    glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix ();
    glPopAttrib ();
    state->pop ();
}

void
IgSoGL2PSAction::markerSet (SoAction* self, SoNode* node)
{
    SoMarkerSet *markerSet = (SoMarkerSet*) node;
    SoState *state = self->getState ();
    state->push ();
    
    SoVertexProperty *vernode = static_cast<SoVertexProperty *>(
	markerSet->vertexProperty.getValue ());
    int starti = 0;
    int pointn = vernode->vertex.getNum ();
      
    const SbVec3f *position = vernode->vertex.getValues (starti);

    SbColor color;
    int totalMarkers = markerSet->markerIndex.getNum ();
    int totalColors  = vernode->orderedRGBA.getNum ();
      
    if (totalColors > 0)
    {
	float transparency;
	color.setPackedValue (*(vernode->orderedRGBA.getValues (starti)),
			      transparency);
    }
    else
    {
	const SoCoordinateElement *coordinateElement = 
            SoCoordinateElement::getInstance (state);
	if (coordinateElement == NULL) return;

	color = SoLazyElement::getDiffuse (state, 0);        
    }
    glPushAttrib ((GLbitfield)(GL_CURRENT_BIT | GL_ENABLE_BIT));
    glDisable (GL_LIGHTING);
    glColor3fv (color.getValue ());
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      
    while (starti < pointn)
    {
	SbBool isLSBFirst;
	SbVec2s size;
	const unsigned char *image;

	if ((totalColors > 0) && (starti < totalColors))
	{
	    SbColor color1;
	    float transparency;
	    color1.setPackedValue (*(vernode->orderedRGBA.getValues (starti)),
				   transparency);
	    if (color1 != color)
	    {
		color = color1;
		glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
		glColor3fv (color.getValue ());
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	    }
	}
	
	if (starti < totalMarkers)
	    markerSet->getMarker (markerSet->markerIndex [starti], size, image, isLSBFirst);
	else
	    markerSet->getMarker (markerSet->markerIndex [totalMarkers - 1], size, image, isLSBFirst);

	gl2psDrawImageMap (size [0], size [1],
			   position->getValue (), image);
	++position; ++starti;
    }

    glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
    glPopAttrib ();
    state->pop ();
}

void
IgSoGL2PSAction::separatorSet (SoAction* self, SoNode* node)
{ node->doAction (self); }

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoAxis.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <cmath>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoAxis);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoAxis::initClass ()
{ SO_KIT_INIT_CLASS (IgSoAxis,IgSoShapeKit,"IgSoShapeKit"); }

IgSoAxis::IgSoAxis ()
{
    SO_KIT_CONSTRUCTOR (IgSoAxis);
    SO_KIT_ADD_FIELD (divisions,      (3));
    SO_KIT_ADD_FIELD (divisionLength, (0.5));
    SO_KIT_ADD_FIELD (label,	      ("Z"));
    SO_KIT_ADD_FIELD (totalLength,    (2.0));
    SO_KIT_ADD_FIELD (offset,	      (false));

    SO_KIT_ADD_CATALOG_ENTRY (line,  SoLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (head, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (headTrans, SoTransform, FALSE, head,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (headCone, SoCone, FALSE, head,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (markers, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (font, SoFont, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelSep, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelTrans, SoTranslation, FALSE, labelSep,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelText, SoText2, FALSE, labelSep,\x0, TRUE);

    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoAxis::refresh ()
{
    
    float axisLength ;
    axisLength = (offset.getValue ()) ? (totalLength.getValue ()): (divisionLength.getValue () * divisions.getValue ());   
	

    SoVertexProperty	*vtx = new SoVertexProperty;
    SoLineSet		*line = new  SoLineSet;
    SoTransform		*headTrans = new SoTransform;
    SoCone		*headCone = new SoCone;
    SoSeparator		*markers = new SoSeparator;
    SoFont		*font = new SoFont;
    SoTranslation	*labelTrans = new SoTranslation;
    SoText2		*labelText = new SoText2;

    // Make the axis line
    vtx->vertex.set1Value (0, SbVec3f (0, 0, 0));
    vtx->vertex.set1Value (1, SbVec3f (0, 0, axisLength));
    line->startIndex = 0;
    line->numVertices = 2;
    line->vertexProperty = vtx;

    // Make arrowhead for the end of the line
    float coneHeight; //, coneRadius;

    if (offset.getValue ()) 
    {
	// if total length is smaller than 2.0 then scale the arrow head according to length
	// else assign a fixed value to the cone height
	coneHeight = (axisLength >= 2.0) ? 0.25: axisLength / 8.0;	
    }
    else
    {
	coneHeight = axisLength / 8.0;
    }
   
    headCone->bottomRadius = divisionLength.getValue () / 10;
    headCone->height = coneHeight;
    if (offset.getValue ()) 
    {
	headTrans->translation.setValue (0, 0, axisLength - coneHeight / 2);
    }
    else
    {
	headTrans->translation.setValue (0, 0, axisLength + coneHeight / 2);
    }
    
    headTrans->rotation.setValue (SbVec3f (1, 0, 0), M_PI / 2);

    // Make division markers
    SoRotation *markerRot = new SoRotation;
    markerRot->rotation.setValue (SbVec3f (1, 0, 0), M_PI / 2);
    markers->addChild (markerRot);

    long divs = divisions.getValue () - 1;
    if (divs >= 1)
    {
	SoCylinder *marker = new SoCylinder;
	marker->radius.setValue (divisionLength.getValue () / 10);
	marker->height.setValue (axisLength / 200);

	for (int div = 0; div < divs ; div++)
	{
	    SoTranslation *offset = new SoTranslation;  
	    offset->translation.setValue (0, divisionLength.getValue (), 0);
	    markers->addChild (offset);
	    markers->addChild (marker);
	}
    }

    // Put letter at the ends of the axes
    font->size.setValue (16.0);
    font->name.setValue ("Times-Roman");
    labelTrans->translation.setValue (0, 0, 1.05 * (axisLength + coneHeight));
    labelText->string = label.getValue ();
    labelText->justification = SoText2::CENTER;

    setPart ("line",		line);
    setPart ("headTrans",	headTrans);
    setPart ("headCone",	headCone);
    setPart ("markers",		markers);
    setPart ("font",		font);
    setPart ("labelTrans",	labelTrans);
    setPart ("labelText",	labelText);
}

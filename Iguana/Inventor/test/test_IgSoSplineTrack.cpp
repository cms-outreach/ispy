//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoPointSet.h>
#include <cmath>
#include "test_main.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

const float M_SQRT3_2 =  0.8660254037844F; // sqrt(3)/2 for sin(60 degrees)

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QString makeTest (SoSeparator *root)
{	
    IgSoShapeKit::initClass ();
    IgSoSplineTrack::initClass ();

    // The first track
    SoSeparator *separator1 = new SoSeparator;
    SoMaterial *material1 = new SoMaterial;                      
    material1->diffuseColor.setValue (1.0, 0.0, 0.0); // red colour 
    separator1->addChild (material1);

    IgSoSplineTrack *obj1 = new IgSoSplineTrack;
    SbVec3f p1 [2] = { SbVec3f (1,0,0), SbVec3f (-0.5,M_SQRT3_2,0)};
    SbVec3f t1 [2] = { SbVec3f (0,1,0), SbVec3f (-M_SQRT3_2,-0.5,0)};
    obj1->points.setValues (0, 2, p1);
    obj1->tangents.setValues (0, 2, t1);

    SoVertexProperty *verticies1 = new SoVertexProperty;
    verticies1->vertex.setValues (0, 2, p1);
    SoPointSet *points1 = new SoPointSet;
    points1->vertexProperty.setValue (verticies1);
    points1->numPoints.setValue (2);
    separator1->addChild (points1);
 
    // The second track
    SoSeparator *separator2 = new SoSeparator;
    SoMaterial *material2 = new SoMaterial;                      
    material2->diffuseColor.setValue (0.0, 1.0, 0.0); // green colour 
    separator2->addChild (material2);

    // perfect planar helix
    IgSoSplineTrack *obj2 = new IgSoSplineTrack;
    SbVec3f p2 [5] = { SbVec3f (1,0,0), SbVec3f (0,1,0), SbVec3f (-1,0,0), SbVec3f (0,-1,0), SbVec3f (1,0,0) };
    SbVec3f t2 [5] = { SbVec3f (0,1,0), SbVec3f (-1,0,0), SbVec3f (0,-1,0), SbVec3f (1,0,0), SbVec3f (0,1,0) };
    obj2->points.setValues (0, 5, p2);
    obj2->tangents.setValues (0, 5, t2);

    SoVertexProperty *verticies2 = new SoVertexProperty;
    verticies2->vertex.setValues (0, 5, p2);
    SoPointSet *points2 = new SoPointSet;
    points2->vertexProperty.setValue (verticies2);
    points2->numPoints.setValue (5);
    separator2->addChild (points2);
    
    // The third track
    SoSeparator *separator3 = new SoSeparator;
    SoMaterial *material3 = new SoMaterial;                      
    material3->diffuseColor.setValue (0.0, 0.0, 1.0); // blue colour 
    separator3->addChild (material3);

    // reversed planar helix
    IgSoSplineTrack *obj3 = new IgSoSplineTrack;
    SbVec3f p3 [5] = { SbVec3f (1,0,0), SbVec3f (0,1,0), SbVec3f (-1,0,0),
		       SbVec3f (-2,-1,0), SbVec3f (-3,0,0) };
    SbVec3f t3 [5] = { SbVec3f (0,1,0), SbVec3f (-1,0,0), SbVec3f (0,-1,0),
		       SbVec3f (-1,0,0), SbVec3f (0,1,0) };
    obj3->points.setValues (0, 5, p3);
    obj3->tangents.setValues (0, 5, t3);

    SoVertexProperty *verticies3 = new SoVertexProperty;
    verticies3->vertex.setValues (0, 5, p3);
    SoPointSet *points3 = new SoPointSet;
    points3->vertexProperty.setValue (verticies3);
    points3->numPoints.setValue (5);
    separator3->addChild (points3);
    
    // The fourth track
    SoSeparator *separator4 = new SoSeparator;
    SoMaterial *material4 = new SoMaterial;                      
    material4->diffuseColor.setValue (1.0, 1.0, 0.0); // yellow colour 
    separator4->addChild (material4);

    // non-helix version
    IgSoSplineTrack *obj4 = new IgSoSplineTrack;
    const float delz = 0.3F;
    const float rho = 1.F;
    const float rzeta = delz/(M_PI/2); // zeta*rho
    const float zrtan = rzeta/sqrt(rho*rho+rzeta*rzeta);
    const float rtan = rho/sqrt(rho*rho+rzeta*rzeta);
    SbVec3f p4 [5] = { SbVec3f (rho,0,0), SbVec3f (0,rho,delz), SbVec3f (-rho,0,2*delz),
		       SbVec3f (-2*rho,-rho,3*delz), SbVec3f (-3*rho,0,4*delz) };
    SbVec3f t4 [5] = { SbVec3f (0, rtan,zrtan), SbVec3f (-rtan,0,zrtan),
		       SbVec3f (0,-rtan,zrtan), SbVec3f (-rtan,0,zrtan),
		       SbVec3f (0, rtan,zrtan) };
    obj4->points.setValues (0, 5, p4);
    obj4->tangents.setValues (0, 5, t4);

    SoVertexProperty *verticies4 = new SoVertexProperty;
    verticies4->vertex.setValues (0, 5, p4);
    SoPointSet *points4 = new SoPointSet;
    points4->vertexProperty.setValue (verticies4);
    points4->numPoints.setValue (5);
    separator4->addChild (points4);
    
    // The fivth track
    SoSeparator *separator5 = new SoSeparator;
    SoMaterial *material5 = new SoMaterial;                      
    material5->diffuseColor.setValue (1.0, 1.0, 1.0); // white colour 
    separator5->addChild (material5);

    // straight line
    IgSoSplineTrack *obj5 = new IgSoSplineTrack;
    SbVec3f p5 [3] = { SbVec3f (1,0,0), SbVec3f (1.5,0,0), SbVec3f (2,0,0) };
    SbVec3f t5 [3] = { SbVec3f (1,0,0), SbVec3f (1,0,0), SbVec3f (1,0,0) };
    obj5->points.setValues (0, 3, p5);
    obj5->tangents.setValues (0, 3, t5);

    SoVertexProperty *verticies5 = new SoVertexProperty;
    verticies5->vertex.setValues (0, 3, p5);
    SoPointSet *points5 = new SoPointSet;
    points5->vertexProperty.setValue (verticies5);
    points5->numPoints.setValue (3);
    separator5->addChild (points5);  

    // The sixth track
    SoSeparator *separator6 = new SoSeparator;
    SoMaterial *material6 = new SoMaterial;                      
    material6->diffuseColor.setValue (1.0, 0.0, 1.0); // magenta colour 
    separator6->addChild (material6);
    
    // helix
    IgSoSplineTrack *obj6 = new IgSoSplineTrack;
    SbVec3f p6 [5] = { SbVec3f (rho,0,0), SbVec3f (0,rho,delz), SbVec3f (-rho,0,2*delz),
		       SbVec3f (0,-rho,3*delz), SbVec3f (rho,0,4*delz) };
    SbVec3f t6 [5] = { SbVec3f (0, rtan,zrtan), SbVec3f (-rtan,0,zrtan),
		       SbVec3f (0,-rtan,zrtan), SbVec3f (rtan,0,zrtan),
		       SbVec3f (0, rtan,zrtan) };
    obj6->points.setValues (0, 5, p6);
    obj6->tangents.setValues (0, 5, t6);

    SoVertexProperty *verticies6 = new SoVertexProperty;
    verticies6->vertex.setValues (0, 5, p6);
    SoPointSet *points6 = new SoPointSet;
    points6->vertexProperty.setValue (verticies6);
    points6->numPoints.setValue (5);
    separator6->addChild (points6);  

    separator1->addChild (obj1);
    separator2->addChild (obj2);
    separator3->addChild (obj3);
    separator4->addChild (obj4);
    separator5->addChild (obj5);
    separator6->addChild (obj6);

    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->lineWidth = 2;
    drawStyle->pointSize = 5.0;
    root->addChild (drawStyle);

    root->addChild (separator1);
    root->addChild (separator2);
    root->addChild (separator3);
    root->addChild (separator4);
    root->addChild (separator5);
    root->addChild (separator6);

    return "IGUANA: IgSoSplineTrack Test";
}

#include "QtGUI/IgDrawTowerHelper.h"
#include "Inventor/nodes/SoShapeHints.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include "Inventor/SbMatrix.h"
#include "Inventor/SbColor.h"
#include "QtGUI/Projectors.h"
#include <iostream>

SbVec3f *
IgDrawTowerHelper::projectVertices(SbVec3f *v)
{
  SbVec3f centre = (v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7]) / 8;
  
  for(size_t i = 0; i < 8; ++i)
    v[i] = m_projectors.projectAsWithOffset(v[i], centre);

  return v;
}

IgDrawTowerHelper::IgDrawTowerHelper(SoGroup *group, Projectors &projectors)
  :m_c(0),
   m_l(0),
   m_f(0),
   m_vertices(new SoVertexProperty),
   m_lineSet(new SoIndexedLineSet),
   m_faceSet(new SoIndexedFaceSet),
   m_group(group),
   m_projectors(projectors)
{
  m_lineSet->vertexProperty = m_vertices;
  m_faceSet->vertexProperty = m_vertices;

  // add here but fill them up later with calls to addTower
  m_group->addChild(m_lineSet);
  m_group->addChild(m_faceSet);
  m_lineSet->coordIndex.startEditing();
  m_lineSet->coordIndex.enableNotify(FALSE);
  m_faceSet->coordIndex.startEditing();
  m_faceSet->coordIndex.enableNotify(FALSE);
  m_vertices->vertex.startEditing();
  m_vertices->vertex.enableNotify(FALSE);
}


IgDrawTowerHelper::~IgDrawTowerHelper()
{
  m_lineSet->coordIndex.finishEditing();
  m_lineSet->coordIndex.enableNotify(TRUE);
  m_faceSet->coordIndex.finishEditing();
  m_faceSet->coordIndex.enableNotify(TRUE);
  m_vertices->vertex.finishEditing();
  m_vertices->vertex.enableNotify(TRUE);
}

void
IgDrawTowerHelper::addTower(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                            SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4)
{
  // Project all the points as requested. We use projectAsWithOffset to
  // avoid having to deal with degenerated cubes.
  SbVec3f vertices[8] = { f1, f2, f3, f4, b1, b2, b3, b4};
  
  // Call the bit which actually generates the inventor faceset.
  drawTower(vertices);
}

void
IgDrawTowerHelper::addTowerProjected(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                                     SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4)
{
  // Calculate the centre of the box to decide how to transform the tower
  // in case of discontinuities of the projection.
  SbVec3f vertices[8] = { f1, f2, f3, f4, b1, b2, b3, b4};
  
  // Call the bit which actually generates the inventor faceset.
  drawTower(projectVertices(vertices));
}

void
IgDrawTowerHelper::addTowerOutline(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                                   SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4)
{
  SbVec3f vertices[8] = { f1, f2, f3, f4, b1, b2, b3, b4};

  drawTower(vertices, true);
}

void
IgDrawTowerHelper::addTowerOutlineProjected(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                                            SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4)
{
  SbVec3f vertices[8] = { f1, f2, f3, f4, b1, b2, b3, b4};

  drawTower(projectVertices(vertices), true);
}

void
IgDrawTowerHelper::addTower(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                            SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4,
                            float heightContent = 1.0,
                            float heightScale   = 1.0)
{
  float scale = heightContent * heightScale;
  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = b1-f1;
  SbVec3f diff2 = b2-f2;
  SbVec3f diff3 = b3-f3;
  SbVec3f diff4 = b4-f4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f vertices[8] = { f1, 
                          f2, 
                          f3, 
                          f4,
                          f1 + (diff1 * scale),
                          f2 + (diff2 * scale),
                          f3 + (diff3 * scale),
                          f4 + (diff4 * scale)};

  drawTower(vertices);
}

void
IgDrawTowerHelper::addTowerProjected(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                                     SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4,
                                     float heightContent = 1.0,
                                     float heightScale   = 1.0)
{
  float scale = heightContent * heightScale;

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures
  SbVec3f diff1 = b1-f1;
  SbVec3f diff2 = b2-f2;
  SbVec3f diff3 = b3-f3;
  SbVec3f diff4 = b4-f4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f vertices[8] = { f1, 
                          f2, 
                          f3, 
                          f4,
                          f1 + (diff1 * scale),
                          f2 + (diff2 * scale),
                          f3 + (diff3 * scale),
                          f4 + (diff4 * scale)};

  drawTower(projectVertices(vertices));
}


void
IgDrawTowerHelper::addTower(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                            SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4,
                            float heightContent = 1, float heightOffset = 1,
                            float heightScale = 1)
{
  float scaleContent = (heightContent+heightOffset) * heightScale;
  float scaleOffset = heightOffset * heightScale;

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = b1-f1;
  SbVec3f diff2 = b2-f2;
  SbVec3f diff3 = b3-f3;
  SbVec3f diff4 = b4-f4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f sc[8] = { f1 + diff1*scaleOffset,
                    f2 + diff2*scaleOffset,
                    f3 + diff3*scaleOffset,
                    f4 + diff4*scaleOffset,
                    f1 + diff1*scaleOffset + diff1*scaleContent,
                    f2 + diff2*scaleOffset + diff2*scaleContent,
                    f3 + diff3*scaleOffset + diff3*scaleContent,
                    f4 + diff4*scaleOffset + diff4*scaleContent};

  drawTower(sc);
}

void
IgDrawTowerHelper::addTowerProjected(SbVec3f &f1, SbVec3f &f2, SbVec3f &f3, SbVec3f &f4,
                                     SbVec3f &b1, SbVec3f &b2, SbVec3f &b3, SbVec3f &b4,
                                     float heightContent = 1, float heightOffset = 1,
                                     float heightScale = 1)
{
  float scaleContent = (heightContent+heightOffset) * heightScale;
  float scaleOffset = heightOffset * heightScale;

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = b1-f1;
  SbVec3f diff2 = b2-f2;
  SbVec3f diff3 = b3-f3;
  SbVec3f diff4 = b4-f4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f sc[8] = {f1 + diff1*scaleOffset,
                   f2 + diff2*scaleOffset,
                   f3 + diff3*scaleOffset,
                   f4 + diff4*scaleOffset,
                   f1 + diff1*scaleOffset + diff1*scaleContent,
                   f2 + diff2*scaleOffset + diff2*scaleContent,
                   f3 + diff3*scaleOffset + diff3*scaleContent,
                   f4 + diff4*scaleOffset + diff4*scaleContent};

  drawTower(projectVertices(sc));
}


void
IgDrawTowerHelper::addScaledBox(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                                float scaleFraction)
{
  SbVec3f centre = (f1 + f2 + f3 + f4 + b1 + b2 + b3 + b4) / 8.0;

  // Coordinates for a scaled version of the original box

  SbVec3f sc[8] = { centre + (f1-centre)*scaleFraction,
                    centre + (f2-centre)*scaleFraction,
                    centre + (f3-centre)*scaleFraction,
                    centre + (f4-centre)*scaleFraction,
                    centre + (b1-centre)*scaleFraction,
                    centre + (b2-centre)*scaleFraction,
                    centre + (b3-centre)*scaleFraction,
                    centre + (b4-centre)*scaleFraction };

  drawTower(sc);
}

void
IgDrawTowerHelper::addScaledBoxProjected(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                                         SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                                         float scaleFraction)
{
  SbVec3f centre = (f1 + f2 + f3 +f4 + b1 + b2 + b3 + b4) / 8.0;

  // Coordinates for a scaled version of the original box

  SbVec3f sc[8] = { centre + (f1-centre)*scaleFraction,
                    centre + (f2-centre)*scaleFraction,
                    centre + (f3-centre)*scaleFraction,
                    centre + (f4-centre)*scaleFraction,
                    centre + (b1-centre)*scaleFraction,
                    centre + (b2-centre)*scaleFraction,
                    centre + (b3-centre)*scaleFraction,
                    centre + (b4-centre)*scaleFraction};

  drawTower(projectVertices(sc));
}


void
IgDrawTowerHelper::drawTower(SbVec3f *vertices, bool outline)
{
  m_vertices->vertex.setNum(m_c + 8); // increase vector length to accomodate next 8 vertices
  m_vertices->vertex.setValues(m_c, 8, vertices);
  
  int lineIndexes[24] = {0 + m_c, 1 + m_c, 2 + m_c, 3 + m_c, 0 + m_c, SO_END_LINE_INDEX,
                         4 + m_c, 5 + m_c, 6 + m_c, 7 + m_c, 4 + m_c, SO_END_LINE_INDEX,
                         0 + m_c, 4 + m_c, SO_END_LINE_INDEX,
                         1 + m_c, 5 + m_c, SO_END_LINE_INDEX,
                         2 + m_c, 6 + m_c, SO_END_LINE_INDEX,
                         3 + m_c, 7 + m_c, SO_END_LINE_INDEX};
  m_lineSet->coordIndex.setNum(m_l + 24);
  m_lineSet->coordIndex.setValues(m_l, 24, lineIndexes);
  m_l += 24;
  
  if (!outline)
  {
    // 6 faces, each with 4 corners(normals should point outwards)
    int faceIndexes[30] = {3 + m_c, 2 + m_c, 1 + m_c, 0 + m_c, SO_END_FACE_INDEX,
                           4 + m_c, 5 + m_c, 6 + m_c, 7 + m_c, SO_END_FACE_INDEX,
                           5 + m_c, 1 + m_c, 2 + m_c, 6 + m_c, SO_END_FACE_INDEX,
                           2 + m_c, 3 + m_c, 7 + m_c, 6 + m_c, SO_END_FACE_INDEX,
                           7 + m_c, 3 + m_c, 0 + m_c, 4 + m_c, SO_END_FACE_INDEX,
                           1 + m_c, 5 + m_c, 4 + m_c, 0 + m_c, SO_END_FACE_INDEX};
    
    m_faceSet->coordIndex.setNum(m_f + 30);
    m_faceSet->coordIndex.setValues(m_f, 30, faceIndexes);
    m_f += 30;
  }
  m_c += 8;
}

void
IgDrawTowerHelper::addLegoTower(SbVec2f position, float energy, float emFraction,
				float scaleFactor, float etaWidth, float phiWidth) 
{
  SoShapeHints *shapeHints = new SoShapeHints;
  SoIndexedFaceSet *faces = new SoIndexedFaceSet;
  SoVertexProperty *vtx = new SoVertexProperty;
 
  int faceIndices [] = 
    {0, 1, 5, 4, SO_END_FACE_INDEX,
     3, 7, 6, 2, SO_END_FACE_INDEX,
     1, 2, 6, 5, SO_END_FACE_INDEX,
     5, 6, 7, 4, SO_END_FACE_INDEX,
     4, 7, 3, 0, SO_END_FACE_INDEX,
     0, 3, 2, 1, SO_END_FACE_INDEX,

      8,  9, 13, 12, SO_END_FACE_INDEX,
     11, 15, 14, 10, SO_END_FACE_INDEX,
      9, 10, 14, 13, SO_END_FACE_INDEX,
     13, 14, 15, 12, SO_END_FACE_INDEX,
     12, 15, 11,  8, SO_END_FACE_INDEX,
      8, 11, 10,  9, SO_END_FACE_INDEX
    };
    
  float	cx, cz;
  float	dx = etaWidth / 2.0F;
  float	dz = phiWidth / 2.0F;
  float cMax = energy / scaleFactor;
  float	cMin = cMax * emFraction;
  bool 	noEm;
  bool 	noHm;
  bool 	top = false;
  bool	bottom = false;
    
  (emFraction == 0.0) ? noEm = true : noEm = false;
  (emFraction == 1.0) ? noHm = true : noHm = false;
    
  position.getValue (cx, cz);

  if (cMax < 0.0)
  {
    shapeHints->vertexOrdering = SoShapeHints::CLOCKWISE;
  }
  else
  {
    shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  }
    
  shapeHints->shapeType = SoShapeHints::SOLID;
  shapeHints->faceType = SoShapeHints::CONVEX;
    
  int nverts = 16;
  std::vector<SbVec3f> vertexData (nverts);

  if (! noEm) 
  {		
    vertexData [0][0] = cx - dx;
    vertexData [0][1] = cMin;
    vertexData [0][2] = cz - dz;
    
    vertexData [1][0] = cx + dx;
    vertexData [1][1] = cMin;
    vertexData [1][2] = cz - dz;

    vertexData [2][0] = cx + dx;
    vertexData [2][1] = cMin;
    vertexData [2][2] = cz + dz;
    
    vertexData [3][0] = cx - dx;
    vertexData [3][1] = cMin;
    vertexData [3][2] = cz + dz;

    vertexData [4][0] = cx - dx;
    vertexData [4][1] = 0;
    vertexData [4][2] = cz - dz;

    vertexData [5][0] = cx + dx;
    vertexData [5][1] = 0;
    vertexData [5][2] = cz - dz;

    vertexData [6][0] = cx + dx;
    vertexData [6][1] = 0;
    vertexData [6][2] = cz + dz;

    vertexData [7][0] = cx - dx;
    vertexData [7][1] = 0;
    vertexData [7][2] = cz + dz;
  
    bottom = true;
  } 
    
  if (! noHm) 
  {
    vertexData [8][0] = cx - dx;
    vertexData [8][1] = cMax;
    vertexData [8][2] = cz - dz;
    
    vertexData [9][0] = cx + dx;
    vertexData [9][1] = cMax;
    vertexData [9][2] = cz - dz;

    vertexData [10][0] = cx + dx;
    vertexData [10][1] = cMax;
    vertexData [10][2] = cz + dz;
    
    vertexData [11][0] = cx - dx;
    vertexData [11][1] = cMax;
    vertexData [11][2] = cz + dz;

    vertexData [12][0] = cx - dx;
    vertexData [12][1] = cMin;
    vertexData [12][2] = cz - dz;

    vertexData [13][0] = cx + dx;
    vertexData [13][1] = cMin;
    vertexData [13][2] = cz - dz;

    vertexData [14][0] = cx + dx;
    vertexData [14][1] = cMin;
    vertexData [14][2] = cz + dz;

    vertexData [15][0] = cx - dx;
    vertexData [15][1] = cMin;
    vertexData [15][2] = cz + dz;
	
    top = true;
  }
    
  vtx->materialBinding = SoMaterialBinding::PER_FACE;

  if (top && (! bottom))
  {
    vtx->orderedRGBA = SbColor(0.4,0.8,1.0).getPackedValue();

    vtx->vertex.setValues (0, 8, &vertexData [8]);
    faces->vertexProperty = vtx;
    faces->coordIndex.setValues (0, 6 * 5, faceIndices);
    m_group->addChild(shapeHints);
    m_group->addChild(faces);
  }
  else if ((! top) && bottom)
  {
    vtx->orderedRGBA = SbColor(1.0, 0.2, 0.0).getPackedValue();

    vtx->vertex.setValues (0, 8, &vertexData [0]);
    faces->vertexProperty = vtx;
    faces->coordIndex.setValues (0, 6 * 5, faceIndices);
    m_group->addChild(shapeHints);
    m_group->addChild(faces);
  }
}

/** Helper function to draw a box that is rotated with respect to 
    the origin. 
  */
void
IgDrawTowerHelper::addRotatedBox(const SbVec3f &pos, const SbVec3f &axis,
                                 double angle, double w, double h, double d)
{
  // 
  // Cube vertices
  // (-1,1,-1) (1,1,-1) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1) (1,-1,-1) (-1,-1,-1)
  SbVec3f vertices[8] = {
    SbVec3f(- w / 2,   h / 2, - d / 2),
    SbVec3f(  w / 2,   h / 2, - d / 2),
    SbVec3f(  w / 2,   h / 2,   d / 2),
    SbVec3f(- w / 2,   h / 2,   d / 2),
    SbVec3f(- w / 2, - h / 2,   d / 2),
    SbVec3f(  w / 2, - h / 2,   d / 2),
    SbVec3f(  w / 2, - h / 2, - d / 2),
    SbVec3f(- w / 2, - h / 2, - d / 2)
  };

  int indices[30] = { 3 + m_c, 2 + m_c, 1 + m_c, 0 + m_c,  SO_END_FACE_INDEX,
                      7 + m_c, 6 + m_c, 5 + m_c, 4 + m_c,  SO_END_FACE_INDEX,
                      4 + m_c, 5 + m_c, 2 + m_c, 3 + m_c,  SO_END_FACE_INDEX,
                      5 + m_c, 6 + m_c, 1 + m_c, 2 + m_c,  SO_END_FACE_INDEX,
                      6 + m_c, 7 + m_c, 0 + m_c, 1 + m_c,  SO_END_FACE_INDEX,
                      7 + m_c, 4 + m_c, 3 + m_c, 0 + m_c,  SO_END_FACE_INDEX};


  SbVec3f tmpVertices[8];
  SbMatrix matrix;
  SbVec3f a = axis;
  a.normalize();
  matrix.setTransform(pos, SbRotation(a, angle), SbVec3f(1,1,1));

  for (int i = 0; i < 8; ++i)
    matrix.multVecMatrix(vertices[i], tmpVertices[i]);

  // Do the projection for all the vertices.
  // Use the center to decide which map to use for the projections.
  SbVec3f outPoints[8];

  for (int i = 0; i < 8; ++i)
  {
    SbVec3f rotatedPoint(tmpVertices[i][0], tmpVertices[i][1],tmpVertices[i][2]);
    outPoints[i] = m_projectors.projectAsWithOffset(rotatedPoint, pos);
  }
  
  // Put all the vertices.
  m_vertices->vertex.setNum(m_c + 8);
  m_vertices->vertex.setValues(m_c, 8, outPoints);
  m_faceSet->coordIndex.setNum(m_f + 30);
  m_faceSet->coordIndex.setValues(m_f, 30, indices);
  m_c += 8;
  m_f += 30;
}

void
IgDrawTowerHelper::setLineIndices(int p1, int p2, int p3, int p4, int p5)
{
  int values[6] = {m_c + p1, m_c + p2, m_c + p3, 
                   m_c + p4, m_c + p5, SO_END_LINE_INDEX};
  m_lineSet->coordIndex.setNum(m_l + 6);
  m_lineSet->coordIndex.setValues(m_l, 6, values);
  m_l += 6;
}

void
IgDrawTowerHelper::setLineIndices(int p1, int p2)
{
  int values[3] = {m_c + p1, m_c + p2, SO_END_LINE_INDEX};
  m_lineSet->coordIndex.setNum(m_l + 3);
  m_lineSet->coordIndex.setValues(m_l, 3, values);
  m_l += 3;
}

void
IgDrawTowerHelper::setFaceIndices(int p1, int p2, int p3, int p4)
{
  int values[5] = {m_c + p1, m_c + p2, m_c + p3, 
                   m_c + p4, SO_END_LINE_INDEX};
  m_faceSet->coordIndex.setNum(m_f + 5);
  m_faceSet->coordIndex.setValues(m_f, 5, values);
  m_f += 5;
}

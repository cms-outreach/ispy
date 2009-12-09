#include "Iguana/QtGUI/interface/IgDrawTowerHelper.h"
#include "Inventor/nodes/SoShapeHints.h"
#include "Inventor/SbMatrix.h"
#include "Inventor/SbColor.h"
#include "Iguana/QtGUI/src/Projectors.h"
#include <iostream>

SbVec3f *
IgDrawTowerHelper::projectVertices(SbVec3f *vertices)
{
  // Doing the projection:
  // Sigh... transforming things back and forward... At least I know
  // where to optimize things.
  SbVec3f soCenter = (  vertices[0] + vertices[1] + vertices[2] + vertices[3]
                     + vertices[4] + vertices[5] + vertices[6] + vertices[7]) / 8;
  IgV3d centre(soCenter[0], soCenter[1], soCenter[2]);
  
  for(size_t i = 0; i < 8; ++i)
  {
    IgV3d v(vertices[i][0], vertices[i][1], vertices[i][2]);
    vertices[i] = m_projectors.projectAsWithOffset(v, centre);
  }
  return vertices;
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
IgDrawTowerHelper::addTower(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                            IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
{
  // Project all the points as requested. We use projectAsWithOffset to
  // avoid having to deal with degenerated cubes.
  SbVec3f vertices[8] = { SbVec3f(f1.x(), f1.y(), f1.z()),
                          SbVec3f(f2.x(), f2.y(), f2.z()),
                          SbVec3f(f3.x(), f3.y(), f3.z()),
                          SbVec3f(f4.x(), f4.y(), f4.z()),
                          SbVec3f(b1.x(), b1.y(), b1.z()),
                          SbVec3f(b2.x(), b2.y(), b2.z()),
                          SbVec3f(b3.x(), b3.y(), b3.z()),
                          SbVec3f(b4.x(), b4.y(), b4.z())};
  
  // Call the bit which actually generates the inventor faceset.
  drawTower(vertices);
}

void
IgDrawTowerHelper::addTowerProjected(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                                     IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
{
  // Calculate the centre of the box to decide how to transform the tower
  // in case of discontinuities of the projection.
  double centreX = (f1.x() + f2.x() + f3.x() + f4.x() + b1.x() + b2.x() + b3.x() + b4.x()) / 8;
  double centreY = (f1.y() + f2.y() + f3.y() + f4.y() + b1.y() + b2.y() + b3.y() + b4.y()) / 8;
  double centreZ = (f1.z() + f2.z() + f3.z() + f4.z() + b1.z() + b2.z() + b3.z() + b4.z()) / 8;
  IgV3d centre(centreX, centreY, centreZ); 

  // Project all the points as requested. We use projectAsWithOffset to
  // avoid having to deal with degenerated cubes.
  SbVec3f vertices[8] = { m_projectors.projectAsWithOffset(f1, centre),
                          m_projectors.projectAsWithOffset(f2, centre),
                          m_projectors.projectAsWithOffset(f3, centre),
                          m_projectors.projectAsWithOffset(f4, centre),
                          m_projectors.projectAsWithOffset(b1, centre),
                          m_projectors.projectAsWithOffset(b2, centre),
                          m_projectors.projectAsWithOffset(b3, centre),
                          m_projectors.projectAsWithOffset(b4, centre)};
  
  // Call the bit which actually generates the inventor faceset.
  drawTower(vertices);
}

void
IgDrawTowerHelper::addTowerOutline(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                                   IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
{
  // Project all the points as requested. We use projectAsWithOffset to
  // avoid having to deal with degenerated cubes.
  SbVec3f vertices[8] = { SbVec3f(f1.x(), f1.y(), f1.z()),
                          SbVec3f(f2.x(), f2.y(), f2.z()),
                          SbVec3f(f3.x(), f3.y(), f3.z()),
                          SbVec3f(f4.x(), f4.y(), f4.z()),
                          SbVec3f(b1.x(), b1.y(), b1.z()),
                          SbVec3f(b2.x(), b2.y(), b2.z()),
                          SbVec3f(b3.x(), b3.y(), b3.z()),
                          SbVec3f(b4.x(), b4.y(), b4.z()) };

  drawTowerOutline(vertices);
}

void
IgDrawTowerHelper::addTowerOutlineProjected(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                                            IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
{
  // Calculate the centre of the box to decide how to transform the tower
  // in case of discontinuities of the projection.
  double centreX = (f1.x() + f2.x() + f3.x() + f4.x() + b1.x() + b2.x() + b3.x() + b4.x()) / 8;
  double centreY = (f1.y() + f2.y() + f3.y() + f4.y() + b1.y() + b2.y() + b3.y() + b4.y()) / 8;
  double centreZ = (f1.z() + f2.z() + f3.z() + f4.z() + b1.z() + b2.z() + b3.z() + b4.z()) / 8;
  IgV3d centre(centreX, centreY, centreZ); 

  // Project all the points as requested. We use projectAsWithOffset to
  // avoid having to deal with degenerated cubes.
  SbVec3f vertices[8] = { m_projectors.projectAsWithOffset(f1, centre),
                          m_projectors.projectAsWithOffset(f2, centre),
                          m_projectors.projectAsWithOffset(f3, centre),
                          m_projectors.projectAsWithOffset(f4, centre),
                          m_projectors.projectAsWithOffset(b1, centre),
                          m_projectors.projectAsWithOffset(b2, centre),
                          m_projectors.projectAsWithOffset(b3, centre),
                          m_projectors.projectAsWithOffset(b4, centre)};

  drawTowerOutline(vertices);
}

void
IgDrawTowerHelper::addTower(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                            IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4,
                            float heightContent = 1.0,
                            float heightScale   = 1.0)
{
  float scale = heightContent * heightScale;

  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(f1.x(), f1.y(), f1.z());
  SbVec3f sf2(f2.x(), f2.y(), f2.z());
  SbVec3f sf3(f3.x(), f3.y(), f3.z());
  SbVec3f sf4(f4.x(), f4.y(), f4.z());

  SbVec3f sb1(b1.x(), b1.y(), b1.z());
  SbVec3f sb2(b2.x(), b2.y(), b2.z());
  SbVec3f sb3(b3.x(), b3.y(), b3.z());
  SbVec3f sb4(b4.x(), b4.y(), b4.z());

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = sb1-sf1;
  SbVec3f diff2 = sb2-sf2;
  SbVec3f diff3 = sb3-sf3;
  SbVec3f diff4 = sb4-sf4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f vertices[8] = { sf1, 
                          sf2, 
                          sf3, 
                          sf4,
                          sf1 + (diff1 * scale),
                          sf2 + (diff2 * scale),
                          sf3 + (diff3 * scale),
                          sf4 + (diff4 * scale)};

  drawTower(vertices);
}

void
IgDrawTowerHelper::addTowerProjected(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                                     IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4,
                                     float heightContent = 1.0,
                                     float heightScale   = 1.0)
{
  float scale = heightContent * heightScale;

  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(f1.x(), f1.y(), f1.z());
  SbVec3f sf2(f2.x(), f2.y(), f2.z());
  SbVec3f sf3(f3.x(), f3.y(), f3.z());
  SbVec3f sf4(f4.x(), f4.y(), f4.z());

  SbVec3f sb1(b1.x(), b1.y(), b1.z());
  SbVec3f sb2(b2.x(), b2.y(), b2.z());
  SbVec3f sb3(b3.x(), b3.y(), b3.z());
  SbVec3f sb4(b4.x(), b4.y(), b4.z());

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = sb1-sf1;
  SbVec3f diff2 = sb2-sf2;
  SbVec3f diff3 = sb3-sf3;
  SbVec3f diff4 = sb4-sf4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f vertices[8] = { sf1, 
                          sf2, 
                          sf3, 
                          sf4,
                          sf1 + (diff1 * scale),
                          sf2 + (diff2 * scale),
                          sf3 + (diff3 * scale),
                          sf4 + (diff4 * scale)};

  drawTower(projectVertices(vertices));
}


void
IgDrawTowerHelper::addTower(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                            IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4,
                            float heightContent = 1, float heightOffset = 1,
                            float heightScale = 1)
{
  float scaleContent = (heightContent+heightOffset) * heightScale;
  float scaleOffset = heightOffset * heightScale;

  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(f1.x(), f1.y(), f1.z());
  SbVec3f sf2(f2.x(), f2.y(), f2.z());
  SbVec3f sf3(f3.x(), f3.y(), f3.z());
  SbVec3f sf4(f4.x(), f4.y(), f4.z());

  SbVec3f sb1(b1.x(), b1.y(), b1.z());
  SbVec3f sb2(b2.x(), b2.y(), b2.z());
  SbVec3f sb3(b3.x(), b3.y(), b3.z());
  SbVec3f sb4(b4.x(), b4.y(), b4.z());

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = sb1-sf1;
  SbVec3f diff2 = sb2-sf2;
  SbVec3f diff3 = sb3-sf3;
  SbVec3f diff4 = sb4-sf4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f sc[8] = {sf1 + diff1*scaleOffset,
                   sf2 + diff2*scaleOffset,
                   sf3 + diff3*scaleOffset,
                   sf4 + diff4*scaleOffset,
                   sf1 + diff1*scaleOffset + diff1*scaleContent,
                   sf2 + diff2*scaleOffset + diff2*scaleContent,
                   sf3 + diff3*scaleOffset + diff3*scaleContent,
                   sf4 + diff4*scaleOffset + diff4*scaleContent};

  drawTower(sc);
}

void
IgDrawTowerHelper::addTowerProjected(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                                     IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4,
                                     float heightContent = 1, float heightOffset = 1,
                                     float heightScale = 1)
{
  float scaleContent = (heightContent+heightOffset) * heightScale;
  float scaleOffset = heightOffset * heightScale;

  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(f1.x(), f1.y(), f1.z());
  SbVec3f sf2(f2.x(), f2.y(), f2.z());
  SbVec3f sf3(f3.x(), f3.y(), f3.z());
  SbVec3f sf4(f4.x(), f4.y(), f4.z());

  SbVec3f sb1(b1.x(), b1.y(), b1.z());
  SbVec3f sb2(b2.x(), b2.y(), b2.z());
  SbVec3f sb3(b3.x(), b3.y(), b3.z());
  SbVec3f sb4(b4.x(), b4.y(), b4.z());

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  SbVec3f diff1 = sb1-sf1;
  SbVec3f diff2 = sb2-sf2;
  SbVec3f diff3 = sb3-sf3;
  SbVec3f diff4 = sb4-sf4;

  diff1.normalize();
  diff2.normalize();
  diff3.normalize();
  diff4.normalize();

  SbVec3f sc[8] = {sf1 + diff1*scaleOffset,
                   sf2 + diff2*scaleOffset,
                   sf3 + diff3*scaleOffset,
                   sf4 + diff4*scaleOffset,
                   sf1 + diff1*scaleOffset + diff1*scaleContent,
                   sf2 + diff2*scaleOffset + diff2*scaleContent,
                   sf3 + diff3*scaleOffset + diff3*scaleContent,
                   sf4 + diff4*scaleOffset + diff4*scaleContent};

  drawTower(projectVertices(sc));
}


void
IgDrawTowerHelper::addScaledBox( IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                                 IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                                 float scaleFraction)
{
  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(f1.x(), f1.y(), f1.z());
  SbVec3f sf2(f2.x(), f2.y(), f2.z());
  SbVec3f sf3(f3.x(), f3.y(), f3.z());
  SbVec3f sf4(f4.x(), f4.y(), f4.z());

  SbVec3f sb1(b1.x(), b1.y(), b1.z());
  SbVec3f sb2(b2.x(), b2.y(), b2.z());
  SbVec3f sb3(b3.x(), b3.y(), b3.z());
  SbVec3f sb4(b4.x(), b4.y(), b4.z());

  SbVec3f centre = (sf1 + sf2 + sf3 +sf4 + sb1 + sb2 + sb3 + sb4) / 8.0;

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  // Coordinates for a scaled version of the original box

  SbVec3f sc[8] = { centre + (sf1-centre)*scaleFraction,
                    centre + (sf2-centre)*scaleFraction,
                    centre + (sf3-centre)*scaleFraction,
                    centre + (sf4-centre)*scaleFraction,
                    centre + (sb1-centre)*scaleFraction,
                    centre + (sb2-centre)*scaleFraction,
                    centre + (sb3-centre)*scaleFraction,
                    centre + (sb4-centre)*scaleFraction};

  drawTower(sc);
}

void
IgDrawTowerHelper::addScaledBoxProjected( IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                                          IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                                          float scaleFraction)
{
  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(f1.x(), f1.y(), f1.z());
  SbVec3f sf2(f2.x(), f2.y(), f2.z());
  SbVec3f sf3(f3.x(), f3.y(), f3.z());
  SbVec3f sf4(f4.x(), f4.y(), f4.z());

  SbVec3f sb1(b1.x(), b1.y(), b1.z());
  SbVec3f sb2(b2.x(), b2.y(), b2.z());
  SbVec3f sb3(b3.x(), b3.y(), b3.z());
  SbVec3f sb4(b4.x(), b4.y(), b4.z());

  SbVec3f centre = (sf1 + sf2 + sf3 +sf4 + sb1 + sb2 + sb3 + sb4) / 8.0;

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  // Coordinates for a scaled version of the original box

  SbVec3f sc[8] = { centre + (sf1-centre)*scaleFraction,
                    centre + (sf2-centre)*scaleFraction,
                    centre + (sf3-centre)*scaleFraction,
                    centre + (sf4-centre)*scaleFraction,
                    centre + (sb1-centre)*scaleFraction,
                    centre + (sb2-centre)*scaleFraction,
                    centre + (sb3-centre)*scaleFraction,
                    centre + (sb4-centre)*scaleFraction};

  drawTower(projectVertices(sc));
}


void
IgDrawTowerHelper::drawTower(SbVec3f *vertices)
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

void
IgDrawTowerHelper::drawTowerOutline(SbVec3f *vertices)
{
  m_vertices->vertex.setValues(m_c, 8 , vertices);

  setLineIndices(0, 1, 2, 3, 0); // front face
  setLineIndices(4, 5, 6, 7, 4); // back face
  setLineIndices(0, 4);          // edge linking front and back face
  setLineIndices(1, 5);          // edge linking front and back face
  setLineIndices(2, 6);          // edge linking front and back face
  setLineIndices(3, 7);          // edge linking front and back face

  m_c += 8;
}

/** Helper function to draw a box that is rotated with respect to 
    the origin. 
  */
void
IgDrawTowerHelper::addRotatedBox(IgV3d &pos, IgV3d &axis, double angle,
                                double w, double h, double d)
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
  SbVec3f a(axis.x(), axis.y(), axis.z());
  a.normalize();
  matrix.setTransform(SbVec3f(pos.x(), pos.y(), pos.z()),
                      SbRotation(a, angle),
                      SbVec3f(1,1,1));

  for (int i = 0; i < 8; ++i)
    matrix.multVecMatrix(vertices[i], tmpVertices[i]);

  // Extremely ugly, but since we need to do the projection 
  // anyways... Do the projection for all the vertices.
  // Use the center to decide which map to use for the projections.
  IgV3d centre(pos.x(), pos.y(), pos.z());
  SbVec3f outPoints[8];

  for (int i = 0; i < 8; ++i)
  {
    IgV3d rotatedPoint(tmpVertices[i][0], tmpVertices[i][1],tmpVertices[i][2]);
    outPoints[i] = m_projectors.projectAsWithOffset(rotatedPoint, centre);
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

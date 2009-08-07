#include "Iguana/QtGUI/interface/IgDrawTowerHelper.h"
#include "Inventor/nodes/SoShapeHints.h"
#include "Inventor/SbColor.h"

IgDrawTowerHelper::IgDrawTowerHelper(SoGroup *group)
  :m_c(0),
   m_l(0),
   m_f(0),
   m_vertices(new SoVertexProperty),
   m_lineSet(new SoIndexedLineSet),
   m_faceSet(new SoIndexedFaceSet),
   m_group(group)

{
  m_lineSet->vertexProperty = m_vertices;
  m_faceSet->vertexProperty = m_vertices;

  // add here but fill them up later with calls to addTower
  m_group->addChild(m_lineSet);
  m_group->addChild(m_faceSet);
}

IgDrawTowerHelper::~IgDrawTowerHelper()
{}

void
IgDrawTowerHelper::addTower(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
                            IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
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

  drawTower(sf1, sf2, sf3, sf4,
            sb1, sb2, sb3, sb4);
}

void
IgDrawTowerHelper::addTowerOutline(IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
				   IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
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

  drawTowerOutline(sf1, sf2, sf3, sf4,
		   sb1, sb2, sb3, sb4);
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

  diff1 *=scale;
  diff2 *=scale;
  diff3 *=scale;
  diff4 *=scale;

  SbVec3f sc_sb1 = sf1 + diff1;
  SbVec3f sc_sb2 = sf2 + diff2;
  SbVec3f sc_sb3 = sf3 + diff3;
  SbVec3f sc_sb4 = sf4 + diff4;

  drawTower(   sf1,    sf2,    sf3,    sf4,
               sc_sb1, sc_sb2, sc_sb3, sc_sb4);
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

  SbVec3f sc_sf1 = sf1 + diff1*scaleOffset;
  SbVec3f sc_sf2 = sf2 + diff2*scaleOffset;
  SbVec3f sc_sf3 = sf3 + diff3*scaleOffset;
  SbVec3f sc_sf4 = sf4 + diff4*scaleOffset;

  SbVec3f sc_sb1 = sc_sf1 + diff1*scaleContent;
  SbVec3f sc_sb2 = sc_sf2 + diff2*scaleContent;
  SbVec3f sc_sb3 = sc_sf3 + diff3*scaleContent;
  SbVec3f sc_sb4 = sc_sf4 + diff4*scaleContent;

  drawTower(sc_sf1, sc_sf2, sc_sf3, sc_sf4,
	    sc_sb1, sc_sb2, sc_sb3, sc_sb4);
}

void
IgDrawTowerHelper::addScaledBox( IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                                 IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                                 float scaleFraction)
{
  // FIXME LT: the following is horribly clunky
  // FIXME LT: somebody clever can reduce the following to a couple of lines

  SbVec3f sf1(static_cast<double>(f1.x()),static_cast<double>(f1.y()),static_cast<double>(f1.z()));
  SbVec3f sf2(static_cast<double>(f2.x()),static_cast<double>(f2.y()),static_cast<double>(f2.z()));
  SbVec3f sf3(static_cast<double>(f3.x()),static_cast<double>(f3.y()),static_cast<double>(f3.z()));
  SbVec3f sf4(static_cast<double>(f4.x()),static_cast<double>(f4.y()),static_cast<double>(f4.z()));

  SbVec3f sb1(static_cast<double>(b1.x()),static_cast<double>(b1.y()),static_cast<double>(b1.z()));
  SbVec3f sb2(static_cast<double>(b2.x()),static_cast<double>(b2.y()),static_cast<double>(b2.z()));
  SbVec3f sb3(static_cast<double>(b3.x()),static_cast<double>(b3.y()),static_cast<double>(b3.z()));
  SbVec3f sb4(static_cast<double>(b4.x()),static_cast<double>(b4.y()),static_cast<double>(b4.z()));

  SbVec3f centre = (sf1 + sf2 + sf3 +sf4 + sb1 + sb2 + sb3 + sb4) / 8.0;

  // FIXME LT: I tried compressing this in the obvious mathematical way
  // FIXME LT: but got in a bizarre mess with types and signatures

  // Coordinates for a scaled version of the original box

  SbVec3f sc_sf1 = centre + (sf1-centre)*scaleFraction ;
  SbVec3f sc_sf2 = centre + (sf2-centre)*scaleFraction ;
  SbVec3f sc_sf3 = centre + (sf3-centre)*scaleFraction ;
  SbVec3f sc_sf4 = centre + (sf4-centre)*scaleFraction ;

  SbVec3f sc_sb1 = centre + (sb1-centre)*scaleFraction ;
  SbVec3f sc_sb2 = centre + (sb2-centre)*scaleFraction ;
  SbVec3f sc_sb3 = centre + (sb3-centre)*scaleFraction ;
  SbVec3f sc_sb4 = centre + (sb4-centre)*scaleFraction ;

  drawTower(sc_sf1, sc_sf2, sc_sf3, sc_sf4,
            sc_sb1, sc_sb2, sc_sb3, sc_sb4);
}

void
IgDrawTowerHelper::drawTower(SbVec3f &sf1, SbVec3f &sf2, SbVec3f &sf3, SbVec3f &sf4,
                             SbVec3f &sb1, SbVec3f &sb2, SbVec3f &sb3, SbVec3f &sb4)
{
  m_vertices->vertex.setNum(m_c + 8); // increase vector length to accomodate next 8 vertices

  m_vertices->vertex.set1Value(m_c,   sf1);
  m_vertices->vertex.set1Value(m_c+1, sf2);
  m_vertices->vertex.set1Value(m_c+2, sf3);
  m_vertices->vertex.set1Value(m_c+3, sf4);

  m_vertices->vertex.set1Value(m_c+4, sb1);
  m_vertices->vertex.set1Value(m_c+5, sb2);
  m_vertices->vertex.set1Value(m_c+6, sb3);
  m_vertices->vertex.set1Value(m_c+7, sb4);

  setLineIndices(0, 1, 2, 3, 0); // front face
  setLineIndices(4, 5, 6, 7, 4); // back face
  setLineIndices(0, 4);          // edge linking front and back face
  setLineIndices(1, 5);          // edge linking front and back face
  setLineIndices(2, 6);          // edge linking front and back face
  setLineIndices(3, 7);          // edge linking front and back face

  // 6 faces, each with 4 corners(normals should point outwards)
  setFaceIndices(3, 2, 1, 0);
  setFaceIndices(4, 5, 6, 7);
  setFaceIndices(5, 1, 2, 6);
  setFaceIndices(2, 3, 7, 6);
  setFaceIndices(7, 3, 0, 4);
  setFaceIndices(1, 5, 4, 0);

  m_c += 8;
}

void
IgDrawTowerHelper::addLegoTower(SbVec2f position, float energy, float emFraction,
				float scaleFactor, float etaWidth, float phiWidth) 
{
  SoMFUInt32 orderedRGBA;	//< materials
  orderedRGBA.set1Value (0, SbColor (0.622, 0.0, 0.622).getPackedValue ());
  orderedRGBA.set1Value (1, orderedRGBA [0]);
  orderedRGBA.set1Value (2, orderedRGBA [0]);
  orderedRGBA.set1Value (3, orderedRGBA [0]);
  orderedRGBA.set1Value (4, orderedRGBA [0]);
  orderedRGBA.set1Value (5, orderedRGBA [0]);
  orderedRGBA.set1Value (6, SbColor (0.0, 0.0, 1.0).getPackedValue ());
  orderedRGBA.set1Value (7, orderedRGBA [6]);
  orderedRGBA.set1Value (8, orderedRGBA [6]);
  orderedRGBA.set1Value (9, orderedRGBA [6]);
  orderedRGBA.set1Value (10, orderedRGBA [6]);
  orderedRGBA.set1Value (11, orderedRGBA [6]);

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

    orderedRGBA.set1Value (0, orderedRGBA [0] xor 0xffffff00);
    orderedRGBA.set1Value (1, orderedRGBA [0]);
    orderedRGBA.set1Value (2, orderedRGBA [0]);
    orderedRGBA.set1Value (3, orderedRGBA [0]);
    orderedRGBA.set1Value (4, orderedRGBA [0]);
    orderedRGBA.set1Value (5, orderedRGBA [0]);
    orderedRGBA.set1Value (6, orderedRGBA [6] xor 0xffffff00);
    orderedRGBA.set1Value (7, orderedRGBA [6]);
    orderedRGBA.set1Value (8, orderedRGBA [6]);
    orderedRGBA.set1Value (9, orderedRGBA [6]);
    orderedRGBA.set1Value (10, orderedRGBA [6]);
    orderedRGBA.set1Value (11, orderedRGBA [6]);
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

  if (top && bottom)
  {
    vtx->vertex.setValues (0, 16, &vertexData [0]);
    vtx->orderedRGBA = orderedRGBA;

    faces->vertexProperty = vtx;
    faces->coordIndex.setValues (0, sizeof (faceIndices) / sizeof (int), faceIndices);
    m_group->addChild(shapeHints);
    m_group->addChild(faces);
  }
  else if (top && (! bottom))
  {
    orderedRGBA.set1Value (0, orderedRGBA [6]);
    orderedRGBA.set1Value (1, orderedRGBA [6]);
    orderedRGBA.set1Value (2, orderedRGBA [6]);
    orderedRGBA.set1Value (3, orderedRGBA [6]);
    orderedRGBA.set1Value (4, orderedRGBA [6]);
    orderedRGBA.set1Value (5, orderedRGBA [6]);
    vtx->orderedRGBA = orderedRGBA;
	
    vtx->vertex.setValues (0, 8, &vertexData [8]);
    faces->vertexProperty = vtx;
    faces->coordIndex.setValues (0, 6 * 5, faceIndices);
    m_group->addChild(shapeHints);
    m_group->addChild(faces);
  }
  else if ((! top) && bottom)
  {
    orderedRGBA.set1Value (6, orderedRGBA [0]);
    orderedRGBA.set1Value (7, orderedRGBA [0]);
    orderedRGBA.set1Value (8, orderedRGBA [0]);
    orderedRGBA.set1Value (9, orderedRGBA [0]);
    orderedRGBA.set1Value (10, orderedRGBA [0]);
    orderedRGBA.set1Value (11, orderedRGBA [0]);
    vtx->orderedRGBA = orderedRGBA;

    vtx->vertex.setValues (0, 8, &vertexData [0]);
    faces->vertexProperty = vtx;
    faces->coordIndex.setValues (0, 6 * 5, faceIndices);
    m_group->addChild(shapeHints);
    m_group->addChild(faces);
  }
}

void
IgDrawTowerHelper::drawTowerOutline(SbVec3f &sf1, SbVec3f &sf2, SbVec3f &sf3, SbVec3f &sf4,
				    SbVec3f &sb1, SbVec3f &sb2, SbVec3f &sb3, SbVec3f &sb4)
{
  m_vertices->vertex.setNum(m_c + 8); // increase vector length to accomodate next 8 vertices

  m_vertices->vertex.set1Value(m_c,   sf1);
  m_vertices->vertex.set1Value(m_c+1, sf2);
  m_vertices->vertex.set1Value(m_c+2, sf3);
  m_vertices->vertex.set1Value(m_c+3, sf4);

  m_vertices->vertex.set1Value(m_c+4, sb1);
  m_vertices->vertex.set1Value(m_c+5, sb2);
  m_vertices->vertex.set1Value(m_c+6, sb3);
  m_vertices->vertex.set1Value(m_c+7, sb4);

  setLineIndices(0, 1, 2, 3, 0); // front face
  setLineIndices(4, 5, 6, 7, 4); // back face
  setLineIndices(0, 4);          // edge linking front and back face
  setLineIndices(1, 5);          // edge linking front and back face
  setLineIndices(2, 6);          // edge linking front and back face
  setLineIndices(3, 7);          // edge linking front and back face

  m_c += 8;
}

void
IgDrawTowerHelper::setLineIndices(int p1, int p2, int p3, int p4, int p5)
{
  m_lineSet->coordIndex.setNum(m_l + 6);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p1);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p2);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p3);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p4);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p5);
  m_lineSet->coordIndex.set1Value(m_l++, SO_END_LINE_INDEX);
}

void
IgDrawTowerHelper::setLineIndices(int p1, int p2)
{
  m_lineSet->coordIndex.setNum(m_l + 3);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p1);
  m_lineSet->coordIndex.set1Value(m_l++, m_c + p2);
  m_lineSet->coordIndex.set1Value(m_l++, SO_END_LINE_INDEX);
}

void
IgDrawTowerHelper::setFaceIndices(int p1, int p2, int p3, int p4)
{
  m_faceSet->coordIndex.setNum(m_f + 5);
  m_faceSet->coordIndex.set1Value(m_f++, m_c + p1);
  m_faceSet->coordIndex.set1Value(m_f++, m_c + p2);
  m_faceSet->coordIndex.set1Value(m_f++, m_c + p3);
  m_faceSet->coordIndex.set1Value(m_f++, m_c + p4);
  m_faceSet->coordIndex.set1Value(m_f++, SO_END_LINE_INDEX);
}

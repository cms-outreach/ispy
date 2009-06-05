

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgDrawTowerHelper.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


IgDrawTowerHelper::IgDrawTowerHelper (SoGroup *group)
  :m_c (0), 
   m_l (0), 
   m_f (0), 
   m_vertices(new SoVertexProperty),  
   m_lineSet(new SoIndexedLineSet),
   m_faceSet(new SoIndexedFaceSet),
   m_group (group)
   
{
  m_lineSet->vertexProperty = m_vertices;
  m_faceSet->vertexProperty = m_vertices;
  
  // add here but fill them up later with calls to addTower
  m_group->addChild (m_lineSet);
  m_group->addChild (m_faceSet);
}


IgDrawTowerHelper::~IgDrawTowerHelper ()
{}


void 
IgDrawTowerHelper::addTower (IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
			     IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4,
			     float heightContent = 1.0,
			     float heightScale   = 1.0)
{
  float scale = heightContent * heightScale;

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

  SbVec3f scaledBack1 = sf1 + diff1;
  SbVec3f scaledBack2 = sf2 + diff2;
  SbVec3f scaledBack3 = sf3 + diff3;
  SbVec3f scaledBack4 = sf4 + diff4;

  drawTower(sf1, sf2, sf3, sf4,
	    scaledBack1,
	    scaledBack2,
	    scaledBack3,
	    scaledBack4);
}


void 
IgDrawTowerHelper::addTower (IgV3d &f1, IgV3d &f2, IgV3d &f3, IgV3d &f4,
			     IgV3d &b1, IgV3d &b2, IgV3d &b3, IgV3d &b4)
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

  drawTower (sf1, sf2, sf3, sf4,
	     sb1, sb2, sb3, sb4);
}


void 
IgDrawTowerHelper::drawTower (SbVec3f &sf1, SbVec3f &sf2, SbVec3f &sf3, SbVec3f &sf4,
			      SbVec3f &sb1, SbVec3f &sb2, SbVec3f &sb3, SbVec3f &sb4)
{
  m_vertices->vertex.setNum (m_c + 8); // increase vector length to accomodate next 8 vertices
       
  m_vertices->vertex.set1Value (m_c,   sf1);
  m_vertices->vertex.set1Value (m_c+1, sf2);
  m_vertices->vertex.set1Value (m_c+2, sf3);
  m_vertices->vertex.set1Value (m_c+3, sf4);

  m_vertices->vertex.set1Value (m_c+4, sb1);
  m_vertices->vertex.set1Value (m_c+5, sb2);
  m_vertices->vertex.set1Value (m_c+6, sb3);
  m_vertices->vertex.set1Value (m_c+7, sb4);
  
  setLineIndices (0, 1, 2, 3, 0); // front face       
  setLineIndices (4, 5, 6, 7, 4); // back face
  setLineIndices (0, 4);          // edge linking front and back face
  setLineIndices (1, 5);          // edge linking front and back face
  setLineIndices (2, 6);          // edge linking front and back face
  setLineIndices (3, 7);          // edge linking front and back face
  
  // 6 faces, each with 4 corners (normals should point outwards) 
  setFaceIndices (3, 2, 1, 0);
  setFaceIndices (4, 5, 6, 7);
  setFaceIndices (5, 1, 2, 6);
  setFaceIndices (2, 3, 7, 6);
  setFaceIndices (7, 3, 0, 4);
  setFaceIndices (1, 5, 4, 0);
  
  m_c += 8; 
}


void 
IgDrawTowerHelper::setLineIndices (int p1, int p2, int p3, int p4, int p5)
{
  m_lineSet->coordIndex.setNum (m_l + 6);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p1);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p2);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p3);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p4);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p5);
  m_lineSet->coordIndex.set1Value (m_l++, SO_END_LINE_INDEX);
}

void 
IgDrawTowerHelper::setLineIndices (int p1, int p2)
{
  m_lineSet->coordIndex.setNum (m_l + 3);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p1);
  m_lineSet->coordIndex.set1Value (m_l++, m_c + p2);
  m_lineSet->coordIndex.set1Value (m_l++, SO_END_LINE_INDEX);
}

void 
IgDrawTowerHelper::setFaceIndices (int p1, int p2, int p3, int p4)
{
  m_faceSet->coordIndex.setNum (m_f + 5);
  m_faceSet->coordIndex.set1Value (m_f++, m_c + p1);
  m_faceSet->coordIndex.set1Value (m_f++, m_c + p2);
  m_faceSet->coordIndex.set1Value (m_f++, m_c + p3);
  m_faceSet->coordIndex.set1Value (m_f++, m_c + p4);
  m_faceSet->coordIndex.set1Value (m_f++, SO_END_LINE_INDEX);
}


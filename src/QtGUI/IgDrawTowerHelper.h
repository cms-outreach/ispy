#ifndef IG_DRAW_TOWER_HELPER_H
#define IG_DRAW_TOWER_HELPER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Framework/IgLinearAlgebra.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include "QtGUI/Projectors.h"
#include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


/**
   Tower drawing helper(e.g. for histogram like energy deposits)

   The class handles a whole bunch of towers at once putting(for
   reasons of performance) all the lines from many towers into a
   single Inventor object(ditto for faces, etc.) rather than
   creating distinct objects per tower.

   "front face" defines the base of the tower
   "back face"  defines orientation of the walls
   height of tower may extend either to
   1) the back face(e.g. if drawing a detector element)
   2) before or beyond the back face depending on e.g. energy
*/


class IgDrawTowerHelper
{

public:

  IgDrawTowerHelper(SoGroup *, Projectors &);

  ~IgDrawTowerHelper();


  /**
      Add a single tower whose height does not scale(i.e. tower floor is front face, roof is back face)
  */
  void addTower(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4);

  void addTowerProjected(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                         SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4);

  /**
      Add a single tower outline whose height does not scale(i.e. tower floor is front face, roof is back face)
  */
  void addTowerOutline(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                       SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4);

  void addTowerOutlineProjected(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4);

  /**
      Add a single tower whose floor is the front face and whose height scales according to
      some scalar variable(heightContent) and an overall scale(heightScale)
  */
  void addTower(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                float heightContent,
                float heightScale);

  void addTowerProjected(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                         SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                         float heightContent,
                         float heightScale);

  /**
      Add a single tower whose floor is offset(heightOffset) from the front face
      and whose height scales according to
      some scalar variable(heightContent) and an overall scale(heightScale)
  */
  void addTower(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                float heightContent, float heightOffset,
                float heightScale);

  void addTowerProjected(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                         SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                         float heightContent, float heightOffset,
                         float heightScale);

  /**
     Add a single lego tower with two energy fractions 
   */
  void addLegoTower(SbVec2f position, float energy, float emFraction, float scaleFactor,
		    float etaWidth, float phiWidth);
  /**
      Add a single box centred on the original box but scaled linearly by scaleFraction
  */
  void addScaledBox(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                    SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                    float scaleFraction);

  void addScaledBoxProjected(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                             SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                             float scaleFraction);

  void addRotatedBox(const SbVec3f &pos, const SbVec3f &axis, double angle,
                     double w, double h, double d);

private:

  int m_c;  /// counter for corners
  int m_l;  /// counter for line indices
  int m_f;  /// counter for face indices
  SoVertexProperty     *m_vertices;
  SoIndexedLineSet     *m_lineSet;
  SoIndexedFaceSet     *m_faceSet;
  SoGroup              *m_group;

  std::vector<int>      m_faceIndices;
  std::vector<int>      m_lineIndices;
  std::vector<SbVec3f>  m_corners;
  Projectors           &m_projectors;


  void setLineIndices(int p1, int p2, int p3, int p4, int p5);
  void setLineIndices(int p1, int p2);
  void setFaceIndices(int p1, int p2, int p3, int p4);
  void drawTower(SbVec3f *vertices, bool outline = false);
  SbVec3f *projectVertices(SbVec3f *vertices);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>


#endif // IG_DRAW_TOWER_HELPER_H

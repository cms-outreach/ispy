#ifndef IG_DRAW_TOWER_HELPER_H
#define IG_DRAW_TOWER_HELPER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgLinearAlgebra.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include "Iguana/QtGUI/src/Projectors.h"
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
  void addTower(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4);

  void addTowerProjected(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                         IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4);

  /**
      Add a single tower outline whose height does not scale(i.e. tower floor is front face, roof is back face)
  */
  void addTowerOutline(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                       IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4);

  void addTowerOutlineProjected(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                                IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4);

  /**
      Add a single tower whose floor is the front face and whose height scales according to
      some scalar variable(heightContent) and an overall scale(heightScale)
  */
  void addTower(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                float heightContent,
                float heightScale);

  void addTowerProjected(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                         IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                         float heightContent,
                         float heightScale);

  /**
      Add a single tower whose floor is offset(heightOffset) from the front face
      and whose height scales according to
      some scalar variable(heightContent) and an overall scale(heightScale)
  */
  void addTower(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                float heightContent, float heightOffset,
                float heightScale);

  void addTowerProjected(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                         IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
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
  void addScaledBox(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                    IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                    float scaleFraction);

  void addScaledBoxProjected(IgV3d &f1,  IgV3d &f2,  IgV3d &f3,  IgV3d &f4,
                             IgV3d &b1,  IgV3d &b2,  IgV3d &b3,  IgV3d &b4,
                             float scaleFraction);

  void addRotatedBox(IgV3d &pos, IgV3d &axis, double angle,
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
  void drawTower(SbVec3f *vertices);
  void drawTowerOutline(SbVec3f *vertices);
  SbVec3f *projectVertices(SbVec3f *vertices);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>


#endif // IG_DRAW_TOWER_HELPER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/IgSoGridPlaneMap.h"
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/SbColor.h>
#include <math.h>

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define SQRT_0_5 0.707106781f

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

static const unsigned X_AXIS = 0;
static const unsigned Z_AXIS = 1;
static const unsigned LL = 0;
static const unsigned LR = 1;
static const unsigned UL = 2;

// 3 edges of the grid we project onto the plane
static const SbVec3f LLCorner(-1, 0,-1);
static const SbVec3f LRCorner( 1, 0,-1);
static const SbVec3f ULCorner(-1, 0, 1);
static const SbVec3f normVec(0, 1, 0);

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE(IgSoGridPlaneMap);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSoGridPlaneMap::IgSoGridPlaneMap(void) :
  m_gridPlane(0)
{
  SO_NODE_CONSTRUCTOR(IgSoGridPlaneMap);
  SO_NODE_ADD_FIELD(plane,(SbPlane(SbVec3f(0.0f, 1.0f, 0.0f), 0.f)));
  SO_NODE_ADD_FIELD(xOrigin,(0.0f));
  SO_NODE_ADD_FIELD(zOrigin,(0.0f));
  SO_NODE_ADD_FIELD(xLen,(7.0f));
  SO_NODE_ADD_FIELD(zLen,(16.0f));
  SO_NODE_ADD_FIELD(spacing,(1000.0f));
  SO_NODE_ADD_FIELD(on,(FALSE));
  SO_NODE_ADD_FIELD(color,(SbVec3f(1,1,1)));
  SO_NODE_ADD_FIELD(refreshScale,(TRUE));

  m_curCorner[LL].setValue(-1.0,0.0,-1.0);
  m_curCorner[LR].setValue( 1.0,0.0,-1.0);
  m_curCorner[UL].setValue(-1.0,0.0, 1.0);

  labelSpot[LL].setValue(-1.0,0.0,-1.0);
  labelSpot[LR].setValue( 1.0,0.0,-1.0);
  labelSpot[UL].setValue(-1.0,0.0, 1.0);
}

void
IgSoGridPlaneMap::setCaller(IgSoGridPlane* caller)
{
  m_gridPlane = caller;
}

void
IgSoGridPlaneMap::initClass(void)
{
  SO_NODE_INIT_CLASS(IgSoGridPlaneMap, SoShape, "Shape");
  SO_ENABLE(SoCallbackAction, SoLazyElement);
  SO_ENABLE(SoPickAction,     SoLazyElement);
  SO_ENABLE(SoGLRenderAction, SoGLLazyElement);
}

IgSoGridPlaneMap::~IgSoGridPlaneMap(void)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgSoGridPlaneMap::computeBBox(SoAction *, SbBox3f &box, SbVec3f &center)
{
  //center.setValue(0,0,0);

  box.extendBy(m_curCorner[LL]);
  box.extendBy(m_curCorner[LR]);
  box.extendBy(m_curCorner[UL]);
  box.extendBy(m_curCorner[LR] + m_curCorner[UL] - m_curCorner[LL]);

  SbVec3f temp(0.5f * (m_curCorner[LR] + m_curCorner[UL]));
  center = temp;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgSoGridPlaneMap::GLRender(SoGLRenderAction *action)
{
  if (!on.getValue() || !shouldGLRender(action) || action->handleTransparency(true))
    return;

  SbPlane currentPlane;
  SbVec3f currentCenter;
  SbVec3f currentNormal;
  SbLine normalLine;

  //plane.setValue(SbPlane(SbVec3f(0.0f, 1.0f, 0.0f), 0.f))
  currentPlane = plane.getValue();
  currentNormal = currentPlane.getNormal();
  normalLine.setValue(SbVec3f(0.,0.,0.), currentNormal);
  currentPlane.intersect(normalLine, currentCenter);

  if (!normVec.equals(currentNormal, 0))
  {
    // calculate the plane normal's deviation from(0,1,0) vector
    const SbRotation rot(normVec, currentNormal);

    // rotate the corners accordingly
    rot.multVec(LLCorner, m_curCorner[LL]);
    rot.multVec(LRCorner, m_curCorner[LR]);
    rot.multVec(ULCorner, m_curCorner[UL]);
  }

  // grid line direction
  SbVec3f dir[2];

  dir[X_AXIS] = m_curCorner[LR] - m_curCorner[LL];
  dir[Z_AXIS] = m_curCorner[UL] - m_curCorner[LL];

  dir[X_AXIS].normalize();
  dir[Z_AXIS].normalize();

  // move the center according to the users wishes
  currentCenter = currentCenter + dir[X_AXIS] * xOrigin.getValue() + dir[Z_AXIS] * zOrigin.getValue();

  // length of the displayed lines
  float lineLen[2] = {xLen.getValue(), zLen.getValue()};

  // length of one step(user value is in milimeters)
  float stepLength = spacing.getValue() / 1000.0f;
  float division[2] = {lineLen[X_AXIS] / stepLength, lineLen[Z_AXIS] / stepLength};

  // correct the length and number of lines to a integer multiplicative
  // of a square
  for (unsigned axis = X_AXIS; axis <= Z_AXIS; axis++)
  {
    float roundedDiv = ceil(division[axis]);
    roundedDiv = (roundedDiv < 1.0f ? 1.0f : roundedDiv);

    if (division[axis] - roundedDiv != 0.f)
    {
      lineLen[axis] = stepLength * roundedDiv;
      division[axis] = roundedDiv;
    }
  }

  // calculate the start and end point of the first vertical line
  SbVec3f firstPos = currentCenter;
  SbVec3f secondPos = firstPos + dir[Z_AXIS] * lineLen[Z_AXIS];

  // set the positions for the corner labels
  labelSpot[LL] = firstPos - dir[X_AXIS] * stepLength - dir[Z_AXIS] * stepLength;
  labelSpot[LR] = firstPos + dir[X_AXIS] * (lineLen[X_AXIS] + stepLength) - dir[Z_AXIS] * stepLength;
  labelSpot[UL] = secondPos - dir[X_AXIS] * stepLength + dir[Z_AXIS] * stepLength;

  SbVec3f step[2] = {dir[X_AXIS] * stepLength, dir[Z_AXIS] * stepLength};

  const SbColor c = color.getValue();
  float mat[4] = {c[0], c[1], c[2]};
  glColor3fv(mat);
  unsigned lineNum = 0;
  //glNormal3f(currentNormal[0], currentNormal[1], currentNormal[2]);

  unsigned axis = X_AXIS;
  const unsigned lineInterval = 10;

  glBegin(GL_LINES);
  // first draw the vertical lines then the horizontal
  for (unsigned j = 0; j < 2; j++)
  {
    for (unsigned i = 0; i <= division[axis]; i++)
    {
      // change color each interval
      if (lineNum == lineInterval)
      {
        for (unsigned k = 0; k < 3; k++)
        {
          mat[k] = 0.9f - mat[k];
        }
        glColor3fv(mat);
      }
      // set the line's start and end point
      glVertex3f(firstPos[0], firstPos[1], firstPos[2]);
      glVertex3f(secondPos[0], secondPos[1], secondPos[2]);
      // changing the x coords
      firstPos += step[axis];
      secondPos += step[axis];

      // change back colors
      if (lineNum == lineInterval)
      {
        for (unsigned k = 0; k < 3; k++)
        {
          mat[k] = c[k];
        }
        glColor3fv(mat);
        lineNum = 0;
      }
      lineNum++;
    }

    // reset the starting points
    firstPos = currentCenter;
    secondPos = firstPos + dir[X_AXIS] * lineLen[X_AXIS];
    axis = Z_AXIS;
    lineNum = 0;
  }
  glEnd();

  // if the refreshScale flag is set, it means that some value important to
  // the gridScale has changed so we need to call the update method
  if (refreshScale.getValue())
  {
    m_gridPlane->updateScale();
    refreshScale.setValue(false);
  }


  //
  //    // Restore old state.
  //((SoGLLazyElement *) SoLazyElement::getInstance(state))
  //    ->reset(state, SoLazyElement::DIFFUSE_MASK);
  //    state->pop();

}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgSoGridPlaneMap::generatePrimitives(SoAction *action)
{
  SoState* state = action->getState();
  state->push();

  SoShapeHintsElement::set(state, SoShapeHintsElement::COUNTERCLOCKWISE,
                           SoShapeHintsElement::UNKNOWN_SHAPE_TYPE,
                           SoShapeHintsElement::CONVEX);

  SoPrimitiveVertex vertex;
  SoLineDetail      lineDetail;
  SoPointDetail     pointDetail;

  vertex.setDetail(&pointDetail);
  vertex.setMaterialIndex(0);

  // same normal and material for all points
  pointDetail.setNormalIndex(0);
  pointDetail.setTextureCoordIndex(0);
  pointDetail.setMaterialIndex(0);

  lineDetail.setPartIndex(0);
  lineDetail.setLineIndex(0);

  SbPlane currentPlane;
  SbVec3f currentCenter;
  SbVec3f currentNormal;
  SbLine normalLine;

  currentPlane = plane.getValue();
  currentNormal = currentPlane.getNormal();
  normalLine.setValue(SbVec3f(0.,0.,0.), currentNormal);
  currentPlane.intersect(normalLine, currentCenter);

  // calculate the plane normal's deviation from(0,1,0) vector
  const SbRotation rot(normVec, currentNormal);
  vertex.setNormal(currentNormal);

  // rotate the corners accordingly
  rot.multVec(LLCorner, m_curCorner[LL]);
  rot.multVec(LRCorner, m_curCorner[LR]);
  rot.multVec(ULCorner, m_curCorner[UL]);

  // grid line direction
  SbVec3f dir[2];

  dir[X_AXIS] = m_curCorner[LR] - m_curCorner[LL];
  dir[Z_AXIS] = m_curCorner[UL] - m_curCorner[LL];

  dir[X_AXIS].normalize();
  dir[Z_AXIS].normalize();

  // move the center according to the users wishes
  currentCenter = currentCenter + dir[X_AXIS] * xOrigin.getValue() + dir[Z_AXIS] * zOrigin.getValue();

  // length of the displayed lines
  float lineLen[2] = {xLen.getValue(), zLen.getValue()};

  // length of one step(user value is in milimeters)
  float stepLength = spacing.getValue() / 1000.0f;
  float division[2] = {lineLen[X_AXIS] / stepLength, lineLen[Z_AXIS] / stepLength};

  // correct the length and number of lines to a integer multiplicative
  // of a square
  for (unsigned axis = X_AXIS; axis <= Z_AXIS; axis++)
  {
    float roundedDiv = ceil(division[axis]);
    roundedDiv = (roundedDiv < 1.0f ? 1.0f : roundedDiv);

    if (division[axis] - roundedDiv != 0.f)
    {
      lineLen[axis] = stepLength * roundedDiv;
      division[axis] = roundedDiv;
    }
  }

  // calculate the start and end point of the first vertical line
  SbVec3f firstPos = currentCenter;
  SbVec3f secondPos = firstPos + dir[Z_AXIS] * lineLen[Z_AXIS];

  SbVec3f step[2] = {dir[X_AXIS] * stepLength, dir[Z_AXIS] * stepLength};

  // draw the vertical lines
  int pointCount = 0;
  for (unsigned i = 0; i <= division[X_AXIS]; i++)
  {
    // set the line's start and end point
    beginShape(action, SoShape::LINES, &lineDetail);
    vertex.setPoint(firstPos);
    pointDetail.setCoordinateIndex(pointCount++);
    shapeVertex(&vertex);

    vertex.setPoint(secondPos);
    pointDetail.setCoordinateIndex(pointCount++);
    shapeVertex(&vertex);
    endShape();
    lineDetail.incLineIndex();

    // changing the x coords
    firstPos += step[X_AXIS];
    secondPos += step[X_AXIS];
  }
  // vertical lines are one part
  lineDetail.incPartIndex();

  // reset the starting points
  firstPos = currentCenter;
  secondPos = firstPos + dir[X_AXIS] * lineLen[X_AXIS];

  // the same for horizontal lines
  for (unsigned i = 0; i <= division[Z_AXIS]; i++)
  {
    beginShape(action, SoShape::LINES, &lineDetail);
    vertex.setPoint(firstPos);
    pointDetail.setCoordinateIndex(pointCount++);
    shapeVertex(&vertex);

    vertex.setPoint(secondPos);
    pointDetail.setCoordinateIndex(pointCount++);
    shapeVertex(&vertex);
    endShape();
    lineDetail.incLineIndex();

    // changing the z coords
    firstPos += step[Z_AXIS];
    secondPos += step[Z_AXIS];
  }
  // Restore old state.
  state->pop();
}


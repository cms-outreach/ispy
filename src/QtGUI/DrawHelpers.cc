#include "QtGUI/DrawHelpers.h"
#include "QtGUI/Style.h"
#include "QtGUI/IgSoJet.h"
#include "QtGUI/IgDrawTowerHelper.h"
#include "QtGUI/IgDrawSplinesHelper.h"
#include "Framework/IgCollection.h"

#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNodes.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoPickStyle.h>

#include <QString>
#include <sstream>

/** This template can be overwritten to transform SbVec3f to foreign types,
    like those used by eigen or even Coin.
  */
template <>
SbVec3f IgCollectionItem::get<SbVec3f>(IgColumn<SbVec3f> &property)
{
  IgV3d& p = get<IgV3d>(property);
  return SbVec3f(p.x(), p.y(), p.z());
}

/** Helper method to decide which point associated to an object should be 
    the one that is used to project.
    
    @a currentId the id of the object to which many points are associated.
    
    @a assoc the associationset which associates objects of a given kind
     to their points.
     
    @the points collection containing all the points.
    
    @the position property in @the points collection. 
    
    @return the point to be used as reference for projections.
*/
SbVec3f
decideProjectionPoint(IgAssociations::iterator begin, 
                      IgAssociations::iterator end, 
                      IgColumn<SbVec3f> &position)
{
  // If there are no associated points (i.e. the range is null),
  // simply return 0,0,0. Notice that end is decremented here,
  // so it points to the last but one element in the range,
  // in case the range is not empty.
  if (begin == end)
    return SbVec3f(0, 0, 0);

  SbVec3f result;
  // Get the last item.
  for(; begin != end; ++begin)
    result = begin->get(position);
  return result;
}

/** Helper method to initialize our nodes.
*/
void initHelpers(void)
{
  IgSoShapeKit::initClass();
  IgSoJet::initClass();
}

// ------------------------------------------------------
// Draw Text Overlays
// ------------------------------------------------------

class OverlayCreatorHelper
{
public:
  /** Helper class to draw overlay information. It also created a background
    bevel for the overlay, so that it stands out from the rest of the scene.
    
    The coordinate system is centered in the middle of the screen. Top left
    corner is (-1, 1), while bottom right one is (1, -1). I'm not particularly 
    sure this is a good typographic convention, but that's ok for now.
    
    @a group where to attach the result.
    
    @a style is the style used for rendering.
    
    FIXME: Notice that deltaY is bound to DPI values although I have
           a factor 10 difference.
  */
  OverlayCreatorHelper(SoGroup *group, Style *style)
  :m_group(group),
   m_style(style),
   m_overlay(new SoAnnotation),
   m_camera(new SoOrthographicCamera),
   m_textStartTranslation(0),
   m_nextLineTranslation(0),
   m_boxSeparator(0),
   m_font(0),
   m_fontScale(1.)
  {
    m_group->enableNotify(FALSE);
	// make annotations unpickable
	SoPickStyle *ps = new SoPickStyle;
	ps->style = SoPickStyle::UNPICKABLE;
	m_group->addChild(ps);
    // Scale is 1. by default. LEAVE_ALONE is used to make sure we can have
    // an almost view independent positioning.
    m_camera->nearDistance = 1;
    m_camera->farDistance = 10;
    m_camera->pointAt(SbVec3f(0.0, 0.0, 0.0));
    m_camera->focalDistance = 1;
    m_camera->viewportMapping = SoCamera::LEAVE_ALONE;
    m_overlay->addChild(m_camera);
  }

  ~OverlayCreatorHelper()
  {
    m_group->addChild(m_overlay);
    m_group->enableNotify(TRUE);
  }
  
  /** Start a new text box in the current annotation layer.
  @a x is the x coordinate of the top left corner of the overlay.
  
  @a y is the y coordinate of the top left corner of the overlay.
  
  @a deltaX is the X increment for the position of a new line when compared 
            to the following one.
  
  @a deltaY is the Y increment for the position of a new line when compared 
            to the following one.
  */
  void beginBox(float x, float y,
                enum SoText2::Justification justification = SoText2::LEFT,
                float fontScale = 1.0,
                float deltaX = 0.0, float deltaY = -0.254/72)
  {
    m_font = new SoFont;
    m_font->size = m_style->font->size.getValue() * fontScale;
    m_font->name = m_style->font->name.getValue();
    m_justification = justification;
    m_textStartTranslation = new SoTranslation;
    m_nextLineTranslation = new SoTranslation;
    float fontHeight = deltaY * m_font->size.getValue();
    m_textStartTranslation->translation.setValue(x, y + fontHeight, 0.);
    m_nextLineTranslation->translation.setValue(deltaX, fontHeight, 0.);
    m_boxSeparator = new SoSeparator;
    m_boxSeparator->addChild(m_textStartTranslation);
    m_boxSeparator->addChild(m_font);
  }
  
  /** Indent text in the box and possibly rescale it.
      
      Useful to create titled lists or so.
      
   */
  void indentText(int indentationLevel, float fontScale = 1.0)
  {
    m_font = new SoFont;
    m_font->size = m_style->font->size.getValue() * fontScale;
    m_font->name = m_style->font->name.getValue();
    SoTranslation *indentation = new SoTranslation;
    m_nextLineTranslation = new SoTranslation;
    float fontHeight = -0.254/72 * m_font->size.getValue();
    indentation->translation.setValue((float)(indentationLevel) * 0.04, 0, 0.);
    m_nextLineTranslation->translation.setValue(0, fontHeight, 0.);
    m_boxSeparator->addChild(indentation);
    m_boxSeparator->addChild(m_font);
  }
  
  // End a text box and attach it to the layer.
  void endBox(void)
  {
    m_overlay->addChild(m_boxSeparator);
  }
  
  void createTextLine (const char *text)
  {
    SoText2 *label = new SoText2;
    label->string = text;
    label->justification = m_justification;
    m_boxSeparator->addChild(label);
    m_boxSeparator->addChild(m_nextLineTranslation);
  }
  
  void createIntLine (const char *text, int value)
  {
    char                  buf [256];
    std::string           str = std::string(text) + (sprintf(buf, "%d", value), buf);
    this->createTextLine(str.c_str());
  }

  void
  addImage (float x, float y, SoImage *image)
  {
    SoSeparator *logoSep = new SoSeparator;
    SoTranslation *translation = new SoTranslation;
    image->vertAlignment = SoImage::TOP;
    translation->translation.setValue(x, y, 0);
    logoSep->addChild(translation);
    logoSep->addChild(image);
    m_overlay->addChild(logoSep);
  }
private:
  SoGroup                       *m_group;
  Style                         *m_style;
  SoAnnotation                  *m_overlay;
  SoOrthographicCamera          *m_camera;
  SoTranslation                 *m_textStartTranslation;
  SoTranslation                 *m_nextLineTranslation;
  SoSeparator                   *m_boxSeparator;
  enum SoText2::Justification   m_justification;
  SoFont                        *m_font;
  float                         m_fontScale;
};

static void
make3DEvent(IgCollectionItem& e, 
            SoSeparator *sep, 
            Style *style,
            std::string& time)
{  
  OverlayCreatorHelper helper(sep, style);
  char buf[1024];

  switch (style->annotationLevel)
  {
    case ISPY_ANNOTATION_LEVEL_NONE:
      // One can turn off event information anyway in the display.
      // Add option here for keeping logo, copyright, and url
      helper.beginBox(-0.95, -0.92, style->textAlign, 0.4);
      helper.createTextLine("(c) CERN. All rights reserved.");
      helper.endBox();
      helper.beginBox(0.95, -0.92, SoText2::RIGHT, 0.4);
      helper.createTextLine("http://cern.ch/ispy");
      helper.endBox();
      
      if (style->background)
        helper.addImage(style->left - 0.2, style->top, style->background);
      break;
    case ISPY_ANNOTATION_LEVEL_PRESS:
      // This is  a view stripped down of information which is suitable for 
      // press releases.
      helper.beginBox(style->left, style->top, style->textAlign);
      helper.createTextLine("CMS Experiment at the LHC, CERN");
      helper.indentText(0, 0.7);
      helper.createTextLine(time.substr (0,11) == "1970-Jan-01" ? "Simulated (MC) event" :
                                   ("Data recorded: " + time).c_str());
      helper.createTextLine((sprintf(buf, "Run / Event: %d / %ld", 
                             e.get<int>("run"), e.get<long>("event")), buf));
      helper.endBox();
      helper.beginBox(-0.95, -0.92, style->textAlign, 0.4);
      helper.createTextLine("(c) CERN. All rights reserved.");
      helper.endBox();
      helper.beginBox(0.95, -0.92, SoText2::RIGHT, 0.4);
      helper.createTextLine("http://cern.ch/ispy");
      helper.endBox();
      
      if (style->background)
        helper.addImage(style->left - 0.2, style->top, style->background);
      break;
    case ISPY_ANNOTATION_LEVEL_NORMAL:
    case ISPY_ANNOTATION_LEVEL_FULL:
      // This is the default view.
      helper.beginBox(style->left, style->top, style->textAlign);
      helper.createTextLine("CMS Experiment at the LHC, CERN");
      helper.indentText(0, 0.7);
      helper.createTextLine("Data recorded: ");
      helper.createTextLine("Run: ");
      helper.createTextLine("Event: ");
      helper.createTextLine("Lumi section: ");
      helper.createTextLine("Orbit: ");
      helper.createTextLine("Crossing: ");
      helper.endBox();

      helper.beginBox(style->left + 0.30, style->top, SoText2::LEFT);
      helper.createTextLine(""); // Empty line to align table.
      helper.indentText(0, 0.7);
      helper.createTextLine(time.substr (0,11) == "1970-Jan-01" ? "Simulated (MC) event" : time.c_str());
      helper.createTextLine((sprintf(buf, "%d", e.get<int>("run")), buf));
      helper.createTextLine((sprintf(buf, "%ld", e.get<long>("event")), buf));
      helper.createTextLine((sprintf(buf, "%d", e.get<int>("ls")), buf));
      helper.createTextLine((sprintf(buf, "%d", e.get<int>("orbit")), buf));
      helper.createTextLine((sprintf(buf, "%d", e.get<int>("bx")), buf));
      helper.endBox();
      
      helper.beginBox(-0.95, -0.92, style->textAlign, 0.4);
      helper.createTextLine("(c) CERN 2011. All rights reserved.");
      helper.endBox();
      helper.beginBox(0.95, -0.92, SoText2::RIGHT, 0.4);
      helper.createTextLine("http://cern.ch/ispy");
      helper.endBox();
      
      if (style->background)
        helper.addImage(style->left - 0.20, style->top, style->background);
      break;
  }
}

static void 
make3DEventV1(IgCollection** collections, IgAssociations**,
              SoSeparator *sep, Style *style, Projectors&)
{
  IgCollection      *c = collections[0];
  IgCollectionItem   e = *c->begin();
  std::string        time  = e.get<std::string>("time");
 
  make3DEvent(e, sep, style, time);
}

static void 
make3DEventV2(IgCollection** collections, IgAssociations**,
              SoSeparator *sep, Style *style, Projectors&)
{
  IgCollection      *c = collections[0];
  IgCollectionItem   e = *c->begin();
  std::string        time  = e.get<std::string>("time");
  std::string        localtime = e.get<std::string>("localtime");
  
  std::string buf;
  std::stringstream ss(localtime);
  std::vector<std::string> subs;

  while ( ss >> buf )
    subs.push_back(buf);
  if ( subs.size() == 6 )
    time += "(" + subs[3] + " " + subs[5] +")";

  make3DEvent(e, sep, style, time);
}

static void
make3DL1Trigger(IgCollection **collections, IgAssociations **, 
                SoSeparator *sep, Style * style, Projectors &)
{
  IgCollection           *c = collections[0];
  char                  buf [256];

  OverlayCreatorHelper  helper(sep, style);

  helper.beginBox(style->left,  style->top, style->textAlign);
  helper.createTextLine("L1 Triggers:");
  helper.createTextLine("------------");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    if (ci->get<int>("result") == 1)
    {   
      sprintf(buf, "%.100s", ci->get<std::string>("algorithm").c_str());
      helper.createTextLine(buf);
    }

  helper.endBox();  
}

static void
make3DHLTrigger(IgCollection **collections, IgAssociations **, 
                 SoSeparator *sep, Style * style, Projectors &)
{
  IgCollection           *c = collections[0];

  char                  buf [256];
  OverlayCreatorHelper  helper(sep, style);

  helper.beginBox(style->left, style->top , SoText2::LEFT);
  helper.createTextLine("HLT Triggers:");
  helper.createTextLine("------------");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    if (ci->get<int>("Accept") == 1)
    {
      sprintf(buf, "%.100s", ci->get<std::string>("Name").c_str());
      helper.createTextLine(buf);
    }

  helper.endBox();
}

static void
make3DTechTrigger(IgCollection **collections, IgAssociations **,
                 SoSeparator *sep, Style * style, Projectors &)
{
  char                   buf[128];
  IgCollection           *c = collections[0];
  OverlayCreatorHelper  helper(sep, style);

  helper.beginBox(style->left, style->top, style->textAlign);
  helper.createTextLine("Tech Triggers:");
  helper.createTextLine("--------------");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    if (ci->get<int>("result") == 1)
      helper.createTextLine((sprintf(buf, "%d", ci->get<int>("bitNumber")), buf));

  helper.endBox();
}

static void
makeAnyTriggerObject(IgCollection **collections, IgAssociations **,
                     SoSeparator *sep, Style * /*style*/, 
                     Projectors &projectors)
{
  IgCollection         *c = collections[0];
  IgColumn<int>        ID(c, "VID");
  IgColumn<double>     PT(c, "pt"), ETA(c, "eta"), PHI(c, "phi");

  SoVertexProperty     *vertices = new SoVertexProperty;
  SoIndexedLineSet     *lineSet = new SoIndexedLineSet;

  std::vector<int>     lineIndices;
  std::vector<SbVec3f> points;
  int                  i = 0;

  SbVec3f direction(0.0,0.0,0.0);

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {    
    int id = ci->get(ID);

    double eta = ci->get(ETA);
    double phi = ci->get(PHI);
    double pt = ci->get(PT);

    SbVec3f p(pt*cos(phi), pt*sin(phi), pt*sinh(eta));
    SbVec3f direction = projectors.project(p);
    direction.normalize();
    direction *= 2.0;

    points.push_back(SbVec3f(0.0,0.0,0.0));
    points.push_back(direction);

    lineIndices.push_back(i);
    lineIndices.push_back(i+1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    i += 2;

    SoSeparator *annSep = new SoSeparator;
    SoTranslation *textPos = new SoTranslation;
    textPos->translation = 1.05*direction;

    SoText2 *label = new SoText2;
    label->justification.setValue(SoText2::CENTER);

    char buf[128];
    std::string text = std::string("id = ") + (sprintf(buf, "%i", id), buf);
    label->string = text.c_str();

    annSep->addChild(textPos);
    annSep->addChild(label);
    sep->addChild(annSep);
  }
  
  vertices->vertex.setValues(0, points.size(), &points[0]);
  vertices->vertex.setNum(points.size());

  lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices[0]);
  lineSet->vertexProperty = vertices;

  sep->addChild(lineSet);
}

// ------------------------------------------------------
// Draw Generic shapes
// ------------------------------------------------------

static void
makeAnyPointSetShapes(IgCollection **collections, IgAssociations **,
                      SoSeparator *sep, Style *style,
                      Projectors &projectors)
{
  IgCollection          *c = collections[0];
  IgColumn<SbVec3f>     POS(c, "pos");
  SoMarkerSet           *points = new SoMarkerSet;
  SoVertexProperty      *vertices = new SoVertexProperty;
  int                   n = 0;

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    vertices->vertex.set1Value(n++, projectors.project(ci->get(POS)));

  vertices->vertex.setNum(n);

  points->markerIndex = style->markerType;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild(points);
}

/** Helper function to draw different kind of boxes 
    FIXME: we should probably have the line style in the Style struct.
  */
static void
makeAnyBoxHelper(IgCollection **collections, IgAssociations **,
                 SoSeparator *sep, Style * /* style */, Projectors &projectors,
                 bool solid, bool projected)
{
  IgCollection *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);
  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    SbVec3f f1  = ci->get(FRONT_1);
    SbVec3f f2  = ci->get(FRONT_2);
    SbVec3f f3  = ci->get(FRONT_3);
    SbVec3f f4  = ci->get(FRONT_4);

    SbVec3f b1  = ci->get(BACK_1);
    SbVec3f b2  = ci->get(BACK_2);
    SbVec3f b3  = ci->get(BACK_3);
    SbVec3f b4  = ci->get(BACK_4);
    
    if (solid && projected)
      drawTowerHelper.addTowerProjected(f1,f2,f3,f4, b1,b2,b3,b4);
    else if (solid && !projected)
      drawTowerHelper.addTower(f1,f2,f3,f4, b1,b2,b3,b4);
    else if (!solid && projected)
      drawTowerHelper.addTowerOutlineProjected(f1,f2,f3,f4, b1,b2,b3,b4);
    else if (!solid && !projected)
      drawTowerHelper.addTowerOutline(f1,f2,f3,f4, b1,b2,b3,b4);
  }
}

static void
make3DAnyBox(IgCollection **collections, IgAssociations **assoc,
             SoSeparator *sep, Style * style, Projectors &projectors)
{
  makeAnyBoxHelper(collections, assoc, sep, style, projectors, false, false);
}

static void
makeAnyBox(IgCollection **collections, IgAssociations **assoc,
           SoSeparator *sep, Style * style, Projectors &projectors)
{
  makeAnyBoxHelper(collections, assoc, sep, style, projectors, false, true);
}

static void
make3DAnyBoxSolid(IgCollection **collections, IgAssociations **assoc,
                  SoSeparator *sep, Style * style, Projectors &projectors)
{
  makeAnyBoxHelper(collections, assoc, sep, style, projectors, true, false);
}

static void
makeAnyBoxSolid(IgCollection **collections, IgAssociations **assoc,
                SoSeparator *sep, Style * style, Projectors &projectors)
{
  makeAnyBoxHelper(collections, assoc, sep, style, projectors, true, true);
}

static void
make3DAnyLine(IgCollection **collections, IgAssociations **, 
              SoSeparator *sep, Style * /*style*/, Projectors &projectors)
{
  IgCollection          *c = collections[0];
  IgColumn<SbVec3f>     P1(c, "pos_1"), P2(c, "pos_2");
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<SbVec3f>  corners;
  std::vector<int>      indices;
  int                   i = 0;

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    SbVec3f p1 = ci->get(P1);
    SbVec3f p2 = ci->get(P2);
    // Discontinuities in projections are handled so that the projection is
    // always done so like for the first point.
    corners.push_back(projectors.project(p1));
    corners.push_back(projectors.projectAs(p2, p1));
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(SO_END_LINE_INDEX);
    i += 2;
  }

  vertices->vertex.setValues(0, corners.size(), &corners [0]);
  vertices->vertex.setNum(corners.size());

  lineSet->coordIndex.setValues(0, indices.size(), &indices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild(lineSet);
}



static void
make3DAnyPoint(IgCollection **collections, IgAssociations **, 
               SoSeparator *sep, Style * /* style */, Projectors &projectors)
{
  IgCollection          *c = collections[0];
  IgColumn<SbVec3f>     POS(c, "pos");
  SoPointSet            *points = new SoPointSet;
  SoVertexProperty      *vertices = new SoVertexProperty;
  int                   n = 0;

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    vertices->vertex.set1Value(n++, projectors.project(ci->get(POS)));

  vertices->vertex.setNum(n);
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild(points);
}

static void
make3DAnyDetId(IgCollection **, IgAssociations **, 
               SoSeparator *, Style * /* style */, Projectors &)
{
}

// Transform energy towers in R-Z view:
// All hits above XZ plane go up, below - down.
// The layer offsets the representation in X to show Ecal
// hits on top of HCAL ones.
// The flag indicats that it is ECAL which needs some special
// adjustment.
//
static void
makeRZEnergyHisto(IgCollection **collections, IgAssociations **, 
                  SoSeparator *sep, Style *style, float layer, 
                  bool flag, bool mirror, Projectors &projectors)
{
  IgCollection *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<double>  ENERGY(c, "energy");
  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");

  // FIXME: can compress the following code
  float maxEnergy = style->maxEnergy;
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);
    if (energy > maxEnergy)
      maxEnergy = energy;
  }
  
  if (maxEnergy == 0.)
    return;

  for(IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);
    if (energy > style->minEnergy)
    {
      SbVec3f f1  = ci->get(FRONT_1);
      SbVec3f f2  = ci->get(FRONT_2);
      SbVec3f f3  = ci->get(FRONT_3);
      SbVec3f f4  = ci->get(FRONT_4);

      SbVec3f b1  = ci->get(BACK_1);
      SbVec3f b2  = ci->get(BACK_2);
      SbVec3f b3  = ci->get(BACK_3);
      SbVec3f b4  = ci->get(BACK_4);

      float yf1 = sqrt (f1[0] * f1[0] + f1[1] * f1[1]);
      float yf2 = sqrt (f2[0] * f2[0] + f2[1] * f2[1]);
      float yf3 = sqrt (f3[0] * f3[0] + f3[1] * f3[1]);
      float yf4 = sqrt (f4[0] * f4[0] + f4[1] * f4[1]);

      float yb1 = sqrt (b1[0] * b1[0] + b1[1] * b1[1]);
      float yb2 = sqrt (b2[0] * b2[0] + b2[1] * b2[1]);
      float yb3 = sqrt (b3[0] * b3[0] + b3[1] * b3[1]);
      float yb4 = sqrt (b4[0] * b4[0] + b4[1] * b4[1]);

      float x = 0.001;
      
      if (f1[1] < 0.)
      {
        yf1 = - yf1;
        yf2 = - yf2;
        yf3 = - yf3;
        yf4 = - yf4;
        yb1 = - yb1;
        yb2 = - yb2;
        yb3 = - yb3;
        yb4 = - yb4;
        x = - x;
      }
      
      if(flag && f2[2] > 0.)
        x = - x;

      SbVec3f tf1  = SbVec3f(layer + x, yf1, f1[2]);
      SbVec3f tf2  = SbVec3f(layer + 2*x, yf2, f2[2]);
      SbVec3f tf3  = SbVec3f(layer + 2*x, yf3, f3[2]);
      SbVec3f tf4  = SbVec3f(layer + x, yf4, f4[2]);

      SbVec3f tb1  = SbVec3f(layer + x, yb1, b1[2]);
      SbVec3f tb2  = SbVec3f(layer + 2*x, yb2, b2[2]);
      SbVec3f tb3  = SbVec3f(layer + 2*x, yb3, b3[2]);
      SbVec3f tb4  = SbVec3f(layer + x, yb4, b4[2]);

      if(mirror && f2[2] < 0.)
        drawTowerHelper.addTower(tb1, tb4, tb3, tb2, tf1, tf4, tf3, tf2,
                                 energy/style->maxEnergy, 
                                 style->energyScale);
      else
        drawTowerHelper.addTower(tf1, tf2, tf3, tf4, tb1, tb2, tb3, tb4,
                                 energy/style->maxEnergy, 
                                 style->energyScale);
    }
  }
}

static void
makeRZECalRecHits(IgCollection **collections, IgAssociations **assocs, 
                  SoSeparator *sep, Style *style, Projectors &projectors)
{
  makeRZEnergyHisto(collections, assocs, sep, style, -0.5, true, false, projectors);
}

static void
makeRZEPRecHits(IgCollection **collections, IgAssociations **assocs, 
                SoSeparator *sep, Style *style, Projectors &projectors)
{
  makeRZEnergyHisto(collections, assocs, sep, style, 0.0, false, true, projectors);
}

static void
makeRZHCalRecHits(IgCollection **collections, IgAssociations **assocs, 
                SoSeparator *sep, Style *style, Projectors &projectors)
{
  makeRZEnergyHisto(collections, assocs, sep, style, 0.0, false, false, projectors);
}

static void
makeLegoGrid(IgCollection **, IgAssociations **, 
             SoSeparator *gridSep, Style * /*style*/, Projectors &)
{
  float scale  = 1.0;

  // HF
  float etaHFBins [43] = { 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609,
			   0.696, 0.783, 0.87, 0.957, 1.044, 1.131, 1.218, 1.305,
			   1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043,
			   2.172, 2.322, 2.5, 2.65, 3,
			   2.853, 2.964, 3.139, 3.314, // HF
			   3.486, 3.664, 3.839, 4.013, 
			   4.191, 4.363, 4.538, 4.716, 
			   4.889, 5.191};
    
  SoVertexProperty *vtx = new SoVertexProperty;
  SoIndexedLineSet *grid = new SoIndexedLineSet;
  SoMFInt32 coords;
  int vertex = 6;
  int coord = 0;
  int tick = 0;
    
  float x = float (2 * M_PI);
  float z = 5.191;
    
  vtx->vertex.set1Value(0, 0, 0, -z);
  vtx->vertex.set1Value(1, 0, 0,  z);
  vtx->vertex.set1Value(2, x, 0,  z);
  vtx->vertex.set1Value(3, x, 0, -z);
  vtx->vertex.set1Value(4, 0, 0, -z);
  vtx->vertex.set1Value(5, 0, 1, -z);
    
  for (coord = 0; coord < vertex; coord++)
    coords.set1Value (coord, coord);
  coords.set1Value (coord++, SO_END_LINE_INDEX);
    
  for (tick = 0; tick < 43; tick++)
  {
    vtx->vertex.set1Value(vertex, 0, 0, etaHFBins[tick]);       
    coords.set1Value(coord++, vertex++);
    vtx->vertex.set1Value(vertex, x, 0, etaHFBins [tick]);
    coords.set1Value(coord++, vertex++);
    coords.set1Value(coord++, SO_END_LINE_INDEX);
	
    vtx->vertex.set1Value(vertex, 0, 0, -etaHFBins [tick]);
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, x, 0, -etaHFBins [tick]);
    coords.set1Value(coord++, vertex++);
    coords.set1Value(coord++, SO_END_LINE_INDEX);
  }
    
  int ticks = 36;
  x = 0;
    
  for (tick = 0; tick < ticks; tick++)
  {
    x = tick * M_PI / 18.0;
    vtx->vertex.set1Value(vertex, x, 0, -z);
    coords.set1Value(coord++, vertex++);
    vtx->vertex.set1Value(vertex, x, 0, z);
    coords.set1Value(coord++, vertex++);
    coords.set1Value(coord++, SO_END_LINE_INDEX);
	
    for (int ttow = 1; ttow < 2; ttow++)
    {
      vtx->vertex.set1Value(vertex, x + ttow * 0.087, 0, -1.74);
      coords.set1Value(coord++, vertex++);
      vtx->vertex.set1Value(vertex, x + ttow * 0.087, 0, 1.74);
      coords.set1Value(coord++, vertex++);
      coords.set1Value(coord++, SO_END_LINE_INDEX);
    }
  }
    
  //
  float ztick = 5;
  float ytick = 0.1;
  float xtick = M_PI / 4;
    
  for (int etaTicks = 0; etaTicks < 11; etaTicks++, ztick--)
  {
    vtx->vertex.set1Value(vertex, 0, ytick, ztick);
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value(vertex, 0, 0, ztick);
    coords.set1Value(coord++, vertex++);
    vtx->vertex.set1Value(vertex, -ytick, 0, ztick);
    coords.set1Value(coord++, vertex++);
    coords.set1Value(coord++, SO_END_LINE_INDEX);
  }
  for (int phiTicks = 0; phiTicks < 8; phiTicks++)
  {
    vtx->vertex.set1Value(vertex, xtick, ytick, -z);
    coords.set1Value(coord++, vertex++);
    vtx->vertex.set1Value(vertex, xtick, 0, -z);
    coords.set1Value(coord++, vertex++);
    vtx->vertex.set1Value(vertex, xtick, 0, -z - ytick);
    coords.set1Value(coord++, vertex++);
    coords.set1Value(coord++, SO_END_LINE_INDEX);
    xtick += M_PI / 4;
  }
  //

  grid->vertexProperty = vtx;
  grid->coordIndex = coords;
    
  gridSep->addChild (grid);
    
  SoSeparator         *phiLabels = new SoSeparator;
  SoText2             *text;
  SoText2             *phiLabelText [8];
  SoTranslation       *phiStart = new SoTranslation;
  SoTranslation       *phiTranslation = new SoTranslation;
    
  z = 5.191;
    
  phiStart->translation.setValue(M_PI / 4, 0, -z - 0.6);
  phiTranslation->translation.setValue(M_PI / 4, 0, 0);
  phiLabels->addChild (phiStart);
    
  for (tick = 0; tick < 8; tick ++) 
  {
    text = phiLabelText [tick] = new SoText2;
    phiLabels->addChild (text);
    phiLabels->addChild (phiTranslation);
  }
    
  phiLabelText [0]->string = "PI/4";
  phiLabelText [1]->string = "PI/2";
  phiLabelText [2]->string = "3PI/4";
  phiLabelText [3]->string = "PI";
  phiLabelText [4]->string = "5PI/4";
  phiLabelText [5]->string = "3PI/2";
  phiLabelText [6]->string = "7PI/4";
  phiLabelText [7]->string = "2PI";
    
  gridSep->addChild (phiLabels);
    
  z = -5.191;
    
  // scale
  SoSeparator         *labelScale = new SoSeparator;
  SoText2             *labelScaleText = new SoText2;
  SoTranslation       *labelScaleOffset = new SoTranslation;
  char scaleChars [12];
  sprintf (scaleChars, "%.2G GeV (Et)", scale);
  labelScaleText->string = scaleChars;
  labelScaleOffset->translation.setValue(-0.6, 1, z - 0.6);
  labelScale->addChild (labelScaleOffset);
  labelScale->addChild (labelScaleText);
  gridSep->addChild (labelScale);
    
  SoSeparator         *etaLabels = new SoSeparator;
  SoTranslation       *etaStart = new SoTranslation;
  SoTranslation       *etaTranslation = new SoTranslation;
    
  etaStart->translation.setValue(-0.6, 0, -5.0);
  etaTranslation->translation.setValue(0, 0, 1);
  etaLabels->addChild (etaStart);
    
  for (tick = 0; tick < 11; tick++, z++)
  {
    text = new SoText2;
    char textLabel [5];
    sprintf (textLabel, "%.0f", z);
    text->string = textLabel;
    etaLabels->addChild (text);
    etaLabels->addChild (etaTranslation);
  }
  gridSep->addChild (etaLabels);
}

static float
phi4eta (float eta)
{
  float phi = 0.087;
  float etaBins [29] = { 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609,
                         0.696, 0.783, 0.87, 0.957, 1.044, 1.131, 1.218, 1.305,
                         1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043,
                         2.172, 2.322, 2.5, 2.65, 3};

  float etaHFBins [14] = {2.853, 2.964, 3.139, 3.314, // HF
                          3.486, 3.664, 3.839, 4.013,
                          4.191, 4.363, 4.538, 4.716,
                          4.889, 5.191};
  if (eta > 2.853)
    for (int i = 0; i < 13; i++)
    {
      if ((eta > etaHFBins [i]) && (eta < etaHFBins [i + 1]))
        phi =  etaHFBins [i + 1] - etaHFBins [i];
    }
  else
    for (int i = 0; i < 28; i++)
    {
      if ((eta > etaBins [i]) && (eta < etaBins [i + 1]))
        phi =  etaBins [i + 1] - etaBins [i];
    }
  return phi;
}

static void
makeLegoCaloTowers(IgCollection **collections, IgAssociations **, 
                   SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];
  IgDrawTowerHelper drawTowerHelper(sep, projectors);
  
  IgColumn<double> EM(c, "emEnergy"), ET(c, "et"), 
                   ETA(c, "eta"), PHI(c, "phi");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double emEnergy = ci->get(EM);
    double et = ci->get(ET);
    double emFraction = emEnergy / et;
 
    if (et > style->minEnergy)
    {
      double eta  = ci->get(ETA);
      double phi  = ci->get(PHI);
      if (phi < 0) phi += 2 * M_PI;
      
      drawTowerHelper.addLegoTower(SbVec2f(phi, eta), et, (emFraction > 0 ? emFraction : 0),
                                   1.00, (fabs (eta) > 1.74 ? 0.174f : 0.087f),
                                   phi4eta (fabs (eta)));
    }
  }
}

static void
makeLegoJets(IgCollection **collections, IgAssociations ** /*assocs*/, 
             SoSeparator *sep, Style *style, Projectors &)
{
  // FIXME: this one will pick up style from the CSS once we implement
  //        the ability to specify different style in different view.
  SoSeparator *top = new SoSeparator;
  sep->addChild(top);
  SoMaterial *mat = new SoMaterial;
  mat->ambientColor = SbColor(1.0, 1.0, 0.0);
  mat->diffuseColor = SbColor(1.0, 1.0, 0.0);
  mat->specularColor = SbColor(1.0, 1.0, 0.0);
  mat->emissiveColor = SbColor(1.0, 1.0, 0.0);
  top->addChild(mat);
  SoDrawStyle *sty = new SoDrawStyle;
  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 3;
  top->addChild(sty);
  
  IgCollection          *c = collections[0];

  IgColumn<double> ETA(c, "eta");
  IgColumn<double> PHI(c, "phi"); 
  IgColumn<double> ET(c, "et");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double et = ci->get(ET);
 
    if (et > style->minEnergy)
    {
      double eta  = ci->get(ETA);
      double phi  = ci->get(PHI);
      if (phi < 0) phi += 2 * M_PI;

      SoVertexProperty    *vtx = new SoVertexProperty;
      SoLineSet           *line = new SoLineSet;
      int                 segments = 60;
      
      double              segAngle = 2 * M_PI / segments;
      double              r = 0.3;
      double              cx = phi;
      double              cz = eta;
      int i = 0;
      
      for (i = 0; i < segments; ++i)
        vtx->vertex.set1Value(i, r * cos (i * segAngle) + cx,
                                 0.01,
                                 r * sin (i * segAngle) + cz);

      vtx->vertex.set1Value(i, r * cos (0.F) + cx,
                               0.01,
                               r * sin (0.F) + cz);
      vtx->normal.setValue(0, 0, 1);
      vtx->normalBinding = SoVertexProperty::OVERALL;
      vtx->materialBinding = SoVertexProperty::OVERALL;
      line->numVertices = segments + 1;
      line->vertexProperty = vtx;

      SoMarkerSet *marker = new SoMarkerSet;
      SoVertexProperty *mvtx = new SoVertexProperty;
      mvtx->vertex.set1Value (0, cx, 0, cz);
      marker->vertexProperty = mvtx;
      marker->markerIndex = style->markerType;
      marker->numPoints = 1;
      marker->startIndex = 0;
      
      top->addChild(marker);
      top->addChild(line);
    }
  }
}

static void
makeLegoHcalRecHits(IgCollection **collections, IgAssociations ** /*assocs*/, 
                    SoSeparator *sep, Style *style, Projectors &projectors)
{
  IgCollection          *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<double> ENERGY(c, "energy"), ETA(c, "eta"), PHI(c, "phi");
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);
    double eta  = ci->get(ETA);
    double et = energy * sin(2*atan(exp(-eta)));
 
    if (et > style->minEnergy)
    {
      double phi  = ci->get(PHI);
      if (phi < 0) phi += 2 * M_PI;
      
      drawTowerHelper.addLegoTower(SbVec2f(phi, eta), et, 0.0,
                                   1., 
                                   (fabs (eta) > 1.74 ? 0.174f : 0.087f),
                                   phi4eta (fabs (eta)));
    }
  }
}

static void
makeLegoEcalRecHits(IgCollection **collections, IgAssociations ** /*assocs*/, 
                    SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<double> ENERGY(c, "energy"), ETA(c, "eta"), PHI(c, "phi");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);
    double eta  = ci->get(ETA);
    double et = energy * sin(2*atan(exp(-eta)));
 
    if (et > style->minEnergy)
    {
      double phi  = ci->get(PHI);
      if (phi < 0) phi += 2 * M_PI;
      
      drawTowerHelper.addLegoTower(SbVec2f(phi, eta), et, 1.0,
                                   1., 0.0174f,
                                   0.0174f);
    }
  }
}

static void 
makeLegoTriggerObjects(IgCollection **collections, IgAssociations **,
                       SoSeparator *sep, Style * /*style*/,
                       Projectors & /*projectors*/)
{
  IgCollection         *c = collections[0];
  IgColumn<int>        ID(c, "VID");
  IgColumn<double>     PT(c, "pt"), ETA(c, "eta"), PHI(c, "phi");
  SoSeparator          *top = new SoSeparator;
  SoVertexProperty     *vertices = new SoVertexProperty;
  SoIndexedLineSet     *lineSet = new SoIndexedLineSet;
  std::vector<int>     lineIndices;
  std::vector<SbVec3f> points;
  int                  i = 0;

  for ( IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci )
  {    
    int id = ci->get(ID);
    double eta = ci->get(ETA);
    double phi = ci->get(PHI);  
    if ( phi < 0 ) phi += 2*M_PI;
    double pt = ci->get(PT);

    // A marker is nice as the lines disappear from view when 
    // viewed from the top
    SoMarkerSet *marker = new SoMarkerSet;
    SoVertexProperty *mvtx = new SoVertexProperty;
    mvtx->vertex.set1Value(0, phi, 0, eta);
    marker->vertexProperty = mvtx;
    marker->markerIndex = SoMarkerSet::CROSS_5_5;
    marker->numPoints = 1;
    marker->startIndex = 0;
    top->addChild(marker);

    points.push_back(SbVec3f(phi, 0.0, eta));
    SbVec3f direction(phi, pt, eta);
    points.push_back(direction);
    lineIndices.push_back(i);
    lineIndices.push_back(i + 1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    i += 2;

    vertices->vertex.setValues(0, points.size(), &points [0]);
    vertices->vertex.setNum(points.size());

    lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices [0]);
    lineSet->vertexProperty = vertices;

    top->addChild(lineSet);

    SoSeparator *annSep = new SoSeparator;
    SoTranslation *textPos = new SoTranslation;
    textPos->translation = 1.05*direction;

    SoText2 *label = new SoText2;
    label->justification.setValue(SoText2::CENTER);

    char buf[128];
    std::string text = std::string("id = ") + (sprintf(buf, "%i", id), buf);
    label->string = text.c_str();

    annSep->addChild(textPos);
    annSep->addChild(label);
    sep->addChild(annSep);
  }

  sep->addChild(top);
}

static void 
makeLegoPhotons(IgCollection **collections, IgAssociations **,
                SoSeparator *sep, Style* style,
                Projectors &/*projectors*/)
{
  IgCollection         *c = collections[0];
  IgColumn<double>     E(c, "energy"), ETA(c, "eta"), PHI(c, "phi");
  SoSeparator          *top = new SoSeparator;
  SoVertexProperty     *vertices = new SoVertexProperty;
  SoIndexedLineSet     *lineSet = new SoIndexedLineSet;
  std::vector<int>     lineIndices;
  std::vector<SbVec3f> points;
  int                  i = 0;

  for ( IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci )
  {    
    if ( ci->get(E) < style->minEnergy )
      continue;

    double eta = ci->get(ETA);
    double phi = ci->get(PHI);  
    if ( phi < 0 ) phi += 2*M_PI;      
    double energy = ci->get(E);
    double et = energy*sin(2*atan(exp(-eta)));

    SoMarkerSet *marker = new SoMarkerSet;
    SoVertexProperty *mvtx = new SoVertexProperty;
    mvtx->vertex.set1Value(0, phi, 0, eta);
    marker->vertexProperty = mvtx;
    marker->markerIndex = SoMarkerSet::CROSS_5_5;
    marker->numPoints = 1;
    marker->startIndex = 0;
    top->addChild(marker);

    points.push_back(SbVec3f(phi, 0.0, eta));
    points.push_back(SbVec3f(phi, -et, eta));
    lineIndices.push_back(i);
    lineIndices.push_back(i + 1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    i += 2;

    vertices->vertex.setValues(0, points.size(), &points [0]);
    vertices->vertex.setNum(points.size());

    lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices [0]);
    lineSet->vertexProperty = vertices;

    top->addChild(lineSet);
  }

  sep->addChild(top);
}

static void
makeLegoTracks(IgCollection **collections, IgAssociations **assocs,
               SoSeparator *sep, Style *style, 
               Projectors & /*projectors*/)
{       
   IgCollection           *tracks = collections[0];
   IgCollection           *extras = collections[1];
   IgAssociations         *assoc = assocs[0];
   IgColumn<double>       PT(tracks, "pt");
   IgColumn<SbVec3f>      DIR2(extras, "dir_2");   
   SoSeparator            *top = new SoSeparator; 
   SoVertexProperty       *vertices = new SoVertexProperty;
   SoIndexedLineSet       *lineSet = new SoIndexedLineSet;
   std::vector<int>       lineIndices;
   std::vector<SbVec3f>   points;
   int i = 0;
   
   SoDrawStyle *vertexLinesStyle = new SoDrawStyle;
   vertexLinesStyle->style = style->drawStyle->style;
   vertexLinesStyle->lineWidth = style->drawStyle->lineWidth.getValue() - 1;
   top->addChild(vertexLinesStyle);

   for (IgCollection::iterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
   {
     IgCollectionItem track = *ci;
     
     double pt = track.get(PT);
 
     for (IgAssociations::iterator ai = assoc->begin(ci), ae = assoc->end(); ai != ae; ++ai)
     {
       IgCollectionItem m = *ai;
       
       // Determine eta and phi from the direction of the outermost state
       SbVec3f d = m.get(DIR2);
       d.normalize();
       double phi = atan2(d[1],d[0]);
       double theta = acos(d[2]);
       double tanThetaOverTwo = tan(theta/2);
       double eta = -log(tanThetaOverTwo);

       if (phi < 0) phi += 2 * M_PI;

       SoMarkerSet *marker = new SoMarkerSet;
       SoVertexProperty *mvtx = new SoVertexProperty;
       mvtx->vertex.set1Value(0, phi, 0, eta);
       marker->vertexProperty = mvtx;
       marker->markerIndex = style->markerType;
       marker->numPoints = 1;
       marker->startIndex = 0;
       top->addChild(marker);

       points.push_back(SbVec3f(phi, 0.0, eta));
       points.push_back(SbVec3f(phi, -pt,  eta));
       lineIndices.push_back(i);
       lineIndices.push_back(i + 1);
       lineIndices.push_back(SO_END_LINE_INDEX);
       i += 2;
      
       vertices->vertex.setValues(0, points.size(), &points [0]);
       vertices->vertex.setNum(points.size());

       lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices [0]);
       lineSet->vertexProperty = vertices;

       top->addChild(lineSet);
     }
   }

   sep->addChild(top);
}

// ------------------------------------------------------
// Draw Tracker data
// ------------------------------------------------------

// Hits and digis use generic helper functions make3DPointSetShapes.

static void 
make3DTrackingParticles(IgCollection **collections, IgAssociations **assocs, 
                        SoSeparator *sep, Style *style, Projectors &projectors)
{
  IgCollection        *tracks   = collections[0];
  IgCollection        *hits     = collections[1];
  IgAssociations    *assoc    = assocs[0];     
  SoVertexProperty    *vertices = new SoVertexProperty;
  SoMarkerSet         *points   = new SoMarkerSet;
  int                 nv        = 0;
  IgDrawSplinesHelper splineDrawHelper;

  IgColumn<double> PT(tracks, "pt");
  IgColumn<SbVec3f> POS(hits, "pos"), DIR(hits, "dir");
  
  for (IgCollection::iterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    double pt = ci->get(PT);
     
    // FIXME: TM: make this configurable
    if ( pt < 0.5 )
      continue;
    
    // Determine the sign of the last tracking rechit and always project the
    // track so that the same map is used for projection.
    //
    // This is needed to get nice looking tracks when they cross a 
    // discontinuity like in the case of RZ. Does not really affect anything
    // in other views.
    SbVec3f lastOutPos = decideProjectionPoint(assoc->begin(ci), assoc->end(), POS);
    
    // FIXME: TM: eventually move the functionality of this class to here
    SoMFVec3f points;
    SoMFVec3f tangents;

    int nt = 0;
    for (IgAssociations::iterator ai = assoc->begin(ci), ae = assoc->end();
         ai != ae; ++ai)
    {
      SbVec3f p = ai->get(POS);
      SbVec3f d = ai->get(DIR);
      SbVec3f dp = p + d;
      
      SbVec3f pTrans = projectors.projectAs(p, lastOutPos);
      SbVec3f dpTrans = projectors.projectAs(dp, lastOutPos);
      SbVec3f dTrans = pTrans - dpTrans;
      dTrans.normalize();
      
      points.set1Value(nt, pTrans);
      tangents.set1Value(nt, dTrans);
      
      ++nt;
      
      vertices->vertex.set1Value(nv, pTrans);
      
      ++nv;
    }
    
    if (nt >= 2)
      splineDrawHelper.create(sep, points, tangents);
  }
  
  vertices->vertex.setNum(nv);
  
  points->markerIndex = style->markerType;
  points->vertexProperty = vertices;
  points->numPoints = nv;
  
  sep->addChild(points);
}


static
void make3DTracksNoVertex(IgCollection **collections, IgAssociations **assocs, 
                          SoSeparator *sep, Style *style, Projectors &projectors)
{
  IgCollection        *tracks = collections[0];
  IgCollection        *extras = collections[1];
  IgAssociations      *assoc = assocs[0];
  IgColumn<double>    PT(tracks, "pt");
  IgColumn<SbVec3f>   POS(tracks, "pos"), 
                      POS1(extras, "pos_1"), DIR1(extras, "dir_1"),
                      POS2(extras, "pos_2"), DIR2(extras, "dir_2");
  IgDrawSplinesHelper helper;

  for (IgCollection::iterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {	
    // Determine the sign of the last tracking rechit and always project the
    // track so that the same map is used for projection.
    //
    // This is needed to get nice looking tracks when they cross a 
    // discontinuity like in the case of RZ. Does not really affect anything
    // in other views.
    SbVec3f lastOutPos = decideProjectionPoint(assoc->begin(ci), assoc->end(), POS2);

    SoMFVec3f points;
    SoMFVec3f tangents;

    SoVertexProperty    *tvertices = new SoVertexProperty;
    SoMarkerSet         *tpoints   = new SoMarkerSet;
    int                 nVtx = 0;

    SbVec3f p = ci->get(POS);

    QString trackName = QString("Track %1 GeV(%2, %3, %4)")
                        .arg(ci->get(PT))
                        .arg(p[0]).arg(p[1]).arg(p[2]);

    if ( ci->get(PT) < style->minPt )
		continue;
	
    for (IgAssociations::iterator ai = assoc->begin(ci), ae = assoc->end(); ai != ae; ++ai)
    {
      p = ai->get(POS1);
      SbVec3f d = ai->get(DIR1);

      SbVec3f dp1 = p + d;
      
      SbVec3f pProj = projectors.projectAs(p, lastOutPos);
      SbVec3f dpProj = projectors.projectAs(dp1, lastOutPos);
      SbVec3f dProj = dpProj - pProj;
      dProj.normalize();
      
      points.set1Value(nVtx, pProj);
      tangents.set1Value(nVtx, dProj);
      tvertices->vertex.set1Value(nVtx, pProj);
      ++nVtx;
      
      p = ai->get(POS2);
      d = ai->get(DIR2);
      
      SbVec3f dp2 = p + d;

      pProj = projectors.projectAs(p, lastOutPos);
      dpProj = projectors.projectAs(dp2, lastOutPos);
      dProj = dpProj - pProj;
      dProj.normalize();
      
      points.set1Value(nVtx, pProj);
      tangents.set1Value(nVtx, dProj);
      tvertices->vertex.set1Value(nVtx, pProj);
      ++nVtx;
    }

    tvertices->vertex.setNum(nVtx);
    tpoints->markerIndex = style->markerType;
    tpoints->vertexProperty = tvertices;
    tpoints->numPoints.setValue(nVtx);

    helper.create(sep, points, tangents);
    sep->addChild(tpoints);
  }
}


static
void makeAnyTracks(IgCollection **collections, IgAssociations **assocs, 
                   SoSeparator *sep, Style *style, 
                   Projectors &projectors)
{
  IgCollection      *tracks = collections[0];
  IgCollection      *extras = collections[1];
  IgAssociations  *assoc = assocs[0];
  IgColumn<double>  PT(tracks, "pt");
  IgColumn<SbVec3f> POS(tracks, "pos"), P(tracks, "dir"),
                    POS1(extras, "pos_1"), DIR1(extras, "dir_1"),
                    POS2(extras, "pos_2"), DIR2(extras, "dir_2");
  
  SoSeparator       *vsep = new SoSeparator;
  IgDrawSplinesHelper helper;

  SoDrawStyle *vertexLinesStyle = new SoDrawStyle;
  vertexLinesStyle->style = style->drawStyle->style;
  vertexLinesStyle->lineWidth = style->drawStyle->lineWidth.getValue() - 1;
  vertexLinesStyle->linePattern = 0xf0f0;
  vsep->addChild(vertexLinesStyle);
  
  for (IgCollection::iterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    // Determine the sign of the last tracking rechit and always project the
    // track so that the same map is used for projection.
    //
    // This is needed to get nice looking tracks when they cross a 
    // discontinuity like in the case of RZ. Does not really affect anything
    // in other views.
    SbVec3f lastOutPos = decideProjectionPoint(assoc->begin(ci), assoc->end(), POS2);

    // Here is the actual track representation.
    SoMFVec3f tsPoints;
    SoMFVec3f tsTangents;
    SoMFVec3f vPoints;
    SoMFVec3f vTangents;

    SoVertexProperty    *tvertices = new SoVertexProperty;
    SoMarkerSet         *tpoints   = new SoMarkerSet;
    int                 nVtx = 0;
    
    SbVec3f p = ci->get(POS);
    SbVec3f d = ci->get(P);
    SbVec3f dp1 = p + d;
    
    SbVec3f pProj = projectors.projectAs(p, lastOutPos);
    SbVec3f dpProj = projectors.projectAs(dp1, lastOutPos);
    SbVec3f dProj = dpProj - pProj;
    dProj.normalize();
    
    vPoints.set1Value(0, pProj);
    vTangents.set1Value(0, dProj);

    QString trackName = QString("Track %1 GeV(%2, %3, %4)")
                        .arg(ci->get(PT)).arg(p[0]).arg(p[1]).arg(p[2]);
    
	if ( ci->get(PT) < style->minPt )
		continue;

    for (IgAssociations::iterator ai = assoc->begin(ci), ae = assoc->end(); ai != ae; ++ai)
    {
      p = ai->get(POS1);
      d = ai->get(DIR1);
      SbVec3f dp2 = p + d;
      // If this is the first hit, then also add it to the vertex property
      // for the dotted line which goes to the vertex. 
      pProj = projectors.projectAs(p, lastOutPos);
      dpProj = projectors.projectAs(dp2, lastOutPos);
      dProj = dpProj - pProj;
      dProj.normalize();
      
      vPoints.set1Value(1, pProj);
      vTangents.set1Value(1, dProj);
      
      tsPoints.set1Value(nVtx, pProj);
      tsTangents.set1Value(nVtx, dProj);
      tvertices->vertex.set1Value(nVtx, pProj);
      ++nVtx;
      
      p = ai->get(POS2);
      d = ai->get(DIR2);
      SbVec3f dp3 = p + d;
      pProj = projectors.projectAs(p, lastOutPos);
      dpProj = projectors.projectAs(dp3, lastOutPos);
      dProj =  dpProj - pProj;
      dProj.normalize();
      
      tsPoints.set1Value(nVtx, pProj);
      tsTangents.set1Value(nVtx, dProj);
      tvertices->vertex.set1Value(nVtx, pProj);
      ++nVtx;
    }

    tvertices->vertex.setNum(nVtx);
    tpoints->markerIndex = style->markerType;
    tpoints->vertexProperty = tvertices;
    tpoints->numPoints.setValue(nVtx);

    helper.create(vsep, vPoints, vTangents);
    helper.create(sep, tsPoints, tsTangents);
    sep->addChild(tpoints);
  }

  // Add a dotted line from the vertex of the track to the first hit.
  sep->addChild(vsep);
}

static void
make3DPreshowerTowers(IgCollection **collections, IgAssociations **, 
                      SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);
  IgColumn<double> ENERGY(c, "energy");
  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);
    if (energy > style->minEnergy)
    {
      SbVec3f f1  = ci->get(FRONT_1);
      SbVec3f f2  = ci->get(FRONT_2);
      SbVec3f f3  = ci->get(FRONT_3);
      SbVec3f f4  = ci->get(FRONT_4);
      
      SbVec3f b1  = ci->get(BACK_1);
      SbVec3f b2  = ci->get(BACK_2);
      SbVec3f b3  = ci->get(BACK_3);
      SbVec3f b4  = ci->get(BACK_4);
    
      // FIXME: There is a bug in Preshower geometry.
      // The corners order is a translation.
      // When it is fixed to mirrored, the fix
      // to flip energy bumps can be removed.
      if(f2[2] > 0.)
        drawTowerHelper.addTower(f1, f2, f3, f4, b1, b2, b3, b4,
                                 energy, style->energyScale);
      else
        drawTowerHelper.addTower(b1, b4, b3, b2, f1, f4, f3, f2,
                                 energy, style->energyScale);	
    }
  }
}


// ------------------------------------------------------
// Draw Calorimeter data
// ------------------------------------------------------

static void
make3DEnergyBoxes(IgCollection **collections, IgAssociations **, 
                  SoSeparator *sep, Style *style, Projectors &projectors)
{
  IgCollection          *c = collections[0];
  float maxEnergy          = style->maxEnergy;  // GeV  Not a cut -- just used to set max box size

  // FIXME: can compress the following code
  IgColumn<double> E(c, "energy");
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(E);
    if (energy > maxEnergy)
      maxEnergy = energy;
  }

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(E);

    if (energy > style->minEnergy)
    {
      SbVec3f f1  = ci->get(FRONT_1);
      SbVec3f f2  = ci->get(FRONT_2);
      SbVec3f f3  = ci->get(FRONT_3);
      SbVec3f f4  = ci->get(FRONT_4);

      SbVec3f b1  = ci->get(BACK_1);
      SbVec3f b2  = ci->get(BACK_2);
      SbVec3f b3  = ci->get(BACK_3);
      SbVec3f b4  = ci->get(BACK_4);

      drawTowerHelper.addScaledBoxProjected(f1,f2,f3,f4, b1,b2,b3,b4, energy/maxEnergy);
    }
  }
}


static void
make3DEnergyTowers(IgCollection **collections, IgAssociations **, 
                   SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");
  IgColumn<double>  ENERGY(c, "energy");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);

    if (energy > style->minEnergy)       // GeV
    {
      SbVec3f f1  = ci->get(FRONT_1);
      SbVec3f f2  = ci->get(FRONT_2);
      SbVec3f f3  = ci->get(FRONT_3);
      SbVec3f f4  = ci->get(FRONT_4);

      SbVec3f b1  = ci->get(BACK_1);
      SbVec3f b2  = ci->get(BACK_2);
      SbVec3f b3  = ci->get(BACK_3);
      SbVec3f b4  = ci->get(BACK_4);

      drawTowerHelper.addTowerProjected(f1,f2,f3,f4,
                                        b1,b2,b3,b4,
                                        energy,
                                        style->energyScale); // m/GeV
    }
  }
}

static void
make3DEmCaloTowerShapes(IgCollection **collections, IgAssociations **, 
                        SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");
  IgColumn<double>  ENERGY(c, "emEnergy");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get(ENERGY);

    if (energy > style->minEnergy)  // GeV
    {
      SbVec3f f1  = ci->get(FRONT_1);
      SbVec3f f2  = ci->get(FRONT_2);
      SbVec3f f3  = ci->get(FRONT_3);
      SbVec3f f4  = ci->get(FRONT_4);
      SbVec3f b1  = ci->get(BACK_1);
      SbVec3f b2  = ci->get(BACK_2);
      SbVec3f b3  = ci->get(BACK_3);
      SbVec3f b4  = ci->get(BACK_4);

      drawTowerHelper.addTowerProjected(f1,f2,f3,f4,
                                        b1,b2,b3,b4,
                                        energy,
                                        style->energyScale); // m / GeV
    }
  }
}

static void
make3DEmPlusHadCaloTowerShapes(IgCollection **collections, IgAssociations **, 
                               SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep, projectors);

  IgColumn<SbVec3f> FRONT_1(c, "front_1"), FRONT_2(c, "front_2"),
                    FRONT_3(c, "front_3"), FRONT_4(c, "front_4"),
                    BACK_1(c, "back_1"), BACK_2(c, "back_2"),
                    BACK_3(c, "back_3"), BACK_4(c, "back_4");
  IgColumn<double> EMENERGY(c, "emEnergy"), HADENERGY(c, "hadEnergy");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double emEnergy = ci->get(EMENERGY);
    double hadEnergy = ci->get(HADENERGY);

    if (hadEnergy > style->minEnergy)
    {
      SbVec3f f1  = ci->get(FRONT_1);
      SbVec3f f2  = ci->get(FRONT_2);
      SbVec3f f3  = ci->get(FRONT_3);
      SbVec3f f4  = ci->get(FRONT_4);

      SbVec3f b1  = ci->get(BACK_1);
      SbVec3f b2  = ci->get(BACK_2);
      SbVec3f b3  = ci->get(BACK_3);
      SbVec3f b4  = ci->get(BACK_4);

      drawTowerHelper.addTowerProjected(f1,f2,f3,f4,
                                        b1,b2,b3,b4,
                                        hadEnergy, (emEnergy > style->minEnergy ? emEnergy : 0),
                                        style->energyScale);
    }
  }
}

static void 
make3DCaloClusters(IgCollection **collections, IgAssociations **assocs,
                   SoSeparator *sep, Style * /*style*/, Projectors &projectors)
{
  IgCollection       *clusters = collections[0];
  IgCollection       *fracs    = collections[1];
  IgAssociations   *assoc    = assocs[0];
  
  IgDrawTowerHelper drawTowerHelper(sep, projectors);
  IgColumn<double>  E(clusters, "energy");
  IgColumn<SbVec3f> POS(clusters, "pos"),
                    FRONT_1(fracs, "front_1"), FRONT_2(fracs, "front_2"),
                    FRONT_3(fracs, "front_3"), FRONT_4(fracs, "front_4");
 
  for (IgCollection::iterator ci = clusters->begin(), ce = clusters->end(); ci != ce; ++ci)
  {
    SbVec3f pos = ci->get(POS);

    double energy = ci->get(E);
    SoText2* label = new SoText2;
    label->justification.setValue(SoText2::CENTER);

    SoSeparator *annSep = new SoSeparator;
    SoTranslation *textPos = new SoTranslation;
    textPos->translation = pos;

    char buf[128];
    std::string text = (sprintf(buf, "%0.2f", energy), buf) + std::string(" GeV");
    label->string = text.c_str();

    annSep->addChild(textPos);
    annSep->addChild(label);
    sep->addChild(annSep);

    for (IgAssociations::iterator ai = assoc->begin(ci), ae = assoc->end(); ai != ae; ++ai)
    {
      SbVec3f f1  = ai->get(FRONT_1);
      SbVec3f f2  = ai->get(FRONT_2);
      SbVec3f f3  = ai->get(FRONT_3);
      SbVec3f f4  = ai->get(FRONT_4);
      
      drawTowerHelper.addTowerOutlineProjected(f1,f2,f3,f4,f1,f2,f3,f4);
    }
  }
}

static void
make3DCaloTowers(IgCollection **collections, IgAssociations **assocs, 
                 SoSeparator *sep, Style *style, Projectors &projectors)
{

  // FIXME LT: draw ECAL and HCAL parts in different colours

  make3DEmPlusHadCaloTowerShapes(collections, assocs, sep, style, projectors);
  //make3DEmCaloTowerShapes(collections, assocs, sep, style, projectors);
}

/*
static void
make3DJet(SoGroup* sep, double et, double theta, double phi)
{
  // FIXME LT: this code not yet used.
  // FIXME LT: it is a first attempt to merge in IgSoJet because
  // FIXME LT:(1) it has a bug and(2)


  // FIXME LT: this jet drawing is utter crap(both physics and graphics)

  //  std::cout << "et=" << et << " theta=" << theta << " phi=" << phi << "\n" << std::endl;

  float thrust   = 1.0;
  float maxZ     = 4.0;               // set these to something more sensible ...
  float maxR     = 2.0 ;
  float maxEnergy=100.0 ;

  SoSeparator   *body = new SoSeparator;
  SoTransform   *bodyTrans = new SoTransform;
  SoCone        *bodyCone = new SoCone;
  SoSeparator   *hat = new SoSeparator;
  SoTransform   *hatTrans = new SoTransform;
  SoCone        *hatCone = new SoCone;


  // private data members

  double        ct = cos(theta);
  double        st = sin(theta);
  double        cp = cos(phi);
  double        sp = sin(phi);

  // Define cone rotations and translations; rotVec is the normal of
  // the thrust-y plane and alpha the angle between the y and thrust
  // axes.
  SbVec3f       rotVec(ct, 0.0, -st * cp);
  float alpha = acos(st * sp);

  // The body cone of the jet(FIXME: Set radius to something
  // meaningful; first attempt to make bodyRadius somehow related to
  // thrust.)  Move cone on its head(M_PI rotation) and vertex at
  // the origin
  float length1 = ct ? maxZ / fabs(ct) : maxZ;
  float length2 = st ? maxR / fabs(st) : maxR;
  float bodyHeight = length1 < length2 ? length1 : length2;
  float bodyRadius = 0.3 * (1.0 /(thrust + 0.001));

  bodyCone->bottomRadius = bodyRadius;
  bodyCone->height = bodyHeight;
  bodyTrans->rotation.setValue(rotVec, alpha + M_PI);
  bodyTrans->translation = SbVec3f(bodyHeight * st * cp / 2.0,
                                   bodyHeight * st * sp / 2.0,
                                   bodyHeight * ct / 2.0);

  // The pointy "hat" on top of the jet.(FIXME: Set height to
  // something meaningful; first attempt to relate hat height to
  // energy.  For now, hat height varies from 0 up to maxR,
  // depending on what max energy is set to.)
  float hatRadius = 1.4 * bodyRadius;
  float hatHeight = maxR * et / maxEnergy ;

  hatCone->bottomRadius = hatRadius;
  hatCone->height = hatHeight;
  hatTrans->rotation.setValue(rotVec, alpha);
  hatTrans->translation = SbVec3f((bodyHeight + hatHeight / 2.0) * st * cp,
                                  (bodyHeight + hatHeight / 2.0) * st * sp,
                                  (bodyHeight + hatHeight / 2.0) * ct);

  sep->addChild(body);
  sep->addChild(bodyTrans);
  sep->addChild(bodyCone);

  sep->addChild(hat);
  sep->addChild(hatTrans);
  sep->addChild(hatCone);

}
*/

static void
makeAnyPhoton(IgCollection **collections, IgAssociations **,
              SoSeparator *sep, Style* style,
              Projectors &projectors)
{
  IgCollection  *c = collections[0];
  IgColumn<double>  E(c, "energy"), ETA(c, "eta"), PHI(c, "phi");
  IgColumn<SbVec3f> POS(c, "pos");

  SoVertexProperty     *vertices = new SoVertexProperty;
  SoIndexedLineSet     *lineSet = new SoIndexedLineSet;

  std::vector<int>     lineIndices;
  std::vector<SbVec3f> points;
  int                  i = 0;

  double lEB = 3.0;  // half-length of the EB (m)
  double rEB = 1.24; // inner radius of the EB (m)

  for ( IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci )
  {    
    if ( ci->get(E) < style->minEnergy )
      continue;

    double eta = ci->get(ETA);
    double phi = ci->get(PHI);
    
    double px = cos(phi);
    double py = sin(phi);
    double pz = sinh(eta);

    SbVec3f p1 = ci->get(POS);

    double t = 0.0;

    double x0 = p1[0];
    double y0 = p1[1];
    double z0 = p1[2];

    if ( fabs(eta) > 1.48 ) // i.e. not in the EB, so propagate to ES
      t = fabs((lEB - z0)/pz); 
    else // propagate to EB
    {
      double a = px*px + py*py;
      double b = 2*x0*px + 2*y0*py;
      double c = x0*x0 + y0*y0 - rEB*rEB;
      t = (-b+sqrt(b*b-4*a*c))/2*a;
    }
    SbVec3f pnt2(x0+px*t, y0+py*t, z0+pz*t);
    points.push_back(projectors.projectAs(p1, pnt2));
    points.push_back(projectors.project(pnt2));

    lineIndices.push_back(i);
    lineIndices.push_back(i+1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    i += 2;
  }
  
  vertices->vertex.setValues(0, points.size(), &points[0]);
  vertices->vertex.setNum(points.size());

  lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices[0]);
  lineSet->vertexProperty = vertices;

  sep->addChild(lineSet);
}

static void
makeAnyJetShapes(IgCollection **collections, IgAssociations **, 
                SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];
  IgColumn<double> ET(c, "et"), THETA(c, "theta"), PHI(c, "phi");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    SbVec3f jet(ci->get(ET), ci->get(THETA), ci->get(PHI));
    
    if (jet[0] > style->minEnergy)
    {
      IgSoJet *recoJet = new IgSoJet;
      SbVec3f jetP = projectors.projectThetaPhi(jet);
      recoJet->energy.setValue(jetP[0]);
      recoJet->theta.setValue(jetP[1]);
      recoJet->phi.setValue(jetP[2]);
      sep->addChild(recoJet);

      //    make3DJet(sep, et, theta, phi);   // FIXME LT: this should replace above lines but does not yet work

    }
  }
}

static void
makeRPhiJets(IgCollection **collections, IgAssociations **,
			SoSeparator *sep, Style *style, Projectors &projectors)
{
	IgCollection *c = collections[0];
	IgColumn<double> ET(c, "et"), PHI(c, "phi");
	
	for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  	{
    	SbVec3f jet(ci->get(ET), M_PI*0.5, ci->get(PHI));

    	if (jet[0] > style->minEnergy)
    	{
      		IgSoJet *recoJet = new IgSoJet;
     
      		recoJet->energy.setValue(jet[0]);
      		recoJet->theta.setValue(jet[1]);
      		recoJet->phi.setValue(jet[2]);
      		sep->addChild(recoJet);
    	}
  	}			
}

static void
makeAnyMET(IgCollection **collections, IgAssociations **, 
           SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection          *c = collections[0];
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<int>      lineIndices;
  std::vector<SbVec3f>  points;
  int                   i = 0;
  float etRadius = 4.0; // radius in x,y, to draw Etmiss vectors --- FIXME: calculate based on scene ???

  // If the collection is empty, look no further.
  if (!c->size())
    return;
  
  SbVec3f direction(0.,0.,0.);
  float etMiss = -999.;

  IgColumn<double> PX(c, "px"), PY(c, "py");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    points.push_back(SbVec3f(0., 0., 0.));

    SbVec3f p(ci->get(PX), ci->get(PY), 0.0);

    direction = projectors.project(p);
    etMiss = direction.length();
    direction.normalize();
    direction *=etRadius;

    points.push_back(direction);
    lineIndices.push_back(i);
    lineIndices.push_back(i + 1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    i += 2;
  }

  if (etMiss > 0)
  {
    vertices->vertex.setValues(0, points.size(), &points [0]);
    vertices->vertex.setNum(points.size());

    lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices [0]);
    lineSet->vertexProperty = vertices;

    sep->addChild(lineSet);

/*
    direction *= 1.05; // Add text label a bit past the end of the line

    SoTranslation *textPos = new SoTranslation;
    SoText2 *label = new SoText2;
    // Make the label position configurable relative to the point of the arrow.
    textPos->translation = direction + SbVec3f(style->top, style->left, 0);
    label->justification.setValue(style->textAlign);

    char buf [128];

    label->string = (sprintf(buf, "Et miss=%4.1f (GeV)", etMiss), buf);

    sep->addChild(textPos);
    sep->addChild(label);
*/
  }
}

// ------------------------------------------------------
// Draw Muon data
// ------------------------------------------------------

static void
makeAnySegmentShapes(IgCollection **collections, IgAssociations **, 
                    SoSeparator *sep, Style * /*style*/, 
                    Projectors &projectors)
{
  IgCollection          *c = collections[0];
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<int>      lineIndices;
  std::vector<SbVec3f>  points;
  int                   i = 0;

  IgColumn<SbVec3f> POS_1(c, "pos_1"), POS_2(c, "pos_2");
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    SbVec3f p2 = ci->get(POS_2);
    points.push_back(projectors.projectAs(ci->get(POS_1), p2));
    points.push_back(projectors.project(p2));
    lineIndices.push_back(i);
    lineIndices.push_back(i + 1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    i += 2;
  }

  vertices->vertex.setValues(0, points.size(), &points [0]);
  vertices->vertex.setNum(points.size());

  lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild(lineSet);
}

static void
make3DCSCDigis(IgCollection **collections, IgAssociations **, SoSeparator *sep,
               double width, double depth, double rotate, Projectors &projectors)
{
  IgCollection  *c = collections[0];
  IgColumn<SbVec3f> POS(c, "pos");
  IgColumn<double>  LEN(c, "length");
  IgDrawTowerHelper helper(sep, projectors);
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    SbVec3f pos = ci->get(POS);
    SbVec3f axis(0.0, 0.0, 1.0);
    double angle = -atan2(pos[0], pos[1]) - rotate;
    helper.addRotatedBox(pos, axis, angle, width, ci->get(LEN), depth);
  }
}

static void
make3DCSCWireDigis(IgCollection **collections, IgAssociations **assocs, 
                   SoSeparator *sep, Style * /*style*/, Projectors &projectors)
{
  // FIXME: we should have something similar to the style
  //        infrastructure also for "cuts", so that the magic numbers here
  //        can disappear and we can have one make3DCSCDigis.
  make3DCSCDigis(collections,assocs,sep,0.02,0.01,M_PI*0.5, projectors);
}

static void
make3DCSCStripDigis(IgCollection **collections, IgAssociations **assocs, 
                    SoSeparator *sep, Style * /*style*/, Projectors &projectors)
{
  // FIXME: we should have something similar to the style
  //        infrastructure also for "cuts", so that the magic numbers here
  //        can disappear and we can have one make3DCSCDigis.
  make3DCSCDigis(collections,assocs,sep,0.01,0.01,0.0, projectors);
}

static void
make3DDTDigis(IgCollection **collections, IgAssociations **, 
              SoSeparator *sep, Style * /*style*/, Projectors &projectors)
{
  IgCollection   *c = collections[0];
  IgColumn<SbVec3f> POS(c, "pos"), AXIS(c, "axis");
  IgColumn<double>  ANGLE(c, "angle"), CELL_L(c, "cellLength"),
                    CELL_W(c, "cellWidth"), CELL_H(c, "cellHeight");

  IgDrawTowerHelper      helper(sep, projectors);
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    helper.addRotatedBox(ci->get(POS), ci->get(AXIS), ci->get(ANGLE),
                         ci->get(CELL_W), ci->get(CELL_L), ci->get(CELL_H));
  }
}

static void
make3DDTRecHits(IgCollection **collections, IgAssociations **, 
                SoSeparator *sep, Style * style, Projectors &projectors)
{
  IgCollection    *c = collections[0];
  IgColumn<SbVec3f> LPLUS_GLOBALPOS(c, "lPlusGlobalPos"),
                    LMINUS_GLOBALPOS(c, "lMinusGlobalPos"),
                    RPLUS_GLOBALPOS(c, "rPlusGlobalPos"),
                    RMINUS_GLOBALPOS(c, "rMinusGlobalPos"),
                    LGLOBALPOS(c, "lGlobalPos"), RGLOBALPOS(c, "rGlobalPos"),
                    WPOS(c, "wirePos"), AXIS(c, "axis");
  IgColumn<double> ANGLE(c, "angle"), CELL_L(c, "cellLength"),
                   CELL_W(c, "cellWidth"), CELL_H(c, "cellHeight");

  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedMarkerSet    *points = new SoIndexedMarkerSet;
  SoIndexedLineSet      *indexedLineSet = new SoIndexedLineSet;
  IgDrawTowerHelper     helper(sep, projectors);

  int                   n = 0;
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    // Point which decides selection.
    SbVec3f ps = ci->get(LGLOBALPOS);

    // All vertices, projected.
    SbVec3f tmpVertices[6] = { projectors.projectAs(ci->get(LPLUS_GLOBALPOS), ps),
                               projectors.projectAs(ci->get(LMINUS_GLOBALPOS), ps),
                               projectors.projectAs(ci->get(RPLUS_GLOBALPOS), ps),
                               projectors.projectAs(ci->get(RMINUS_GLOBALPOS), ps),
                               projectors.projectAs(ci->get(LGLOBALPOS), ps),
                               projectors.projectAs(ci->get(RGLOBALPOS), ps) };
    
    // Line indices.
    int lineIndices[6] = { n*6    , n*6 + 1, SO_END_LINE_INDEX,
                           n*6 + 2 , n*6 + 3, SO_END_LINE_INDEX};
    // Point indices.
    int pointIndices[2] = {n*6 + 4, n*6+5};
    
    vertices->vertex.setValues(n*6, 6, tmpVertices);
    indexedLineSet->coordIndex.setValues(n*6, 6, lineIndices);
    points->coordIndex.setValues(n*2, 2, pointIndices);
    n++;

    helper.addRotatedBox(ci->get(WPOS), ci->get(AXIS), ci->get(ANGLE),
                         ci->get(CELL_W), ci->get(CELL_L), ci->get(CELL_H));
  }

  points->markerIndex = style->markerType;
  sep->addChild(vertices);
  // Points
  sep->addChild(points);
  // Short segments.
  sep->addChild(indexedLineSet);
}

static void
make3DRPCRecHits(IgCollection **collections, IgAssociations **, 
                 SoSeparator *sep, Style * /*style*/, Projectors &projectors)
{

  IgCollection          *c = collections[0];
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<int>      lineIndices;
  std::vector<SbVec3f>  points;
  int                   i = 0;

  IgColumn<SbVec3f> U1(c, "u1"), U2(c, "u2"), V1(c, "v1"), V2(c, "v2"),
                    W1(c, "w1"), W2(c, "w2");

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    SbVec3f u1 = ci->get(U1), u2 = ci->get(U2);
    SbVec3f v1 = ci->get(V1), v2 = ci->get(V2);
    SbVec3f w1 = ci->get(W1), w2 = ci->get(W2);

    points.push_back(projectors.project(u1));
    points.push_back(projectors.projectAs(u2, u1));
    points.push_back(projectors.project(v1));
    points.push_back(projectors.projectAs(v2, v1));
    points.push_back(projectors.project(w1));
    points.push_back(projectors.projectAs(w2, w1));

    lineIndices.push_back(i);
    lineIndices.push_back(i + 1);
    lineIndices.push_back(SO_END_LINE_INDEX);
    lineIndices.push_back(i + 2);
    lineIndices.push_back(i + 3);
    lineIndices.push_back(SO_END_LINE_INDEX);
    lineIndices.push_back(i + 4);
    lineIndices.push_back(i + 5);
    lineIndices.push_back(SO_END_LINE_INDEX);

    i += 6;
  }

  vertices->vertex.setValues(0, points.size(), &points [0]);
  vertices->vertex.setNum(points.size());

  lineSet->coordIndex.setValues(0, lineIndices.size(), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild(lineSet);
}

static void
make3DTrackPoints(IgCollection **collections, IgAssociations **assocs, 
                  SoSeparator *sep, Style *style, 
                  Projectors &projectors)
{
  IgCollection          *tracks = collections[0];
  IgCollection          *points = collections[1];
  IgAssociations      *assoc = assocs[0];

  SoDrawStyle *trackPointLineStyle = new SoDrawStyle;
  trackPointLineStyle->style = style->drawStyle->style;
  trackPointLineStyle->lineWidth = style->drawStyle->lineWidth.getValue();
  sep->addChild(trackPointLineStyle);

  IgColumn<SbVec3f> POS(points, "pos");
  for (IgCollection::iterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    SoVertexProperty      *properties = new SoVertexProperty;
    int n = 0;
    
    // Determine the sign of the last tracking rechit and always project the
    // track so that the same map is used for projection.
    //
    // This is needed to get nice looking tracks when they cross a 
    // discontinuity like in the case of RZ. Does not really affect anything
    // in other views.
    SbVec3f lastOutPos = decideProjectionPoint(assoc->begin(ci), assoc->end(), POS);
    
    for (IgAssociations::iterator ai = assoc->begin(ci), ae = assoc->end(); ai != ae; ++ai)
    {
      SbVec3f pos = projectors.projectAs(ai->get(POS), lastOutPos);
      properties->vertex.set1Value(n, pos);
      n++;
    }

    SoLineSet     *line = new SoLineSet; // FIXME: SoNurbsCurve
    //SoPointSet    *markers = new SoPointSet;

    line->vertexProperty = properties;
    //markers->vertexProperty = properties;
    
    sep->addChild(line);
    //sep->addChild(markers);
  }
}

/** Shows the drawing limits on screen */
void
make3DLimits(IgCollection **collections, IgAssociations **, 
             SoSeparator *sep, Style * style, Projectors &)
{
  IgCollection           *c = collections[0];

  char                  buf [256];
  OverlayCreatorHelper  helper(sep, style);

  helper.beginBox(style->left,  style->top, style->textAlign);
  helper.createTextLine("Drawing cuts & scales");
  helper.indentText(0, 0.7);
  helper.createTextLine("Name:");
  helper.indentText(0, 0.9);

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    helper.createTextLine((sprintf(buf, "%s", ci->get<std::string>("name").c_str()), buf));

  helper.endBox();
  helper.beginBox(style->left + 0.25,  style->top, style->textAlign);
  helper.createTextLine("");
  helper.indentText(0, 0.7);
  helper.createTextLine("Min energy (GeV):");
  helper.indentText(0, 0.9);

  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    helper.createTextLine((sprintf(buf, "%.3f", ci->get<double>("minEnergy")), buf));

  helper.endBox();
  helper.beginBox(style->left + 0.45,  style->top, style->textAlign);
  helper.createTextLine("");
  helper.indentText(0, 0.7);
  helper.createTextLine("Energy scale (m/GeV):");
  helper.indentText(0, 0.9);
  
  for (IgCollection::iterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    helper.createTextLine((sprintf(buf, "%.3f", ci->get<double>("energyScale")), buf));
  helper.endBox();  
}

void
registerDrawHelpers(std::map<std::string, Make3D> &helpers)
{
  helpers.insert(std::pair<std::string,Make3D>("make3DAnyBox", make3DAnyBox));
  helpers.insert(std::pair<std::string,Make3D>("makeAnyBox", makeAnyBox));
  helpers.insert(std::pair<std::string,Make3D>("make3DAnyBoxSolid", make3DAnyBoxSolid));
  helpers.insert(std::pair<std::string,Make3D>("makeAnyBoxSolid", makeAnyBoxSolid));
  helpers.insert(std::pair<std::string,Make3D>("make3DAnyDetId", make3DAnyDetId));
  helpers.insert(std::pair<std::string,Make3D>("make3DAnyLine", make3DAnyLine));
  helpers.insert(std::pair<std::string,Make3D>("make3DAnyPoint", make3DAnyPoint));
  helpers.insert(std::pair<std::string,Make3D>("make3DCSCStripDigis", make3DCSCStripDigis));
  helpers.insert(std::pair<std::string,Make3D>("make3DCSCWireDigis", make3DCSCWireDigis));
  helpers.insert(std::pair<std::string,Make3D>("make3DCaloClusters", make3DCaloClusters));
  helpers.insert(std::pair<std::string,Make3D>("make3DCaloTowers", make3DCaloTowers));
  helpers.insert(std::pair<std::string,Make3D>("make3DDTDigis", make3DDTDigis));
  helpers.insert(std::pair<std::string,Make3D>("make3DDTRecHits", make3DDTRecHits));
  helpers.insert(std::pair<std::string,Make3D>("make3DEnergyBoxes", make3DEnergyBoxes));
  helpers.insert(std::pair<std::string,Make3D>("make3DEnergyTowers", make3DEnergyTowers));
  helpers.insert(std::pair<std::string,Make3D>("make3DEventV1", make3DEventV1));
  helpers.insert(std::pair<std::string,Make3D>("make3DEventV2", make3DEventV2));
  helpers.insert(std::pair<std::string,Make3D>("make3DHLTrigger", make3DHLTrigger));
  helpers.insert(std::pair<std::string,Make3D>("make3DJetShapes", makeAnyJetShapes));
  helpers.insert(std::pair<std::string,Make3D>("makeRPhiJets", makeRPhiJets));
  helpers.insert(std::pair<std::string,Make3D>("make3DL1Trigger", make3DL1Trigger));
  helpers.insert(std::pair<std::string,Make3D>("make3DLimits", make3DLimits));
  helpers.insert(std::pair<std::string,Make3D>("make3DMET", makeAnyMET));
  helpers.insert(std::pair<std::string,Make3D>("make3DPhoton", makeAnyPhoton));
  helpers.insert(std::pair<std::string,Make3D>("makeRZPhoton", makeAnyPhoton));
  helpers.insert(std::pair<std::string,Make3D>("makeAnyPhoton", makeAnyPhoton));
  helpers.insert(std::pair<std::string,Make3D>("make3DPointSetShapes", makeAnyPointSetShapes));
  helpers.insert(std::pair<std::string,Make3D>("makeRZPointSetShapes", makeAnyPointSetShapes));
  helpers.insert(std::pair<std::string,Make3D>("makeAnyPointSetShapes", makeAnyPointSetShapes));
  helpers.insert(std::pair<std::string,Make3D>("make3DPreshowerTowers", make3DPreshowerTowers));
  helpers.insert(std::pair<std::string,Make3D>("make3DRPCRecHits", make3DRPCRecHits));
  helpers.insert(std::pair<std::string,Make3D>("make3DSegmentShapes", makeAnySegmentShapes));
  helpers.insert(std::pair<std::string,Make3D>("make3DTechTrigger", make3DTechTrigger));
  helpers.insert(std::pair<std::string,Make3D>("make3DTrackPoints", make3DTrackPoints));
  helpers.insert(std::pair<std::string,Make3D>("make3DTrackingParticles", make3DTrackingParticles));
  helpers.insert(std::pair<std::string,Make3D>("make3DTracks", makeAnyTracks));
  helpers.insert(std::pair<std::string,Make3D>("makeRZTracks", makeAnyTracks));
  helpers.insert(std::pair<std::string,Make3D>("makeAnyTracks", makeAnyTracks));
  helpers.insert(std::pair<std::string,Make3D>("make3DTracksNoVertex", make3DTracksNoVertex));
  helpers.insert(std::pair<std::string,Make3D>("make3DTriggerObject", makeAnyTriggerObject));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoTriggerObjects", makeLegoTriggerObjects));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoCaloTowers", makeLegoCaloTowers));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoEcalRecHits", makeLegoEcalRecHits));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoGrid", makeLegoGrid));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoHcalRecHits", makeLegoHcalRecHits));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoJets", makeLegoJets));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoTracks", makeLegoTracks));
  helpers.insert(std::pair<std::string,Make3D>("makeRZECalRecHits", makeRZECalRecHits));
  helpers.insert(std::pair<std::string,Make3D>("makeRZEPRecHits", makeRZEPRecHits));
  helpers.insert(std::pair<std::string,Make3D>("makeRZHCalRecHits", makeRZHCalRecHits));
  helpers.insert(std::pair<std::string,Make3D>("makeLegoPhotons", makeLegoPhotons));
}

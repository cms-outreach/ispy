//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/QtGUI/interface/ISpyApplication.h"
#include "Iguana/QtGUI/interface/ISpy3DView.h"
#include "Iguana/QtGUI/interface/ISpyDigitalClock.h"
#include "Iguana/QtGUI/src/ISpyEventSelectorDialog.h"
#include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
#include "Iguana/QtGUI/interface/ISpyEventFilter.h"
#include "Iguana/QtGUI/interface/ISpyMainWindow.h"
#include "Iguana/QtGUI/src/ISpyPicturePublishingDialog.h"
#include "Iguana/QtGUI/interface/ISpyRestartPlayDialog.h"
#include "Iguana/QtGUI/interface/ISpySplashScreen.h"
#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/src/IgCollectionListModel.h"
#include "Iguana/QtGUI/interface/IgCollectionTableModel.h"
#include "Iguana/QtGUI/interface/IgNetworkReplyHandler.h"
#include "Iguana/QtGUI/interface/IgDrawTowerHelper.h"

#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"

// FIXME : these should be migrated from shapes into draw functions

#include "Iguana/QtGUI/src/IgSoJet.h"
#include "Iguana/QtGUI/src/IgSoSimpleTrajectory.h"
#include "Iguana/QtGUI/src/IgSoSplineTrack.h"

#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/Error.h"
#include "classlib/utils/Regexp.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZipArchive.h"

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/fields/SoSFVec2f.h>
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

#include <QApplication>
#include <QEventLoop>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QtGui>
#include <QtNetwork>
#include <iostream>
#include <set>
#include <sstream>
#include <string>


using namespace lat;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static void initShapes(void)
{
  IgSoShapeKit::initClass();
  IgSoJet::initClass();
  IgSoSimpleTrajectory::initClass();
  IgSoSplineTrack::initClass();
}

void
stripWhitespaces(std::string &s)
{
  s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

/** Fills in the an array of 3 floats with the default color.
  */
void 
defaultColor(float *colors)
{
  colors[0] = .7;
  colors[1] = .7;
  colors[2] = .7;  
}

/** Converts the iSpy specific marker description to an openInventor 
    SoMarkerSet::MarkerType. This is to facilitate the creation of CSS
    where only a part of the marker description is provided.
    
    @a marker style
    @a marker size hint
    @a marler shape
    
  */
SoMarkerSet::MarkerType
ISpyApplication::getMarkerType(enum ISPY_MARKER_STYLE style,
                               enum ISPY_MARKER_SIZE size,
                               enum ISPY_MARKER_SHAPE shape)
{
  static SoMarkerSet::MarkerType convert[] = {
    SoMarkerSet::SQUARE_LINE_5_5, SoMarkerSet::SQUARE_FILLED_5_5, 
    SoMarkerSet::SQUARE_LINE_7_7, SoMarkerSet::SQUARE_FILLED_7_7, 
    SoMarkerSet::SQUARE_LINE_9_9, SoMarkerSet::SQUARE_FILLED_9_9,
    SoMarkerSet::CROSS_5_5, SoMarkerSet::CROSS_5_5,
    SoMarkerSet::CROSS_7_7, SoMarkerSet::CROSS_7_7,
    SoMarkerSet::CROSS_9_9, SoMarkerSet::CROSS_9_9,
    SoMarkerSet::PLUS_5_5, SoMarkerSet::PLUS_5_5, 
    SoMarkerSet::PLUS_7_7, SoMarkerSet::PLUS_7_7, 
    SoMarkerSet::PLUS_9_9, SoMarkerSet::PLUS_9_9,
    SoMarkerSet::CIRCLE_LINE_5_5, SoMarkerSet::CIRCLE_FILLED_5_5, 
    SoMarkerSet::CIRCLE_LINE_7_7, SoMarkerSet::CIRCLE_FILLED_7_7, 
    SoMarkerSet::CIRCLE_LINE_9_9, SoMarkerSet::CIRCLE_FILLED_9_9,
  };
  
  return convert[style + size + shape];
}


/** Parse a string in the format
  
        #rrggbb
    
    or
      
        rgb(r, g, b)
    
    to a set of 3 float.
    
    @a rgb string to convert.
    
    @a colors an array of floats containing the red, green and blue components.
*/
void
parseColor(const char *rgb, float *color)
{  
  defaultColor(color);
  if (!rgb || ! rgb[0])
    return;

  char *endptr;
  
  // Case for #RRGGBB like format:
  //
  // Check for proper #RRGGBB format and return otherwhise.
  // Convert each component one by one, to avoid endianness issues.
  if (rgb[0] == '#')
  {
    if (strlen(rgb) != 7)
      return;  

    unsigned int value = strtol(rgb+1, &endptr, 16);
    if (*endptr)
      return;
    
    color[0] = ((value >> 16) & 0xff) / 255.;
    color[1] = ((value >> 8)  & 0xff) / 255.;
    color[2] = ((value >> 0)  & 0xff) / 255.;
    
    return;
  }
  
  // Case for rgb (r, g , b) format.
  //
  // Make sure the syntax is correct.
  // Split string into components and make sure we have 3 of them.
  // Convert each component one by one, making sure they are normalized.
  if (strncmp(rgb, "rgb(", 4) == 0)
  {
    if (rgb[strlen(rgb)-1] != ')')
      return;
    
    std::string s(rgb, 4, strlen(rgb) - 6);
    StringList components = StringOps::split(s, ",");
    if (components.size() != 3)
      return;

    for (int i = 0, e = 3; i != e; i++)
    {
      color[i] = strtod(components[i].c_str(), &endptr);
      if (*endptr)
      {
        defaultColor(color);
        ASSERT(false && "Unable to parse color.");
        return;
      }
      if (color[i] < 0. || color[i] > 1.0)
      {
        defaultColor(color);
        ASSERT(false && "Color not normalized");
        return;
      }
    }
  }
}

/** Defines a new cascading style.
    
    @a rule

    the rule to be matched in order to apply the style. For the moment this
    only means either "*" (matches anything) or the collection name.
    
    @a css
    
    a set of property definitions in the form:
    
    key1:value1;key2:value2;...
    
    see ISpyApplication::parseCss for a description of the changeable 
    properties.
*/
void
ISpyApplication::style(const char *rule, const char *css)
{
  m_styleSpecs.resize(m_styleSpecs.size() + 1);
  StyleSpec &spec = m_styleSpecs.back();

  // Define the defaults.
  spec.viewName = "*";
  spec.collectionName = "*";
  defaultColor(spec.diffuseColor);
  spec.transparency = 0.0;
  spec.lineWidth = 1.0;
  spec.linePattern = 0xffff;
  spec.fontSize = 12;
  spec.fontName = "Helvetica";
  spec.drawStyle = ISPY_SOLID_DRAW_STYLE;
  spec.markerShape = ISPY_SQUARE_MARKER_SHAPE;
  spec.markerSize = ISPY_NORMAL_MARKER_SIZE;
  spec.markerStyle = ISPY_FILLED_MARKER_STYLE;
  
  // Parse the rule.
  StringList ruleParts = StringOps::split(rule, "::");
  if (ruleParts.size() == 1)
    spec.collectionName = ruleParts[0];
  else if (!ruleParts.size() || ruleParts.size() > 2)
    ASSERT(false && "Wrong syntax for rule!");
  else
  {
    spec.viewName = ruleParts[0];
    spec.collectionName = ruleParts[1];
  }

  // Copy over properties from previously matching specs.
  for (size_t ssi = 0, sse = m_styleSpecs.size() - 1; ssi != sse; ++ssi)
  {
    StyleSpec &previous = m_styleSpecs[ssi];

    if (previous.viewName != "*"
        && previous.viewName != spec.viewName)
      continue;
     
    if (previous.collectionName != "*" &&
        previous.collectionName != spec.collectionName)
      continue;

    memcpy(spec.diffuseColor, previous.diffuseColor, 3*sizeof(float));
    spec.transparency = previous.transparency;
    spec.lineWidth = previous.lineWidth;
    spec.linePattern = previous.linePattern;
    spec.fontSize = previous.fontSize;
    spec.fontName = previous.fontName;
    spec.drawStyle = previous.drawStyle;
    spec.markerShape = previous.markerShape;
    spec.markerSize = previous.markerSize;
    spec.markerStyle = previous.markerStyle;
  }

  // Parse the property declarations and deposit new value on top of those
  // coming from previous declarations.
  // Remove trailing spaces
  //
  // FIXME: converting to a string is lame, but effective...
  // FIXME: move bracket parsing one level up.
  std::string cssString = css;
  stripWhitespaces(cssString);
  
  StringList properties = StringOps::split(cssString.c_str(), ";", StringOps::TrimEmpty);
  
  std::string key, value;

  for (size_t pi = 0, pe = properties.size(); pi != pe; pi++)
  {
    std::string &property = properties[pi];
    StringList pairs = StringOps::split(property, ":");
    
    ASSERT(pairs.size() == 2);
    
    key = pairs[0];
    value = pairs[1];

    char *endptr;
    
    if (key == "diffuse-color")
      parseColor(value.c_str(), spec.diffuseColor);
    else if (key == "transparency")
    {
      spec.transparency = strtod(value.c_str(), &endptr);
      ASSERT(! (*endptr) && "Error while parsing transparency value");
    }
    else if (key == "line-width")
    {
      spec.lineWidth = strtod(value.c_str(), &endptr);
      ASSERT(! (*endptr) && "Error while parsing line-width value");
    }
    else if (key == "line-pattern")
    {
      spec.linePattern = strtol(value.c_str(), &endptr, 16);
      ASSERT(! (*endptr) && "Error while parsing line-pattern value");
    }
    else if (key == "font-size")
    {
      spec.fontSize = strtod(value.c_str(), &endptr);
      ASSERT(! (*endptr) && "Error while parsing font-size value");
    }
    else if (key == "font-name")
      spec.fontName = value;
    else if (key == "draw-style" && value == "solid")
      spec.drawStyle = ISPY_SOLID_DRAW_STYLE;
    else if (key == "draw-style" && value == "lines")
      spec.drawStyle = ISPY_LINES_DRAW_STYLE;
    else if (key == "draw-style" && value == "points")
      spec.drawStyle = ISPY_POINTS_DRAW_STYLE;
    else if (key == "draw-style")
      ASSERT(false && "Syntax error while defining draw-style");    
    else if (key == "marker-shape" && value == "square")
      spec.markerShape = ISPY_SQUARE_MARKER_SHAPE;
    else if (key == "marker-shape" && value == "cross")
      spec.markerShape = ISPY_CROSS_MARKER_SHAPE;
    else if (key == "marker-shape" && value == "plus")
      spec.markerShape = ISPY_PLUS_MARKER_SHAPE;
    else if (key == "marker-shape" && value == "circle")
      spec.markerShape = ISPY_CIRCLE_MARKER_SHAPE;
    else if (key == "marker-shape")
      ASSERT(false && "Syntax error while defining marker-shape");
    else if (key == "marker-size" && value == "normal")
      spec.markerSize = ISPY_NORMAL_MARKER_SIZE;
    else if (key == "marker-size" && value == "big")
      spec.markerSize = ISPY_BIG_MARKER_SIZE;
    else if (key == "marker-size" && value == "huge")
      spec.markerSize = ISPY_HUGE_MARKER_SIZE;
    else if (key == "marker-style" && value == "filled")
      spec.markerStyle = ISPY_FILLED_MARKER_STYLE;
    else if (key == "marker-style" && value == "outline")
      spec.markerStyle = ISPY_OUTLINE_MARKER_STYLE;
    else if (key == "marker-style")
      ASSERT(false && "Syntax error while defining marker-style");
    else
    {
      std::cerr << "Unknown property " << key << std::endl;
      ASSERT(false && "Unknown property");
    }
  }
}

/** Parses a string contain a CSS to be used for rendering...

    The syntax for the CSS is the following:
    
        style-stamement := rule { property-definition; 
                                  [property-definition; ...] 
                                }
    
    where rule is in the form:
    
        rule := [view::][collection-name]
    
    where view can either be the name of a view as defined using the 
    IgCollection::view or "*" to indicate all views. `property-definition`
    is instead a column separated key - value pair.
    
        property := property-key: value-type;
        
    For the time being the `property-key` understood are:
    
    * diffuse-color: A color in the form of either the usual #RRGGBB, 
                     or rgb(Rf, Gf, Bf) where Rf, Gf, and Bf are normalized 
                     color components.
    * transparency: the transparency level of the main material, in the form
                    of a normalized float.
    * line-width: a float corresponding to the line width.
    * line-pattern: an hex number specifing the bit mask to be used for the 
                    line, e.g. 0x00ffff for a long dash, dashed line.
    * font-size: a float indicating the size of the font used.
    * font-name: an unquoted string indicating the name of the font to be used.
    * draw-style: either "lines", "solid" or "points".
    * marker-shape: the style to be used for markers. Either square, plus, cross, circle.
    * marker-size: the size of the marker. Either "normal", "big" or "huge".
    * marker-style: either "filled" or "outline".
    
    you can also have C and C++ style comments.
*/
void 
ISpyApplication::parseCss(const char *css)
{
  // The parsing works by first removing all the comments and spaces,
  // then it divides up different CSS statements by splitting at the final
  // } found in every definition and then divides the rule from the
  // property by splitting at the {. 
  // FIXME: the parsing is somewhat rudimentary but does its job.
  //        Write a better one when you have time.
  // FIXME: remove the ASSERT and have proper error handling, once
  //        we start having user specified css.
  std::string cssStr;
  cssStr = StringOps::remove(css, Regexp("//.*\n"));
  stripWhitespaces(cssStr);
  cssStr = StringOps::remove(cssStr, Regexp("/\\*.*?\\*/"));
  
  StringList classes = StringOps::split(cssStr, "}", StringOps::TrimEmpty); 
  
  for (size_t i = 0, e = classes.size(); i != e; ++i)
  {
    StringList parts = StringOps::split(classes[i], "{", StringOps::TrimEmpty);
    ASSERT(parts.size() == 2);
    style(parts[0].c_str(), parts[1].c_str());
  }
}

// ------------------------------------------------------
// Draw Text Overlays
// ------------------------------------------------------


static void
createTextLine(SoGroup *group, SoTranslation *trans, const std::string &text)
{
  SoText2 *label = new SoText2;
  label->string = text.c_str();
  group->addChild(trans);
  group->addChild(label);
}

static void
make3DEvent(IgCollection **collections, IgAssociationSet **, 
            SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  char                  buf [128];
  IgCollection          *c = collections[0];
  SoAnnotation          *overlay = new SoAnnotation;
  SoOrthographicCamera  *camera = new SoOrthographicCamera;
  SoTranslation         *textStartTranslation = new SoTranslation;
  SoTranslation         *nextLineTranslation  = new SoTranslation;
  IgCollectionItem      e = *c->begin();

  std::string           timestr;
  std::string           time  = e.get<std::string>("time");
  
  if (time.substr (0,11) == "1970-Jan-01") 
    timestr = std::string("Simulated (MC) event");
  else 
    timestr = std::string("Data_taken ") + time;

  std::string           run     = std::string("Run_no____ ") + (sprintf(buf, "%d", e.get<int>("run")), buf);
  std::string           event   = std::string("Event_no__ ") + (sprintf(buf, "%d", e.get<int>("event")), buf);
  std::string           orbit   = std::string("Orbit_____ ") + (sprintf(buf, "%d", e.get<int>("orbit")), buf);
  std::string           bx      = std::string("Crossing__ ") + (sprintf(buf, "%d", e.get<int>("bx")), buf);
  std::string           ls      = std::string("Lumi_sec__ ") + (sprintf(buf, "%d", e.get<int>("ls")), buf);

  // FIXME LT: make text positioning independent of window resize
  // FIXME LT: make visibilty of each line switchable in the interface(and settings) e.g. as for ig collections

  camera->nearDistance = 1;
  camera->farDistance = 10;
  camera->pointAt(SbVec3f(0.0, 0.0, 0.0));
  camera->scaleHeight(5.5f);
  camera->focalDistance = 1;
  overlay->addChild(camera);

  textStartTranslation->translation = SbVec3f(-5.0,  5.0,  0.0);
  nextLineTranslation ->translation = SbVec3f( 0.0, -0.25, 0.0);

  createTextLine(overlay, textStartTranslation, "CMS Experiment, CERN");
  createTextLine(overlay, nextLineTranslation, " ");
  createTextLine(overlay, nextLineTranslation, timestr);
  createTextLine(overlay, nextLineTranslation, " ");
  createTextLine(overlay, nextLineTranslation, run);
  createTextLine(overlay, nextLineTranslation, event);
  createTextLine(overlay, nextLineTranslation, ls);
  createTextLine(overlay, nextLineTranslation, orbit);
  createTextLine(overlay, nextLineTranslation, bx);
  createTextLine(overlay, nextLineTranslation, "http://iguana.cern.ch/ispy");
  sep->addChild(overlay);
}


static void
make3DL1Trigger(IgCollection **collections, IgAssociationSet **, 
                SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  char                   buf[128];
  IgCollection           *c = collections[0];
  SoAnnotation           *overlay = new SoAnnotation;
  SoOrthographicCamera   *camera = new SoOrthographicCamera;
  SoTranslation          *textStartTranslation = new SoTranslation;
  SoTranslation          *nextLineTranslation  = new SoTranslation;
   
  camera->nearDistance = 1;
  camera->farDistance = 10;
  camera->pointAt(SbVec3f(0.0, 0.0, 0.0));
  camera->scaleHeight(5.5f);
  camera->focalDistance = 1;
  overlay->addChild(camera);

  textStartTranslation->translation = SbVec3f(4.5,  5.0,  0.0);
  nextLineTranslation ->translation = SbVec3f(0.0, -0.25, 0.0);
  createTextLine (overlay, textStartTranslation, "L1 Triggers:");
  createTextLine (overlay, nextLineTranslation,  "------------");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    if (ci->get<int>("result") == 1)
    {   
      sprintf(buf, "%.100s", ci->get<std::string>("algorithm").c_str());
      createTextLine(overlay, nextLineTranslation, buf);
    }
  }
 
  sep->addChild (overlay);
}

static void
make3DTriggerObject(IgCollection **collections, IgAssociationSet **,
                    SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection         *c = collections[0];
  IgProperty           ID = c->getProperty("VID");
  IgProperty           PT = c->getProperty("pt");
  IgProperty           ETA = c->getProperty("eta");
  IgProperty           PHI = c->getProperty("phi");

  SoVertexProperty     *vertices = new SoVertexProperty;
  SoIndexedLineSet     *lineSet = new SoIndexedLineSet;

  std::vector<int>     lineIndices;
  std::vector<SbVec3f> points;
  int                  i = 0;

  SbVec3f direction(0.0,0.0,0.0);

  for ( IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci )
  {    
    int id = ci->get<int>(ID);

    double eta = ci->get<double>(ETA);
    double phi = ci->get<double>(PHI);

    double pt = ci->get<double>(PT);

    double px = pt*cos(phi);
    double py = pt*sin(phi);
    double pz = pt*sinh(eta);

    direction.setValue(px,py,pz);
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
make3DPointSetShapes(IgCollection **collections, IgAssociationSet **,
                     SoSeparator *sep, ISpyApplication::Style *style)
{
  IgCollection          *c = collections[0];
  IgProperty            POS = c->getProperty("pos");
  SoMarkerSet           *points = new SoMarkerSet;
  SoVertexProperty      *vertices = new SoVertexProperty;
  int                   n = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d>(POS);

    double x = p1.x();
    double y = p1.y();
    double z = p1.z();
    vertices->vertex.set1Value(n++, SbVec3f(x, y, z));
  }
  vertices->vertex.setNum(n);

  points->markerIndex = style->markerType;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild(points);
}

static void
make3DAnyBox(IgCollection **collections, IgAssociationSet **, 
             SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection *c = collections[0];

  IgDrawTowerHelper drawTowerHelper(sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d f1  = ci->get<IgV3d>("front_1");
    IgV3d f2  = ci->get<IgV3d>("front_2");
    IgV3d f3  = ci->get<IgV3d>("front_3");
    IgV3d f4  = ci->get<IgV3d>("front_4");

    IgV3d b1  = ci->get<IgV3d>("back_1");
    IgV3d b2  = ci->get<IgV3d>("back_2");
    IgV3d b3  = ci->get<IgV3d>("back_3");
    IgV3d b4  = ci->get<IgV3d>("back_4");

    drawTowerHelper.addTowerOutline(f1,f2,f3,f4, b1,b2,b3,b4);
  }
}


static void
make3DAnyLine(IgCollection **collections, IgAssociationSet **, 
              SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  IgProperty            P1 = c->getProperty("pos_1");
  IgProperty            P2 = c->getProperty("pos_2");
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<SbVec3f>  corners;
  std::vector<int>      indices;
  int                   i = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d>(P1);
    IgV3d p2 = ci->get<IgV3d>(P2);
    corners.push_back(SbVec3f(p1.x(), p1.y(), p1.z() ));
    corners.push_back(SbVec3f(p2.x(), p2.y(), p2.z() ));
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
make3DAnyPoint(IgCollection **collections, IgAssociationSet **, 
               SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];
  IgProperty            POS = c->getProperty("pos");
  SoPointSet            *points = new SoPointSet;
  SoVertexProperty      *vertices = new SoVertexProperty;
  int                   n = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d>(POS);

    double x = p1.x();
    double y = p1.y();
    double z = p1.z();
    vertices->vertex.set1Value(n++, SbVec3f(x, y, z));
  }

  vertices->vertex.setNum(n);
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild(points);
}

static void
make3DAnyDetId(IgCollection **, IgAssociationSet **, 
               SoSeparator *, ISpyApplication::Style * /* style */)
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
makeRZEnergyHisto(IgCollection **collections, IgAssociationSet **, SoSeparator *sep, float minE, float layer, bool flag)
{
  IgCollection *c = collections[0];
  float energyScaleFactor = 0.03;
  
  IgDrawTowerHelper drawTowerHelper(sep);

  IgProperty ENERGY = c->getProperty("energy");
  IgProperty FRONT_1 = c->getProperty("front_1");
  IgProperty FRONT_2 = c->getProperty("front_2");
  IgProperty FRONT_3 = c->getProperty("front_3");
  IgProperty FRONT_4 = c->getProperty("front_4");
  IgProperty BACK_1 = c->getProperty("back_1");
  IgProperty BACK_2 = c->getProperty("back_2");
  IgProperty BACK_3 = c->getProperty("back_3");
  IgProperty BACK_4 = c->getProperty("back_4");

  for(IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>(ENERGY);
    if(energy > minE)
    {
      IgV3d f1  = ci->get<IgV3d>(FRONT_1);
      IgV3d f2  = ci->get<IgV3d>(FRONT_2);
      IgV3d f3  = ci->get<IgV3d>(FRONT_3);
      IgV3d f4  = ci->get<IgV3d>(FRONT_4);

      IgV3d b1  = ci->get<IgV3d>(BACK_1);
      IgV3d b2  = ci->get<IgV3d>(BACK_2);
      IgV3d b3  = ci->get<IgV3d>(BACK_3);
      IgV3d b4  = ci->get<IgV3d>(BACK_4);

      float yf1 = sqrt (f1.x () * f1.x () + f1.y () * f1.y ());
      float yf2 = sqrt (f2.x () * f2.x () + f2.y () * f2.y ());
      float yf3 = sqrt (f3.x () * f3.x () + f3.y () * f3.y ());
      float yf4 = sqrt (f4.x () * f4.x () + f4.y () * f4.y ());

      float yb1 = sqrt (b1.x () * b1.x () + b1.y () * b1.y ());
      float yb2 = sqrt (b2.x () * b2.x () + b2.y () * b2.y ());
      float yb3 = sqrt (b3.x () * b3.x () + b3.y () * b3.y ());
      float yb4 = sqrt (b4.x () * b4.x () + b4.y () * b4.y ());

      float x = 0.001;
      
      if (f1.y () < 0.)
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
      
      if(flag && f2.z() > 0.)
	x = - x;

      IgV3d tf1  = IgV3d(layer + x, yf1, f1.z());
      IgV3d tf2  = IgV3d(layer + 2*x, yf2, f2.z());
      IgV3d tf3  = IgV3d(layer + 2*x, yf3, f3.z());
      IgV3d tf4  = IgV3d(layer + x, yf4, f4.z());

      IgV3d tb1  = IgV3d(layer + x, yb1, b1.z());
      IgV3d tb2  = IgV3d(layer + 2*x, yb2, b2.z());
      IgV3d tb3  = IgV3d(layer + 2*x, yb3, b3.z());
      IgV3d tb4  = IgV3d(layer + x, yb4, b4.z());
      
      drawTowerHelper.addTower(tf1, tf2, tf3, tf4, tb1, tb2, tb3, tb4,
                               energy,
                               energyScaleFactor);
    }
  }
}

static void
makeRZECalRecHits(IgCollection **collections, IgAssociationSet **assocs, 
                  SoSeparator *sep, ISpyApplication::Style *style)
{
  makeRZEnergyHisto(collections, assocs, sep, 0.2f, -0.5, true);
}

static void
makeRZEPRecHits(IgCollection **collections, IgAssociationSet **assocs, 
                SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  makeRZEnergyHisto(collections, assocs, sep, 0.001f, 0.0, false);
}

static void
makeRZHCalRecHits(IgCollection **collections, IgAssociationSet **assocs, 
                SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  makeRZEnergyHisto(collections, assocs, sep, 0.2f, 0.0, false);
}

static void
makeLegoGrid(IgCollection **, IgAssociationSet **, 
             SoSeparator *gridSep, ISpyApplication::Style * /*style*/)
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
    
  vtx->vertex.set1Value (0, SbVec3f (0, 0, -z));
  vtx->vertex.set1Value (1, SbVec3f (0, 0,  z));
  vtx->vertex.set1Value (2, SbVec3f (x, 0,  z));
  vtx->vertex.set1Value (3, SbVec3f (x, 0, -z));
  vtx->vertex.set1Value (4, SbVec3f (0, 0, -z));
  vtx->vertex.set1Value (5, SbVec3f (0, 1, -z));
    
  for (coord = 0; coord < vertex; coord++)
    coords.set1Value (coord, coord);
  coords.set1Value (coord++, SO_END_LINE_INDEX);
    
  for (tick = 0; tick < 43; tick++)
  {
    vtx->vertex.set1Value (vertex, SbVec3f (0, 0, etaHFBins [tick]));       
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (x, 0, etaHFBins [tick]));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
	
    vtx->vertex.set1Value (vertex, SbVec3f (0, 0, -etaHFBins [tick]));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -etaHFBins [tick]));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
  }
    
  int ticks = 36;
  x = 0;
    
  for (tick = 0; tick < ticks; tick++)
  {
    x = tick * M_PI / 18.0;
    vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
	
    for (int ttow = 1; ttow < 2; ttow++)
    {
      vtx->vertex.set1Value (vertex, SbVec3f (x + ttow * 0.087, 0, -1.74));
      coords.set1Value (coord++, vertex++);
      vtx->vertex.set1Value (vertex, SbVec3f (x + ttow * 0.087, 0, 1.74));
      coords.set1Value (coord++, vertex++);
      coords.set1Value (coord++, SO_END_LINE_INDEX);
    }
  }
    
  //
  float ztick = 5;
  float ytick = 0.1;
  float xtick = M_PI / 4;
    
  for (int etaTicks = 0; etaTicks < 11; etaTicks++, ztick--)
  {
    vtx->vertex.set1Value (vertex, SbVec3f (0, ytick, ztick));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (0, 0, ztick));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (-ytick, 0, ztick));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
  }
  for (int phiTicks = 0; phiTicks < 8; phiTicks++)
  {
    vtx->vertex.set1Value (vertex, SbVec3f (xtick, ytick, -z));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (xtick, 0, -z));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (xtick, 0, -z - ytick));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
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
    
  phiStart->translation = SbVec3f (M_PI / 4, 0, -z - 0.6);
  phiTranslation->translation = SbVec3f (M_PI / 4, 0, 0);
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
  char scaleChars [12] = "1.0 GeV";
  sprintf (scaleChars, "%.2G GeV", scale);
  labelScaleText->string = scaleChars;
  labelScaleOffset->translation
    = SbVec3f (-0.6, 1, z - 0.6);
  labelScale->addChild (labelScaleOffset);
  labelScale->addChild (labelScaleText);
  gridSep->addChild (labelScale);
    
  SoSeparator         *etaLabels = new SoSeparator;
  SoTranslation       *etaStart = new SoTranslation;
  SoTranslation       *etaTranslation = new SoTranslation;
    
  etaStart->translation = SbVec3f (-0.6, 0, -5.0);
  etaTranslation->translation = SbVec3f (0, 0, 1);
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
makeLegoCaloTowers(IgCollection **collections, IgAssociationSet **, 
                   SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  float energyScaleFactor = 1.0;  // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.5;  // GeV      FIXME LT: should get it from some service

  IgDrawTowerHelper drawTowerHelper(sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double emEnergy = ci->get<double>("emEnergy");
    double et = ci->get<double>("et");
    double emFraction = emEnergy / et;
 
    if (et > minimumEnergy)
    {
      double eta  = ci->get<double>("eta");
      double phi  = ci->get<double>("phi");
      if (phi < 0) phi += 2 * M_PI;
      
      drawTowerHelper.addLegoTower(SbVec2f(phi, eta), et, (emFraction > 0 ? emFraction : 0),
                                   energyScaleFactor, (fabs (eta) > 1.74 ? 0.174f : 0.087f),
                                   phi4eta (fabs (eta)));
    }
  }
}

static void
makeLegoJets(IgCollection **collections, IgAssociationSet ** /*assocs*/, 
             SoSeparator *sep, ISpyApplication::Style *style)
{
  // FIXME: this one will pick up style from the CSS once we implement
  //        the ability to specify different style in different view.
  SoSeparator *top = new SoSeparator;
  sep->addChild(top);
  SoMaterial *mat = new SoMaterial;
  mat->ambientColor = SbColor(1.0, 0.0, 0.0);
  mat->diffuseColor = SbColor(1.0, 0.0, 0.0);
  mat->specularColor = SbColor(1.0, 0.0, 0.0);
  mat->emissiveColor = SbColor(1.0, 0.0, 0.0);
  top->addChild(mat);
  SoDrawStyle *sty = new SoDrawStyle;
  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 3;
  top->addChild(sty);
  
  IgCollection          *c = collections[0];
  //  float energyScaleFactor = 1.0;  // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 5.0;  // GeV      FIXME LT: should get it from some service

  IgProperty ETA = c->getProperty("eta");
  IgProperty PHI = c->getProperty("phi"); 
  IgProperty ET = c->getProperty("et");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double et = ci->get<double>(ET);
 
    if (et > minimumEnergy)
    {
      double eta  = ci->get<double>(ETA);
      double phi  = ci->get<double>(PHI);
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
        vtx->vertex.set1Value (i, SbVec3f (r * cos (i * segAngle) + cx,
                                           0.01,
                                           r * sin (i * segAngle) + cz));

      vtx->vertex.set1Value (i, SbVec3f (r * cos (0) + cx,
                                         0.01,
                                         r * sin (0) + cz));
      vtx->normal = SbVec3f (0, 0, 1);
      vtx->normalBinding = SoVertexProperty::OVERALL;
      vtx->materialBinding = SoVertexProperty::OVERALL;
      line->numVertices = segments + 1;
      line->vertexProperty = vtx;

      SoMarkerSet *marker = new SoMarkerSet;
      SoVertexProperty *mvtx = new SoVertexProperty;
      mvtx->vertex.set1Value (0, SbVec3f (cx, 0, cz));
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
makeLegoHcalRecHits(IgCollection **collections, IgAssociationSet ** /*assocs*/, 
                    SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];
  float energyScaleFactor = 1.0;  // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.2;  // GeV      FIXME LT: should get it from some service

  IgDrawTowerHelper drawTowerHelper(sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>("energy");
    double eta  = ci->get<double>("eta");
    double et = energy * sin(2*atan(exp(-eta)));
 
    if (et > minimumEnergy)
    {
      double phi  = ci->get<double>("phi");
      if (phi < 0) phi += 2 * M_PI;
      
      drawTowerHelper.addLegoTower(SbVec2f(phi, eta), et, 0.0,
				   energyScaleFactor, (fabs (eta) > 1.74 ? 0.174f : 0.087f),
				   phi4eta (fabs (eta)));
    }
  }
}

static void
makeLegoEcalRecHits(IgCollection **collections, IgAssociationSet **assocs, 
                    SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  float energyScaleFactor = 1.0;  // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.2;  // GeV      FIXME LT: should get it from some service

  IgDrawTowerHelper drawTowerHelper(sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>("energy");
    double eta  = ci->get<double>("eta");
    double et = energy * sin(2*atan(exp(-eta)));
 
    if (et > minimumEnergy)
    {
      double phi  = ci->get<double>("phi");
      if (phi < 0) phi += 2 * M_PI;
      
      drawTowerHelper.addLegoTower(SbVec2f(phi, eta), et, 1.0,
				   energyScaleFactor, 0.0174f,
				   0.0174f);
    }
  }
}

// ------------------------------------------------------
// Draw Tracker data
// ------------------------------------------------------

// Hits and digis use generic helper functions make3DPointSetShapes.

static void 
make3DTrackingParticles(IgCollection **collections, IgAssociationSet **assocs, 
                        SoSeparator *sep, ISpyApplication::Style *style)
{
  IgCollection        *tracks   = collections[0];
  IgCollection        *hits     = collections[1];
  IgAssociationSet    *assoc    = assocs[0];     
  SoVertexProperty    *vertices = new SoVertexProperty;
  SoMarkerSet         *points   = new SoMarkerSet;
  int                 nv        = 0;

  IgProperty PT  = tracks->getProperty("pt");
  IgProperty POS = hits->getProperty("pos");
  IgProperty DIR = hits->getProperty("dir");
  
  for (IgCollectionIterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    double pt = ci->get<double>(PT);
     
    // FIXME: TM: make this configurable
    if ( pt < 0.5 )
      continue;
    
    // FIXME: TM: eventually move the functionality of this class to here
    IgSoSplineTrack *trackRep = new IgSoSplineTrack;
      
    int nt = 0;
    
    for (IgAssociationSet::Iterator ai = assoc->begin(), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first().objectId() == ci->currentRow())
      {
        IgCollectionItem m(hits, ai->second().objectId());
        
        IgV3d p = m->get<IgV3d>(POS);
        IgV3d d = m->get<IgV3d>(DIR);
        
        trackRep->points.set1Value(nt, SbVec3f(p.x(), p.y(), p.z()));
        trackRep->tangents.set1Value(nt, SbVec3f(d.x(), d.y(), d.z()));
        
        ++nt;
        
        vertices->vertex.set1Value(nv, SbVec3f(p.x(),p.y(), p.z()));

        ++nv;
      }
    }
    
    if (nt >= 2)
      sep->addChild(trackRep);
  }
  
  vertices->vertex.setNum(nv);
  
  points->markerIndex = style->markerType;
  points->vertexProperty = vertices;
  points->numPoints = nv;
  
  sep->addChild(points);
}

static 
void make3DTracks(IgCollection **collections, IgAssociationSet **assocs, 
                  SoSeparator *sep, ISpyApplication::Style *style)
{
  IgCollection          *tracks = collections[0];
  IgCollection          *extras = collections[1];
  IgAssociationSet      *assoc = assocs[0];
  IgProperty            PT  = tracks->getProperty("pt");
  IgProperty            POS = tracks->getProperty("pos");
  IgProperty            POS1 = extras->getProperty("pos_1");
  IgProperty            DIR1 = extras->getProperty("dir_1");
  IgProperty            POS2 = extras->getProperty("pos_2");
  IgProperty            DIR2 = extras->getProperty("dir_2");
  SoSeparator           *vsep = new SoSeparator;
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoMarkerSet           *mpoints = new SoMarkerSet;
  int                   nv = 0;
 
  for (IgCollectionIterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    IgSoSplineTrack     *trackRep  = new IgSoSplineTrack;
    SoVertexProperty    *tvertices = new SoVertexProperty;
    SoMarkerSet         *tpoints   = new SoMarkerSet;
    int                 nVtx = 0;

    IgV3d p = ci->get<IgV3d>(POS);
    vertices->vertex.set1Value(nv++, SbVec3f(p.x(), p.y(), p.z()));
    
    QString trackName = QString("Track %1 GeV(%2, %3, %4)")
                        .arg(ci->get<double>(PT))
                        .arg(p.x()).arg(p.y()).arg(p.z());

    for (IgAssociationSet::Iterator ai = assoc->begin(), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first().objectId() == ci->currentRow())
      {
        IgCollectionItem m(extras, ai->second().objectId());
        p = ci->get<IgV3d>(POS1);
        IgV3d d = ci->get<IgV3d>(DIR1);
        SbVec3f diri(d.x(), d.y(), d.z());
        diri.normalize();

        trackRep->points.set1Value(nVtx, SbVec3f(p.x(), p.y(), p.z()));
        trackRep->tangents.set1Value(nVtx, diri);
        tvertices->vertex.set1Value(nVtx, SbVec3f(p.x(), p.y(), p.z()));
        ++nVtx;

        p = ci->get<IgV3d>(POS2);
        d = ci->get<IgV3d>(DIR2);
        SbVec3f diro(d.x(), d.y(), d.z());
        diro.normalize();

        trackRep->points.set1Value(nVtx, SbVec3f(p.x(), p.y(), p.z()));
        trackRep->tangents.set1Value(nVtx, diro);
        tvertices->vertex.set1Value(nVtx, SbVec3f(p.x(), p.y(), p.z()));
        ++nVtx;
      }
    }

    tvertices->vertex.setNum(nVtx);
    tpoints->markerIndex = style->markerType;
    tpoints->vertexProperty = tvertices;
    tpoints->numPoints.setValue(nVtx);

    sep->addChild(trackRep);
    sep->addChild(tpoints);
  }

  vertices->vertex.setNum(nv);
  mpoints->markerIndex = SoMarkerSet::CROSS_5_5;
  mpoints->vertexProperty.setValue(vertices);
  mpoints->numPoints.setValue(nv);
  vsep->addChild(mpoints);

}

static void
make3DPreshowerTowers(IgCollection **collections, IgAssociationSet **, 
                      SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  SoMarkerSet           *points = new SoMarkerSet;
  SoVertexProperty      *vertices = new SoVertexProperty;
  int                   n = 0;

  IgDrawTowerHelper drawTowerHelper(sep);
  IgProperty FRONT_1 = c->getProperty("front_1");
  IgProperty FRONT_2 = c->getProperty("front_2");
  IgProperty FRONT_3 = c->getProperty("front_3");
  IgProperty FRONT_4 = c->getProperty("front_4");

  IgProperty BACK_1 = c->getProperty("back_1");
  IgProperty BACK_2 = c->getProperty("back_2");
  IgProperty BACK_3 = c->getProperty("back_3");
  IgProperty BACK_4 = c->getProperty("back_4");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d f1  = ci->get<IgV3d>(FRONT_1);
    IgV3d f2  = ci->get<IgV3d>(FRONT_2);
    IgV3d f3  = ci->get<IgV3d>(FRONT_3);
    IgV3d f4  = ci->get<IgV3d>(FRONT_4);

    IgV3d b1  = ci->get<IgV3d>(BACK_1);
    IgV3d b2  = ci->get<IgV3d>(BACK_2);
    IgV3d b3  = ci->get<IgV3d>(BACK_3);
    IgV3d b4  = ci->get<IgV3d>(BACK_4);
    drawTowerHelper.addTowerOutline(f1,f2,f3,f4, b1,b2,b3,b4);

    vertices->vertex.set1Value(n++, SbVec3f(f1.x(), f1.y(), f1.z()));
    vertices->vertex.set1Value(n++, SbVec3f(f2.x(), f2.y(), f2.z()));
    vertices->vertex.set1Value(n++, SbVec3f(f3.x(), f3.y(), f3.z()));
    vertices->vertex.set1Value(n++, SbVec3f(f4.x(), f4.y(), f4.z()));
  }
  vertices->vertex.setNum(n);

  points->markerIndex = SoMarkerSet::PLUS_5_5;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild(points);
}



// ------------------------------------------------------
// Draw Calorimeter data
// ------------------------------------------------------

static void
make3DEnergyBoxes(IgCollection **collections, IgAssociationSet **, 
                  SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];
  float minEnergy     = 0.2;   // GeV  FIXME LT: should get it from some service
  float maxEnergy     = 5.0;  // GeV  Not a cut -- just used to set max box size

  // FIXME: can compress the following code

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>("energy");
    if (energy > maxEnergy)
    {
      maxEnergy = energy;
    }
  }

  IgDrawTowerHelper drawTowerHelper(sep);

  IgProperty FRONT_1 = c->getProperty("front_1");
  IgProperty FRONT_2 = c->getProperty("front_2");
  IgProperty FRONT_3 = c->getProperty("front_3");
  IgProperty FRONT_4 = c->getProperty("front_4");

  IgProperty BACK_1 = c->getProperty("back_1");
  IgProperty BACK_2 = c->getProperty("back_2");
  IgProperty BACK_3 = c->getProperty("back_3");
  IgProperty BACK_4 = c->getProperty("back_4");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>("energy");

    if (energy > minEnergy)
    {
      IgV3d f1  = ci->get<IgV3d>(FRONT_1);
      IgV3d f2  = ci->get<IgV3d>(FRONT_2);
      IgV3d f3  = ci->get<IgV3d>(FRONT_3);
      IgV3d f4  = ci->get<IgV3d>(FRONT_4);

      IgV3d b1  = ci->get<IgV3d>(BACK_1);
      IgV3d b2  = ci->get<IgV3d>(BACK_2);
      IgV3d b3  = ci->get<IgV3d>(BACK_3);
      IgV3d b4  = ci->get<IgV3d>(BACK_4);

      drawTowerHelper.addScaledBox(f1,f2,f3,f4, b1,b2,b3,b4, energy/maxEnergy);
    }
  }
}


static void
make3DEnergyTowers(IgCollection **collections, IgAssociationSet **, 
                   SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];

  float energyScaleFactor = 0.03;  // m/GeV    FIXME LT: should get it from some service
  // float energyScaleFactor = 0.2;  // increase to help PF electrons show up  
  float minEnergy     = 0.2;  // GeV      FIXME LT: should get it from some service

  IgDrawTowerHelper drawTowerHelper(sep);

  IgProperty FRONT_1 = c->getProperty("front_1");
  IgProperty FRONT_2 = c->getProperty("front_2");
  IgProperty FRONT_3 = c->getProperty("front_3");
  IgProperty FRONT_4 = c->getProperty("front_4");

  IgProperty BACK_1 = c->getProperty("back_1");
  IgProperty BACK_2 = c->getProperty("back_2");
  IgProperty BACK_3 = c->getProperty("back_3");
  IgProperty BACK_4 = c->getProperty("back_4");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>("energy");

    if (energy > minEnergy)
    {
      IgV3d f1  = ci->get<IgV3d>(FRONT_1);
      IgV3d f2  = ci->get<IgV3d>(FRONT_2);
      IgV3d f3  = ci->get<IgV3d>(FRONT_3);
      IgV3d f4  = ci->get<IgV3d>(FRONT_4);

      IgV3d b1  = ci->get<IgV3d>(BACK_1);
      IgV3d b2  = ci->get<IgV3d>(BACK_2);
      IgV3d b3  = ci->get<IgV3d>(BACK_3);
      IgV3d b4  = ci->get<IgV3d>(BACK_4);

      drawTowerHelper.addTower(f1,f2,f3,f4,
                               b1,b2,b3,b4,
                               energy,
                               energyScaleFactor);
    }
  }
}





static void
make3DEmCaloTowerShapes(IgCollection **collections, IgAssociationSet **, 
                        SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];
  float energyScaleFactor = 0.04; // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.5;  // GeV      FIXME LT: should get it from some service

  IgDrawTowerHelper drawTowerHelper(sep);

  IgProperty FRONT_1 = c->getProperty("front_1");
  IgProperty FRONT_2 = c->getProperty("front_2");
  IgProperty FRONT_3 = c->getProperty("front_3");
  IgProperty FRONT_4 = c->getProperty("front_4");

  IgProperty BACK_1 = c->getProperty("back_1");
  IgProperty BACK_2 = c->getProperty("back_2");
  IgProperty BACK_3 = c->getProperty("back_3");
  IgProperty BACK_4 = c->getProperty("back_4");

  IgProperty ENERGY = c->getProperty("emEnergy");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>(ENERGY);

    if (energy > minimumEnergy)
    {
      IgV3d f1  = ci->get<IgV3d>(FRONT_1);
      IgV3d f2  = ci->get<IgV3d>(FRONT_2);
      IgV3d f3  = ci->get<IgV3d>(FRONT_3);
      IgV3d f4  = ci->get<IgV3d>(FRONT_4);

      IgV3d b1  = ci->get<IgV3d>(BACK_1);
      IgV3d b2  = ci->get<IgV3d>(BACK_2);
      IgV3d b3  = ci->get<IgV3d>(BACK_3);
      IgV3d b4  = ci->get<IgV3d>(BACK_4);

      drawTowerHelper.addTower(f1,f2,f3,f4,
                               b1,b2,b3,b4,
                               energy,
                               energyScaleFactor);
    }
  }
}

static void
make3DEmPlusHadCaloTowerShapes(IgCollection **collections, IgAssociationSet **, 
                               SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  float energyScaleFactor = 0.04; // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.5;  // GeV      FIXME LT: should get it from some service

  IgDrawTowerHelper drawTowerHelper(sep);

  IgProperty FRONT_1 = c->getProperty("front_1");
  IgProperty FRONT_2 = c->getProperty("front_2");
  IgProperty FRONT_3 = c->getProperty("front_3");
  IgProperty FRONT_4 = c->getProperty("front_4");

  IgProperty BACK_1 = c->getProperty("back_1");
  IgProperty BACK_2 = c->getProperty("back_2");
  IgProperty BACK_3 = c->getProperty("back_3");
  IgProperty BACK_4 = c->getProperty("back_4");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double emEnergy = ci->get<double>("emEnergy");
    double hadEnergy = ci->get<double>("hadEnergy");

    if (hadEnergy > minimumEnergy)
    {
      IgV3d f1  = ci->get<IgV3d>(FRONT_1);
      IgV3d f2  = ci->get<IgV3d>(FRONT_2);
      IgV3d f3  = ci->get<IgV3d>(FRONT_3);
      IgV3d f4  = ci->get<IgV3d>(FRONT_4);

      IgV3d b1  = ci->get<IgV3d>(BACK_1);
      IgV3d b2  = ci->get<IgV3d>(BACK_2);
      IgV3d b3  = ci->get<IgV3d>(BACK_3);
      IgV3d b4  = ci->get<IgV3d>(BACK_4);

      drawTowerHelper.addTower(f1,f2,f3,f4,
                               b1,b2,b3,b4,
                               hadEnergy, (emEnergy>minimumEnergy?emEnergy:0),
                               energyScaleFactor);
    }
  }
}

static void 
make3DCaloClusters(IgCollection **collections, IgAssociationSet **assocs,
                   SoSeparator *sep, ISpyApplication::Style */*style*/)
{
  IgCollection       *clusters = collections[0];
  IgCollection       *fracs    = collections[1];
  IgAssociationSet   *assoc    = assocs[0];
  
  IgDrawTowerHelper drawTowerHelper(sep);

  IgProperty POS = clusters->getProperty("pos");
  IgProperty E   = clusters->getProperty("energy");

  IgProperty FRONT_1 = fracs->getProperty("front_1");
  IgProperty FRONT_2 = fracs->getProperty("front_2");
  IgProperty FRONT_3 = fracs->getProperty("front_3");
  IgProperty FRONT_4 = fracs->getProperty("front_4");
 
  for (IgCollectionIterator ci = clusters->begin(), ce = clusters->end(); ci != ce; ++ci)
  {
    IgV3d pos = ci->get<IgV3d>(POS);

    double energy = ci->get<double>(E);
    SoText2* label = new SoText2;
    label->justification.setValue(SoText2::CENTER);

    SoSeparator *annSep = new SoSeparator;
    SoTranslation *textPos = new SoTranslation;
    textPos->translation = SbVec3f(pos.x(), pos.y(), pos.z());

    char buf[128];
    std::string text = (sprintf(buf, "%0.2f", energy), buf) + std::string(" GeV");
    label->string = text.c_str();

    annSep->addChild(textPos);
    annSep->addChild(label);
    sep->addChild(annSep);

    for (IgAssociationSet::Iterator ai = assoc->begin(), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first().objectId() == ci->currentRow())
      { 
        IgCollectionItem rh(fracs, ai->second().objectId());

        IgV3d f1  = rh.get<IgV3d>(FRONT_1);
        IgV3d f2  = rh.get<IgV3d>(FRONT_2);
        IgV3d f3  = rh.get<IgV3d>(FRONT_3);
        IgV3d f4  = rh.get<IgV3d>(FRONT_4);
 
        drawTowerHelper.addTowerOutline(f1,f2,f3,f4,f1,f2,f3,f4);
      }
    }
  }
}


static void
make3DCaloTowers(IgCollection **collections, IgAssociationSet **assocs, 
                 SoSeparator *sep, ISpyApplication::Style *style)
{

  // FIXME LT: draw ECAL and HCAL parts in different colours

  make3DEmPlusHadCaloTowerShapes(collections, assocs, sep, style);
  make3DEmCaloTowerShapes(collections, assocs, sep, style);
}

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

static void
make3DPhoton(IgCollection **collections, IgAssociationSet **,
             SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection         *c = collections[0];
  IgProperty           E = c->getProperty("energy");
  IgProperty           ETA = c->getProperty("eta");
  IgProperty           PHI = c->getProperty("phi");
  IgProperty           POS = c->getProperty("pos");

  SoVertexProperty     *vertices = new SoVertexProperty;
  SoIndexedLineSet     *lineSet = new SoIndexedLineSet;

  std::vector<int>     lineIndices;
  std::vector<SbVec3f> points;
  int                  i = 0;

  double lEB = 3.0;  // half-length of the EB (m)
  double rEB = 1.24; // inner radius of the EB (m)

  for ( IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci )
  {    
    double eta = ci->get<double>(ETA);
    double phi = ci->get<double>(PHI);
    
    double px = cos(phi);
    double py = sin(phi);
    double pz = sinh(eta);

    IgV3d p1 = ci->get<IgV3d>(POS);

    double t = 0.0;

    double x0 = p1.x();
    double y0 = p1.y();
    double z0 = p1.z();

    if ( fabs(eta) > 1.48 ) // i.e. not in the EB, so propagate to ES
      t = fabs((lEB - z0)/pz); 

    else // propagate to EB
    {
      double a = px*px + py*py;
      double b = 2*x0*px + 2*y0*py;
      double c = x0*x0 + y0*y0 - rEB*rEB;
      t = (-b+sqrt(b*b-4*a*c))/2*a;
    }
    
    points.push_back(SbVec3f(x0, y0, z0));
    points.push_back(SbVec3f(x0+px*t, y0+py*t, z0+pz*t));

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
make3DJetShapes(IgCollection **collections, IgAssociationSet **, 
                SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  double                ecut = 5.0;  // FIXME LT: get value from some service
  IgProperty ET = c->getProperty("et");
  IgProperty THETA = c->getProperty("theta");
  IgProperty PHI = c->getProperty("phi");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double et    = ci->get<double>(ET);
    double theta = ci->get<double>(THETA);
    double phi   = ci->get<double>(PHI);

    if (et > ecut)
    {
      IgSoJet *recoJet = new IgSoJet;
      recoJet->theta.setValue(theta);
      recoJet->phi.setValue(phi);
      recoJet->energy.setValue(et);
      sep->addChild(recoJet);

      //    make3DJet(sep, et, theta, phi);   // FIXME LT: this should replace above lines but does not yet work

    }
  }
}


static void
make3DMET(IgCollection **collections, IgAssociationSet **, 
          SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<int>      lineIndices;
  std::vector<SbVec3f>  points;
  int                   i = 0;
  float etRadius = 8.0; // radius in x,y, to draw Etmiss vectors --- FIXME: calculate based on scene ???

  SbVec3f direction(0.,0.,0.);
  float etMiss = -999.;

  IgProperty PX = c->getProperty("px");
  IgProperty PY = c->getProperty("py");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    points.push_back(SbVec3f(0., 0., 0.));

    float px = ci->get<double>(PX);
    float py = ci->get<double>(PY);
    float pz = 0.0;

    direction.setValue(px, py, pz);
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

    direction *=1.05;     // Add text label a bit past the end of the line

    SoTranslation *textPos = new SoTranslation;
    textPos->translation = direction;

    SoText2 *label = new SoText2;
    label->justification.setValue(SoText2::CENTER);

    char buf [128];

    //      std::string run  = std::string("Run number") + (sprintf(buf, "%d", e.get<int>("run")), buf);

    std::string text = std::string("Et miss=")   + (sprintf(buf, "%4.1f", etMiss),buf);
    label->string = text.c_str();

    sep->addChild(textPos);
    sep->addChild(label);
  }
}




// ------------------------------------------------------
// Draw Muon data
// ------------------------------------------------------




static void
make3DSegmentShapes(IgCollection **collections, IgAssociationSet **, 
                    SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<int>      lineIndices;
  std::vector<SbVec3f>  points;
  int                   i = 0;

  IgProperty POS_1 = c->getProperty("pos_1");
  IgProperty POS_2 = c->getProperty("pos_2");
  
  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d>(POS_1);
    IgV3d p2 = ci->get<IgV3d>(POS_2);

    points.push_back(SbVec3f(p1.x(), p1.y(), p1.z() ));
    points.push_back(SbVec3f(p2.x(), p2.y(), p2.z() ));
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
make3DCSCDigis(IgCollection **collections, IgAssociationSet **, SoSeparator *sep,
               double width, double depth, double rotate)
{
  IgCollection  *c = collections[0];
  IgProperty    POS = c->getProperty("pos");
  IgProperty    LEN = c->getProperty("length");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d pos = ci->get<IgV3d>(POS);

    SoTranslation* trans = new SoTranslation;
    trans->translation = SbVec3f(pos.x(),pos.y(),pos.z());
   
    SbVec3f axis(0.0, 0.0, 1.0);
    double angle = -atan2(pos.x(),pos.y()) - rotate;

    SoTransform* xform = new SoTransform;
    xform->rotation = SbRotation(axis,angle);

    SoCube* hit = new SoCube;
    hit->width  = width;
    hit->height = ci->get<double>(LEN);
    hit->depth  = depth;

    SoSeparator* pulse = new SoSeparator;
    pulse->addChild(trans);
    pulse->addChild(xform);
    pulse->addChild(hit);
    sep->addChild(pulse);
  }
}

static void
make3DCSCWireDigis(IgCollection **collections, IgAssociationSet **assocs, 
                   SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  // FIXME: we should have something similar to the style
  //        infrastructure also for "cuts", so that the magic numbers here
  //        can disappear and we can have one make3DCSCDigis.
  make3DCSCDigis(collections,assocs,sep,0.02,0.01,M_PI*0.5);
}

static void
make3DCSCStripDigis(IgCollection **collections, IgAssociationSet **assocs, 
                    SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  // FIXME: we should have something similar to the style
  //        infrastructure also for "cuts", so that the magic numbers here
  //        can disappear and we can have one make3DCSCDigis.
  make3DCSCDigis(collections,assocs,sep,0.01,0.01,0.0);
}

static void
make3DDTDigis(IgCollection **collections, IgAssociationSet **, 
              SoSeparator *sep, ISpyApplication::Style * /*style*/)
{
  IgCollection          *c = collections[0];
  IgProperty            POS = c->getProperty("pos");
  IgProperty            AXIS = c->getProperty("axis");
  IgProperty            ANGLE = c->getProperty("angle");
  IgProperty            CELL_L = c->getProperty("cellLength");
  IgProperty            CELL_W = c->getProperty("cellWidth");
  IgProperty            CELL_H = c->getProperty("cellHeight");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d pos = ci->get<IgV3d>(POS);

    IgV3d axis = ci->get<IgV3d>(AXIS);
    double angle = ci->get<double>(ANGLE);

    SoTransform *transform = new SoTransform;
    transform->translation.setValue(pos.x(), pos.y(), pos.z());
    transform->rotation.setValue(SbVec3f(axis.x(),axis.y(),axis.z()), angle);

    SoCube *cube = new SoCube;
    cube->width = ci->get<double>(CELL_W);
    cube->height = ci->get<double>(CELL_L);
    cube->depth = ci->get<double>(CELL_H);

    SoSeparator *separator = new SoSeparator;
    separator->addChild(transform);
    separator->addChild(cube);
    sep->addChild(separator);
  }
}

static void
make3DDTRecHits(IgCollection **collections, IgAssociationSet **, 
                SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *c = collections[0];
  IgProperty            LPLUS_GLOBALPOS = c->getProperty("lPlusGlobalPos");
  IgProperty            LMINUS_GLOBALPOS = c->getProperty("lMinusGlobalPos");
  IgProperty            RPLUS_GLOBALPOS = c->getProperty("rPlusGlobalPos");
  IgProperty            RMINUS_GLOBALPOS = c->getProperty("rMinusGlobalPos");
  IgProperty            LGLOBALPOS = c->getProperty("lGlobalPos");
  IgProperty            RGLOBALPOS = c->getProperty("rGlobalPos");
  IgProperty            WPOS = c->getProperty("wirePos");
  IgProperty            AXIS = c->getProperty("axis");
  IgProperty            ANGLE = c->getProperty("angle");
  IgProperty            CELL_L = c->getProperty("cellLength");
  IgProperty            CELL_W = c->getProperty("cellWidth");
  IgProperty            CELL_H = c->getProperty("cellHeight");
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoMarkerSet           *points = new SoMarkerSet;
  int                   n = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d lPlusGlobalPos = ci->get<IgV3d>(LPLUS_GLOBALPOS);
    IgV3d lMinusGlobalPos = ci->get<IgV3d>(LMINUS_GLOBALPOS);
    IgV3d rPlusGlobalPos = ci->get<IgV3d>(RPLUS_GLOBALPOS);
    IgV3d rMinusGlobalPos = ci->get<IgV3d>(RMINUS_GLOBALPOS);
    IgV3d lGlobalPos = ci->get<IgV3d>(LGLOBALPOS);
    IgV3d rGlobalPos = ci->get<IgV3d>(RGLOBALPOS);

    SoLineSet *linel = new SoLineSet;
    linel->numVertices = 2;
    SoVertexProperty* vtxl = new SoVertexProperty;
    vtxl->vertex.set1Value(0, SbVec3f(lPlusGlobalPos.x(),
                                      lPlusGlobalPos.y(),
                                      lPlusGlobalPos.z()));

    vtxl->vertex.set1Value(1, SbVec3f(lMinusGlobalPos.x(),
                                      lMinusGlobalPos.y(),
                                      lMinusGlobalPos.z()));
    linel->vertexProperty = vtxl;

    SoLineSet *liner = new SoLineSet;
    liner->numVertices = 2;
    SoVertexProperty *vtxr = new SoVertexProperty;
    vtxr->vertex.set1Value(0, SbVec3f(rPlusGlobalPos.x(),
                                      rPlusGlobalPos.y(),
                                      rPlusGlobalPos.z()));
    vtxr->vertex.set1Value(1, SbVec3f(rMinusGlobalPos.x(),
                                      rMinusGlobalPos.y(),
                                      rMinusGlobalPos.z()));
    liner->vertexProperty = vtxr;

    sep->addChild(linel);
    sep->addChild(liner);

    vertices->vertex.set1Value(n++, SbVec3f(lGlobalPos.x(),
                                            lGlobalPos.y(),
                                            lGlobalPos.z()));

    vertices->vertex.set1Value(n++, SbVec3f(rGlobalPos.x(),
                                            rGlobalPos.y(),
                                            rGlobalPos.z()));

    IgV3d pos = ci->get<IgV3d>(WPOS);
    IgV3d axis = ci->get<IgV3d>(AXIS);
    double angle = ci->get<double>(ANGLE);
    SoTransform *transform = new SoTransform;
    transform->translation.setValue(pos.x(),pos.y(),pos.z());
    transform->rotation.setValue(SbVec3f(axis.x(),axis.y(),axis.z()), angle);

    SoCube *cube = new SoCube;
    cube->width = ci->get<double>(CELL_W);
    cube->height = ci->get<double>(CELL_L);
    cube->depth = ci->get<double>(CELL_H);

    SoSeparator *separator = new SoSeparator;
    separator->addChild(transform);
    separator->addChild(cube);
    sep->addChild(separator);
  }

  points->markerIndex = SoMarkerSet::PLUS_7_7;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild(points);
}

static void
make3DRPCRecHits(IgCollection **collections, IgAssociationSet **, 
                 SoSeparator *sep, ISpyApplication::Style * /*style*/)
{

  IgCollection          *c = collections[0];
  SoVertexProperty      *vertices = new SoVertexProperty;
  SoIndexedLineSet      *lineSet = new SoIndexedLineSet;
  std::vector<int>      lineIndices;
  std::vector<SbVec3f>  points;
  int                   i = 0;

  IgProperty U1 = c->getProperty("u1");
  IgProperty U2 = c->getProperty("u2");
  IgProperty V1 = c->getProperty("v1");
  IgProperty V2 = c->getProperty("v2");
  IgProperty W1 = c->getProperty("w1");
  IgProperty W2 = c->getProperty("w2");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d u1 = ci->get<IgV3d>(U1);
    IgV3d u2 = ci->get<IgV3d>(U2);
    IgV3d v1 = ci->get<IgV3d>(V1);
    IgV3d v2 = ci->get<IgV3d>(V2);
    IgV3d w1 = ci->get<IgV3d>(W1);
    IgV3d w2 = ci->get<IgV3d>(W2);

    points.push_back(SbVec3f( u1.x(), u1.y(), u1.z() ));
    points.push_back(SbVec3f( u2.x(), u2.y(), u2.z() ));
    points.push_back(SbVec3f( v1.x(), v1.y(), v1.z() ));
    points.push_back(SbVec3f( v2.x(), v2.y(), v2.z() ));
    points.push_back(SbVec3f( w1.x(), w1.y(), w1.z() ));
    points.push_back(SbVec3f( w2.x(), w2.y(), w2.z() ));

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
make3DTrackPoints(IgCollection **collections, IgAssociationSet **assocs, 
                  SoSeparator *sep, ISpyApplication::Style * /* style */)
{
  IgCollection          *tracks = collections[0];
  IgCollection          *points = collections[1];
  IgAssociationSet      *assoc = assocs[0];

  IgProperty POS = points->getProperty("pos");
  for (IgCollectionIterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;

    int n = 0;

    for (IgAssociationSet::Iterator ai = assoc->begin(), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first().objectId() == ci->currentRow())
      {
        IgCollectionItem hm(points, ai->second().objectId());
        IgV3d pos = hm.get<IgV3d>(POS);
        track->controlPoints.set1Value(n, SbVec3f(pos.x(), pos.y(), pos.z()));
        track->markerPoints.set1Value(n, SbVec3f(pos.x(), pos.y(), pos.z()));
        n++;
      }
    }
    sep->addChild(track);
  }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Initialise but do not yet run the application object. */
ISpyApplication::ISpyApplication(void)
  : m_argc(-1),
    m_argv(0),
    m_appname(0),
    m_eventIndex(0),
    m_currentViewIndex(0),
    m_listModel(0),
    m_tableModel(0),
    m_3DModel(0),
    m_mainWindow(0),
    m_treeWidget(0),
    m_splash(0),
    m_online (false),
    m_autoEvents(false),
    m_exiting(false),
    m_animationTimer(new QTimer(this)),
    m_timer(new QTimer(this)),
    m_idleTimer(new QTimer(this)),
    m_networkManager(new QNetworkAccessManager),
    m_progressDialog(0),
    m_3DToolBar(0),
    m_actionCameraPerspective(0),
    m_actionCameraOrthographic(0),
    m_printTimer(new QTimer(this))
{
  m_archives[0] = m_archives[1] = 0;
  m_storages[0] = new IgDataStorage;
  m_storages[1] = new IgDataStorage;

#ifndef Q_WS_MAC
  QCoreApplication::setApplicationName("ispy");
#else
  QCoreApplication::setApplicationName("iSpy");
#endif
  QCoreApplication::setApplicationVersion("1.3.1");
  QCoreApplication::setOrganizationDomain("iguana");
  QCoreApplication::setOrganizationName("iguana");

  if (QDir::home().isReadable())
    defaultSettings();

  // Define event filters
  // A filter is defined by a friendly collection name.
  // The collection must have the fields specified here.
  //
  filter("Provenance/L1 Triggers", "L1GtTrigger_V1:algorithm:result");
  filter("Provenance/HLT Trigger Paths", "TriggerPaths_V1:Name:Accept");

  // Create the default style reading the specification from a QT resource.
  style("*", "");
  style("Background","diffuse-color: rbg(1.,0,0);");
  QFile cssFile(":/css/default-style.css");
  bool ok = cssFile.open(QIODevice::ReadOnly | QIODevice::Text);
  ASSERT(ok && "Default style not compiled as resource.");
  QByteArray cssData = cssFile.readAll();
  ASSERT(cssData.size());
  parseCss(cssData.data());

// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
  
  float position1[3] = {-18.1, 8.6, 14.0};
  float pointAt1[3] = {0, 0, 0};
  camera(position1, pointAt1, 10.6, true, true);
  visibilityGroup();

  view("Standard 3D View", true);
  
  collection("Provenance/Event information",
             "Event_V1:time:run:event:ls:orbit:bx",
             0,
             0,
             make3DEvent,
             Qt::Checked);

  collection("Provenance/L1 Triggers",
             "L1GtTrigger_V1",
             0,
             0,
             make3DL1Trigger,
             Qt::Checked);

  collection("Provenance/HLT Trigger Paths",
             "TriggerPaths_V1",
             0,
             0,
             0,
             Qt::Unchecked);

  collection("Provenance/Trigger Objects",
             "TriggerObjects_V1:VID:pt:eta:phi",
             0,
             0,
             make3DTriggerObject,
             Qt::Unchecked);

  collection("Provenance/Data Products found", 
             "Products_V1", 
             0, 
             0,
             0,
             Qt::Unchecked);

  collection("Provenance/Data Products not found", 
             "Errors_V1", 
             0, 
             0, 
             0,
             Qt::Unchecked);
  
// -------------------------------------------------------------------------------------

  // For now, draw the detector with the default "make3DAnyBox" code but explicit state
  // this in each case so lables in visibility widget a nice than default ones
  
  collection("Detector/Tracker",
             "Tracker3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/ECAL Barrel",
             "EcalBarrel3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/ECAL Endcap",
             "EcalEndcap3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/Preshower",
             "EcalPreshower3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/HCAL Barrel",
             "HcalBarrel3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/HCAL Endcap",
             "HcalEndcap3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/HCAL Outer",
             "HcalOuter3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/HCAL Forward",
             "HcalForward3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/Drift Tubes",
             "DTs3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/Cathode Strip Chambers",
             "CSC3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/Resistive Plate Chambers",
             "RPC3D_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("Tracking/Tracks (reco.)",
             "Tracks_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "TrackExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Tracking/Tracks (GSF)",
             "GsfTracks_V1:pt:pos:dir",
             "GsfExtras_V1:pos_1:dir_1:pos_2:dir_2",
             "GsfTrackExtras_V1",
             make3DTracks,
             Qt::Unchecked);
  
  collection("Tracking/Digis (Si Pixels)",
             "PixelDigis_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Digis (Si Strips)",
             "SiStripDigis_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Clusters (Si Pixels)",
             "SiPixelClusters_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Clusters (Si Strips)",
             "SiStripClusters_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Rec. Hits (Si Pixels)",
             "SiPixelRecHits_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Rec. Hits (Tracking)",
             "TrackingRecHits_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("ECAL/ECAL Rec. Hits",     // pre-Aug 2009 ig files only
             "EcalRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Checked);

  collection("ECAL/Barrel Rec. Hits",
             "EBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Checked);

  collection("ECAL/Endcap Rec. Hits",
             "EERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Checked);
  
  collection("ECAL/Preshower Rec. Hits",
             "ESRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DPreshowerTowers,
             Qt::Checked);

  collection("ECAL/CaloClusters",
             "CaloClusters_V1:pos:energy",
             "RecHitFractions_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             "CaloClusterRecHitFractions_V1",
             make3DCaloClusters,
             Qt::Checked);

  collection("ECAL/SuperClusters",
             "SuperClusters_V1:pos:energy",
             "RecHitFractions_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             "SuperClusterRecHitFractions_V1",
             make3DCaloClusters,
             Qt::Checked);

// -------------------------------------------------------------------------------------
  
  collection("HCAL/Barrel Rec. Hits",
             "HBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyBoxes,
             Qt::Checked);

  collection("HCAL/Endcap Rec. Hits",
             "HERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyBoxes,
             Qt::Checked);

  collection("HCAL/Forward Rec. Hits",
             "HFRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyBoxes,
             Qt::Checked);

  collection("HCAL/Outer Rec. Hits",
             "HORecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyBoxes,
             Qt::Checked);

// -------------------------------------------------------------------------------------
  
  collection("Muon/DT Digis",
             "DTDigis_V1:pos:axis:angle:cellWidth:cellLength:cellWidth:cellHeight",
             0,
             0,
             make3DDTDigis,
             Qt::Unchecked);

  collection("Muon/DT Rec. Hits",
             "DTRecHits_V1:lPlusGlobalPos:lMinusGlobalPos:rPlusGlobalPos:rMinusGlobalPos"
             ":lGlobalPos:rGlobalPos:wirePos:axis:angle:cellWidth:cellLength:cellHeight",
             0,
             0,
             make3DDTRecHits,
             Qt::Unchecked);

  collection("Muon/DT Rec. Segments (4D)",
             "DTRecSegment4D_V1:pos_1:pos_2",
             0,
             0,
             make3DSegmentShapes,
             Qt::Checked);

  collection("Muon/CSC Segments",
             "CSCSegments_V1:pos_1:pos_2",
             0,
             0,
             make3DSegmentShapes,
             Qt::Checked);

  collection("Muon/CSC Wire Digis",
             "CSCWireDigis_V1:pos:length",
             0,
             0,
             make3DCSCWireDigis,
             Qt::Checked);

  collection("Muon/CSC Strip Digis",
             "CSCStripDigis_V1:pos:length",
             0,
             0,
             make3DCSCStripDigis,
             Qt::Checked);

  collection("Muon/RPC Rec. Hits",
             "RPCRecHits_V1:u1:u2:v1:v2:w2",
             0,
             0,
             make3DRPCRecHits,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Particle Flow/Rec. Tracks",
             "PFRecTracks_V1:pt:charge:phi:eta",
             "PFTrajectoryPoints_V1:pos",
             "PFRecTrackTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

  collection("Particle Flow/GSF Tracks",
             "GsfPFRecTracks_V1:pt:charge:phi:eta",
             "PFTrajectoryPoints_V1:pos",
             "GsfPFRecTrackTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

  collection("Particle Flow/ECAL Barrel Rec. Hits",
             "PFEBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Unchecked);

  collection("Particle Flow/ECAL Endcap Rec. Hits",
             "PFEERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Unchecked);

  collection("Particle Flow/Bremsstrahlung candidate tangents",
             "PFBrems_V1:deltaP:sigmadeltaP",
             "PFTrajectoryPoints_V1:pos",
             "PFBremTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("Physics Objects/Muons (Reco)",
             "Muons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Tracker Muons (Reco)",
             "TrackerMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (Reco)",
             "StandaloneMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonStandalonePoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (Reco)",
             "StandaloneMuons_V2:pt:charge:pos:phi:eta",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "MuonTrackExtras_V1",
             make3DTracks,
             Qt::Checked);
  
  collection("Physics Objects/Global Muons (Reco)",
             "GlobalMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonGlobalPoints_V1",
             make3DTrackPoints,
             Qt::Checked);
  
  collection("Physics Objects/Tracker Muons (PAT)",
             "PATTrackerMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "PATMuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (PAT)",
             "PATStandaloneMuons_V1:pt:charge:pos:phi:eta",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "PATMuonTrackExtras_V1",
             make3DTracks,
             Qt::Checked);
  
  collection("Physics Objects/Global Muons (PAT)",
             "PATGlobalMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "PATMuonGlobalPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Electrons (GSF)",
             "GsfElectrons_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "GsfElectronExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Physics Objects/Electrons (PAT)",
             "PATElectrons_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "PATElectronExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Physics Objects/Photons (Reco)",
             "Photons_V1:energy:eta:phi:pos",
             0,
             0,
             make3DPhoton,
             Qt::Checked);

  collection("Physics Objects/Photons (PAT)",
             "PATPhotons_V1:energy:eta:phi:pos",
             0,
             0,
             make3DPhoton,
             Qt::Checked);

  collection("Physics Objects/Calorimeter Energy Towers",
             "CaloTowers_V1:emEnergy:hadEnergy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DCaloTowers,
             Qt::Unchecked);

  collection("Physics Objects/Jets",
             "Jets_V1:et:theta:phi",
             0,
             0,
             make3DJetShapes,
             Qt::Unchecked);

  collection("Physics Objects/Missing Et",
             "METs_V1:pt:px:py:phi",
             0,
             0,
             make3DMET,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("Monte-Carlo/Sim. tracks with hits",
             "TrackingParticles_V1",
             "PSimHits_V1:pos:dir",
             "TrackingParticlePSimHits_V1",
             make3DTrackingParticles,
             Qt::Unchecked);


// -------------------------------------------------------------------------------------

// First specify collections that exist that never appear in tree widget 
  collection("Collections not drawn/Extras_V1", "Extras_V1", 0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/Hits_V1", "Hits_V1", 0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/Points_V1", "Points_V1", 0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/DetIds_V1", "DetIds_V1", 0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/PSimHits_V1","PSimHits_V1",0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/GsfExtras_V1", "GsfExtras_V1", 0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/PFTrajectoryPoints_V1", "PFTrajectoryPoints_V1", 0, 0, 0, Qt::Unchecked);
  collection("Collections not drawn/PFRecHitFractions_V1", "PFRecHitFractions_V1", 0, 0, 0, Qt::Unchecked);

  // Do not draw the geometries which are only for the other views.

  collection("Collections not drawn/TrackerRPhi_V1",
             "TrackerRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/TrackerRZ_V1",
	     "TrackerRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/EcalBarrelRPhi_V1",
	     "EcalBarrelRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/EcalBarrelRZ_V1",
	     "EcalBarrelRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/EcalPreshowerRZ_V1",
	     "EcalPreshowerRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/EcalEndcapRZ_V1",
	     "EcalEndcapRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/HcalBarrelRPhi_V1",
	     "HcalBarrelRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/HcalBarrelRZ_V1",
	     "HcalBarrelRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/HcalEndcapRZ_V1",
	     "HcalEndcapRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/CaloLego_V1",
	     "CaloLego_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/HcalOuterRPhi_V1",
	     "HcalOuterRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/HcalOuterRZ_V1",
	     "HcalOuterRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/HcalForwardRZ_V1",
	     "HcalForwardRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/CSCRZ_V1",
	     "CSCRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);
  
  collection("Collections not drawn/DTsRPhi_V1",
	     "DTsRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/DTsRZ_V1",
	     "DTsRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/RPCRPhi_V1",
	     "RPCRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);

  collection("Collections not drawn/RPCRZ_V1",
	     "RPCRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             0,
             Qt::Checked);


// ///////////////////////////////////////////////////////////////////////////////
  

  // Attempt generic default drawing operations if none of the above explicitly matched

  collection(0, ":front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4", 0, 0, make3DAnyBox, Qt::Unchecked);
  collection(0, ":pos_1:pos_2", 0, 0, make3DAnyLine, Qt::Unchecked);
  collection(0, ":pos", 0, 0, make3DAnyPoint, Qt::Unchecked);
  collection(0, ":detid", 0, 0, make3DAnyDetId, Qt::Unchecked);



// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////

  float position2[] = {0, 0, 1};
  float pointAt2[] = {0, 0, 0};
  camera(position2, pointAt2, 8.5, true, false);
  visibilityGroup();

  view("Standard R-Phi View", true);

// -------------------------------------------------------------------------------------

  collection("Provenance/Event information",
             "Event_V1:time:run:event:ls:orbit:bx",
             0,
             0,
             make3DEvent,
             Qt::Checked);

  collection("Provenance/L1 Triggers",
             "L1GtTrigger_V1",
             0,
             0,
             make3DL1Trigger,
             Qt::Checked);

  collection("Provenance/HLT Trigger Paths",
             "TriggerPaths_V1",
             0,
             0,
             0,
             Qt::Unchecked);

  collection("Provenance/Trigger Objects",
             "TriggerObjects_V1:VID:pt:eta:phi",
             0,
             0,
             make3DTriggerObject,
             Qt::Unchecked);

  collection("Provenance/Data Products found", 
             "Products_V1", 
             0, 
             0,
             0,
             Qt::Checked);

  collection("Provenance/Data Products not found", 
             "Errors_V1", 
             0, 
             0, 
             0,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Detector/Tracker",
             "TrackerRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/ECAL Barrel",
             "EcalBarrelRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/HCAL Barrel",
             "HcalBarrelRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/HCAL Outer",
             "HcalOuterRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/Drift Tubes",
             "DTsRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/Resistive Plate Chambers",
             "RPCRPhi_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("Tracking/Tracks (reco.)",
             "Tracks_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "TrackExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Tracking/Tracks (GSF)",
             "GsfTracks_V1:pt:pos:dir",
             "GsfExtras_V1:pos_1:dir_1:pos_2:dir_2",
             "GsfTrackExtras_V1",
             make3DTracks,
             Qt::Unchecked);
  
  collection("Tracking/Digis (Si Pixels)",
             "PixelDigis_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Digis (Si Strips)",
             "SiStripDigis_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Clusters (Si Pixels)",
             "SiPixelClusters_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Clusters (Si Strips)",
             "SiStripClusters_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Rec. Hits (Si Pixels)",
             "SiPixelRecHits_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Rec. Hits (Tracking)",
             "TrackingRecHits_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("ECAL/ECAL Rec. Hits",   // pre-Aug 2009 ig files only
             "EcalRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Checked);

  collection("ECAL/Barrel Rec. Hits",
             "EBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Checked);

 collection("ECAL/CaloClusters",
             "CaloClusters_V1:pos:energy",
             "RecHitFractions_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             "CaloClusterRecHitFractions_V1",
             make3DCaloClusters,
             Qt::Checked);

  collection("ECAL/SuperClusters",
             "SuperClusters_V1:pos:energy",
             "RecHitFractions_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             "SuperClusterRecHitFractions_V1",
             make3DCaloClusters,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("HCAL/Barrel Rec. Hits",
             "HBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyBoxes,
             Qt::Checked);

  collection("HCAL/Outer Rec. Hits",
             "HORecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyBoxes,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Muon/DT Digis",
             "DTDigis_V1:pos:axis:angle:cellWidth:cellLength:cellWidth:cellHeight",
             0,
             0,
             make3DDTDigis,
             Qt::Unchecked);

  collection("Muon/DT Rec. Hits",
             "DTRecHits_V1:lPlusGlobalPos:lMinusGlobalPos:rPlusGlobalPos:rMinusGlobalPos"
             ":lGlobalPos:rGlobalPos:wirePos:axis:angle:cellWidth:cellLength:cellHeight",
             0,
             0,
             make3DDTRecHits,
             Qt::Unchecked);

  collection("Muon/DT Rec. Segments (4D)",
             "DTRecSegment4D_V1:pos_1:pos_2",
             0,
             0,
             make3DSegmentShapes,
             Qt::Checked);

  collection("Muon/CSC Segments",
             "CSCSegments_V1:pos_1:pos_2",
             0,
             0,
             make3DSegmentShapes,
             Qt::Checked);

  collection("Muon/RPC Rec. Hits",
             "RPCRecHits_V1:u1:u2:v1:v2:w2",
             0,
             0,
             make3DRPCRecHits,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Particle Flow/Rec. Tracks",
             "PFRecTracks_V1:pt:charge:phi:eta",
             "PFTrajectoryPoints_V1:pos",
             "PFRecTrackTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

  collection("Particle Flow/GSF Tracks",
             "GsfPFRecTracks_V1:pt:charge:phi:eta",
             "PFTrajectoryPoints_V1:pos",
             "GsfPFRecTrackTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

  collection("Particle Flow/ECAL barrel Rec. Hits",
             "PFEBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Unchecked);

  collection("Particle Flow/Bremsstrahlung candidate tangents",
             "PFBrems_V1:deltaP:sigmadeltaP",
             "PFTrajectoryPoints_V1:pos",
             "PFBremTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("Physics Objects/Muons (Reco)",
             "Muons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Tracker Muons (Reco)",
             "TrackerMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (Reco)",
             "StandaloneMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonStandalonePoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (Reco)",
             "StandaloneMuons_V2:pt:charge:pos:phi:eta",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "MuonTrackExtras_V1",
             make3DTracks,
             Qt::Checked);
  
  collection("Physics Objects/Global Muons (Reco)",
             "GlobalMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonGlobalPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Tracker Muons (PAT)",
             "PATTrackerMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "PATMuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (PAT)",
             "PATStandaloneMuons_V1:pt:charge:pos:phi:eta",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "PATMuonTrackExtras_V1",
             make3DTracks,
             Qt::Checked);
  
  collection("Physics Objects/Global Muons (PAT)",
             "PATGlobalMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "PATMuonGlobalPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

   collection("Physics Objects/Electrons (GSF)",
             "GsfElectrons_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "GsfElectronExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Physics Objects/Electrons (PAT)",
             "PATElectrons_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "PATElectronExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Physics Objects/Photons (Reco)",
             "Photons_V1:energy:eta:phi:pos",
             0,
             0,
             make3DPhoton,
             Qt::Checked);

  collection("Physics Objects/Photons (PAT)",
             "PATPhotons_V1:energy:eta:phi:pos",
             0,
             0,
             make3DPhoton,
             Qt::Checked);

  collection("Physics Objects/Calorimeter Energy Towers",
             "CaloTowers_V1:emEnergy:hadEnergy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DCaloTowers,
             Qt::Unchecked);

  collection("Physics Objects/Jets",
             "Jets_V1:et:theta:phi",
             0,
             0,
             make3DJetShapes,
             Qt::Unchecked);

  collection("Physics Objects/Missing Et",
             "METs_V1:pt:px:py:phi",
             0,
             0,
             make3DMET,
             Qt::Unchecked);

  collection("Monte-Carlo/Sim. tracks with hits",
             "TrackingParticles_V1",
             "PSimHits_V1:pos:dir",
             "TrackingParticlePSimHits_V1",
             make3DTrackingParticles,
             Qt::Unchecked);

// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////

  float positionRZ[3] = {-1, 0, 0};
  float pointAtRZ[3] = {0, 0, 0};
  camera(positionRZ, pointAtRZ, 8.5, true, false);
  visibilityGroup();

  view("Standard R-Z View", true);


// -------------------------------------------------------------------------------------

  collection("Provenance/Event information",
             "Event_V1:time:run:event:ls:orbit:bx",
             0,
             0,
             make3DEvent,
             Qt::Checked);

  collection("Provenance/L1 Triggers",
             "L1GtTrigger_V1",
             0,
             0,
             make3DL1Trigger,
             Qt::Checked);

  collection("Provenance/HLT Trigger Paths",
             "TriggerPaths_V1",
             0,
             0,
             0,
             Qt::Unchecked);

  collection("Provenance/Trigger Objects",
             "TriggerObjects_V1:VID:pt:eta:phi",
             0,
             0,
             make3DTriggerObject,
             Qt::Unchecked);

  collection("Provenance/Data Products found", 
             "Products_V1", 
             0, 
             0,
             0,
             Qt::Checked);

  collection("Provenance/Data Products not found", 
             "Errors_V1", 
             0, 
             0, 
             0,
             Qt::Checked);
  
// -------------------------------------------------------------------------------------

  collection("Detector/Tracker",
             "TrackerRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/ECAL Barrel",
             "EcalBarrelRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/ECAL Endcap",
             "EcalEndcapRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/Preshower",
             "EcalPreshowerRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/HCAL Barrel",
             "HcalBarrelRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/HCAL Endcap",
             "HcalEndcapRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/HCAL Outer",
             "HcalOuterRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

  collection("Detector/HCAL Forward",
             "HcalForwardRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/Drift Tubes",
             "DTsRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/Cathode Strip Chambers",
             "CSCRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Checked);

  collection("Detector/Resistive Plate Chambers",
             "RPCRZ_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DAnyBox,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------


  collection("Tracking/Tracks (reco.)",
             "Tracks_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "TrackExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Tracking/Tracks (GSF)",
             "GsfTracks_V1:pt:pos:dir",
             "GsfExtras_V1:pos_1:dir_1:pos_2:dir_2",
             "GsfTrackExtras_V1",
             make3DTracks,
             Qt::Unchecked);
  
  collection("Tracking/Digis (Si Pixels)",
             "PixelDigis_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Digis (Si Strips)",
             "SiStripDigis_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Clusters (Si Pixels)",
             "SiPixelClusters_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Clusters (Si Strips)",
             "SiStripClusters_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Rec. Hits (Si Pixels)",
             "SiPixelRecHits_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

  collection("Tracking/Rec. Hits (Tracking)",
             "TrackingRecHits_V1:pos",
             0,
             0,
             make3DPointSetShapes,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("ECAL/ECAL Rec. Hits",   // pre-Aug 2009 ig files only
             "EcalRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZECalRecHits,
             Qt::Checked);

  collection("ECAL/Barrel Rec. Hits",
             "EBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZECalRecHits,
             Qt::Checked);

  collection("ECAL/Endcap Rec. Hits",
             "EERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZECalRecHits,
             Qt::Checked);
  
  collection("ECAL/Preshower Rec. Hits",
             "ESRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZEPRecHits,
             Qt::Checked);

   collection("ECAL/CaloClusters",
             "CaloClusters_V1:pos:energy",
             "RecHitFractions_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             "CaloClusterRecHitFractions_V1",
             make3DCaloClusters,
             Qt::Checked);

  collection("ECAL/SuperClusters",
             "SuperClusters_V1:pos:energy",
             "RecHitFractions_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             "SuperClusterRecHitFractions_V1",
             make3DCaloClusters,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("HCAL/Barrel Rec. Hits",
             "HBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZHCalRecHits,
             Qt::Checked);

  collection("HCAL/Endcap Rec. Hits",
             "HERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZHCalRecHits,
             Qt::Checked);

  collection("HCAL/Forward Rec. Hits",
             "HFRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZHCalRecHits,
             Qt::Checked);

  collection("HCAL/Outer Rec. Hits",
             "HORecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeRZHCalRecHits,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Muon/DT Digis",
             "DTDigis_V1:pos:axis:angle:cellWidth:cellLength:cellWidth:cellHeight",
             0,
             0,
             make3DDTDigis,
             Qt::Unchecked);

  collection("Muon/DT Rec. Hits",
             "DTRecHits_V1:lPlusGlobalPos:lMinusGlobalPos:rPlusGlobalPos:rMinusGlobalPos"
             ":lGlobalPos:rGlobalPos:wirePos:axis:angle:cellWidth:cellLength:cellHeight",
             0,
             0,
             make3DDTRecHits,
             Qt::Unchecked);

  collection("Muon/DT Rec. Segments (4D)",
             "DTRecSegment4D_V1:pos_1:pos_2",
             0,
             0,
             make3DSegmentShapes,
             Qt::Checked);

  collection("Muon/CSC Segments",
             "CSCSegments_V1:pos_1:pos_2",
             0,
             0,
             make3DSegmentShapes,
             Qt::Checked);

  collection("Muon/CSC Wire Digis",
             "CSCWireDigis_V1:pos:length",
             0,
             0,
             make3DCSCWireDigis,
             Qt::Checked);

  collection("Muon/CSC Strip Digis",
             "CSCStripDigis_V1:pos:length",
             0,
             0,
             make3DCSCStripDigis,
             Qt::Checked);

  collection("Muon/RPC Rec. Hits",
             "RPCRecHits_V1:u1:u2:v1:v2:w2",
             0,
             0,
             make3DRPCRecHits,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Particle Flow/Rec. Tracks",
             "PFRecTracks_V1:pt:charge:phi:eta",
             "PFTrajectoryPoints_V1:pos",
             "PFRecTrackTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

  collection("Particle Flow/GSF Tracks",
             "GsfPFRecTracks_V1:pt:charge:phi:eta",
             "PFTrajectoryPoints_V1:pos",
             "GsfPFRecTrackTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

  collection("Particle Flow/ECAL Barrel Rec. Hits",
             "PFEBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Unchecked);

  collection("Particle Flow/ECAL Endcap Rec. Hits",
             "PFEERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DEnergyTowers,
             Qt::Unchecked);

  collection("Particle Flow/Bremsstrahlung candidate tangents",
             "PFBrems_V1:deltaP:sigmadeltaP",
             "PFTrajectoryPoints_V1:pos",
             "PFBremTrajectoryPoints_V1",
             make3DTrackPoints,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------
   
  collection("Physics Objects/Muons (Reco)",
             "Muons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Tracker Muons (Reco)",
             "TrackerMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (Reco)",
             "StandaloneMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonStandalonePoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (Reco)",
             "StandaloneMuons_V2:pt:charge:pos:phi:eta",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "MuonTrackExtras_V1",
             make3DTracks,
             Qt::Checked);
  
  collection("Physics Objects/Global Muons (Reco)",
             "GlobalMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "MuonGlobalPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Tracker Muons (PAT)",
             "PATTrackerMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "PATMuonTrackerPoints_V1",
             make3DTrackPoints,
             Qt::Checked);

  collection("Physics Objects/Stand-alone Muons (PAT)",
             "PATStandaloneMuons_V1:pt:charge:pos:phi:eta",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "PATMuonTrackExtras_V1",
             make3DTracks,
             Qt::Checked);
  
  collection("Physics Objects/Global Muons (PAT)",
             "PATGlobalMuons_V1:pt:charge:phi:eta",
             "Points_V1:pos",
             "PATMuonGlobalPoints_V1",
             make3DTrackPoints,
             Qt::Checked);
  
  collection("Physics Objects/Electrons (GSF)",
             "GsfElectrons_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "GsfElectronExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Physics Objects/Electrons (PAT)",
             "PATElectrons_V1:pt:pos:dir",
             "Extras_V1:pos_1:dir_1:pos_2:dir_2",
             "PATElectronExtras_V1",
             make3DTracks,
             Qt::Checked);

  collection("Physics Objects/Photons (Reco)",
             "Photons_V1:energy:eta:phi:pos",
             0,
             0,
             make3DPhoton,
             Qt::Checked);

  collection("Physics Objects/Photons (PAT)",
             "PATPhotons_V1:energy:eta:phi:pos",
             0,
             0,
             make3DPhoton,
             Qt::Checked);

  collection("Physics Objects/Calorimeter Energy Towers",
             "CaloTowers_V1:emEnergy:hadEnergy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             make3DCaloTowers,
             Qt::Unchecked);

  collection("Physics Objects/Jets",
             "Jets_V1:et:theta:phi",
             0,
             0,
             make3DJetShapes,
             Qt::Unchecked);

  collection("Physics Objects/Missing Et",
             "METs_V1:pt:px:py:phi",
             0,
             0,
             make3DMET,
             Qt::Unchecked);

// -------------------------------------------------------------------------------------

  collection("Monte Carlo/Sim. tracks with hits",
             "TrackingParticles_V1",
             "PSimHits_V1:pos:dir",
             "TrackingParticlePSimHits_V1",
             make3DTrackingParticles,
             Qt::Unchecked);


// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////

  float position4[3] = {7.2, 4.5, 2.5};
  float pointAt4[3] = {0, 0, 0};
  camera(position4, pointAt4, 6.5, true, true);
  visibilityGroup();


  view("Standard LEGO View", true);

  collection("Provenance/Event information",
             "Event_V1:time:run:event:ls:orbit:bx",
             0,
             0,
             make3DEvent,
             Qt::Checked);

  collection("Provenance/L1 Triggers",
             "L1GtTrigger_V1",
             0,
             0,
             make3DL1Trigger,
             Qt::Checked);

  collection("Provenance/HLT Trigger Paths",
             "TriggerPaths_V1",
             0,
             0,
             0,
             Qt::Unchecked);

  collection("Provenance/Trigger Objects",
             "TriggerObjects_V1:VID:pt:eta:phi",
             0,
             0,
             make3DTriggerObject,
             Qt::Unchecked);

  collection("Provenance/Data Products found", 
             "Products_V1", 
             0, 
             0,
             0,
             Qt::Checked);

  collection("Provenance/Data Products not found", 
             "Errors_V1", 
             0, 
             0, 
             0,
             Qt::Checked);

// -------------------------------------------------------------------------------------

  collection("Detector/Eta-Phi Grid",
             "CaloLego_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeLegoGrid,
             Qt::Checked);

// -------------------------------------------------------------------------------------

/*
  TM: Doesn't the had energy need to be dran as well?

  collection("Physics Objects/Calorimeter Energy Towers",
             "CaloTowers_V1:emEnergy:hadEnergy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
             0,
             0,
             makeLegoCaloTowers,
             Qt::Checked);
*/
  collection("Physics Objects/Jets",
             "Jets_V1:et:eta:phi",
             0,
             0,
             makeLegoJets,
             Qt::Checked);

  collection("ECAL/ECAL Rec. Hits",     // pre-Aug 2009 ig files only
             "EcalRecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoEcalRecHits,
             Qt::Checked);

  collection("HCAL/Barrel Rec. Hits",
             "HBRecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoHcalRecHits,
             Qt::Checked);

  collection("ECAL/Barrel Rec. Hits",
             "EBRecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoEcalRecHits,
             Qt::Checked);

  collection("ECAL/Endcap Rec. Hits",
             "EERecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoEcalRecHits,
             Qt::Checked);

  collection("HCAL/Endcap Rec. Hits",
             "HERecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoHcalRecHits,
             Qt::Checked);

  collection("HCAL/Forward Rec. Hits",
             "HFRecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoHcalRecHits,
             Qt::Checked);

  collection("HCAL/Outer Rec. Hits",
             "HORecHits_V1:energy:eta:phi",
             0,
             0,
             makeLegoHcalRecHits,
             Qt::Checked);

// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
  


}






/** Destroy the application.  A no-op since everything is done on exit. */
ISpyApplication::~ISpyApplication(void)
{
}

/** Specify a new collection.  Call this during the application
    initialisation to register known collection handlers.

    When reading input data, the information will be used to look for
    collections matching the specifications(see updateCollections())
    and create display lists accordingly.  The drawing method used
    will be invoked if and only if a non-empty collection matching all
    the requirements is found.

    The order of calls to collection() is significant.  Only the
    first matching specification is used, so less specific matches
    should be added last.

    @a friendlyName

    If non-null, use this name instead of the collection name in
    the tree view.

    @a collectionSpec

    Collection to use.  Must be non-null and non-empty, either
    NAME[:PROPERTY[:...]] or :PROPERTY[:...].  The former looks for
    a named collection, and requires the colon-separated list of
    properties in it.  The latter looks for any collection with
    those properties.

    @a otherCollectionSpec

    Another collection that is required, usually linked by an
    association.  Can be either null pointer or a string in the
    format specified for @a collectionSpec.

    @a associationSpec

    Either null pointer or the name of an association required
    to display the collection.  This would normally be non-null
    if @a otherCollectionSpec is non-null and would name the
    association linking the two together.

    @a make3D

    Method to invoke to render a collection matching these
    requirements.  
 
    @the visibility
 
    Either Qt::Checked (i.e. collection visible) or Qt::Unchecked 
    (i.e. collection invisible). This option controls the 
    visibility of the collection at startup. It matters only in 
    the case @a make3D is provided. */
void
ISpyApplication::collection(const char *friendlyName,
                            const char *collectionSpec,
                            const char *otherCollectionSpec,
                            const char *associationSpec,
                            Make3D make3D,
                            Qt::CheckState visibility)
{
  ASSERT(collectionSpec);

  m_specs.resize(m_specs.size() + 1);
  CollectionSpec &spec = m_specs.back();
  StringList parts;

  if (friendlyName)
    spec.friendlyName = friendlyName;
  
  parts = StringOps::split(collectionSpec, ':');
  ASSERT(! parts.empty());
  spec.collection = parts[0];
    
  spec.requiredFields.insert(spec.requiredFields.end(),
                             parts.begin()+1, parts.end());

  if (otherCollectionSpec)
  {
    parts = StringOps::split(otherCollectionSpec, ':');
    ASSERT(! parts.empty());
    spec.otherCollection = parts[0];
    spec.otherRequiredFields.insert(spec.otherRequiredFields.end(),
                                    parts.begin()+1, parts.end());
  }

  if (associationSpec)
  {
    parts = StringOps::split(associationSpec, ':');
    ASSERT(parts.size() == 1);
    spec.otherAssociation = parts[0];
  }

  spec.make3D = make3D;
  spec.defaultVisibility = visibility;
  
  // Assign the visibility place-holder for this collection.
  // Within a same visibility group, CollectionSpec referring to the same
  // collection use the same visibility settings.
  VisibilityGroupMap::iterator i = m_visibilityGroupMap.find(spec.collection);
  if (i != m_visibilityGroupMap.end())
    spec.visibilityIndex = i->second;
  else
  {
    size_t size = m_visibility.size();
    m_visibilityGroupMap.insert(std::make_pair(spec.collection, size));
    spec.visibilityIndex = size;
    m_visibility.resize(size + 1);
    m_visibility.back() = visibility;
  }
  
  // Update the view to include one more CollectionSpec.
  ASSERT(!m_viewSpecs.empty());
  ViewSpec &view = m_viewSpecs.back();
  view.endCollIndex++;
}

/** Specify a new event filter.  Call this during the application
     initialisation to register filters for known collection handlers.
 */
void
ISpyApplication::filter(const char *friendlyName,
			const char *collectionSpec)
{
  ASSERT(collectionSpec);
  
  m_filterSpecs.resize(m_filterSpecs.size() + 1);
  FilterSpec &spec = m_filterSpecs.back();
  StringList parts;

  if (friendlyName)
    spec.friendlyName = friendlyName;
  
  parts = StringOps::split(collectionSpec, ':');
  
  ASSERT(! parts.empty());
  spec.collection = parts[0];
  spec.requiredFields.insert(spec.requiredFields.end(),
                             parts.begin()+1, parts.end());
}

/** Accumulate results from all registered filters
 */
bool
ISpyApplication::filter(void)
{
  bool flag = true;

  for(size_t it = 0; it < m_filters.size(); ++it)
  {
    Filter &f = m_filters[it];
    flag &= f.result;
  }

  return flag;
}

/** Do the actual event filterng.
    Assume that a filter is done on

    @a algoName

    which an is an algorithm name - std::string type in the collection for
    
    @a result
    
    which is an int type in the collection.
    The result should be == 1 for the filter to pass.
    The algo names are defined in a GUI as a list of strings.
    The filter returns true on the first match (e.g. OR).
 */
bool
ISpyApplication::doFilterCollection(const Collection &collection, const char* algoName, const char *result)
{
  QStringList strList = m_selector->filterText(QString::fromStdString(collection.spec->friendlyName)).split("|", QString::SkipEmptyParts);
  if(strList.isEmpty())
    return true;

  QStringList fullList;
  
  IgCollection *lc = collection.data[0];
  for (IgCollectionIterator ci = lc->begin(), ce = lc->end(); ci != ce; ++ci)
  {
    if(ci->get<int>(result) == 1)
      fullList << QString::fromStdString(ci->get<std::string>(algoName));
  }
  foreach (QString str, strList) {
    if(fullList.contains(str.trimmed()))
      return true;
  }
  
  return false;
}

/** Find the collection by friendly name and trigger an update
    of its filter list model.
  */
void
ISpyApplication::updateFilterListModel(const QString &title)
{
  CollectionSpecs::iterator it = find_if (m_specs.begin(), m_specs.end(),
					  MatchByFriendlyName(title.toStdString()));
  if(it != m_specs.end())
  {
    Collections::iterator cit = find_if (m_collections.begin(), m_collections.end(), MatchByName(it->collection));
    if(cit != m_collections.end())
    {
      doUpdateFilterListModel(*cit);
    }
  }
}

/** Actual update of the filter list model.
  */
void
ISpyApplication::doUpdateFilterListModel(const Collection &collection)
{
  if (! m_listModel)
  {
    m_listModel = new IgCollectionListModel(collection.data[0]);
    m_selector->setModel(m_listModel);
  }
  else
    m_listModel->setCollection(collection.data[0]);
}

/** Begins the definition of a new view. 
    A view is simply a set of CollectionSpecs, as defined by the 
    ISpyApplication::collection() method.

    @a prettyName
    
    the pretty name of the view.
    
    @specialized.
    
    if true the view will not show in "Other" the collections that do
    not match any CollectionSpec.
*/
void
ISpyApplication::view(const char *prettyName, bool specialized)
{
  m_viewSpecs.resize(m_viewSpecs.size() + 1);
  ViewSpec &view = m_viewSpecs.back();
  view.name = prettyName;
  ASSERT(!m_cameraSpecs.empty());
  view.specialized = specialized;
  view.cameraIndex = m_cameraSpecs.size() - 1;
  view.startCollIndex = view.endCollIndex = m_specs.size();
}

/** Defines a camera to be used by all the subsequent views, defined via the
    view method, until a new call to "camera" is done. The camera is shared 
    among the views.
    
    @the position
    
    the default (startup) position of the camera.
    
    @the pointAt
    
    the default point (startup) to which the camera points at. 
    Notice that the focalDistance will be calculated so that it also works 
    as point of rotation.
    
    @the scale
    
    the default (startup) zoom factor used by the camera. This means
    heightAngle in the case of a perspective camera, scaleHeight in the case
    of an orthographic one.
    
    @orthographic
    
    whether the camera is orthographic or not (i.e. perspective) at startup.
    
    @rotating
    
    whether or not the camera is allowed to rotate.
 */
void
ISpyApplication::camera(float *position,
                        float *pointAt,
                        float scale,
                        bool orthographic,
                        bool rotating)
{
  m_cameraSpecs.resize(m_cameraSpecs.size() + 1);
  CameraSpec &camera = m_cameraSpecs.back();
  for (size_t i = 0, e = 3; i < e; i++)
  {
    camera.position[i] = position[i];
    camera.pointAt[i] = pointAt[i];
  }  
  camera.scale = scale;
  camera.orthographic = orthographic;
  camera.rotating = rotating;
}

/** Defines a visibility group. A visibility group is a set of Collections 
    which share the same visibility state, if the associated IgCollection is
    the same. Every collection specification defined via the `collection()` 
    method belong to the visibility group defined by the previous call to 
    visibilityGroup().
    
    The main use for this is to separate to separare the visibility settings 
    of the same IgCollection in different views.
  */
void
ISpyApplication::visibilityGroup(void)
{
  m_visibilityGroupMap.clear();
}

/** Begin to exit application.  Clears all internal data structures
    and displays, saves settings, and exits. */
void
ISpyApplication::onExit(void)
{
  QObject::disconnect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(onExit()));

  if (m_consumer.isRunning ())
  {
    m_consumer.finalize();
    m_consumer.quit();
  }
  
  m_exiting = true;
  delete m_listModel;
  m_listModel = 0;
  delete m_tableModel;
  m_tableModel = 0;
  m_3DModel->sceneGraph()->removeAllChildren();
  m_events.clear();
  m_collections.clear();
  m_specs.clear();
  m_views.clear();
  m_cameraSpecs.clear();
  
  for (int i = 0; i < 2; ++i)
  {
    delete m_storages[i];
  }

  if (m_archives[0] != m_archives[1])
    delete m_archives[1];
  delete m_archives[0];

  m_mainWindow->saveSettings();
  exit();
}

/** Actually exit the application.  */
void
ISpyApplication::exit(void)
{
  qApp->closeAllWindows();
}

/** Return the number of command line arguments given to the
    application.  These have already been cleaned up from known
    options, e.g. those destined to Qt.  @sa argv */
int
ISpyApplication::argc(void) const
{ return m_argc; }


/** Return the command line arguments given to the application.  These
    have already been cleaned up from known options, e.g. those
    destined to Qt.  @sa argc */
char **
ISpyApplication::argv(void) const
{ return m_argv; }

/** Return a clean application name: argv[0] without directories. */
const char *
ISpyApplication::appname(void) const
{
  const char *name = m_appname;
  if (const char *ptr = strrchr(name, '/'))
    name = ptr + 1;
  return name;
}

/** Show instruction on how to invoke this application(unix). */
int
ISpyApplication::usage(void)
{
  const char *app = appname();
  std::cerr << "Usage: " << app << " [OPTION-OR-FILENAME]...\n"
            << "   or: " << app << " --help\n"
            << "   or: " << app << " --version\n"
	    << "   or: " << app << " --online HOST[:PORT]\n";

  return EXIT_FAILURE;
}

/** Show application version information. */
int
ISpyApplication::version(void)
{
  std::cerr << QCoreApplication::applicationName().toStdString() << " "
            << QCoreApplication::applicationVersion().toStdString() << "\n";

  return EXIT_SUCCESS;
}

/** Run the application with the given command line arguments.
    Returns the exit code, but note that the application may exit
    without returning in situations such as log-out. */
int
ISpyApplication::run(int argc, char *argv[])
{
  m_appname = argv [0];
  m_argc = argc;
  m_argv = argv;
  int i = 1;
  
  while (*++argv)
  {
    if (!strcmp(*argv, "--help"))
      return usage();
    else if (!strcmp(*argv, "--version"))
      return version();
    else if (!strcmp(*argv, "--online"))
    {      
#ifdef Q_WS_MAC
      std::cerr << "Option --online is not currently supported on MacOSX.\n" << std::endl;
      return EXIT_FAILURE;
#endif
      if (! *++argv)
      {
	std::cerr << "--online requires an argument\n";
	return usage();
      }
      else
      {
	m_online = true;
	m_autoEvents = true;
	onlineConfig(*argv);
	*++argv;
	// Remove the command line option so that
	// it would not be treated as a filename.
	m_argv[i][0] = '\0'; m_argv[i+1][0] = '\0';
      }
    }
    i++;
  }

  return doRun();
}

/** Default application settings will be set to
    default values if a user has not saved them
    yet.

    Users normally expect an application to remember
    its settings (window sizes and positions, options,
    etc.) across sessions. We (FIXME: should) give them a
    possibility to choose. */
void
ISpyApplication::defaultSettings(void)
{
  QSettings settings;

  if (settings.isWritable())
  {
  }
}

/** DOCUMENT ME */
void
ISpyApplication::restoreSettings(void)
{
  QSettings settings;
}

/** */
void
ISpyApplication::setupActions(void)
{
  QAction *actionZoomIn = new QAction(parent());
  actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
  QIcon icon3;
  icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/zoom_in.png")), QIcon::Normal, QIcon::Off);
  actionZoomIn->setIcon(icon3);
  actionZoomIn->setText(QApplication::translate("ISpy3DView", "Zoom In", 0, QApplication::UnicodeUTF8));
  actionZoomIn->setShortcut(QApplication::translate("ISpy3DView", "Ctrl++", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
  actionZoomIn->setToolTip(QApplication::translate("ISpy3DView", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  QAction *actionZoomOut = new QAction(parent());
  actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
  QIcon icon4;
  icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/zoom_out.png")), QIcon::Normal, QIcon::Off);
  actionZoomOut->setIcon(icon4);
  actionZoomOut->setText(QApplication::translate("ISpy3DView", "Zoom Out", 0, QApplication::UnicodeUTF8));
  actionZoomOut->setShortcut(QApplication::translate("ISpy3DView", "Ctrl+-", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
  actionZoomOut->setToolTip(QApplication::translate("ISpy3DView", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  QAction *actionHome = new QAction(parent());
  actionHome->setObjectName(QString::fromUtf8("actionHome"));
  QIcon icon2;
  icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/home.xpm")), QIcon::Normal, QIcon::Off);
  actionHome->setIcon(icon2);
  actionHome->setText(QApplication::translate("ISpy3DView", "Home", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
  actionHome->setToolTip(QApplication::translate("ISpy3DView", "Home", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  QAction *actionViewPlaneX = new QAction(parent());
  actionViewPlaneX->setObjectName(QString::fromUtf8("actionViewPlaneX"));
  QIcon icon5;
  icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/yz_small.xpm")), QIcon::Normal, QIcon::Off);
  actionViewPlaneX->setIcon(icon5);
  actionViewPlaneX->setText(QApplication::translate("ISpy3DView", "Plane X", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
  actionViewPlaneX->setToolTip(QApplication::translate("ISpy3DView", "Plane X", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  QAction *actionViewPlaneY = new QAction(parent());
  actionViewPlaneY->setObjectName(QString::fromUtf8("actionViewPlaneY"));
  QIcon icon6;
  icon6.addPixmap(QPixmap(QString::fromUtf8(":/images/xz_small.xpm")), QIcon::Normal, QIcon::Off);
  actionViewPlaneY->setIcon(icon6);
  actionViewPlaneY->setText(QApplication::translate("ISpy3DView", "Plane Y", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
  actionViewPlaneY->setToolTip(QApplication::translate("ISpy3DView", "Plane Y", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  QAction *actionViewPlaneZ = new QAction(parent());
  actionViewPlaneZ->setObjectName(QString::fromUtf8("actionViewPlaneZ"));
  QIcon icon7;
  icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/yx_small.xpm")), QIcon::Normal, QIcon::Off);
  actionViewPlaneZ->setIcon(icon7);
  actionViewPlaneZ->setText(QApplication::translate("ISpy3DView", "Plane Z", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
  actionViewPlaneZ->setToolTip(QApplication::translate("ISpy3DView", "Plane Z", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  m_viewPlaneGroup = new QActionGroup(parent());
  m_viewPlaneGroup->setExclusive(false);
  m_viewPlaneGroup->addAction(actionViewPlaneX);
  m_viewPlaneGroup->addAction(actionViewPlaneY);
  m_viewPlaneGroup->addAction(actionViewPlaneZ);

  m_viewModeGroup = new QActionGroup(parent());
  m_viewModeGroup->setExclusive(true);

  m_actionCameraPerspective = new QAction(parent());
  m_actionCameraPerspective->setObjectName(QString::fromUtf8("actionCameraToggle"));
  QIcon icon8;
  icon8.addPixmap(QPixmap(QString::fromUtf8(":/images/perspective.xpm")), QIcon::Normal, QIcon::Off);
  m_actionCameraPerspective->setIcon(icon8);
  m_actionCameraPerspective->setCheckable(true);
  m_actionCameraPerspective->setText(QApplication::translate("ISpy3DView", "Perspective View", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
  m_actionCameraPerspective->setToolTip(QApplication::translate("ISpy3DView", "Perspective View", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  m_actionCameraOrthographic = new QAction(parent());
  m_actionCameraOrthographic->setObjectName(QString::fromUtf8("actionCameraToggle"));
  QIcon icon9;
  icon9.addPixmap(QPixmap(QString::fromUtf8(":/images/ortho.xpm")), QIcon::Normal, QIcon::On);
  m_actionCameraOrthographic->setIcon(icon9);
  m_actionCameraOrthographic->setText(QApplication::translate("ISpy3DView", "Orthographic View", 0, QApplication::UnicodeUTF8));
  m_actionCameraOrthographic->setCheckable(true);
  m_actionCameraOrthographic->setChecked(true);
#ifndef QT_NO_TOOLTIP
  m_actionCameraOrthographic->setToolTip(QApplication::translate("ISpy3DView", "Orthographic View", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

  m_viewModeGroup->addAction(m_actionCameraPerspective);
  m_viewModeGroup->addAction(m_actionCameraOrthographic);

  QObject::connect(actionHome, SIGNAL(triggered()), 
                   this, SLOT(resetToHomePosition()));
  QObject::connect(actionZoomIn, SIGNAL(triggered()), 
                   m_viewer, SLOT(zoomIn()));
  QObject::connect(actionZoomOut, SIGNAL(triggered()), 
                   m_viewer, SLOT(zoomOut()));
  QObject::connect(actionViewPlaneX, SIGNAL(triggered()), 
                   m_viewer, SLOT(viewPlaneX()));
  QObject::connect(actionViewPlaneY, SIGNAL(triggered()), 
                   m_viewer, SLOT(viewPlaneY()));
  QObject::connect(actionViewPlaneZ, SIGNAL(triggered()), 
                   m_viewer, SLOT(viewPlaneZ()));
  QObject::connect(m_viewModeGroup, SIGNAL(selected(QAction*)), 
                   m_viewer, SLOT(setCameraType(QAction*)));

  m_3DToolBar = new QToolBar(m_mainWindow);
  m_3DToolBar->setObjectName(QString::fromUtf8("ISpy3DView::toolBar"));
  m_3DToolBar->addAction(actionHome);
  m_3DToolBar->addAction(actionZoomIn);
  m_3DToolBar->addAction(actionZoomOut);
  m_3DToolBar->addAction(actionViewPlaneZ);
  m_3DToolBar->addAction(actionViewPlaneY);
  m_3DToolBar->addAction(actionViewPlaneX);
  m_3DToolBar->addAction(m_actionCameraPerspective);
  m_3DToolBar->addAction(m_actionCameraOrthographic);

  m_3DToolBar->setWindowTitle(QApplication::translate("ISpy3DView", 
                              "toolBar", 0, QApplication::UnicodeUTF8));
}

/** Configure the online consumer. */
void
ISpyApplication::onlineConfig(const char* server) 
{
  QString str(server);
  QStringList opts = str.split(":");
  
  QSettings settings;
  settings.beginGroup("igsource");
  std::string host = settings.value("host", QString("localhost")).value<QString>().toStdString();
  int port = settings.value("port", 9000).value<int>();
  settings.endGroup();
  
  if (opts.size() >= 1)
    host = opts.at(0).toStdString();
  if (opts.size() == 2)
    port = opts.at(1).toInt ();
  
  m_consumer.listenTo(false, host, port);
  ASSERT (m_storages[0]);
  m_consumer.nextEvent(m_storages[0]);
}

/** Fills in the camera object with the specifications found in the spec.
    Useful to reset camera's to their default or to fill them in the first
    place. 
    
    The focal distance is always given by the distance between the camera 
    position pos and the pointAt point.
    We don't set the far and near planes and for the moment we rely on the
    auto adjustment to work correctly.
    
    Notice that we need to ref() the cameras otherwise they will get lost
    once removed from the scenegraph, on a view change.
    
    Notice that this method does not handle what to do with the old 
    SoCamera in Camera.node, if present. It is therefore responsibility of 
    the caller to make sure the object is not leaked.

    @the spec
    
    the specification with the default parameter for the camera.
    
    @the camera
    
    a reference to the "Camera" object to be updated.
  */
void
ISpyApplication::restoreCameraFromSpec(CameraSpec *spec, Camera &camera)
{
  camera.spec = spec;
  SbVec3f position(spec->position);
  SbVec3f pointAt(spec->pointAt);
  if (spec->orthographic)
  {
    SoOrthographicCamera *socamera = new SoOrthographicCamera;
    socamera->scaleHeight(spec->scale);
    camera.node = socamera;
  }
  else
  {
    SoPerspectiveCamera *socamera = new SoPerspectiveCamera;
    socamera->heightAngle = spec->scale;
    camera.node = socamera;
  }
  camera.node->position = position;
  camera.node->pointAt(pointAt);
  camera.node->focalDistance = (position-pointAt).length();
  camera.node->ref();
}

/** Set up the main window. */
void
ISpyApplication::setupMainWindow(void)
{
  m_mainWindow = new ISpyMainWindow;
  m_mainWindow->setUnifiedTitleAndToolBarOnMac(true);

  m_selector = new ISpyEventSelectorDialog (m_mainWindow);
  QObject::connect(m_selector, SIGNAL(pageChanged(QString)), this, SLOT(updateFilterListModel(QString)));

  QObject::connect(m_mainWindow, SIGNAL(open()),          this, SLOT(openFileDialog()));
  QObject::connect(m_mainWindow, SIGNAL(autoEvents()),    this, SLOT(autoEvents()));
  QObject::connect(m_mainWindow, SIGNAL(nextEvent()),     this, SLOT(nextEvent()));
  QObject::connect(m_mainWindow, SIGNAL(previousEvent()), this, SLOT(previousEvent()));
  QObject::connect(m_mainWindow, SIGNAL(rewind()),        this, SLOT(rewind()));
  QObject::connect(m_mainWindow, SIGNAL(print()),         this, SIGNAL(print()));
  QObject::connect(m_mainWindow, SIGNAL(save()),          this, SIGNAL(save()));
  QObject::connect(m_mainWindow, SIGNAL(showAbout()),     this, SLOT(showAbout()));
  QObject::connect(m_mainWindow->actionEvent_Filter, SIGNAL(triggered()), m_selector, SLOT(show()));
  QObject::connect(m_mainWindow->actionPicture_Publishing, SIGNAL(triggered()), this, SLOT(showPublish()));

  m_mainWindow->actionAuto->setChecked(false);
  m_mainWindow->actionAuto->setEnabled(false);
  m_mainWindow->actionNext->setEnabled(false);
  m_mainWindow->actionPrevious->setEnabled(false);

  QStringList headers;
  headers << "Collection" << "Items" << "Show";
  m_treeWidget = m_mainWindow->treeWidget;
#ifdef Q_WS_MAC
  m_treeWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
  m_treeWidget->setHeaderLabels(headers);
  m_treeWidget->setAlternatingRowColors(false);
  
  m_groupFont = new QFont("Helvetica Neue");
  m_groupFont->setPixelSize(11);
  m_groupFont->setWeight(QFont::Bold);
  m_groupFont->setCapitalization(QFont::AllUppercase);
  
  //m_groupFont->setStretch(QFont::SemiCondensed);
  m_itemFont = new QFont("Helvetica Neue");
  m_itemFont->setPixelSize(11);

  m_mainWindow->restoreSettings();
  m_pubDialog = new ISpyPicturePublishingDialog(m_mainWindow->centralWidget());
  m_printTimer->setInterval(m_pubDialog->delay());
  if(m_pubDialog->isAuto())
    m_printTimer->start();

  // Create the various cameras as defined by the various CameraSpecs.
  // We use an extra Camera struct to maintain this information, because
  // we want to be able to reset different cameras to their original values,
  // not to some generic default value.
  // The focal distance is always given by the distance between the camera 
  // position pos and the pointAt point.
  // We don't set the far and near planes and for the moment we rely on the
  // auto adjustment to work correctly.
  // Notice that we need to ref() the cameras otherwise they will get lost
  // once removed from the scenegraph, on a view change.
  for (size_t csi = 0, cse = m_cameraSpecs.size(); csi != cse; ++csi)
  {
    m_cameras.resize(m_cameras.size() + 1);
    restoreCameraFromSpec(&(m_cameraSpecs[csi]), m_cameras.back());
  }

  // Create the various views, associating each one of them with their camera.
  for (size_t vsi = 0, vse = m_viewSpecs.size(); vsi != vse; ++vsi)
  {
    ViewSpec *spec = &(m_viewSpecs[vsi]);
    m_views.resize(m_views.size() + 1);
    View &view = m_views.back();
    view.spec = spec;
    view.camera = &(m_cameras[spec->cameraIndex]);
  }

  // Create the filters
  for(size_t fsi = 0, fse = m_filterSpecs.size(); fsi != fse; ++fsi)
  {
    FilterSpec *spec = &(m_filterSpecs[fsi]);
    m_filters.resize(m_filters.size() + 1);
    Filter &filter = m_filters.back();
    filter.spec = spec;
  }

  // Setup the combo box with the possible views.
  m_mainWindow->viewSelector->clear();
  for (size_t vi = 0, ve = m_views.size(); vi != ve; ++vi)
    m_mainWindow->viewSelector->addItem(m_views[vi].spec->name.c_str());
  
  m_mainWindow->viewSelector->setCurrentIndex(m_currentViewIndex);
  m_mainWindow->viewSelector->setFocusPolicy(Qt::ClickFocus);


  QObject::connect(m_treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                   this, SLOT(setCurrentItem(QTreeWidgetItem *, QTreeWidgetItem *)));
  QObject::connect(m_treeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
                   this, SLOT(itemActivated(QTreeWidgetItem *, int)));
  QObject::connect(m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
                   this, SLOT(itemActivated(QTreeWidgetItem *, int)));
  QObject::connect(m_treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem *)),
                   this, SLOT(handleGroupsClicking(QTreeWidgetItem *)));
  QObject::connect(m_treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem *)),
                   this, SLOT(handleGroupsClicking(QTreeWidgetItem *)));
  QObject::connect(m_mainWindow->viewSelector, SIGNAL(currentIndexChanged(int)),
                   this, SLOT(switchView(int)));
}

void
ISpyApplication::showPublish(void)
{
  m_pubDialog->show();
  if(m_pubDialog->exec() == QDialog::Accepted)
  {
    if(m_printTimer->isActive())
      m_printTimer->stop();
    m_printTimer->setInterval(m_pubDialog->delay());
    
    if(m_pubDialog->isAuto())
      m_printTimer->start();
  }
}

void
ISpyApplication::showAbout(void)
{
  m_splash->showAbout();
}

/** Main application run loop.  Initialises the application, shows its
    windows, opens any files requested(on command line or from
    operating system open file events) and executes the run loop.
    Returns the application exit code.  Note that this function may
    never return in certain situations such as GUI log-out. */
int
ISpyApplication::doRun(void)
{
  ISpyEventFilter filter;
  QApplication app(m_argc, m_argv);
  SoQt::init(m_argc, m_argv, m_argv[0]);
  QEventLoop evloop;

  setupMainWindow();

  restoreSettings();
  initShapes();

  m_3DModel = new Ig3DBaseModel;
  m_viewer = new ISpy3DView(m_3DModel, m_mainWindow->workspace());
  m_viewer->setFeedbackVisibility(true);
  setupActions();
  m_mainWindow->addToolBar(Qt::TopToolBarArea, m_3DToolBar);

  QObject::connect(this, SIGNAL(save()), m_viewer, SLOT(save()));
  QObject::connect(this, SIGNAL(print()), m_viewer, SLOT(print()));
  QObject::connect(m_viewer, SIGNAL(cameraToggled()), 
                   this, SLOT(cameraToggled()));
  QObject::connect(m_printTimer, SIGNAL(timeout()), this, SLOT(autoPrint()));
  QObject::connect(m_mainWindow->actionQuit, SIGNAL(triggered()), this, SLOT(onExit()));
  QObject::connect(this, SIGNAL(showMessage(const QString &)), m_mainWindow->statusBar(), SLOT(showMessage(const QString &)));
  QObject::connect(&filter, SIGNAL(open(const QString &)),
                   this, SLOT(openWithFallbackGeometry(const QString &)));
  app.installEventFilter(&filter);

  // Make sure we clean up when QApplication
  // implicitly quits when this signal is emitted.
  QObject::connect(&app, SIGNAL(lastWindowClosed()), this, SLOT(onExit()));

  // Activate but do not show the main window yet. We want to show
  // it only once we know what to do with the splash screen.
  m_mainWindow->actionSave->setEnabled(true);
  m_mainWindow->actionPrint->setEnabled(true);

  // Process pending events now to allow file open events through
  //(mac), calling "open()" on the files. Exclude user input and
  // don't wait for events to occur.
  evloop.processEvents(QEventLoop::ExcludeUserInputEvents);

  // Open file names given on the command line(unix, windows).
  for (int i = 1; i < m_argc; ++i)
    simpleOpen(m_argv[i]);
  
  // If the geometry is not there after loading all the ig files
  // passed on command line, get it from the web.
  if (m_argc != 1 && !m_archives[1])
    downloadGeometry();

  // If we don't have any files open, show the splash screen as a file
  // opening wizard. The file open dialog will eventually show the main
  // window. If we opened files above just make the "About iSpy" splash
  // screen visible for a few seconds, show the main window and proceed
  // immediatly to the main app event loop.
  m_splash = new ISpySplashScreen;
  m_splash->initActions(this);
  QObject::connect(m_mainWindow->actionOpenWizard, SIGNAL(triggered()),
                   m_splash, SLOT(showWizard()));

  // Add a timer which indicates how long it has been since the last online event
  // Run in maximized mode for online
  // FIXME: put it in a more approprate place
  if (m_online)
  {
    m_mainWindow->actionAuto->setEnabled(true);
    m_mainWindow->actionNext->setEnabled(true);
    m_mainWindow->actionAuto->setChecked(true);
    m_mainWindow->showMaximized();
    autoEvents();

    ISpyDigitalClock *clock = new ISpyDigitalClock(m_mainWindow->toolBarEvent);
    m_mainWindow->toolBarEvent->addWidget (clock);
    QObject::connect(this, SIGNAL(resetCounter()), clock, SLOT(resetTime()));
    clock->show();
  }
  else if (! m_archives[0] && ! m_archives[1])
  {
    m_splash->showWizard();
  }
  else
  {
    m_mainWindow->show();
  }

  
  // Now run.
  SoQt::mainLoop();
  
  std::cout << "Thank you for using iSpy!" << std::endl;
  delete m_viewer;
  delete m_splash;
  return 0;
}

/** Request a View to print its scene in a png format.
    Assuming that the status bar hold event nd run information,
    it will be added as the image metadata.

    Print images in a directory specified by a publisher dialog.
 */
void
ISpyApplication::autoPrint(void)
{
  // Check if the view should be printed
  QStringList viewsToPublish = m_pubDialog->views();
  
  bool flag = false;
  foreach(QString viewStr, viewsToPublish)
  {
    if(m_metaData.contains(viewStr, Qt::CaseInsensitive))
    {
      flag = true;
    }
  }
  
  if(! flag) { return; }
  
  QDir curDir = QDir::current();
  QDir::setCurrent(m_pubDialog->dir());
  m_viewer->autoPrint(m_metaData);
  QDir::setCurrent(curDir.path());
}

/** Respond to selection changes in the tree view.  This may be due to
    a GUI action such as mouse clicking or using arrow keys, or may
    result indirectly from moving between events or opening a file.

    Forwards to itemActivated().

    @a current   The new "current" item in the tree view.  */
void
ISpyApplication::setCurrentItem(QTreeWidgetItem *current, QTreeWidgetItem *)
{
  itemActivated(current, 0);
}

/** Handles clicking on the group items. It simply records the state of the
    group item in a Group structure, so that we can reuse the information on
    "updateCollections" to repristinate the full state of the tree.
*/
void
ISpyApplication::handleGroupsClicking(QTreeWidgetItem *current)
{
  if (current == m_treeWidget->headerItem() || current->parent() )
    return;

  int index = m_treeWidget->indexOfTopLevelItem(current);
  if (index < 0)
    return;
  ASSERT(index < m_groupIndex.size());
  size_t groupIndex = m_groupIndex[index];
  ASSERT(groupIndex < m_groups.size());
  Group &group = m_groups[groupIndex];
  group.expanded = m_treeWidget->isItemExpanded(current);
}

/** Return the Collection index in m_collection associated to the
    QTreeWidgetItem @item. Returns -1 in case the item is not a collection.
 */
int
ISpyApplication::getCollectionIndex(QTreeWidgetItem *item)
{
  if (!item)
    return -1;
  QTreeWidgetItem *parent = item->parent();
  if (!parent)
    return -1;

  // Handle the case a collection was clicked.
  // * Retrieve the group to which the clicked item belongs to.
  // * Retrieve the index of the collection in the m_collections.
  // * Retrieve the collection itself.
  // FIXME: Do we need to do anything if current item was cleared?
  int parentIndex = m_treeWidget->indexOfTopLevelItem(parent);
  if (parentIndex < 0)
    return -1;
  ASSERT(parentIndex < m_groupIndex.size());
  size_t groupIndex = m_groupIndex[parentIndex];
  ASSERT(groupIndex < m_groups.size());
  Group &group = m_groups[groupIndex];
  int childIndex = parent->indexOfChild(item);
  ASSERT(childIndex >= 0);
  ASSERT(childIndex < group.children.size());
  size_t index = group.children[childIndex];
  Collection &c = m_collections[index];
  ASSERT(c.item == item);
  return index;
}

/** Respond to selection changes in the tree view.  This may be due to
    a GUI action such as mouse clicking or using arrow keys, or may
    result indirectly from moving between events or opening a file.

    This is the master controller for what content is shown, and
    adjusts the 3D and table views to match.

    @a current   The new "current" item in the tree view.  */
void
ISpyApplication::itemActivated(QTreeWidgetItem *current, int)
{
  int index = getCollectionIndex(current);
  if (index < 0)
    return;
  Collection &c = m_collections[index];
  ASSERT(c.item == current);
  
  // Record visibility state in the assigned position in the m_visibility
  // vector. 
  // Notice that in case there is no spec, there is no 3D 
  // representation as well, hence we do not update the state.
  if (c.spec)
    m_visibility[c.spec->visibilityIndex] = current->checkState(2);
  
  // Show the contents in 3D, as appropriate.
  displayCollection(c);
  
  // Show table collection.
  if (! m_tableModel)
  {
    m_tableModel = new IgCollectionTableModel(c.data[0]);
    m_tableProxyModel = new QSortFilterProxyModel(this);
    m_tableProxyModel->setSourceModel(m_tableModel);
    
    m_mainWindow->tableView->setModel(m_tableProxyModel);
    m_mainWindow->tableView->setSortingEnabled(true);
    m_mainWindow->tableView->sortByColumn(0, Qt::AscendingOrder);
  }
  else
    m_tableModel->setCollection(c.data[0]);
  
#if 0
  // Clear any past 3D model selection and highlight this collection
  //(FIXME: do we want this?).
  m_3DModel->selection()->removeAllChildren();
  if (c.visibility == Qt::Checked)
    m_3DModel->selection()->addChild(c->sep);
#endif
}

/** Control collection display in the 3D window(only).  Toggles the
    SoSwitch to allow the content to be seen, and if the content
    wasn't created yet, creates it first. */
void
ISpyApplication::displayCollection(Collection &c)
{
  ASSERT(c.data[0]);
  ASSERT(c.item);
  ASSERT(c.node);
  ASSERT(c.sep);

  // Show or hide 3D as appropriate.
  // If the spec is not there, we simply skip this bit.
  if (! c.spec)
    return;
  else if (m_visibility[c.spec->visibilityIndex] == Qt::Unchecked)
    c.node->whichChild = SO_SWITCH_NONE;
  else
  {
    if (! c.sep->getNumChildren()
        && c.data[0]->size() > 0
        && c.spec->make3D)
    {
      Style *style = &(m_styles[c.style]);
      ASSERT(style);
      c.sep->addChild(style->material);
      c.sep->addChild(style->drawStyle);
      c.sep->addChild(style->font);
      (*c.spec->make3D)(c.data, &c.assoc, c.sep, style);
    }
    c.node->whichChild = SO_SWITCH_ALL;
  }
}

/** Find the matching style, given a string.

    This looks up backward the list of all defined style, looking for a style 
    that matches @a pattern.
    
    
*/
size_t
ISpyApplication::findStyle(const char *pattern)
{
  size_t reverse = m_styleSpecs.size();

  for (size_t ssi = 0, sse = m_styleSpecs.size(); ssi != sse; ++ssi)
  {
    size_t i = reverse - ssi - 1;
    StyleSpec &spec = m_styleSpecs[i];
    
    // Ignore if the current spec does not match the collection name.
    // FIXME: match on the view name as well!!
    // FIXME: match on selection and other possible attributes as well!!
    
    if (spec.collectionName != "*" && spec.collectionName != pattern)
      continue;
    
    // If this StyleSpec was already used, we point to the Style which 
    // actually uses it.
    //
    // If it was not we: 
    // * create a new Style,
    // * use the StyleSpec to fill it and associate it to the StyleSpec,
    //   just in case we wanted to revert to defaults.
    StylesMap::iterator smi = m_stylesMap.find(i);
    if (smi != m_stylesMap.end())
      return smi->second;
      
    size_t styleIndex = m_styles.size();
    m_styles.resize(styleIndex + 1);
    Style &style = m_styles.back();
    m_stylesMap.insert(std::make_pair(i, styleIndex));
    style.spec = i;
    style.material = new SoMaterial;
    style.material->diffuseColor = spec.diffuseColor;
    style.material->transparency = spec.transparency;
    style.drawStyle = new SoDrawStyle;
    style.drawStyle->style = spec.drawStyle;
    style.drawStyle->lineWidth = spec.lineWidth;
    style.drawStyle->linePattern = spec.linePattern;
    style.font = new SoFont;
    style.font->size = spec.fontSize;
    style.font->name = spec.fontName.c_str();
    style.markerType = getMarkerType(spec.markerStyle, spec.markerSize, 
                                     spec.markerShape);
    // Style nodes do not get deleted  by dropping a given
    // collection representation.
    style.material->ref();
    style.drawStyle->ref();
    style.font->ref();

    // First matching style interrupts the traversal.
    return styleIndex;
  }
  qDebug() << "Could not find any matching style for " << pattern;
  ASSERT(false && "Could not find any matching style ");
  return 0;
}

/** Update the collection list in the tree view.

    This method should be invoked any time the list of available
    collections has changed(such as on file open), or the content of
    the collections has changed(such as on new event).

    Rebuilds the tree, table and 3D views from scratch to match the
    new contents, using collection specifications as content guide.
    The current item(selection) and visibility are restored across
    the rebuild.

    FIXME: Restore tree view, table view viewport settings? */
void
ISpyApplication::updateCollections(void)
{
  // Change the background to match the current style.
  // This will allow to have different backgrounds for different
  // events, if required.
  SbColor bgColor = m_styles[findStyle("Background")].material->diffuseColor[0];
  m_viewer->setBackgroundColor(bgColor);

  // Remember currently selected collection.
  QSettings settings;
  std::string selectedCollection;
  QTreeWidgetItem *current = m_treeWidget->currentItem();
  int currentIndex = getCollectionIndex(current);

  if (currentIndex != -1) 
    selectedCollection = m_collections[currentIndex].collectionName;
  
  // Clear tree and table views. We clear 3D view at the end.
  if (m_tableModel)
    m_tableModel->clear();
  m_treeWidget->clear();

  // Rebuild the collections. Loop over collections in the order they
  // appear in the input storages, event first, geometry after.
  // Record these in our collections list for later reference.
  Collections oldcollections(m_storages[0]->collectionNames().size() +
                             m_storages[1]->collectionNames().size());
  Groups oldgroups;

  m_groupIndex.clear();
  m_groups.swap(oldgroups);
  m_collections.swap(oldcollections);
  View &view = m_views[m_currentViewIndex];

  // Add a name of the view to the meta-data of
  // an automatically printed image
  m_metaData.clear();
  m_metaData.append("View: ").append(QString::fromStdString(view.spec->name));

  for (size_t sti = 0, ste = 2, i = 0; sti < ste; ++sti)
  {
    // Try to locate a matching collection spec.  Accept the spec only
    // if it qualifies all stated requirements.  If it matches, use
    // the "friendly" collection name from the spec and its drawing
    // utility.
    StringList &names = m_storages[sti]->collectionNames();
    for (size_t ci = 0, ce = names.size(); ci != ce; ++ci, ++i)
    {
      std::string       collectionName = names[ci];
      std::string       name = collectionName;
      IgCollection      *coll = m_storages[sti]->getCollectionPtr(ci);
      IgCollection      *other = 0;
      IgAssociationSet  *assoc = 0;
      CollectionSpec    *spec = 0;

      // Get the current view and iterate on all the specs found there.
      // FIXME: pick up something different from the first one.
      ASSERT(m_currentViewIndex < m_views.size());

      for (size_t spi = view.spec->startCollIndex, spe = view.spec->endCollIndex; 
           spi != spe && !spec;
           ++spi)
      {
        CollectionSpec &cand = m_specs[spi];
        if (cand.collection.empty() || cand.collection == name)
        {
          bool hasFields = true;
          bool hasOther = true;
          bool hasOtherFields = true;
          bool hasAssoc = true;

          for (size_t fi = 0, fe = cand.requiredFields.size(); fi != fe && hasFields; ++fi)
            hasFields = coll->hasProperty(cand.requiredFields[fi]);

          if (! cand.otherCollection.empty())
          {
            if (!(other = m_storages[sti]->getCollectionPtr(cand.otherCollection)))
              hasOther = hasOtherFields = false;

            for (size_t fi = 0, fe = cand.otherRequiredFields.size(); fi != fe && hasOtherFields; ++fi)
              hasOtherFields = other->hasProperty(cand.otherRequiredFields[fi]);
          }

          if (! cand.otherAssociation.empty())
          {
            if (!(assoc = m_storages[sti]->getAssociationSetPtr(cand.otherAssociation)))
              hasAssoc = false;
          }

          if (hasFields && hasOther && hasOtherFields && hasAssoc)
          {
            if (! cand.friendlyName.empty())
              name = cand.friendlyName;
            spec = &cand;
          }
        }
      }
      
      // Handles the collections groups:
      // * Determine the group name from the spec name.
      // * Lookup the groups directory by name to see if the group is already 
      //   there, create it otherwise.
      // * Save the group index to store it in the Collection.
      // Actual addition of the children to a group is done later on, so that
      // we can make sure that the group ordering is the same as the 
      // one in which the collections where declared.
      lat::StringList parts = lat::StringOps::split(name, '/');
      name = parts.back();
      std::string groupName;
      if (parts.size() == 2)
        groupName = parts.front();
      else
        groupName = "Other";
      
      int groupIdx = -1;
      for (size_t gi = 0, ge = m_groups.size(); gi != ge; gi++)
        if(m_groups[gi].name == groupName)
        {
          groupIdx = gi;
          break;
        }
        
      if (groupIdx < 0)
      {
        groupIdx = m_groups.size();
        m_groups.resize(m_groups.size() + 1);
        Group &group = m_groups.back();
        group.name = groupName;
        group.item = new QTreeWidgetItem;
        group.item->setFont(0, *m_groupFont);
        group.item->setText(0, groupName.c_str());
        group.item->setTextColor(0, QColor(93, 110, 129));
        group.item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        // Loop over the old groups and bring over the "expanded" property.
        bool expanded = true;
        for (size_t ogi = 0, oge = oldgroups.size(); ogi != oge; ogi++)
          if (oldgroups[ogi].name == groupName)
          {
            expanded = oldgroups[ogi].expanded;
            break;
          }
        group.expanded = expanded;
      }
      
      
      // Create new items in the tree view and a placeholder content
      // node in the 3D model. The latter will be filled in on first
      // display, and directly here if the visibility is on.
      // 
      // Notice taht tree items will be added to the widget only 
      // later on, once we have sorted the collections by the
      // associated specs.
      QTreeWidgetItem *item = new QTreeWidgetItem;
      item->setFont(0, *m_itemFont);
      item->setFont(1, *m_itemFont);
      item->setText(0, name.c_str());
      item->setTextAlignment(1, Qt::AlignRight|Qt::AlignVCenter);
      item->setTextAlignment(2, Qt::AlignCenter);
      item->setText(1, QString("%1").arg(coll->size()));
      
      SoSwitch *sw = new SoSwitch;
      SoSeparator *sep = new SoSeparator;
      sw->setName(SbName(collectionName.c_str()));
      sw->addChild(sep);
      
      // If this is geometry, and we had the same geometry, transfer
      // it over to the new collection to avoid recomputing it, but
      // just in the case in the new view the make3D method is the same.
      // Note that reading files in deliberately clears the .sep fields so
      // we won't be fooled here to think a new collection is the same
      // as the old one just because it has the same pointer.
      
      for (size_t oi = 0, oe = oldcollections.size(); oi != oe; ++oi)
      {
        if (sti == 1)
          if ((oldcollections[oi].data[0] == coll && oldcollections[oi].sep)
              && (spec 
                  && oldcollections[oi].spec 
                  && oldcollections[oi].spec->make3D == spec->make3D))
            for (int zi = 0, ze = oldcollections[oi].sep->getNumChildren(); zi != ze; ++zi)
              sep->addChild(oldcollections[oi].sep->getChild(zi));
      }
      
      // Fill in the collection item. This needs to be in place before
      // we modify the tree as our slots on tree notifications use it.
      // We also determine at this point which style needs to be used by 
      // to render the collection.
      m_collections[i].spec = spec;
      m_collections[i].data[0] = coll;
      m_collections[i].data[1] = other;
      m_collections[i].assoc = assoc;
      m_collections[i].item = item;
      m_collections[i].node = sw;
      m_collections[i].sep = sep;
      m_collections[i].collectionName = collectionName;
      m_collections[i].groupIndex = groupIdx;
      m_collections[i].style = findStyle(collectionName.c_str());
    }
  }  
  
  // Sort the collections by spec so that they are displayed in the 
  // tree according to the order of the various "collection" calls
  // rather than in the one they happen to be in the file.
  std::sort(m_collections.begin(), m_collections.end(), SortBySpecAndName());

  std::set<size_t> createdGroups;
  m_groupIndex.clear();
  for (size_t i = 0, e = m_collections.size(); i != e; ++i)
  {
    Collection &coll = m_collections[i];
    if (!coll.spec && view.spec->specialized)
      continue;
    // Get the associated group, check if it is already in the tree widget 
    // (create and add it eventually, with correct expand settings).
    // Add the children to the group.
    Group &group = m_groups[coll.groupIndex];
    
    if (createdGroups.find(coll.groupIndex) == createdGroups.end())
    {
      createdGroups.insert(coll.groupIndex);
      m_groupIndex.push_back(coll.groupIndex);
      m_treeWidget->addTopLevelItem(group.item);
      m_treeWidget->setItemExpanded(group.item, group.expanded);
    }
    
    group.children.push_back(i);
    group.item->addChild(coll.item);
    
    // Finish setting up the tree items and append them to the tree widget.
    if (coll.spec && coll.spec->make3D)
      coll.item->setCheckState(2, m_visibility[coll.spec->visibilityIndex]);
    else
      coll.item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    
    // Set current item. Updates table and 3D views too.
    // If this is not current and visible, show in 3D.
    if (! selectedCollection.empty() && selectedCollection == coll.collectionName)
    {
      // FIXME: this is required because otherwise setCurretItem() forces the
      //        group to be expanded on next event, even if it is not.
      //        I could not find any better solution.
      if (m_groups[coll.groupIndex].expanded)
        m_treeWidget->setCurrentItem(coll.item);
    }
    else
      displayCollection(coll);
  }

  // Looks like because we called "clear" we need to reset all the 
  // tree properties to have the QTreeWidget behave correctly.
  m_treeWidget->header()->setMinimumSectionSize(33);
  m_treeWidget->header()->setDefaultSectionSize(33);
  m_treeWidget->header()->setStretchLastSection(false);
  m_treeWidget->header()->setCascadingSectionResizes(true);
  m_treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
  m_treeWidget->header()->setResizeMode(1, QHeaderView::ResizeToContents);
  m_treeWidget->header()->setResizeMode(2, QHeaderView::Fixed);
  
  // Clear and re-fill the 3D now that we don't need old data.
  // Update the camera.
  m_3DModel->contents()->removeAllChildren();
  if (view.camera->node)
  {
    m_viewer->setCamera(view.camera->node);
    if (view.camera->spec && view.camera->spec->rotating)
    {
      m_viewer->setViewMode(ISpy3DView::ROTATION_MODE);
      m_viewPlaneGroup->setEnabled(true);
      m_viewModeGroup->setEnabled(true);
    }
    else
    {
      m_viewer->setViewMode(ISpy3DView::PAN_MODE);
      m_viewPlaneGroup->setEnabled(false);
      m_viewModeGroup->setEnabled(false);
    }
    
    if (view.camera->node->getTypeId() == SoPerspectiveCamera::getClassTypeId())
      m_actionCameraPerspective->setChecked(true);
    else
      m_actionCameraOrthographic->setChecked(true);
  }
  for (size_t i = 0, e = m_collections.size(); i != e; ++i)
    m_3DModel->contents()->addChild(m_collections[i].node);

  // For now keep all controls enabled for online
  if (m_online)
  {
    m_mainWindow->actionAuto->setEnabled(true);
    m_mainWindow->actionNext->setEnabled(true);
    m_mainWindow->actionPrevious->setEnabled(true);
  }
  else
  {   
    // Finally adjust buttons to what can be done here.
    m_mainWindow->actionAuto->setEnabled(! m_events.empty() && m_eventIndex < m_events.size()-1);
    m_mainWindow->actionNext->setEnabled(! m_events.empty() && m_eventIndex < m_events.size()-1);
    m_mainWindow->actionPrevious->setEnabled(m_eventIndex > 0);
  
    if (m_eventIndex >= (m_events.empty() ? 0 : m_events.size()-1))
    {
      m_mainWindow->showNormal();
      m_mainWindow->dockTreeWidget->setShown(true);
      m_mainWindow->dockTableWidget->setShown(true);
      m_mainWindow->menubar->show();
      m_3DToolBar->show();
      m_idleTimer->stop();
      m_idleTimer->disconnect();
      m_animationTimer->stop();
      m_animationTimer->disconnect();
      m_timer->stop();
      m_timer->disconnect();
      m_mainWindow->actionAuto->setChecked(false);
    }
  }

  // Update registered filters
  //
  for(size_t fi = 0; fi < m_filters.size(); ++fi)
  {
    Filter &f = m_filters[fi];
    Collections::iterator cit = find_if (m_collections.begin(), m_collections.end(), MatchByName(f.spec->collection));
    if(cit != m_collections.end())
    {
      f.data[0] = (*cit).data[0];
      f.collectionName = (*cit).collectionName;
      
      // There must be two fields
      ASSERT(f.spec->requiredFields.size() == 2);
      f.result = doFilterCollection(*cit, f.spec->requiredFields[0].c_str(), f.spec->requiredFields[1].c_str());
    }
  }
  
  m_selector->updateDialog();
}

/** Read in and display a new event. */
void
ISpyApplication::newEvent(void)
{
  delete m_storages[0];

  ASSERT(m_eventIndex < m_events.size());
  readData(m_storages[0] = new IgDataStorage,
	   m_events[m_eventIndex].archive,
	   m_events[m_eventIndex].contents);
  
  updateCollections();

  // Skip an event if it did not pass the filters
  if(!filter() && m_nextEvent)
    nextEvent();
}

/** Prompt for a new file to be openend. */
void
ISpyApplication::openFileDialog(void)
{
  QString fileName;
  QFileDialog f(m_mainWindow->centralwidget, tr("Open File"), ".",
                tr("Ig Files(*.ig *.iguana)"));
  f.setFileMode(QFileDialog::ExistingFiles);

  QList<QUrl> shortcuts = f.sidebarUrls();
  shortcuts.append(QUrl("file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/ispy-files"));
  f.setSidebarUrls(shortcuts);

  if (f.exec())
    openWithFallbackGeometry(f.selectedFiles().front());

  // If we didn't show the main window yet, show it now
  if (! m_mainWindow->isVisible())
    m_mainWindow->show();

}

/** SLOT to opens a new file. In addition it will try to get the default 
    geometry file from the web if no geometry is present in the actual file 
    being opened.
 */
void 
ISpyApplication::openWithFallbackGeometry(const QString &fileName)
{
  simpleOpen(fileName);
  // If the geometry was not there, initiate the process of getting it silently
  // from the web.
  // Notice that this will work only once. If any geometry is actually loaded,
  // nothing will happen.
  if (!m_archives[1])
    downloadGeometry();
}

/** Open a new file.  Auto-detects file contents to judge what to do
    with the file.  Supports opening a geometry-only file, an
    events-only file, or mixed files.  If the file contains geometry,
    shows it.  If the file contains events, shows the first event.
    Apart from those, only the zip index is read and is used to
    rebuild the in-memory event list.
*/
void
ISpyApplication::simpleOpen(const QString &fileName)
{
  if (fileName.isEmpty())
    return;

  qDebug() << "Try to open " << fileName;
  showMessage(QString("Opening ") + fileName + tr("..."));

  // Read the file in.
  ZipArchive *file = loadFile(fileName);
  if (! file)
    return;

  // See what the file contains.
  Events                        events;
  lat::ZipMember                *geometry = 0;
  lat::ZipArchive::Iterator     zi, ze;
  size_t                        index = 0;

  events.reserve(file->size());
  for (zi = file->begin(), ze = file->end(); zi != ze; ++zi, ++index)
    if ((*zi)->isDirectory() ||(*zi)->size(ZipMember::UNCOMPRESSED) == 0)
      continue;
    else if (! strncmp((*zi)->name(), "Geometry/", 9))
    {
      if (geometry)
      {
        qDebug() << "Oopsla, multiple geometries, keeping last one.";
      }
      geometry = *zi;
    }
    else if (! strncmp((*zi)->name(), "Events/", 7))
    {
      events.resize(events.size()+1);
      Event &e = events.back();
      e.index = index;
      e.archive = file;
      e.contents = *zi;
    }

  // If the file had only geometry, take it as the geometry file.  If
  // the file has events and geometry, take both from it.  If the file
  // has no geometry, take event list from it, whether it had any
  // events or not.
  ZipArchive *deleted = 0;
  bool update = false;
  if (geometry)
  {
    // Invalidate all collections so updateCollections() won't think
    // it has valid geometry if happen to get back identical pointers.
    for (size_t ci = 0, ce = m_collections.size(); ci != ce; ++ci)
      m_collections[ci].sep = 0;

    if (m_archives[1])
    {
      m_archives[1]->close();
      delete m_archives[1];
      deleted = m_archives[1];
    }
    delete m_storages[1];
    m_storages[1] = new IgDataStorage;
    m_archives[1] = file;
    readData(m_storages[1], file, geometry);
    update = true;
  }

  if (! events.empty() || ! geometry)
  {
    if (m_archives[0] && m_archives[0] != deleted)
    {
      m_archives[0]->close();
      delete m_archives[0];
    }
    delete m_storages[0];
    events.swap(m_events);
    m_storages[0] = new IgDataStorage;
    m_archives[0] = file;
    m_eventIndex = 0;
    if (m_events.empty())
      update = true;
    else
    {
      newEvent();
      update = false;
    }
  }

  if (update)
    updateCollections();

  if (m_splash && m_splash->isVisible())
  {
    m_mainWindow->show();
    m_splash->hide();
  }

}

/** Downloads geometry from a fixed web location if not 
    available in ig file.
*/
void
ISpyApplication::downloadGeometry(void)
{
  QNetworkReply *reply = getUrl(QUrl("http://iguana.web.cern.ch/iguana/ispy/igfiles/other/cms-geometry.ig"));
  QTemporaryFile *tmpFile = new QTemporaryFile();
  IgNetworkReplyHandler *handler = new IgNetworkReplyHandler(reply, tmpFile);
  QObject::connect(handler, SIGNAL(done(IgNetworkReplyHandler *)),
                   this, SLOT(backgroundFileDownloaded(IgNetworkReplyHandler *)));
}

/** Helper function to load zip archive index contents. */
ZipArchive *
ISpyApplication::loadFile(const QString &filename)
{
  ZipArchive *file = 0;
  try
  {
    file = new ZipArchive(Filename(filename.toStdString())
                          .substitute(ShellEnvironment()),
                          IOFlags::OpenRead);
  }
  catch(lat::Error &e)
  {
    std::ostringstream str;
    str << "Unable parse file: " << filename.toStdString() << ".\nMaybe it got truncated?";
    QMessageBox::critical(m_mainWindow, "Error while opening file.",
                          str.str().c_str());

    std::cerr << (const char *) filename.toAscii()
              << ": error: cannot read: "
              << e.explain() << std::endl;
  }
  return file;
}

/** Read an object out of a zip file: an individual event or geometry.

    @a to       Storage to fill in with the file contents.  This should
    be a new IgDataStorage created for this call.
    @a archive  Archive file containing @a source.
    @a source   The source data file to read in. */
void
ISpyApplication::readData(IgDataStorage *to, ZipArchive *archive, ZipMember *source)
{
  qDebug() << QString("Reading ") << source->name() << tr("...");
  showMessage(QString("Reading ") + source->name() + tr("..."));

  try
  {
    InputStream *is = archive->input(source);
    IOSize length = source->size(ZipMember::UNCOMPRESSED);
    std::vector<char> buffer(length+1, 0);
    is->xread(&buffer[0], length);
    is->close();
    delete is;

    IgParser parser(to);
    parser.parse(&buffer[0]);
  }
  catch(ParseError &e)
  {
    std::ostringstream str;
    str << "Unable parse file: " << source->name() << ". Maybe it got truncated?";
    QMessageBox::critical(m_mainWindow, "Error while opening file.",
                          str.str().c_str());
  }
  catch(lat::Error &e)
  {
    std::ostringstream str;
    str << "Unable parse file: " << source->name() 
        << ".\nMaybe it got truncated on because disk is full?"
        << "\nFull debug log:\n" << e.explain();
    QMessageBox::critical(m_mainWindow, "Error while opening file.",
                          str.str().c_str());
    std::cerr << source->name() << ": error: " << e.explain() << std::endl;
  }
  catch(std::exception &e)
  {
    std::ostringstream str;
    str << "Unable parse file: " << source->name()
        << ".\nFull debug log:\n" << e.what();
    QMessageBox::critical(m_mainWindow, "Error while opening file.",
                          str.str().c_str());
    std::cerr << source->name() << ": error: " << e.what() << std::endl;
  }

  showMessage("");
}

/** DOCUMENT ME */
void
ISpyApplication::connect(void)
{
  qDebug() << "Connect not implemented.";
}

void
ISpyApplication::handleWizardLinks(const QUrl &link)
{
  QString linkString = link.toString();

  if (linkString == "OpenUrl")
  {
    m_splash->hide();
    m_mainWindow->show();
    m_mainWindow->raise();
    openUrlDialog();
  }
  else if (linkString == "OpenWeb")
  {
    m_splash->setRightPage(QUrl("http://iguana.web.cern.ch/iguana/ispy/igfiles/"));
  }
  else if (linkString == "OpenFile")
  {
    m_splash->hide();
    m_mainWindow->show();
    this->openFileDialog();
  }
  else if (linkString.endsWith(".ig"))
  {
    m_splash->hide();
    m_mainWindow->show();
    this->downloadFile(m_splash->rightPage()
                       .toString()
                       .replace(QRegExp("/[?].*"), "/") + linkString);
  }
  else if (linkString.startsWith("mailto:"))
  {
    QDesktopServices::openUrl(link);
  }
  else if (linkString.startsWith("http://"))
  {
    QDesktopServices::openUrl(link);
  }
  else
  {
    QUrl baseUrl = m_splash->rightPage();
    if (linkString[0] != '?' && !linkString.endsWith("/"))
    {
      std::cerr << linkString.replace(QRegExp("/[?].*"), "/").toStdString()<< std::endl;
      QDesktopServices::openUrl(baseUrl
                                .toString()
                                .replace(QRegExp("/[?].*"), "/")
                                + linkString);
      return;
    }
    if (linkString[0] == '/')
    {
      baseUrl.setPath(linkString);
      linkString = baseUrl.toString().replace(QRegExp("/[?].*"), "/");
    }
    else
      linkString = baseUrl
                   .toString()
                   .replace(QRegExp("/[?].*"), "/") + linkString;
    m_splash->setRightPage(linkString);
  }
}

void
ISpyApplication::downloadFile(const QUrl &link)
{
  if (m_splash)
  {
    m_splash->hide();
  }

  QNetworkReply *reply = getUrl(link);

  // FIXME: pickup download location from QSettings, hardcode to Desktop for the time being.
  QString saveDir = QDir::homePath();
  if (QDir::home().exists("Desktop"))
  {
    saveDir = QDir::home().filePath("Desktop");
  }

  QString savedFileName = link.toString().replace(QRegExp(".*/"), saveDir + "/");
  IgNetworkReplyHandler *handler = new IgNetworkReplyHandler(reply,
                                                             new QFile(savedFileName));
  QObject::connect(handler, SIGNAL(done(IgNetworkReplyHandler *)),
                   this, SLOT(fileDownloaded(IgNetworkReplyHandler *)));

  if (!m_progressDialog)
    m_progressDialog = new QProgressDialog;
  m_progressDialog->setLabelText("Downloading " + savedFileName + "...");
  m_progressDialog->show();
  // FIXME: handle case for files larger than 2GB.
  QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                   this, SLOT(setProgress(qint64,qint64)));
  QObject::connect(m_progressDialog, SIGNAL(canceled()),
                   handler, SLOT(abort()));
  QObject::connect(handler, SIGNAL(downloadError(IgNetworkReplyHandler *)),
                   this, SLOT(handleDownloadError(IgNetworkReplyHandler *)));
}

void
ISpyApplication::setProgress(qint64 current, qint64 final)
{
  if (current == final)
  {
    m_progressDialog->reset();
    m_progressDialog->hide();
    return;
  }

  if (final > 0)
    m_progressDialog->setRange(0, final/1024);
  m_progressDialog->setValue(current/1024);
}

/** Helper functon to create QNetworkReplies which have iSpy user agent set*/
QNetworkReply *
ISpyApplication::getUrl(const QUrl &url)
{
  QNetworkRequest request;
  request.setUrl(url);
  request.setRawHeader("User-Agent", "iSpy 1.3.1");
  return m_networkManager->get(request);
}

/** Handles the case a file has been downloaded.*/
void
ISpyApplication::fileDownloaded(IgNetworkReplyHandler *handler)
{
  m_progressDialog->disconnect();
  bool success = backgroundFileDownloaded(handler);
  if (!success)
  {
    QFile *file = static_cast<QFile *>(handler->device());
    QMessageBox::critical(m_mainWindow, "Error while downloading file.",
                          "Unable to open downloaded file: " + file->fileName());
    delete file;
    //FIXME: handle various errors and in particula lat::ZError.
  }
}

/** Handles the case a file has been downloaded, without interrupting the user
    in case of failure.
*/
bool
ISpyApplication::backgroundFileDownloaded(IgNetworkReplyHandler *handler)
{
  QFile *file = static_cast<QFile *>(handler->device());
  try
  {
    openWithFallbackGeometry(file->fileName());
  }
  catch(...)
  {
    showMessage("");
    return false;
  }
  delete file;
  return true;
}

/** Handles various kinds of failures when downloading a file*/
void
ISpyApplication::handleDownloadError(IgNetworkReplyHandler *handler)
{
  m_progressDialog->disconnect();
  handler->reply()->disconnect();
  handler->disconnect();
  m_progressDialog->reset();
  m_progressDialog->hide();
  showMessage("");
  if (handler->aborted())
  {
    QFile *file = static_cast<QFile *>(handler->device());
    QMessageBox::critical(m_mainWindow->centralWidget(), "Download canceled.",
                          "Download cancelled for file " + file->fileName() + ".");    
  }
  else
    QMessageBox::critical(m_mainWindow->centralWidget(), "Error while downloading file.",
                          "Cannot download file from url: "
                          + handler->reply()->request().url().toString());
  // FIXME: if we actually delete it, it sometimes crashes. But this clearly 
  //        leaks...
  //delete handler;
}

/** Handles the open url action */
void
ISpyApplication::openUrlDialog(void)
{
  QInputDialog dialog;
  dialog.setLabelText("Specify an ig-file url:");
  dialog.setTextValue("http://iguana.web.cern.ch/iguana/ispy/igfiles/mc/electroweak/RelValZEE.ig");
  dialog.resize(430,72);
  // FIXME: use the latest file downloaded as default.
  if (!dialog.exec())
    return;

  this->downloadFile(dialog.textValue());
}

/** Toggle events auto-play. */
void
ISpyApplication::autoEvents(void)
{
  QSettings settings;
  settings.beginGroup("igevents");
  int animationTimeout = settings.value("animation", 20000).value<int>();
  int timeout = settings.value("timeout", 60000).value<int>();
  int idleTimeout = settings.value("idle", 15*60000).value<int>();
  settings.endGroup();
  m_autoEvents = m_mainWindow->actionAuto->isChecked();

  if (m_autoEvents)
  {
    m_mainWindow->showMaximized();
    m_mainWindow->dockTreeWidget->setShown(false);
    m_mainWindow->dockTableWidget->setShown(false);
    m_mainWindow->menubar->hide();
    m_3DToolBar->hide();
    // Stops animation and restores all the camera.
    if (m_viewer->isAnimating())
      m_viewer->stopAnimating();

    for(size_t i = 0, e = m_cameras.size(); i < e; ++i)
      restoreCameraFromSpec(m_cameras[i].spec, m_cameras[i]);    
    
    QObject::connect(m_animationTimer, SIGNAL(timeout()), SLOT(animateViews()));

    // Keep an eye on idle application
    // Auto-play should automatically restart if iSpy is idle for some time
    if(! m_idleTimer->isActive())
    {      
      m_idleTimer->stop();
      m_idleTimer->disconnect();
      QObject::connect(m_idleTimer, SIGNAL(timeout()), SLOT(restartPlay()));
      m_idleTimer->start(idleTimeout);
    }
    QObject::connect(m_timer, SIGNAL(timeout()), SLOT(nextEvent()));

    m_animationTimer->setInterval(animationTimeout);
    m_animationTimer->start();
    m_timer->setInterval(timeout);
    m_timer->start();
  }
  else
  {
    m_mainWindow->showNormal();
    m_mainWindow->dockTreeWidget->setShown(true);
    m_mainWindow->dockTableWidget->setShown(true);
    m_mainWindow->menubar->show();
    m_3DToolBar->show();
    m_animationTimer->stop();
    m_animationTimer->disconnect();
    m_timer->stop();
    m_timer->disconnect();
  }
}

/** Poor mans animation: auto-switching between the views */
void
ISpyApplication::animateViews(void) 
{
  int next = (m_currentViewIndex + 1) % m_mainWindow->viewSelector->count();
  m_mainWindow->viewSelector->setCurrentIndex (next);
  switchView(next);
}

/** Go to the next event. */
void
ISpyApplication::nextEvent(void)
{
  m_nextEvent = true;
  if (m_online)
  {
    delete m_storages[0];
    showMessage (QString::fromStdString(m_consumer.nextEvent(m_storages[0] = new IgDataStorage)));
    if (! m_storages[0]->empty())
      resetCounter();

    updateCollections();
  }
  if (++m_eventIndex < m_events.size())
  {
    showMessage(m_events[m_eventIndex].contents->name().name());
    newEvent();
  }
  else
    m_eventIndex = (m_events.empty() ? 0 : m_events.size()-1);
}

/** Go to the preveious event. */
void
ISpyApplication::previousEvent(void)
{
  m_nextEvent = false;
  if (m_online)
  {
    delete m_storages[0];

    showMessage (QString::fromStdString(m_consumer.previousEvent(m_storages[0] = new IgDataStorage)));
    if (! m_storages[0]->empty())
      resetCounter();

    updateCollections();
  }
  else if (m_eventIndex > 0 && --m_eventIndex < m_events.size())
  {
    showMessage(m_events[m_eventIndex].contents->name().name());
    newEvent();
  }
  else
    m_eventIndex = 0;
}

/** Go to the first event. */
void
ISpyApplication::rewind(void)
{
  m_eventIndex = 0;
  if (m_eventIndex < m_events.size())
  {
    showMessage(m_events[m_eventIndex].contents->name().name());
    newEvent();
  }
}

/** SLOT to make sure we update the camera when we toggle perspective / 
    orthographic mode. This is required since toggling actually replaces
    the camera with a new one.
  */
void
ISpyApplication::cameraToggled(void)
{
  SoCamera *socamera = m_viewer->getCamera();
  if (!socamera)
    return;
  Camera *camera = m_views[m_currentViewIndex].camera;
  SoCamera *oldcamera = camera->node;
  camera->node = socamera;
  camera->node->ref();
  oldcamera->unref();
}

/** SLOT to reset the camera of a current view to Home and update the view */
void
ISpyApplication::resetToHomePosition(void) 
{
  if (m_viewer->isAnimating())
    m_viewer->stopAnimating();

  View &view = m_views[m_currentViewIndex];
  SoCamera *oldcamera = view.camera->node;
  restoreCameraFromSpec(view.camera->spec, *(view.camera));
  if (oldcamera)
    oldcamera->unref();
  
  // FIXME: Need to find a faster way to update the view 
  switchView(m_currentViewIndex);
}

/** SLOT to restart play if the play has been paused for a long time */
void
ISpyApplication::restartPlay (void)
{
  if (! m_autoEvents)
  {
    ISpyRestartPlayDialog dialog(m_mainWindow->centralWidget());
    if (dialog.exec() == QDialog::Accepted)
    {
      m_mainWindow->actionAuto->setChecked(true);
      autoEvents();
    }
    else 
    {
      m_idleTimer->stop();
      m_idleTimer->disconnect();
    }
  }
}

/** Switch view. */
void
ISpyApplication::switchView(int viewIndex)
{
  if (m_viewer->isAnimating())
    m_viewer->stopAnimating();
  
  m_currentViewIndex = viewIndex;
  updateCollections();
  m_mainWindow->viewSelector->clearFocus();
  m_treeWidget->setFocus(Qt::MouseFocusReason);
  m_mainWindow->viewSelector->setFocusPolicy(Qt::ClickFocus);
}

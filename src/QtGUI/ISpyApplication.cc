#define QT_NO_EMIT
#include "QtGUI/ISpyApplication.h"
#include "QtGUI/ISpy3DView.h"
#include "QtGUI/ISpyDigitalClock.h"
#include "QtGUI/ISpyEventSelectorDialog.h"
#include "QtGUI/ISpyEventFilter.h"
#include "QtGUI/ISpyMainWindow.h"
#include "QtGUI/ISpyPicturePublishingDialog.h"
#include "QtGUI/ISpyRestartPlayDialog.h"
#include "QtGUI/ISpySplashScreen.h"
#include "QtGUI/Ig3DBaseModel.h"
#include "QtGUI/IgCollectionListModel.h"
#include "QtGUI/IgCollectionTableModel.h"
#include "QtGUI/IgNetworkReplyHandler.h"

#include "Framework/IgCollection.h"
#include "Framework/IgParser.h"

// FIXME : these should be migrated from shapes into draw functions


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
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>

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
#include <QFileSystemWatcher>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <cassert>

using namespace lat;
const static size_t ISPY_MAX_STYLES = (size_t) -1;



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
        throw CssParseError("Unable to parse color.", rgb);
      }
      if (color[i] < 0. || color[i] > 1.0)
      {
        defaultColor(color);
        throw CssParseError("Color not normalized.", rgb);
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
  spec.background = "";
  defaultColor(spec.diffuseColor);
  spec.transparency = 0.0;
  spec.lineWidth = 1.0;
  spec.linePattern = 0xffff;
  spec.fontSize = 12;
  spec.fontFamily = "Helvetica";
  spec.drawStyle = ISPY_SOLID_DRAW_STYLE;
  spec.markerShape = ISPY_SQUARE_MARKER_SHAPE;
  spec.markerSize = ISPY_NORMAL_MARKER_SIZE;
  spec.markerStyle = ISPY_FILLED_MARKER_STYLE;
  spec.textAlign = ISPY_TEXT_ALIGN_LEFT;
  spec.minEnergy = 0.2;   // Default value is 0.2 GeV
  spec.maxEnergy = 5.;    // Default value is 5.0 GeV
  spec.energyScale = 1.;  // Default value is 0.1 m/GeV
  spec.annotationLevel = ISPY_ANNOTATION_LEVEL_NORMAL;
  // Default position it top left corner. Coordinate system
  // is like the web one.
  spec.left = 0;
  spec.top = 0;
  
  // Parse the rule.
  StringList ruleParts = StringOps::split(rule, "::");
  if (ruleParts.size() == 1)
    spec.collectionName = ruleParts[0];
  else if (!ruleParts.size() || ruleParts.size() > 2)
    throw CssParseError("Wrong syntax for rule!", rule);
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
    spec.fontFamily = previous.fontFamily;
    spec.drawStyle = previous.drawStyle;
    spec.markerShape = previous.markerShape;
    spec.markerSize = previous.markerSize;
    spec.markerStyle = previous.markerStyle;
    spec.textAlign = previous.textAlign;
    spec.minEnergy = previous.minEnergy;
    spec.maxEnergy = previous.maxEnergy;
    spec.energyScale = previous.energyScale;
    spec.background = previous.background;
    spec.annotationLevel = previous.annotationLevel;
    spec.top = previous.top;
    spec.left = previous.left;
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
    // Split a string at the first ":" and use the first part as property key,
    // the second as value.
    std::string &property = properties[pi];
    size_t sep_pos = property.find(':');
    key.assign(property.c_str(), 0, sep_pos);
    value.assign(property.c_str(), sep_pos + 1, property.size());

    char *endptr;
    
    if (key == "diffuse-color")
      parseColor(value.c_str(), spec.diffuseColor);
    else if (key == "transparency")
    {
      spec.transparency = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing transparency value", value);
    }
    else if (key == "line-width")
    {
      spec.lineWidth = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing line-width value", value);
    }
    else if (key == "line-pattern")
    {
      spec.linePattern = strtol(value.c_str(), &endptr, 16);
      if (*endptr)
        throw CssParseError("Error while parsing line-pattern value", value);
    }
    else if (key == "font-size")
    {
      spec.fontSize = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing font-size value", value);
    }
    else if (key == "font-family")
      spec.fontFamily = value;
    else if (key == "draw-style" && value == "solid")
      spec.drawStyle = ISPY_SOLID_DRAW_STYLE;
    else if (key == "draw-style" && value == "lines")
      spec.drawStyle = ISPY_LINES_DRAW_STYLE;
    else if (key == "draw-style" && value == "points")
      spec.drawStyle = ISPY_POINTS_DRAW_STYLE;
    else if (key == "draw-style")
      throw CssParseError("Syntax error while defining draw-style", value);    
    else if (key == "marker-shape" && value == "square")
      spec.markerShape = ISPY_SQUARE_MARKER_SHAPE;
    else if (key == "marker-shape" && value == "cross")
      spec.markerShape = ISPY_CROSS_MARKER_SHAPE;
    else if (key == "marker-shape" && value == "plus")
      spec.markerShape = ISPY_PLUS_MARKER_SHAPE;
    else if (key == "marker-shape" && value == "circle")
      spec.markerShape = ISPY_CIRCLE_MARKER_SHAPE;
    else if (key == "marker-shape")
      throw CssParseError("Syntax error while defining marker-shape", value);
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
      throw CssParseError("Syntax error while defining marker-style", value);
    else if (key == "text-align" && value == "left")
      spec.textAlign = ISPY_TEXT_ALIGN_LEFT;
    else if (key == "text-align" && value == "right")
      spec.textAlign = ISPY_TEXT_ALIGN_RIGHT;
    else if (key == "text-align" && value == "center")
      spec.textAlign = ISPY_TEXT_ALIGN_CENTER;
    else if (key == "text-align")
      throw CssParseError("Syntax error while defining text-align", value);
    else if (key == "annotation-level" && value == "none")
      spec.annotationLevel = ISPY_ANNOTATION_LEVEL_NONE;
    else if (key == "annotation-level" && value == "press")
      spec.annotationLevel = ISPY_ANNOTATION_LEVEL_PRESS;
    else if (key == "annotation-level" && value == "normal")
      spec.annotationLevel = ISPY_ANNOTATION_LEVEL_NORMAL;
    else if (key == "annotation-level" && value == "full")
      spec.annotationLevel = ISPY_ANNOTATION_LEVEL_FULL;
    else if (key == "annotation-level")
      throw CssParseError("Syntax error while defining annotation-level", value);      
    else if (key == "min-energy")
    {
      spec.minEnergy = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing min-energy value", value);
    }
    else if (key == "energy-scale")
    {
      spec.energyScale = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing energy-scale value", value);
    }
    else if (key == "max-energy")
    {
      spec.maxEnergy = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing max-energy value", value);
    }
    else if (key == "background")
      spec.background = value;
    else if (key == "left")
    {
      spec.left = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing left value", value);
    }
    else if (key == "top")
    {
      spec.top = strtod(value.c_str(), &endptr);
      if (*endptr)
        throw CssParseError("Error while parsing top value", value);
    }
    else
    {
      throw CssParseError("Unknown property", key);
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
    assert(parts.size() == 2);
    style(parts[0].c_str(), parts[1].c_str());
  }
}


/** Helper method to parse attributes that contain a 3-vector where components
    are comma separated, e. g.: "1.0, 0.0, 2.0".
    
    @a value to set according to the attribute.
    
    @a attr containing all the attributes.

    @a name for the attribute containing the vector.
    
    @throw ViewSpecParseError in case of mistakes.
*/
void
parseVectorAttribute(float *v, QXmlStreamAttributes &attr, const char *name)
{
  QStringList sl = attr.value(name).toString().simplified().split(QRegExp("[ ]*,[ ]*"));
  if (sl.count() != 3)
    throw ViewSpecParseError();

  for (size_t i = 0; i != 3; ++i)
  {
    bool ok;
    v[i] = sl[i].toFloat(&ok);
    if (!ok)
      throw ViewSpecParseError();
  }
}

/** Helper method to parse attributes that can be either "true" or "false".
    
    @a value to set according to the attribute.
    
    @a attr containing all the attributes in the element.
    
    @a name containing the name of the attribute to be used.
    
    @a trueString containing the mnemonic for a true condition.
    
    @a falseString containing the mnemonic for a false condition.
    
    @throw ViewSpecParseError on parse errors.
*/
void
parseBooleanAttribute(bool &value, QXmlStreamAttributes &attr, const char *name, 
                      const char *trueString = "true", 
                      const char * falseString = "false")
{
  if (!attr.hasAttribute(name))
    return;
  
  QStringRef str = attr.value(name);
  
  if (str != trueString && str != falseString)
    throw ViewSpecParseError();

  value = (str == trueString);
}

/** Helper method to parse attributes float attributes.
    
    @a value to set according to the attribute.
    
    @a attr containing all attributes.
    
    @a name for the attribute.
    
    @throw ViewSpecParseError on parse errors.
*/
void
parseFloatAttribute(float &value, QXmlStreamAttributes &attr, const char *name)
{
  if (!attr.hasAttribute(name))
    return;
  
  QString str = attr.value(name).toString();

  bool ok;
  value = str.toFloat(&ok);
  
  if (!ok)
    throw ViewSpecParseError();
}

/** Helper method to parse string attributes.
    
    @a value to set according to the attribute.
    
    @a attr containing all attributes.
    
    @a name for the attribute.
    
    @throw ViewSpecParseError on parse errors.
*/
void
parseStringAttribute(QString &value, QXmlStreamAttributes &attr, const char *name)
{
  if (!attr.hasAttribute(name))
    return;
  
  value = attr.value(name).toString();
}

/** Read the view definition from @a device and create the various views.

    TODO: this is a first step in a direction where the views are actually not
          hardcoded in the application but could even be read from the ig file
          itself. For example an analiser could decide to selectively turn on
          and off displaying some collections depending on the trigger status.
*/
void
ISpyApplication::parseViewsDefinition(QByteArray &data)
{
  QXmlStreamReader xml;
  xml.addData(data);

  while (!xml.atEnd())
  {
    xml.readNext();
    if (xml.tokenType() == QXmlStreamReader::StartElement)
    {
      QXmlStreamAttributes attr = xml.attributes();
      if (xml.name() == "collection")
      {
        QString label, collectionSpec, otherCollectionSpec, 
                associationSpec, make3DName;
        bool visibility = true;
        
        parseStringAttribute(label, attr, "label");
        parseStringAttribute(collectionSpec, attr, "spec");
        parseStringAttribute(otherCollectionSpec, attr, "other");
        parseStringAttribute(associationSpec, attr, "associations");
        parseStringAttribute(make3DName, attr, "draw");
        parseBooleanAttribute(visibility, attr, "visibility", "block", "none");

        DrawFunctionsRegistry::iterator i = m_drawFunctions.find(make3DName.toStdString().c_str());
        Make3D df = 0;

        if (i != m_drawFunctions.end())
          df = i->second;

        collection(label.toStdString().c_str(),
                   collectionSpec.toStdString().c_str(),
                   otherCollectionSpec.toStdString().c_str(),
                   associationSpec.toStdString().c_str(),
                   df,
                   visibility);
      }
      else if (xml.name() == "view")
      {
        QString label = "Unnamed view";
        bool specialised = true;
        bool autoplay = true;
        
        if (attr.hasAttribute("label"))
          label = attr.value("label").toString();
        parseBooleanAttribute(specialised, attr, "specialised");
        parseBooleanAttribute(autoplay, attr, "autoplay");
        
        view(label.toAscii(), specialised, autoplay);
      }
      else if (xml.name() == "camera")
      {
        float position[3] = {0., 0., 0.};
        float pointAt[3] = {1., 0., 0.};
        float scale = 1.0;
        bool ortho = false;
        bool rotating = true;
        QString projection = "identity";
        
        parseVectorAttribute(position, attr, "position");
        parseVectorAttribute(pointAt, attr, "pointAt");
        parseFloatAttribute(scale, attr, "scale");
        parseBooleanAttribute(ortho, attr, "orthographic");
        parseBooleanAttribute(rotating, attr, "rotating");
        parseStringAttribute(projection, attr, "projection");
        camera(position, pointAt, scale, ortho, rotating, projection.toStdString());
      }
      else if (xml.name() == "visibilityGroup")
      {  
        visibilityGroup(); 
      }
      else if (xml.name() == "layout")
      {
        continue;
      }
      else
      {  
        throw ViewSpecParseError(xml.lineNumber());
      }
    }
  }
  
  if (xml.hasError())
    throw ViewSpecParseError(xml.lineNumber());
}

/** Initialise but do not yet run the application object. 
  */
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
    m_host("localhost"),
    m_port(9000),
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
    m_fileWatcher(new QFileSystemWatcher),
    m_printTimer(new QTimer(this)),
    m_filterProgressDialog(0),
    m_counter(0),
    m_eventName("NOEVENT")
{
  m_archives[0] = m_archives[1] = 0;
  m_storages[0] = new IgDataStorage;
  m_storages[1] = new IgDataStorage;

#ifndef Q_WS_MAC
  QCoreApplication::setApplicationName("ispy");
#else
  QCoreApplication::setApplicationName("iSpy");
#endif
  QCoreApplication::setApplicationVersion("1.4.5");
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
  // Notice that we keep track of how big the vector containing the style
  // specifications is, so that we can revert back to default simply by 
  // resizing such a vector.
  style("*", "");
  style("Background","diffuse-color: rbg(1.,0,0);");
  bool ok = parseCssFile(":/css/default-style.iss");
  m_defaultStyleLevel = m_styleSpecs.size();
  assert(ok && "Default style not compiled as resource.");
  
  // Register draw functions which will be later used to draw various 
  // collections.
  registerDrawHelpers(m_drawFunctions);
  
  // Notice that the actual parsing of the configuration file is now deferred
  // until the ISpyApplication::doRun method is invoked, so that we can
  // use a view layout specified from the command line or even inside the ig
  // file itself.
}

/** Destroy the application.  A no-op since everything is done on exit. */
ISpyApplication::~ISpyApplication(void)
{
}

/** Helper function which parses a CSS file pointed by @a filename 

    @a filename the name of the css file.
    
    @return true if the file contains a valid css.
*/
bool
ISpyApplication::parseCssFile(const char *filename)
{
  QFile cssFile(filename);
  
  bool ok = cssFile.open(QIODevice::ReadOnly | QIODevice::Text);
  if (!ok)
    return false;
  
  QByteArray cssData = cssFile.readAll();
  assert(cssData.size());
  try 
  {
    parseCss(cssData.data());
  }
  catch (CssParseError &e)
  {
    qDebug() << e.why.c_str() << ": " << e.what.c_str();
    cssFile.close();
    return false;
  }

  cssFile.close();
  return true;
}

/** Helper function which parses a view layout file pointed by @a filename 

    @a filename the name of the css file.
    
    @return true if the file contains a valid layout.
*/
bool
ISpyApplication::parseViewsDefinitionFile(const char *filename)
{
  QFile viewsDefinitionFile(filename);
  bool ok = viewsDefinitionFile.open(QIODevice::ReadOnly | QIODevice::Text);

  if (!ok)
    return false;

  QByteArray viewData = viewsDefinitionFile.readAll();
  assert(viewData.size());
  try
  {
    parseViewsDefinition(viewData);
  }
  catch (ViewSpecParseError &e)
  {
    qDebug() << "Error while parsing file " << filename
             << " at line " << e.lineNumber;
    return false;
  }
  return true;
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
 
    Either true (i.e. collection visible) or false 
    (i.e. collection invisible). This option controls the 
    visibility of the collection at startup. It matters only in 
    the case @a make3D is provided. */
void
ISpyApplication::collection(const char *friendlyName,
                            const char *collectionSpec,
                            const char *otherCollectionSpec,
                            const char *associationSpec,
                            Make3D make3D,
                            bool visibility)
{
  assert(collectionSpec);

  m_specs.resize(m_specs.size() + 1);
  CollectionSpec &spec = m_specs.back();
  StringList parts;

  if (friendlyName)
    spec.friendlyName = friendlyName;
  
  parts = StringOps::split(collectionSpec, ':');
  assert(! parts.empty());
  spec.collection = parts[0];
    
  spec.requiredFields.insert(spec.requiredFields.end(),
                             parts.begin()+1, parts.end());

  if (otherCollectionSpec)
  {
    parts = StringOps::split(otherCollectionSpec, ':');
    assert(! parts.empty());
    spec.otherCollection = parts[0];
    spec.otherRequiredFields.insert(spec.otherRequiredFields.end(),
                                    parts.begin()+1, parts.end());
  }

  if (associationSpec)
  {
    parts = StringOps::split(associationSpec, ':');
    assert(parts.size() == 1);
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
  assert(!m_viewSpecs.empty());
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
  assert(collectionSpec);
  
  m_filterSpecs.resize(m_filterSpecs.size() + 1);
  FilterSpec &spec = m_filterSpecs.back();
  StringList parts;

  if (friendlyName)
    spec.friendlyName = friendlyName;
  
  parts = StringOps::split(collectionSpec, ':');
  
  assert(! parts.empty());
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
  for (IgCollection::iterator ci = lc->begin(), ce = lc->end(); ci != ce; ++ci)
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

    @autoplay

    true if the view has to be included among the ones that get shown in
    autoplay mode.
*/
void
ISpyApplication::view(const char *prettyName, bool specialized, bool autoplay)
{
  m_viewSpecs.resize(m_viewSpecs.size() + 1);
  ViewSpec &view = m_viewSpecs.back();
  view.name = prettyName;
  assert(!m_cameraSpecs.empty());
  view.specialized = specialized;
  view.cameraIndex = m_cameraSpecs.size() - 1;
  view.startCollIndex = view.endCollIndex = m_specs.size();
  view.autoplay = autoplay;
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
                        bool rotating,
                        const std::string &projection)
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

  // Select which projectors to add. Projectors themselves are defined in 
  // Projectors.cc
  getProjectors(projection, camera.projectors);
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
  m_exiting = true;
  stopFiltering();
  m_idleTimer->stop();
  m_idleTimer->disconnect();
  m_animationTimer->stop();
  m_animationTimer->disconnect();
  m_timer->stop();
  m_timer->disconnect();
  
  QObject::disconnect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(onExit()));
  
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
  std::cerr << "Usage: " << app << " [CSS-FILE] [LAYOUT-FILE] [IG-FILE]...\n"
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

/** Checks if @a filename ends with @a extension.
 */
bool
hasExtension(const std::string& filename, const std::string &extension)
{
  size_t pos = filename.rfind(extension);
  if (filename.size() == pos + extension.size())
    return true;
  return false;
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
    else if (hasExtension(*argv, ".iss"))
    {
      m_cssFilename = *argv;
      // Remove the command line option so that
      // it would not be treated as a filename.
      m_argv[i][0] = '\0';
    }
    else if (hasExtension(*argv, ".iml"))
    {
      m_viewsLayoutFilename = *argv;
      // Remove the command line option so that
      // it would not be treated as a filename.
      m_argv[i][0] = '\0';
    }
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
        i++;
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

  m_filterProgressDialog = new QProgressDialog(m_mainWindow);
  if(m_online)
  {    
    m_filterProgressDialog->setMaximum(0);
    m_mainWindow->setWindowTitle(QString("IGUANA iSpy - ONLINE %1:%2")
				 .arg(QString::fromStdString(m_host))
				 .arg(m_port));
  }
  
  m_filterProgressDialog->setModal(true);
  QObject::connect(m_filterProgressDialog, SIGNAL(canceled()), this, SLOT(stopFiltering()));

  QObject::connect(m_mainWindow, SIGNAL(open()),          this, SLOT(openFileDialog()));
  QObject::connect(m_mainWindow, SIGNAL(autoEvents()),    this, SLOT(autoEvents()));
  QObject::connect(m_mainWindow, SIGNAL(nextEvent()),     this, SLOT(nextEvent()));
  QObject::connect(this, SIGNAL(getNewEvent()), this, SLOT(nextEvent()));
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
ISpyApplication::updateEventMessage()
{
  QString eventMessage = m_eventName;
  eventMessage += QString::fromStdString("-" + m_views[m_currentViewIndex].spec->name);
  eventMessage.replace("/", "-");
  eventMessage.replace(" ", "_");
  m_viewer->setEventMessage(eventMessage);
}

void
ISpyApplication::stopFiltering(void)
{
  m_counter = 0;
  m_nextEvent = false;
}

void
ISpyApplication::showAbout(void)
{
  m_splash->showAbout();
}

/** Sometimes the file does not exists because what the editor does is to
    remove the old version and save a new one.
    In this case we watch for changes in the directory and read the file
    as soon as it reappears.  
*/
void
ISpyApplication::cssDirChanged(const QString &/*filename*/)
{
  QFileInfo info(m_cssFilename.c_str());
  if (!info.exists())
    QTimer::singleShot(1000, this, SLOT(checkCss()));
}

void
ISpyApplication::checkCss(void)
{
  QFileInfo info(m_cssFilename.c_str());
  if (info.exists())
  {
    openCss(m_cssFilename.c_str());
    m_fileWatcher->addPath(m_cssFilename.c_str());
  }
}

/** Resets the css stack to a given level. Notice that it correctly pops all 
    the unneded styles and unrefs all the associated SoNodes derived classes.
    
    FIXME: we should also remove unneeded elements in m_styles.
 */
void
ISpyApplication::resetStyleStack(size_t level)
{
  // We should never pop more levels than there are in the stack!
  // If this happens it means that the logic is broken.
  assert(level <= m_styleSpecs.size());

  // Process all the Styles to be popped and unref any SoNode that they 
  // define. This means that when all the scenegraphs referring to the style
  // will be dropped, the style will go as well (I hope).
  for (size_t i = level, e = m_styleSpecs.size(); i != e; ++i)
  {
    size_t styleIndex = m_stylesMap[i];
    if (styleIndex == ISPY_MAX_STYLES)
      continue;
    Style &style = m_styles[styleIndex];
    if (style.material)
      style.material->unref();
    if (style.drawStyle)
      style.drawStyle->unref();
    if (style.font)
      style.font->unref();
    if (style.background)
      style.background->unref();
  }

  m_styleSpecs.resize(level);
  m_stylesMap.resize(level, ISPY_MAX_STYLES);
}

/** Slot method which opens a new .iss file, parses it, and updates the 
    display. Notice that whenever we do this we assume that whatever file
    was read in is not valid anymore, and we therefore revert to the default 
    CSS.
    
    In case the .iss file is not valid, it pops up a dialog and reverts any 
    changes done.
    
    updateCollections is then called to update the display.
*/
void
ISpyApplication::openCss(const QString &filename)
{
  resetStyleStack(m_defaultStyleLevel);
  
  bool ok = parseCssFile(filename.toStdString().c_str());
  if (!ok)
  {
    QMessageBox::critical(m_mainWindow, "CSS parsing error ",
                          (filename + " is not a valid .iss file."
                           "Reverting to default."));
    resetStyleStack(m_defaultStyleLevel);
  }
  
  updateCollections();
}

/** Main application run loop. If specified, read and additional .iss file
    from the command line. If specified read a view layout file from the 
    command-line, if not read the default one.
    
    Notice that in case of an .iss file we track the changes for the file 
    in order to update the style on the fly.
    
    Initialises the application, shows its windows, opens any files requested
    (on command line or from operating system open file events) and executes 
    the run loop.
    Returns the application exit code.  Note that this function may
    never return in certain situations such as GUI log-out. */
int
ISpyApplication::doRun(void)
{
  if (!m_viewsLayoutFilename.empty())
  {
    qDebug() << "Reading " << m_viewsLayoutFilename.c_str();
    parseViewsDefinitionFile(m_viewsLayoutFilename.c_str());
  }
  else
  {
      bool ok = parseViewsDefinitionFile(":/views/default-views.iml");
      assert(ok && "Default views are broken!!!");
      qDebug() << "Reading default views.";
  }
  
  ISpyEventFilter filter;
  QApplication app(m_argc, m_argv);
  SoQt::init(m_argc, m_argv, m_argv[0]);
  QEventLoop evloop;

  setupMainWindow();

  restoreSettings();
  initHelpers();

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
  QObject::connect(this, SIGNAL(showMessage(const QString &)), 
                   m_mainWindow->statusBar(), 
                   SLOT(showMessage(const QString &)));
  
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
  // If the geometry is not there after loading all the ig files
  // passed on command line, get it from the web.
  bool didOpen = false;
  for (int i = 1; i < m_argc; ++i)
    didOpen |= simpleOpen(m_argv[i]);
  if (didOpen && !m_archives[1])
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
    nextEvent();
  }
  else if (! m_archives[0] && ! m_archives[1])
  {
    m_splash->showWizard();
  }
  else
  {
    m_mainWindow->show();
  }

  // We look up for changes in the CSS file only at the end, so that 
  // we are sure that there is actually something to update.
  // Notice that I need to convert the relative path to an absolute one,
  // otherwise the filewatcher gets confused.
  if (!m_cssFilename.empty())
  {
    QFileInfo cssFilename(m_cssFilename.c_str());
    m_cssFilename = cssFilename.absoluteFilePath().toStdString();
    openCss(m_cssFilename.c_str());
    QFileInfo info(m_cssFilename.c_str());
    m_fileWatcher->addPath(m_cssFilename.c_str());
    m_fileWatcher->addPath(info.dir().absolutePath());
    QObject::connect(m_fileWatcher, SIGNAL(fileChanged(const QString &)),
                     this, SLOT(openCss(const QString &)));
    QObject::connect(m_fileWatcher, SIGNAL(directoryChanged(const QString &)),
                     this, SLOT(cssDirChanged(const QString &)));
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
  assert((size_t) index < m_groupIndex.size());
  size_t groupIndex = m_groupIndex[index];
  assert(groupIndex < m_groups.size());
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
  assert((size_t)(parentIndex) < m_groupIndex.size());
  size_t groupIndex = m_groupIndex[parentIndex];
  assert((size_t)(groupIndex) < m_groups.size());
  Group &group = m_groups[groupIndex];
  int childIndex = parent->indexOfChild(item);
  assert(childIndex >= 0);
  assert((size_t)(childIndex) < group.children.size());
  size_t index = group.children[childIndex];
  Collection &c = m_collections[index];
  assert(c.item == item);
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
  assert(c.item == current);
  
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
  assert(c.data[0]);
  assert(c.item);
  assert(c.node);
  assert(c.sep);
  // Get the current view in order to know which projector to use.
  View &view = m_views[m_currentViewIndex];
  Projectors &projectors = view.camera->spec->projectors;
  
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
      assert(style);
      c.sep->enableNotify(FALSE);
      style->viewport = m_viewer->getViewportRegion();
      c.sep->addChild(style->material);
      c.sep->addChild(style->drawStyle);
      c.sep->addChild(style->font);
      try
      {
        (*c.spec->make3D)(c.data, &c.assoc, c.sep, style, projectors);
      }
      catch (IgSchemaError &e)
      {
        QMessageBox::critical(m_mainWindow, 
                              "Schema error",
                              ("The following collection" 
                              + c.spec->collection +
                              "does not match the requested schema.").c_str());
      }
      c.sep->enableNotify(TRUE);
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
  // Get the last element and resize the map between StyleSpecs to 
  // actual Style structs so that they have the same size.
  // Fill the map with ISPY_MAX_STYLES in case extra elements are
  // needed, because that means that those specs for sure have not been 
  // used yet.
  // FIXME: we should also iterate over m_stylesMap and remove any item
  //        in m_styles which does not appear in m_stylesMap
  //        otherwise we will leak styles every time we read a new .iss in.
  //        This is clearly minor, but worth doing at some point.
  size_t reverse = m_styleSpecs.size();
  m_stylesMap.resize(m_styleSpecs.size(), ISPY_MAX_STYLES);
  
  for (size_t ssi = 0, sse = m_styleSpecs.size(); ssi != sse; ++ssi)
  {
    size_t i = reverse - ssi - 1;
    StyleSpec &spec = m_styleSpecs[i];
    
    // Ignore if the current spec does not match the collection name.
    // FIXME: match on the view name as well!!
    // FIXME: match on selection and other possible attributes as well!!
    
    if (spec.collectionName != "*" && spec.collectionName != pattern)
      continue;
    
    // If this StyleSpec was already used (i.e. the stylesMap vector
    // returns a valid index in m_style), we simply return the index to 
    // such a Style.
    //
    // If this is the first time we actually find it:
    // 
    // * create a new Style,
    // * use the StyleSpec to fill it and associate it to the StyleSpec,
    //   just in case we wanted to revert to defaults.
    size_t styleIndex = m_stylesMap[i];
    if (styleIndex != ISPY_MAX_STYLES)
      return styleIndex;
    
    styleIndex = m_styles.size();
    m_styles.resize(styleIndex + 1);
    Style &style = m_styles.back();
    m_stylesMap[i] = styleIndex;
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
    style.font->name = spec.fontFamily.c_str();
    style.markerType = getMarkerType(spec.markerStyle, spec.markerSize, 
                                     spec.markerShape);
    // Determine the openinventor property from the ISPY style.
    switch(spec.textAlign)
    {
      case ISPY_TEXT_ALIGN_LEFT: style.textAlign = SoText2::LEFT; break;
      case ISPY_TEXT_ALIGN_RIGHT: style.textAlign = SoText2::RIGHT; break;
      case ISPY_TEXT_ALIGN_CENTER: style.textAlign = SoText2::CENTER; break;        
    }
    style.minEnergy = spec.minEnergy;
    style.maxEnergy = spec.maxEnergy;
    style.energyScale = spec.energyScale;
    style.annotationLevel = spec.annotationLevel;
    
    // Read the background file and put it in a SoImage so that it can be
    // used as required. In case no file are specified, the texture pointer 
    // will be zero.
    if (spec.background.empty())
      style.background = 0;
    else
    {
      QImage backgroundImage = QImage(spec.background.c_str()).mirrored(false, true).rgbSwapped();
      int bytesPerPixel = backgroundImage.depth() / 8;
      
      if (!backgroundImage.isNull() && bytesPerPixel)
      {
        style.background = new SoImage;
        style.background->image.setValue(SbVec2s(backgroundImage.width(), 
                                                 backgroundImage.height()), 
                                                 bytesPerPixel,
                                                 backgroundImage.bits());
        style.background->ref();
      }
      else
        style.background = 0;
    }
    
    // Positioning information we need to rescale them to [-1, 1] interval 
    // from [0, 1].
    style.left = (spec.left * 2) - 1;
    style.top = 1 - (spec.top * 2);

    // Style nodes do not get deleted  by dropping a given
    // collection representation.
    style.material->ref();
    style.drawStyle->ref();
    style.font->ref();

    // First matching style interrupts the traversal.
    return styleIndex;
  }
  qDebug() << "Could not find any matching style for " << pattern;
  assert(false && "Could not find any matching style ");
  return 0;
}

/** Create a few dynamic collections which contain stats about the rest of
    the IgDataStorage. This is useful to create summary tables etc.
    
    In particular this means that we can use it to populate a collection
    containing the limits that are currently used for various collections.
*/
void
ISpyApplication::createStats(void)
{
  IgDataStorage *es = m_storages[0];
  
  if (es->hasCollection("Internal_Limits_V1"))
    return;
  
  // Create a collection to hold all the requested limits.
  // To do so look fo
  IgCollection &limits = es->getCollection("Internal_Limits_V1");
  
  IgProperty NAME = limits.addProperty("name", std::string());
  IgProperty MINENERGY = limits.addProperty("minEnergy", 0.);
  IgProperty MAXENERGY = limits.addProperty("maxEnergy", 0.);
  IgProperty ENERGYSCALE = limits.addProperty("energyScale", 0.);
  
  const char *interestingLimits[15] = {"EcalRecHits_V1",
                                       "EBRecHits_V1",
                                       "EERecHits_V1",
                                       "ESRecHits_V1",
                                       "HBRecHits_V1",
                                       "HERecHits_V1",
                                       "HFRecHits_V1",
                                       "HORecHits_V1",
                                       "EBRecHits_V2",
                                       "EERecHits_V2",
                                       "ESRecHits_V2",
                                       "HBRecHits_V2",
                                       "HERecHits_V2",
                                       "HFRecHits_V2",
                                       "HORecHits_V2"};

  double energyScales[15] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  double lengthScales[15] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  for (size_t i = 0, e = sizeof(interestingLimits) / sizeof(char *); i != e; ++i)
  {
    const char *collectionName = interestingLimits[i];

    if ( es->hasCollection(collectionName) )
    {
      IgCollectionItem item = limits.create();
      item[NAME] = std::string(collectionName);
      Style &style = m_styles[findStyle(collectionName)];
      item[MINENERGY] = (double) style.minEnergy * energyScales[i];
      item[MAXENERGY] = (double) style.maxEnergy * energyScales[i];
      item[ENERGYSCALE] = (double) style.energyScale * lengthScales[i];
    }
  }
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
  // Create a few dynamic collections which contain stats about the rest of
  // the IgDataStorage. This is useful to create summary tables etc.
  createStats();
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
      IgAssociations  *assoc = 0;
      CollectionSpec    *spec = 0;

      // Get the current view and iterate on all the specs found there.
      // FIXME: pick up something different from the first one.
      assert(m_currentViewIndex < m_views.size());

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
            if (!(assoc = m_storages[sti]->getAssociationsPtr(cand.otherAssociation)))
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
    bool visibility = m_visibility[coll.spec->visibilityIndex];
    
    if (coll.spec && coll.spec->make3D)
      coll.item->setCheckState(2, visibility ? Qt::Checked : Qt::Unchecked);
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
      assert(f.spec->requiredFields.size() == 2);
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

  assert(m_eventIndex < m_events.size());
  readData(m_storages[0] = new IgDataStorage,
	   m_events[m_eventIndex].archive,
	   m_events[m_eventIndex].contents);
  
  updateCollections();

  m_eventName = m_events[m_eventIndex].contents->name().name();
  showMessage(m_eventName);
  updateEventMessage();
}

/* Run current event through the filters */
void
ISpyApplication::filterEvent(void)
{
  bool restorePlay = false;
  bool restoreAnim = false;
  bool restoreIdle = false;
  if(m_animationTimer->isActive())
  {
    restorePlay = true;
    m_animationTimer->stop();
  }
  if(m_idleTimer->isActive())
  {
    restoreIdle = true;
    m_idleTimer->stop();
  }
  
  if(m_timer->isActive())
  {
    restoreAnim = true;
    m_timer->stop();
  }
  
  // Skip an event if it did not pass the filters.
  // Filter only forward.
  if(!filter() && m_nextEvent)
  {
    QCoreApplication::processEvents();
    m_filterProgressDialog->show();

    // When the filtering is canceled from the dialog above,
    // the flag is set to false.
    if(m_nextEvent)
      getNewEvent();
  }  
  else
    m_counter = 0;
  
  m_filterProgressDialog->hide();
  if(restorePlay)
    m_animationTimer->start();
  if(restoreIdle)
    m_idleTimer->start();
  if(restoreAnim)
    m_timer->start();
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
bool
ISpyApplication::simpleOpen(const QString &fileName)
{
  if (fileName.isEmpty())
    return false;

  qDebug() << "Try to open " << fileName;
  showMessage(QString("Opening ") + fileName + tr("..."));

  // Read the file in.
  ZipArchive *file = loadFile(fileName);
  if (! file)
    return false;

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
  else
      m_mainWindow->setWindowTitle(QString("IGUANA iSpy - %1").arg(fileName));
  
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
      m_filterProgressDialog->setMaximum(m_events.size());
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

  return true;
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
  request.setRawHeader("User-Agent", "iSpy 1.4.5");
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

/** Poor mans animation: auto-switching between the views.

    Skip views that do not have "autoplay" flag set.

    In the case no views have the autoplay flag set, simply stick with the 
    current one.
*/
void
ISpyApplication::animateViews(void) 
{
  // Check if any view in autoplay mode. Simply exit in case none is found.
  bool anyOn = false;
  for (size_t i = 0, e = m_views.size(); i != e; ++i)
    anyOn |= m_views[i].spec->autoplay;
  if (!anyOn)
    return;

  // Rotate views that have the autoplay flag set.
  int next = (m_currentViewIndex + 1) % m_mainWindow->viewSelector->count();

  if (m_views[next].spec->autoplay)
  {
    m_mainWindow->viewSelector->setCurrentIndex (next);
    switchView(next);
  }
  else
  {
    m_currentViewIndex = next;
    animateViews();
  }
}

/** Go to the next event. */
void
ISpyApplication::nextEvent(void)
{
  m_nextEvent = true;

  if (++m_eventIndex < m_events.size())
  {
    m_filterProgressDialog->setMaximum(m_events.size() - m_eventIndex);
    m_filterProgressDialog->setValue(++m_counter);
    m_filterProgressDialog->setLabelText("Filtered out " + QString::number(m_counter) + " events.");
    newEvent();
    filterEvent();
  }
  else
    m_eventIndex = (m_events.empty() ? 0 : m_events.size()-1);
}

/** Go to the preveious event. */
void
ISpyApplication::previousEvent(void)
{
  m_nextEvent = false;
  if (m_eventIndex > 0 && --m_eventIndex < m_events.size())
    newEvent();
  else
    m_eventIndex = 0;
}

/** Go to the first event. */
void
ISpyApplication::rewind(void)
{
  m_eventIndex = 0;
  if (m_eventIndex < m_events.size())
    newEvent();
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
  updateEventMessage();
}

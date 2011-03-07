#ifndef IGUANA_STYLE_H
# define IGUANA_STYLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <Inventor/nodes/SoMarkerSet.h>
# include <Inventor/nodes/SoDrawStyle.h>
# include <Inventor/nodes/SoText2.h>
# include <Inventor/SbViewportRegion.h>

# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>

/** Define a few annotation levels that can be used to produce overlays for
    different occasions.
  */
enum ISPY_ANNOTATION_LEVEL {
  ISPY_ANNOTATION_LEVEL_NONE,
  ISPY_ANNOTATION_LEVEL_PRESS,
  ISPY_ANNOTATION_LEVEL_NORMAL,
  ISPY_ANNOTATION_LEVEL_FULL
};


enum ISPY_DRAW_STYLE {
  ISPY_SOLID_DRAW_STYLE = SoDrawStyle::FILLED,
  ISPY_LINES_DRAW_STYLE = SoDrawStyle::LINES,
  ISPY_POINTS_DRAW_STYLE = SoDrawStyle::POINTS
};

static const size_t ISPY_MARKER_STYLES = 2;

enum ISPY_MARKER_STYLE {
  ISPY_OUTLINE_MARKER_STYLE = 0,
  ISPY_FILLED_MARKER_STYLE  = 1
};

static const size_t ISPY_MARKER_SIZES  = 3;

enum ISPY_MARKER_SIZE {
  ISPY_NORMAL_MARKER_SIZE = 0,
  ISPY_BIG_MARKER_SIZE    = 1 * ISPY_MARKER_STYLES,
  ISPY_HUGE_MARKER_SIZE   = 2 * ISPY_MARKER_STYLES,
};

enum ISPY_MARKER_SHAPE {
  ISPY_SQUARE_MARKER_SHAPE = 0 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES),
  ISPY_CROSS_MARKER_SHAPE  = 1 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES),
  ISPY_PLUS_MARKER_SHAPE   = 2 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES),
  ISPY_CIRCLE_MARKER_SHAPE = 3 * (ISPY_MARKER_SIZES * ISPY_MARKER_STYLES)
};

enum ISPY_TEXT_ALIGN {
  ISPY_TEXT_ALIGN_LEFT,
  ISPY_TEXT_ALIGN_RIGHT,
  ISPY_TEXT_ALIGN_CENTER
};

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class SoMaterial;
class SoDrawStyle;
class SoFont;
class SoImage;

//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct StyleSpec
{
  std::string                 viewName;
  std::string                 collectionName;
  std::string                 background;
  float                       diffuseColor[3];
  float                       transparency;
  float                       lineWidth;
  unsigned int                linePattern;
  float                       fontSize;
  std::string                 fontFamily;
  ISPY_DRAW_STYLE             drawStyle;
  ISPY_MARKER_SHAPE           markerShape;
  ISPY_MARKER_SIZE            markerSize;
  ISPY_MARKER_STYLE           markerStyle;
  ISPY_TEXT_ALIGN             textAlign;
  ISPY_ANNOTATION_LEVEL       annotationLevel;
  double                      minPt;
  double                      minEnergy;
  double                      maxEnergy;
  double                      energyScale;
  double                      left;
  double                      top;
};

// The style structure is used to keep track of the context used to render
// a collection. It includes stuff that is graphics related (like the font
// and the material) and stuff which is physics related (like min-energy, 
// max-pt, etc).
struct Style
{
  size_t                      spec;
  SoMaterial                  *material;
  SoDrawStyle                 *drawStyle;
  SoFont                      *font;
  SoMarkerSet::MarkerType     markerType;
  SbViewportRegion            viewport;
  SoText2::Justification      textAlign;
  double                      minPt;
  double                      minEnergy;
  double                      maxEnergy;
  double                      energyScale;
  SoImage                     *background;
  ISPY_ANNOTATION_LEVEL       annotationLevel;
  double                      left;
  double                      top;
};

#endif // IGUANA_STYLE

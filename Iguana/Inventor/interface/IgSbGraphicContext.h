#ifndef IG_OPEN_INVENTOR_IG_SB_GRAPHIC_CONTEXT_H
# define IG_OPEN_INVENTOR_IG_SB_GRAPHIC_CONTEXT_H

# ifndef IGUANA_OBSOLETE
#  error This code is probably broken.  If you use it, please let us know.
#  error If it is needed, it should be cleaned up and use IgSoShapeKit.
#  error Do not release it in CMS IGUANA before it is rewritten.
# endif

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/SbLinear.h>
# include <Inventor/SbString.h>
 
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

typedef enum
{
    SbPrimitivePoints,
    SbPrimitiveLineStrip,
    SbPrimitiveLineLoop,
    SbPrimitiveLines,
    SbPrimitivePolygon
} SbPrimitiveType;

typedef enum
{
    SbLineSolid = 0,
    SbLineDashed,
    SbLineDotted,
    SbLineDashDotted
} SbLineStyle; 

typedef enum
{
    SbMarkerDot = 0,
    SbMarkerPlus,
    SbMarkerAsterisk,
    SbMarkerCircle,
    SbMarkerCross,
    SbMarkerStar,
    SbMarkerTriangle
} SbMarkerStyle;

typedef enum
{
    SbAreaSolid = 0,
    SbAreaHatched,
    SbAreaChecker
} SbAreaStyle;

typedef enum
{
    SbModelingWireFrame,
    SbModelingSolid,
    SbModelingCloud,
    SbModelingSegments,
    SbModelingPoints
} SbModelingPolicy;

typedef enum
{
    SbPaintingColor,
    SbPaintingShadow,
    SbPaintingLUT,
    SbPaintingGradient
} SbPaintingPolicy;

typedef enum
{
    SbColorAquamarine = 0,
    SbColorMediumaquamarine,
    SbColorBlack,    /*2*/
    SbColorBlue,     /*3*/
    SbColorCadetblue,
    SbColorCornflowerblue,
    SbColorDarkslateblue,
    SbColorLightblue,
    SbColorLightsteelblue,
    SbColorMediumblue,
    SbColorMediumslateblue, /*10*/
    SbColorMidnightblue,
    SbColorNavyblue,
    SbColorNavy,
    SbColorSkyblue,
    SbColorSlateblue,
    SbColorSteelblue,
    SbColorCoral,
    SbColorCyan,      /*18*/
    SbColorFirebrick,
    SbColorBrown,     /*20*/
    SbColorGold,
    SbColorGoldenrod,
    SbColorGreen,     /*23*/
    SbColorDarkgreen,
    SbColorDarkolivegreen,
    SbColorForestgreen,
    SbColorLimegreen,
    SbColorMediumseagreen,
    SbColorMediumspringgreen,
    SbColorPalegreen, /*30*/
    SbColorSeagreen,
    SbColorSpringgreen,
    SbColorYellowgreen,
    SbColorDarkslategrey,
    SbColorDimgrey,
    SbColorLightgrey,
    SbColorGrey,
    SbColorKhaki,
    SbColorMagenta, /*39*/
    SbColorMaroon,  /*40*/
    SbColorOrange,
    SbColorOrchid,
    SbColorDarkorchid,
    SbColorMediumorchid,
    SbColorPink,
    SbColorPlum,
    SbColorRed,    /*47*/
    SbColorIndianred,
    SbColorMediumvioletred,
    SbColorOrangered, /*50*/
    SbColorVioletred,
    SbColorSalmon,
    SbColorSienna,
    SbColorTan,
    SbColorThistle,
    SbColorTurquoise,   
    SbColorDarkturquoise,
    SbColorMediumturquoise,
    SbColorViolet,
    SbColorBlueviolet,  /*60*/
    SbColorWheat,
    SbColorWhite,       /*62*/
    SbColorYellow,      /*63*/
    SbColorGreenyellow  /*64*/
} SbColorName; 

class IG_OPEN_INVENTOR_API SbGraphicContext
{
public:
    
    SbGraphicContext ();
    ~SbGraphicContext ();
    void reset ();
    void dump ();
    void setFromScript (const SbString&);
    const SbVec3f& getRGB () const;
    void setColorIndex (int);
    int getColorIndex () const;
    void setModelingPolicy (SbModelingPolicy);
    SbModelingPolicy getModelingPolicy () const;
    void setLineStyle (SbLineStyle);
    SbLineStyle getLineStyle () const;
    int getLineWidth () const;
    void setMarkerStyle (SbMarkerStyle);
    SbMarkerStyle getMarkerStyle () const;
    int getMarkerSize () const;
    int getFontSize () const;
    void setAreaStyle (SbAreaStyle);
    SbAreaStyle getAreaStyle () const;
    SbPaintingPolicy getPaintingPolicy () const;
    void setFontName (const SbName&);
    const SbName& getFontName () const;
    int getMultiNodeLimit () const;
    static int getValueColorMapping (const SbString&, float** ,int**);

private:
    
    int fColorIndex;
    int fLineWidth;
    int fMarkerSize;
    int fFontSize;
    SbLineStyle fLineStyle;
    SbMarkerStyle fMarkerStyle;
    SbAreaStyle fAreaStyle;
    SbModelingPolicy fModelingPolicy; 
    SbPaintingPolicy fPaintingPolicy;
    SbName fFontName;
    int fMultiNodeLimit;
};

SbString* SbStringCreateF (int, char*, ...);

class SbPainterContext
{
public:
    float fRed;
    float fGreen;
    float fBlue;
    int fLineWidth;
    SbLineStyle fLineStyle;
    int fMarkerSize;
    SbMarkerStyle fMarkerStyle;
    SbAreaStyle fAreaStyle;
    int fPolygonMode;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SB_GRAPHIC_CONTEXT_H

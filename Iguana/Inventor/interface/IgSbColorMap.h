#ifndef IG_OPEN_INVENTOR_IG_SB_COLOR_MAP_H
# define IG_OPEN_INVENTOR_IG_SB_COLOR_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** @class IgSbColorMap Iguana/Inventor/interface/IgSbColorMap.h

    @brief Colour map abstraction and some default colour maps from 
           matlab/octave.  */

class IG_OPEN_INVENTOR_API IgSbColorMap
{
public:
    enum ColorMap {
      Autumn,
      Bone,
      Cool,
      Copper,
      //Cube,
      Flag,
      Gray,
      Hot,
      HSV,
      Jet,
      Pink,
      Prism,
      Spring,
      Summer,
      VGA,
      Winter
    };

    static IgSbColorMap &autumn;
    static IgSbColorMap &bone;
    static IgSbColorMap &cool;
    static IgSbColorMap &copper;
    // static IgSbColorMap &cube;
    static IgSbColorMap &flag;
    static IgSbColorMap &gray;
    static IgSbColorMap &hot;
    static IgSbColorMap &hsv;
    static IgSbColorMap &jet;
    static IgSbColorMap &pink;
    static IgSbColorMap &prism;
    static IgSbColorMap &spring;
    static IgSbColorMap &summer;
    static IgSbColorMap &vga;
    static IgSbColorMap &winter;

    static const IgSbColorMap* getColorMap (ColorMap name);
    
    virtual ~IgSbColorMap (void);

    /** Convert @a value, a number between zero and one, inclusive, to
        @a rgb values also between zero and one, inclusive.  */
    virtual void	color (float value, float rgb [3]) const = 0;

    /** Helper utility to split 32-bit colours into floats. Note that
        alpha channel is also decomposed!  */
    static void		unpack (unsigned value, float components [4]);

    /** Helper utility to pack float components into a 32-bit colour.
        Note that the alpha channel is also expected!  */
    static unsigned	pack (const float components [4]);

    /** Helper utility to convert 32-bit colours using the float conversions. */
    static unsigned	translate (void (*func) (const float [3], float [3]), unsigned color);

    /** Helper utility to convert @a color from HSV to RGB. */
    static void		hsv2rgb (const float hsv [3], float rgb [3]);

    /** Helper utility to convert @a color from RGB to HSV. */
    static void		rgb2hsv (const float rgb [3], float hsv [3]);

    /** Helper utility to convert @a color from RGB to YIQ. */
    static void		rgb2yiq (const float rgb [3], float yiq [3]);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline void
IgSbColorMap::unpack (unsigned value, float components [4])
{
    components [0] = ((value >> 24) & 0xff) / 255.;
    components [1] = ((value >> 16) & 0xff) / 255.;
    components [2] = ((value >> 8)  & 0xff) / 255.;
    components [3] = ((value >> 0)  & 0xff) / 255.;
}

inline unsigned
IgSbColorMap::pack (const float components [4])
{
    return (unsigned (components [0] * 255 + .5) << 24
	    | unsigned (components [1] * 255 + .5) << 16
	    | unsigned (components [2] * 255 + .5) << 8
	    | unsigned (components [3] * 255 + .5));
}

inline unsigned
IgSbColorMap::translate (void (*func) (const float [], float []), unsigned value)
{
    float components [4];
    float ret [4];
    unpack (value, components);
    (*func) (components, ret);
    ret [3] = components [3];
    return pack (ret);
}

#endif // IG_OPEN_INVENTOR_IG_SB_COLOR_MAP_H

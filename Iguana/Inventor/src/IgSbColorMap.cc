//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <assert.h>
#include <cmath>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbTableColorMap : public IgSbColorMap { public:
    const float (*m_table) [3];
    int		m_colors;
    int		m_repeats;
    float	m_division;

    IgSbTableColorMap (const float table [][3], int colors, int repeats)
        : m_table (table),
          m_colors (colors),
	  m_repeats (repeats),
	  m_division (1.0f / (repeats * colors))
    {}

    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	int color = static_cast<int> (value / m_division) % m_colors;
	rgb [0] = m_table [color][0];
	rgb [1] = m_table [color][1];
	rgb [2] = m_table [color][2];
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbAutumnColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = 1; rgb [1] = value; rgb [2] = 0;
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbBoneColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	// (7 * gray + hot) / 8
	assert (value >= 0.f); assert (value <= 1.f);
	float rgbhot [3];  hot.color (value, rgbhot);
	rgb [0] = (7 * value + rgbhot [2]) / 8;
	rgb [1] = (7 * value + rgbhot [1]) / 8;
	rgb [2] = (7 * value + rgbhot [0]) / 8;
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbCoolColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = value; rgb [1] = 1 - value; rgb [2] = 1;
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbCopperColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	// min(1, gray * [1.2500 0.7812 0.4975])
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = MINIMUM (1, value * 1.2500);
	rgb [1] = MINIMUM (1, value * 0.7812);
	rgb [2] = MINIMUM (1, value * 0.4975);
    }
};

//////////////////////////////////////////////////////////////////////
#if 0
class IG_OPEN_INVENTOR_API IgSbColorCubeColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	// FIXME
    }
};
#endif

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbFlagColorMap : public IgSbTableColorMap { public:
    static const float s_table [4][3];
    IgSbFlagColorMap (int repeats = 8)
        : IgSbTableColorMap (s_table, 4, repeats)
    {}
};

const float IgSbFlagColorMap::s_table [4][3] = {
    { 1, 0, 0 }, // red
    { 1, 1, 1 }, // white
    { 0, 0, 1 }, // blue
    { 0, 0, 0 }  // black
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbGrayColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = rgb [1] = rgb [2] = value;
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbHotColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	static const float N = 0.375; // 3/8
	// Each of R, G and B ramp one at a time from zero to one in space of N.
	if (value <= N) {
	    rgb [0] = value / N;
	    rgb [1] = 0;
	    rgb [2] = 0;
	} else if (value <= 2 * N) {
	    rgb [0] = 1;
	    rgb [1] = (value - N) / N;
	    rgb [2] = 0;
	} else {
	    rgb [0] = 1;
	    rgb [1] = 1;
	    rgb [2] = (value - 2*N) / (1. - 2*N);
	}
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbHSVColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	float hsv [3] = { value, 1, 1 };
	hsv2rgb (hsv, rgb);
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbJetColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	// y is a ramp from .5 to 1 in space of 1/8
	// x is a ramp from 0 to 1 in space of 1/4
	if (value <= 1./8) {
	    rgb [0] = 0;
	    rgb [1] = 0;
	    rgb [2] = .5 + value / (1./8) / 2; // y (0 .. 1/8)
	} else if (value <= 3./8) {
	    rgb [0] = 0;
	    rgb [1] = (value - 1./8) / (1./4); // x (1/8 .. 3/8)
	    rgb [2] = 1;
	} else if (value <= 5./8) {
	    rgb [0] = (value - 3./8) / (1./4); // x (3/8 .. 5/8)
	    rgb [1] = 1;
	    rgb [2] = 1 - rgb [0]; // 1-x (3/8 .. 5/8)
	} else if (value <= 7./8) {
	    rgb [0] = 1;
	    rgb [1] = 1 - ((value - 5./8) / (1./4)); // 1-x (5/8 .. 7/8)
	    rgb [2] = 0;
	} else {
	    rgb [0] = 1 - (.5 + (value - 7./8) / (1./8) / 2); // 1-y (7/8 .. 1)
	    rgb [1] = 0;
	    rgb [2] = 0;
	}
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbPinkColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	float rgbgray [3] = { value, value, value };
	float rgbhot [3]; hot.color (value, rgbhot);
	for (int i = 0; i < 3; ++i)
	    rgb [i] = sqrt ((2 * rgbgray [i] + rgbhot [i]) / 3);
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbPrismColorMap : public IgSbTableColorMap { public:
    static const float s_table [6][3];
    IgSbPrismColorMap (int repeats = 8)
        : IgSbTableColorMap (s_table, 6, repeats)
    {}
};

const float IgSbPrismColorMap::s_table [6][3] = {
    { 1, 0, 0 },    // red
    { 1, .5, 0 },   // orange
    { 1, 1, 0 },    // yellow
    { 0, 1, 0 },    // green
    { 0, 0, 1 },    // blue
    { 2./3, 0, 1 }  // violet
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbSpringColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = 1; rgb [1] = value; rgb [2] = 1.-value;
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbSummerColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = value; rgb [1] = .5+value/2; rgb [2] = .4;
    }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbVGAColorMap : public IgSbTableColorMap { public:
    static const float s_table [16][3];
    IgSbVGAColorMap (void)
        : IgSbTableColorMap (s_table, 16, 1)
    {}
};

const float IgSbVGAColorMap::s_table [16][3] = {
    { 1,   1,   1 },
    { .75, .75, .75 },
    { 1,   0,   0   },
    { 1,   1,   0   },
    { 0,   1,   0   },
    { 0,   1,   1   },
    { 0,   0,   1   },
    { 1,   0,   1   },
    { 0,   0,   0   },
    { .5,  .5,  .5  },
    { .5,  0,   0   },
    { .5,  .5,  0   },
    { 0,   .5,  0   },
    { 0,   .5,  .5  },
    { 0,   0,   .5  },
    { .5,  0,   .5  }
};

//////////////////////////////////////////////////////////////////////
class IG_OPEN_INVENTOR_API IgSbWinterColorMap : public IgSbColorMap { public:
    virtual void color (float value, float rgb [3]) const {
	assert (value >= 0.f); assert (value <= 1.f);
	rgb [0] = 0; rgb [1] = value; rgb [2] = .5+(1.-value)/2;
    }
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgSbColorMap &IgSbColorMap::autumn	= *(new IgSbAutumnColorMap);
IgSbColorMap &IgSbColorMap::bone	= *(new IgSbBoneColorMap);
IgSbColorMap &IgSbColorMap::cool	= *(new IgSbCoolColorMap);
IgSbColorMap &IgSbColorMap::copper	= *(new IgSbCopperColorMap);
//IgSbColorMap &IgSbColorMap::cube	= *(new IgSbColorCubeColorMap);
IgSbColorMap &IgSbColorMap::flag	= *(new IgSbFlagColorMap);
IgSbColorMap &IgSbColorMap::gray	= *(new IgSbGrayColorMap);
IgSbColorMap &IgSbColorMap::hot		= *(new IgSbHotColorMap);
IgSbColorMap &IgSbColorMap::hsv		= *(new IgSbHSVColorMap);
IgSbColorMap &IgSbColorMap::jet		= *(new IgSbJetColorMap);
IgSbColorMap &IgSbColorMap::pink	= *(new IgSbPinkColorMap);
IgSbColorMap &IgSbColorMap::prism	= *(new IgSbPrismColorMap);
IgSbColorMap &IgSbColorMap::spring	= *(new IgSbSpringColorMap);
IgSbColorMap &IgSbColorMap::summer	= *(new IgSbSummerColorMap);
IgSbColorMap &IgSbColorMap::vga		= *(new IgSbVGAColorMap);
IgSbColorMap &IgSbColorMap::winter	= *(new IgSbWinterColorMap);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const IgSbColorMap*
IgSbColorMap::getColorMap (ColorMap name){
  switch (name) {
    case Autumn: return &IgSbColorMap::autumn;
    case Bone:   return &IgSbColorMap::bone;
    case Cool:   return &IgSbColorMap::cool;
    case Copper: return &IgSbColorMap::copper;
    //case Cube: return &IgSbColorMap::cube;
    case Flag:   return &IgSbColorMap::flag;
    case Gray:   return &IgSbColorMap::gray;
    case Hot:    return &IgSbColorMap::hot;
    case HSV:    return &IgSbColorMap::hsv;
    case Jet:    return &IgSbColorMap::jet;
    case Pink:   return &IgSbColorMap::pink;
    case Prism:  return &IgSbColorMap::prism;
    case Spring: return &IgSbColorMap::spring;
    case Summer: return &IgSbColorMap::summer;
    case VGA:    return &IgSbColorMap::vga;
    case Winter: return &IgSbColorMap::winter;
    default:     return &IgSbColorMap::jet;
  }
}

IgSbColorMap::~IgSbColorMap (void)
{}

void
IgSbColorMap::hsv2rgb (const float hsv [3], float rgb [3])
{
    float h = hsv [0];
    float s = hsv [1];
    float v = hsv [2];

    if (s == 0)
	rgb [0] = rgb [1] = rgb [2] = v;
    else
    {
	if (h == 1.) h = 0.;
	    h *= 6.;
	int i = int (h);
	float f = h - i;
	float p = v * (1. - s);
	float q = v * (1. - s * f);
	float t = v * (1. - s * (1. - f));

	switch (i)
	{
	case 0: rgb [0] = v; rgb [1] = t; rgb [2] = p; break;
	case 1: rgb [0] = q; rgb [1] = v; rgb [2] = p; break;
	case 2: rgb [0] = p; rgb [1] = v; rgb [2] = t; break;
	case 3: rgb [0] = p; rgb [1] = q; rgb [2] = v; break;
	case 4: rgb [0] = t; rgb [1] = p; rgb [2] = q; break;
	case 5: rgb [0] = v; rgb [1] = p; rgb [2] = q; break;

	default:
	    assert (false);
	}
    }
}

void
IgSbColorMap::rgb2hsv (const float rgb [3], float hsv [3])
{
    float r = rgb [0];
    float g = rgb [1];
    float b = rgb [2];
    float maxval = MAXIMUM (r, MAXIMUM (g, b));
    float minval = MINIMUM (r, MINIMUM (g, b));


    float h;
    float v = maxval;
    float d = maxval - minval;
    float s = (maxval ? d/maxval : 0.f);
    if (! s)
	h = 0;
    else
    {
	if (r == maxval)
	    h = 0.f + (g - b)/d;
	else if (g == maxval)
	    h = 2.f + (b - r)/d;
	else // b == maxval
	    h = 4.f + (r - g)/d;

	h /= 6.f;

	if (h < 0.f)
	    h += 1.f;
    }

    hsv [0] = h;
    hsv [1] = s;
    hsv [2] = v;
}

void
IgSbColorMap::rgb2yiq (const float rgb [3], float yiq [3])
{
    float r = rgb [0];
    float g = rgb [1];
    float b = rgb [2];
    yiq [0] = .299f * r + .587f * g + .115f * b;
    yiq [1] = .596f * r - .275f * g - .321f * b;
    yiq [2] = .212f * r - .523f * g + .311f * b;
}

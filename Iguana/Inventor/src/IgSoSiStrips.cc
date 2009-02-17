//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSiStrips.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoSiStrips);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoSiStrips::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoSiStrips, IgSoShapeKit, "IgSoShapeKit"); }

IgSoSiStrips::IgSoSiStrips (void)
{
    SO_KIT_CONSTRUCTOR (IgSoSiStrips);
    SO_KIT_ADD_FIELD (thickness, (0.03));
    SO_KIT_ADD_FIELD (width,     (1.28));
    SO_KIT_ADD_FIELD (length,    (8.0));
    SO_KIT_ADD_FIELD (pitch,     (0.005));
    SO_KIT_ADD_FIELD (offset,    (0.000));
    SO_KIT_ADD_FIELD (strip,     (0));
    SO_KIT_ADD_FIELD (adc,       (0));
    SO_KIT_ADD_FIELD (gain,      (0.001));
    SO_KIT_ADD_CATALOG_ENTRY (translation, SoTranslation, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (pulses, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoSiStrips::refresh ()
{
    if (strip.getNum () != adc.getNum ()) 
    {
	setPart ("translation", NULL);
	setPart ("pulses", NULL);

	return;
    }
    

    SoTranslation	*translation = new SoTranslation;
    SoSeparator		*pulses = new SoSeparator;

    translation->translation = SbVec3f (0, -width.getValue ()/2 + offset.getValue (), 0);

    for (int i = 0; i < strip.getNum (); i++)
    {
	SoSeparator	*pulse = new SoSeparator;
	SoTranslation	*inc = new SoTranslation;
	SoCube		*hit = new SoCube;

	inc->translation = SbVec3f (0, pitch.getValue () * strip[i], 0);
	hit->width  = thickness.getValue () + adc[i] * gain.getValue ();
	hit->height = pitch.getValue ();
	hit->depth  = length.getValue () - pitch.getValue ();

	pulse->addChild (inc);
	pulse->addChild (hit);
	pulses->addChild (pulse);
    }

    setPart ("translation", translation);
    setPart ("pulses", pulses);
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgParticleChar.h"
#include <iostream>
#include <string>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SbBool					IgParticleChar::inited = FALSE;
IgParticleChar::CMapToParticleList	*IgParticleChar::particleList = NULL;
          
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: in the following, a local copy of a field is often used, which
// shadows the field. 

// constructor
IgParticleChar::IgParticleChar (int g, int p, int isa, const char *nm, const char *lbl,
				float  mas, float q)
    : geantCode (g),
      pythiaCode (p),
      isajetCode (isa),
      name (nm),
      label (lbl),
      mass (mas),
      charge (q)
{
    std::pair< CMapToParticleList::iterator, bool > test;
    test = particleList->insert (CMapToParticleList::value_type (std::string (nm), this));
    if (!test.second)
    {
	std::cout << "IgParticleChar: error creating particle " << name.getString () << std::endl;
    }
}

//! set the SoMaterial used to represent the particle
void
IgParticleChar::setMaterial (SoMaterial* m)
{
    particleMaterial = m;
    m->ref ();
}

//! set the SoDrawStyle used to represent the particle
void
IgParticleChar::setStyle (SoDrawStyle* s)
{
    particleStyle = s;
    s->ref ();
}

//! accessor for the mass of the particle (GeV/c^2)
float
IgParticleChar::getMass () const
{ return mass; }

//! accessor for the charge of the particle
float
IgParticleChar::getCharge () const
{ return charge; }           

//! accessor for the name of the particle
const char *
IgParticleChar::getName () const
{ return name.getString (); }

IgParticleChar::~IgParticleChar ()
{
    particleMaterial->unref ();
    particleMaterial = NULL;
    particleStyle->unref ();
    particleStyle = NULL;
}

//! method to initialize the database with the standard particle set
void
IgParticleChar::initParticles ()
{
    if (inited)
	return;

    inited = TRUE;
    particleList = new CMapToParticleList ();
    IgParticleChar* unknown     = new IgParticleChar (-1,     0,     0,     "unknown",  "unk-",         1.0F, -1.0F);
    IgParticleChar* antiunknown = new IgParticleChar (-1,     0,     0, "antiunknown",  "unk+",         1.0F, +1.0F);
    new IgParticleChar (-1,     0,     0, "neutralunknown",  "unk0",    1.0F, +0.0F);
    IgParticleChar* electron    = new IgParticleChar (3,    11,    12,     "electron",    "e-", 0.000510999F, -1.0F);
    IgParticleChar* positron    = new IgParticleChar (2,   -11,   -12,     "positron",    "e+", 0.000510999F, +1.0F);
    IgParticleChar* muon        = new IgParticleChar (6,    13,    14,         "muon",   "mu-",    0.105658F, -1.0F);
    IgParticleChar* antimuon    = new IgParticleChar (5,   -13,   -14,     "antimuon",   "mu+",    0.105658F, +1.0F);
    IgParticleChar* proton      = new IgParticleChar (14,  2212,  1120,      "proton",     "p",   0.9382723F,  1.0F);
    IgParticleChar* antiproton  = new IgParticleChar (15, -2212, -1120,  "antiproton",  "pbar",   0.9382723F, -1.0F);
    new IgParticleChar (1,    22,     0,        "gamma", "gamma",  0.0000E+00F,  0.0F);
    new IgParticleChar (4,     12,    11,   "eneutrino",  "eneu",          0.F,  0.0F);
    new IgParticleChar (4,     14,    13,  "muneutrino", "muneu",          0.F,  0.0F);
    new IgParticleChar (4,     16,    15,  "tauneutrino","tauneu",         0.F,  0.0F);
    new IgParticleChar (4,   -12,   -11, "antieneutrino","eneubar",       0.F,  0.0F);
    new IgParticleChar (4,    -14,    -13, "antimuneutrino","muneubar",    0.F,  0.0F);
    new IgParticleChar (4,    -16,    -15, "antitauneutrino","tauneubar",  0.F,  0.0F);
    new IgParticleChar (13,  2112,  1220, "neutron",     "n",      0.939566F,  0.0F);
    new IgParticleChar (25, -2112, -1220, "antineutron", "nbar",   0.939566F,  0.0F);
    IgParticleChar* pion        = new IgParticleChar (9,  -211,  -120, "pion",        "pi-",    0.139567F, -1.0F);
    IgParticleChar* antipion    = new IgParticleChar (8,   211,   120, "antipion",    "pi+",    0.139567F, +1.0F);
    new IgParticleChar (45,     0,     0, "deuteron",    "d",      1.875613F,  1.0F);
    new IgParticleChar (46,     0,     0, "tritium",     "tritium", 2.80925F,  1.0F);
    new IgParticleChar (47,     0,     0, "alpha",       "alpha",  3.727417F,  2.0F);
    new IgParticleChar (49,     0,     0, "he3",         "he3",     2.80923F,  2.0F);
    IgParticleChar* kplus       = new IgParticleChar (11,   321,   130, "kplus",       "k+",     0.493677F, +1.0F);
    IgParticleChar* kminus      = new IgParticleChar (12,  -321,  -130, "kminus",      "k-",     0.493677F, -1.0F);
    new IgParticleChar (10,   130,   -20, "klong",       "k0l",    0.497672F,   0.F);
    new IgParticleChar (16,   310,    20, "kshort",      "k0s",    0.497672F,   0.F);
    IgParticleChar* ximinus     = new IgParticleChar (11,  3312,  2330, "ximinus",     "Xi-",    1.32132F,  -1.0F);

    SoDrawStyle* curveStyle = new SoDrawStyle ();
    curveStyle->lineWidth = 1.0;
    curveStyle->pointSize = 4;
	SoDrawStyle* dottedCurveStyle = new SoDrawStyle ();
    dottedCurveStyle->lineWidth = 1.0;
    dottedCurveStyle->pointSize = 4;
	dottedCurveStyle->linePattern = 0xff00; // bitmask: solid is 0xffff;
    SoMaterial* curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.4F, 0.4F, 0.4F);

	CMapToParticleList::const_iterator i = particleList->begin ();
	for (i = particleList->begin (); i != particleList->end (); i++)
	{
		IgParticleChar* pChar = i->second;
		pChar-> setMaterial (curveColour);
		if  ( pChar->charge == 0 )
		{
			pChar->setStyle (dottedCurveStyle);
		}
		else
		{
			pChar-> setStyle (curveStyle);
		}
	}
  
//	set particle-specific values: colour and line style

	curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (1.0, 0.0, 0.0);
    muon->setMaterial (curveColour);
    antimuon->setMaterial (curveColour);
   
    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.0, 1.0, 0.0);
    electron->setMaterial (curveColour);
    positron->setMaterial (curveColour);
   
    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.0, 0.0, 1.0);
    pion->setMaterial (curveColour);
    antipion->setMaterial (curveColour);
   
    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.5, 0.5, 0.0);
    proton->setMaterial (curveColour);
   
    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.45F, 0.55F, 0.0F);
    antiproton->setMaterial (curveColour);

    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.2F, 0.2F, 0.2F);
    unknown->setMaterial (curveColour);
    antiunknown->setMaterial (curveColour);

    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.0, 0.65F, 0.65F);
    kplus->setMaterial (curveColour);
    kminus->setMaterial (curveColour);
    ximinus->setMaterial (curveColour);

}

//! method to remove a particle from the database
void
IgParticleChar::deleteParticles ()
{}

//! method to look up a ParticleChar by name
IgParticleChar *
IgParticleChar::getByName (const char *string)
{
    CMapToParticleList::iterator it = particleList->find (string);
    return it != particleList->end () ? it->second : 0;
}

//! method to look up a ParticleChar by GEANT id
IgParticleChar *
IgParticleChar::getByGeantID (const int p_code)
{
	CMapToParticleList::const_iterator i = particleList->begin ();
    for (i = particleList->begin (); i != particleList->end (); i++)
    {
	IgParticleChar* pChar = i->second;
	if (pChar->geantCode == p_code)
	    return pChar;
    }
    // nothing found
    return getByName ("unknown");
}

//! method to look up a ParticleChar by PYTHIA id
IgParticleChar *
IgParticleChar::getByPythiaID (const int p_code)
{
    CMapToParticleList::const_iterator i = particleList->begin ();
    for (i = particleList->begin (); i != particleList->end (); i++)
    {
	IgParticleChar* pChar = i->second;
	if (pChar->pythiaCode == p_code)
	    return pChar;
    }
    // nothing found
    return getByName ("unknown");
}

//! method to look up a ParticleChar by ISAJET id
IgParticleChar *
IgParticleChar::getByIsajetID (const int p_code)
{
    CMapToParticleList::const_iterator i = particleList->begin ();
    for (i = particleList->begin (); i != particleList->end (); i++)
    {
	IgParticleChar* pChar = i->second;
	if (pChar->isajetCode == p_code)
	    return pChar;
    }
    // nothing found
    return getByName ("unknown");
}

//! dump the entire internal database
void
IgParticleChar::dumpDB (std::ostream & os)
{
    os << "Dumping the ParticleChar Database..." << std::endl;
    for (CMapToParticleList::const_iterator it=particleList->begin (); it != particleList->end (); it++)
    {
	os << it->first << std::endl;
	if (it->second)  os << *it->second;
    }
} 

//! print information
void
IgParticleChar::print (std::ostream & os) const
{
    os << "Particle: " << name.getString ()
       << ", Label: " << label.getString ()
       << ", Charge: " << charge
       << ", Mass: " << mass << std::endl;
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgParticleChar.h"
#include <iostream>
#include <string>

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

//! constructor
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
    IgParticleChar* unknown     = new IgParticleChar (-1,     0,     0,     "unknown",  "unk-",         1.0, -1.0);
    IgParticleChar* antiunknown = new IgParticleChar (-1,     0,     0, "antiunknown",  "unk+",         1.0, +1.0);
    IgParticleChar* electron    = new IgParticleChar (3,    11,    12,     "electron",    "e-", 0.000510999, -1.0);
    IgParticleChar* positron    = new IgParticleChar (2,   -11,   -12,     "positron",    "e+", 0.000510999, +1.0);
    IgParticleChar* muon        = new IgParticleChar (6,    13,    14,         "muon",   "mu-",    0.105658, -1.0);
    IgParticleChar* antimuon    = new IgParticleChar (5,   -13,   -14,     "antimuon",   "mu+",    0.105658, +1.0);
    //IgParticleChar* gamma       = new IgParticleChar (1,    22,     0,        "gamma", "gamma",  0.0000E+00,  0.0);
    IgParticleChar* proton      = new IgParticleChar (14,  2212,  1120,      "proton",     "p",   0.9382723,  1.0);
    IgParticleChar* antiproton  = new IgParticleChar (15, -2212, -1120,  "antiproton",  "pbar",   0.9382723, -1.0);
    // FIXME: the nutrals give nans and infs
    //new IgParticleChar (13,  2112,  1220, "neutron",     "n",      0.939566,  0.0);
    //new IgParticleChar (25, -2112, -1220, "antineutron", "nbar",   0.939566,  0.0);
    IgParticleChar* pion        = new IgParticleChar (9,  -211,  -120, "pion",        "pi-",    0.139567, -1.0);
    IgParticleChar* antipion    = new IgParticleChar (8,   211,   120, "antipion",    "pi+",    0.139567, +1.0);
    new IgParticleChar (45,     0,     0, "deuteron",    "d",      1.875613,  1.0);
    new IgParticleChar (46,     0,     0, "tritium",     "tritium", 2.80925,  1.0);
    new IgParticleChar (47,     0,     0, "alpha",       "alpha",  3.727417,  2.0);
    new IgParticleChar (49,     0,     0, "he3",         "he3",     2.80923,  2.0);
    IgParticleChar* kplus       = new IgParticleChar (11,   321,   130, "kplus",       "k+",     0.493677, +1.0);
    IgParticleChar* kminus      = new IgParticleChar (11,  -321,  -130, "kminus",      "k-",     0.493677, -1.0);
    IgParticleChar* ximinus     = new IgParticleChar (11,  3312,  2330, "ximinus",     "Xi-",     1.32132,  -1.0);

    SoDrawStyle* curveStyle = new SoDrawStyle ();
    curveStyle->lineWidth = 1.0;
    curveStyle->pointSize = 4;
    SoMaterial* curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.2, 0.2, 0.2);

    CMapToParticleList::const_iterator i = particleList->begin ();
    for (i = particleList->begin (); i != particleList->end (); i++)
    {
	IgParticleChar* pChar = i->second;
	pChar-> setMaterial (curveColour);
	pChar-> setStyle (curveStyle);
    }
  
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
    curveColour->diffuseColor.setValue (0.45, 0.55, 0.0);
    antiproton->setMaterial (curveColour);

    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.2, 0.2, 0.2);
    unknown->setMaterial (curveColour);
    antiunknown->setMaterial (curveColour);

    curveColour = new SoMaterial ();
    curveColour->diffuseColor.setValue (0.0, 0.65, 0.65);
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

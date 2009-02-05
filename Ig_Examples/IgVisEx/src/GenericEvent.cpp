#include "Ig_Examples/IgVisEx/interface/GenericEvent.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoIdealTrack.h"

#include <cstdlib>
#include <list>
#include <cmath>

Event::Event () : IgQtTwig ("Event")
{
    // construct the parts of what we need in the event
    addChild (new TrackTwig ("Tracks"));
}

Event::~Event ()
{
}

void Event::drawMe ()
{
}

// for generating Gaussian deviates
// from Numerical Recipes

// convert MS integer rand [0,RAND_MAX] to [0,1]
double igrand (void) 
{
    //	return double (std::rand()) / double (RAND_MAX);
	
    double d1 = double (std::rand ());
    double d2 = double (RAND_MAX);
    return d1/d2;
}

class GaussianDeviates 
{
public:
    GaussianDeviates (void);
    double deviate (void);
    ~GaussianDeviates () {};

private:
    //! generates two at a time. save one
    double	saved; 
    //! is a saved deviate available?
    bool	flag;
};

GaussianDeviates::GaussianDeviates ()
    : flag (false)
{
}

double GaussianDeviates::deviate ()
{
    if (flag) 
    {
	flag = false;
	return saved;
    }
    else
    {
	double v1 = 0, v2 = 0, r = -1;
	while ( r >= 1. || r <= 0. )
	{
	    v1 = 2. * igrand () - 1.;
	    v2 = 2. * igrand () - 1.;
	    r = v1 * v1 + v2 * v2;
	}
	flag = true;
	double fac = std::sqrt (-2. * std::log (r)/r);
	saved = v1 * fac;
	return v2 * fac;
    }
}

///////////////////////////////////////

TrackTwig::TrackTwig (SbString name, bool visibility, bool cascade) 
    : IgQtLiveTwig< TestTrack > (name, visibility, cascade)
{
    p_list = new std::list< TestTrack >;
}

TrackTwig::~TrackTwig ()
{
    if (p_list) delete p_list;
}

void TrackTwig::drawMe ()
{
    GaussianDeviates g;
    // pick some track parameters from a hat
    double px0 = 5., py0 = 5., pz0 = 10.;
    double pxdelta = 2., pydelta = 2., pzdelta = 5.;
    float x0 = 0.F, y0 = 0.F, z0 = 0.F, t0 = 0.F;

    // pick a random number of tracks between 0 & 10
    int ntracks = int (10. * igrand ());
    p_list->resize (ntracks);
    int label=0; // track counter
    for (std::list< TestTrack >::iterator it = p_list->begin (); it != p_list->end (); it++, label++ )
    {
	double px = pxdelta * g.deviate () + px0;
	px *= igrand () > 0.5 ? 1 : -1;
	double py = pydelta * g.deviate () + py0;
	py *= igrand() > 0.5 ? 1 : -1;
	double pz = pzdelta * g.deviate () + pz0;
	pz *= igrand () > 0.5 ? 1 : -1;
	double E = std::sqrt (1. + px * px + py * py + pz * pz);
	*it = TestTrack (px, py, pz, E);
	IgSoIdealTrack *kit = new IgSoIdealTrack;
	kit->initialise (x0, y0, z0, px, py, pz, t0, 6); //6=> muon track type
	// using the LiveTwig
	insert (kit, &(*it));
	// using individual twigs just to test twig save/restore. may need to sort in widget to 
	// get response...
//	QString tag = QString("Track_%1").arg(label,2);
//	IgQtTwig *twig = new IgQtTwig(tag.ascii());
//	addChild(twig);
	// 
//	twig->addIvNode(kit);
    }
}

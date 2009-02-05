#if !defined GENERIC_DETECTOR
#define GENERIC_DETECTOR
/*!   
 *     \brief Example detector
 *
 *     \author G. Alverson, Northeastern University
 *     \data  20-Jul-2002
 */

#include "Ig_Modules/IgVis/interface/IgQtTwig.h"
/*!
 * \class Detector
 * \brief Builds a generic detector description twig
 */


class Detector : public IgQtTwig
{
public:
	Detector();
	void drawMe();
	virtual ~Detector();
};



//////////////////////////////////////////////////////////////////
class Infrastructure : public IgQtTwig
{
public:
	Infrastructure();
	void drawMe();
	virtual ~Infrastructure();
};

//////////////////////////////////////////////////////////////////
class Tracker : public IgQtTwig
{
public:
	Tracker();
	void drawMe();
	virtual ~Tracker();
};


//////////////////////////////////////////////////////////////////
class Ecal : public IgQtTwig
{
public:
	Ecal();
	void drawMe();
	virtual ~Ecal();
};


//////////////////////////////////////////////////////////////////
class Hcal : public IgQtTwig
{
public:
	Hcal();
	void drawMe();
	virtual ~Hcal();
};

//////////////////////////////////////////////////////////////////
class ForwardCalorimeter : public IgQtTwig
{
public:
	ForwardCalorimeter();
	void drawMe();
	virtual ~ForwardCalorimeter();
};


//////////////////////////////////////////////////////////////////
class MuonChambers : public IgQtTwig
{
public:
	MuonChambers();
	void drawMe();
	virtual ~MuonChambers();
};





//////////////////////////////////////////////////////////////////////////////////////////
class wheelsOfBoxes : public SoSeparator
/** 
 * OpenInventor node containing a set of boxes in a wheel (e.g. for a silicon detector).
 * The axis (axel) of the wheel is oriented along z.
 * The boxes repeat in azimuth enough times to fill 2*pi.
 *
 */
{
public:
    wheelsOfBoxes( int   n_r,              // no. boxes in radial direction (e.g. layers for barrel)
 	           int   n_z,              // no. boxes in z direction      (e.g. wheels for barrel)
	           float boxHeight,        // radial dimension  
	           float boxWidth,         // azimuthal dimension
	           float boxDepth,         // z-axis dimension 
                   float r_min,            // Inner radius of the whole thing  
		   float z_ave,            // Average z of whole thing
		   float r_gap,            // Gap between radial layers => r spacing = r_gap + boxHeight 
                   float z_gap,            // Gap between z layers      => z spacing = z_gap + boxDepth
		   float phi_overlap);     // Relative overlap in azimuth (e.g. 0.1 gives 10% overlap; -0.1 leaves a gap)
    
    virtual ~wheelsOfBoxes(){};
};



#endif // GENERIC_DETECTOR




















#if !defined IG_GENERIC_DETECTOR_DEMO_IG_GD_TWIG_H
#define IG_GENERIC_DETECTOR_DEMO_IG_GD_TWIG_H
/*!   
 *     \brief Example detector
 *
 *     \author G. Alverson, Northeastern University
 *     \data  20-Jul-2002
 */
/*!
  *    Modified for the new IGUANA arch
  *
  *    changed by Shahzad Muzaffar, Northeastern University
  *    date 18-Mar-2003
  */
   
# include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Inventor/nodes/SoSeparator.h"
#include <qobject.h>

class IgState;
class IgSoFieldPlane;
/*!
 * \class Detector
 * \brief Builds a generic detector description twig
 */

class Detector : public IgSimpleTwig
{
public:
	Detector(IgTwig *parent, IgState *state);
};

//////////////////////////////////////////////////////////////////
class Infrastructure : public IgSimpleTwig
{
public:
	Infrastructure(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class BeamPipe : public IgSimpleTwig
{
public:
	BeamPipe(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class SupportTube : public IgSimpleTwig
{
public:
	SupportTube(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class Solenoid : public IgSimpleTwig
{
public:
	Solenoid(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class Tracker : public IgSimpleTwig
{
public:
	Tracker(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  InnerBarrel: public IgSimpleTwig
{
public:
	InnerBarrel(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  OuterBarrel: public IgSimpleTwig
{
public:
	OuterBarrel(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  InnerForward: public IgSimpleTwig
{
public:
	InnerForward(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  OuterForward: public IgSimpleTwig
{
public:
	OuterForward(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class Ecal : public IgSimpleTwig
{
public:
	Ecal(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class BarrelECAL : public IgSimpleTwig
{
public:
	BarrelECAL(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  EndcapECALZPlus: public IgSimpleTwig
{
public:
	EndcapECALZPlus(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  EndcapECALZMinus: public IgSimpleTwig
{
public:
	EndcapECALZMinus(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class Hcal : public IgSimpleTwig
{
public:
	Hcal(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class BarrelHCAL : public IgSimpleTwig
{
public:
	BarrelHCAL(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  EndcapHCALZPlus: public IgSimpleTwig
{
public:
	EndcapHCALZPlus(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  EndcapHCALZMinus: public IgSimpleTwig
{
public:
	EndcapHCALZMinus(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class ForwardCalorimeter : public IgSimpleTwig
{
public:
	ForwardCalorimeter(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class ForwardCalorimeterZPlus : public IgSimpleTwig
{
public:
	ForwardCalorimeterZPlus(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class  ForwardCalorimeterZMinus: public IgSimpleTwig
{
public:
	ForwardCalorimeterZMinus(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class MuonChambers : public IgSimpleTwig
{
public:
	MuonChambers(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class BarrelChambers : public IgSimpleTwig
{
public:
	BarrelChambers(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class EndcapChambers : public IgSimpleTwig
{
public:
	EndcapChambers(IgTwig *parent);
};

//////////////////////////////////////////////////////////////////
class testCube : public IgSimpleTwig
{
public:
	testCube(IgTwig *parent);
};


//////////////////////////////////////////////////////////////////
class Event : public IgSimpleTwig
{
public:
    Event (IgTwig *parent, IgState *state);    
};

//////////////////////////////////////////////////////////////////
class Ig3DFieldPlanesCategory;
class IgControlCentre;

class MagneticField : public QObject, public IgSimpleTwig
{
	Q_OBJECT
public:
	MagneticField(IgTwig *parent, IgState *state);
	~MagneticField();
	IgSoFieldPlane*		getFieldPlane();
        
public slots:
	void showFieldPlanes (void);

private:
	IgControlCentre 	*m_centre;
	Ig3DFieldPlanesCategory	*m_fieldPlanesCategory;
	IgSoFieldPlane 		*m_plane;
};

//////////////////////////////////////////////////////////////////////////////////////////
class IgGDWheelsOfBoxes : public SoSeparator
/** 
 * OpenInventor node containing a set of boxes in a wheel (e.g. for a silicon detector).
 * The axis (axel) of the wheel is oriented along z.
 * The boxes repeat in azimuth enough times to fill 2*pi.
 *
 */
{
public:
    IgGDWheelsOfBoxes( int   n_r,              // no. boxes in radial direction (e.g. layers for barrel)
 	           int   n_z,              // no. boxes in z direction      (e.g. wheels for barrel)
	           float boxHeight,        // radial dimension  
	           float boxWidth,         // azimuthal dimension
	           float boxDepth,         // z-axis dimension 
                   float r_min,            // Inner radius of the whole thing  
		   float z_ave,            // Average z of whole thing
		   float r_gap,            // Gap between radial layers => r spacing = r_gap + boxHeight 
                   float z_gap,            // Gap between z layers      => z spacing = z_gap + boxDepth
		   float phi_overlap);     // Relative overlap in azimuth (e.g. 0.1 gives 10% overlap; -0.1 leaves a gap)
    
    virtual ~IgGDWheelsOfBoxes(){};
};

#endif // IG_GENERIC_DETECTOR_DEMO_IG_GD_TWIG_H

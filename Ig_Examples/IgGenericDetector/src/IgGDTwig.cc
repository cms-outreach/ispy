//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Ig_Examples/IgGenericDetector/interface/IgGDTwig.h"
#include "Ig_Examples/IgGenericDetector/interface/xtypeinfo.h"
#include "Ig_Modules/IgCommon3DReps/interface/IgCommon3DReps.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DRep.h"
#include "Ig_Modules/Ig3DModel/interface/xtypeinfo.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoRep.h"
#include "Ig_Modules/IgLegoModel/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoRotSolid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSbField.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlane.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlaneMap.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSbField.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSbColorMap.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoLegoPlot.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSimpleTrajectory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DFieldPlanesCategory.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"

#include <list>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include <qpopupmenu.h>

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/SbLinear.h"
#include "Inventor/nodes/SoTranslation.h"
#include "Inventor/nodes/SoTransform.h"
#include "Inventor/nodes/SoRotation.h"
#include "Inventor/nodes/SoCube.h"
#include "Inventor/nodes/SoUnits.h"
#include "Inventor/nodes/SoMatrixTransform.h"
#include "Inventor/nodes/SoShapeHints.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include "Inventor/nodes/SoVertexProperty.h"
#include "Inventor/nodes/SoMaterialBinding.h"


Detector::Detector (IgTwig *parent, IgState *state) 
    : IgSimpleTwig (parent, "Generic Detector", true, true, true)
/**
 *
 *    Top-level detector ("container") twig.
 *    This contains ("container") sub-twigs for subdetectors which in
 *    turn contain the actual barrel and endcap elements.
 *
 */
{
    new Infrastructure (this);
    new Tracker (this);
    new Ecal (this);
    new Hcal (this);
    new ForwardCalorimeter (this);
    new MuonChambers (this);
    new MagneticField (this, state);
}

//////////////////////////////////////////////////////////////////////////////////////////

Infrastructure::Infrastructure (IgTwig *parent) 
    : IgSimpleTwig (parent, "Infrastructure", true, true, true)
/**
 *
 *    Infrastructure (some of the large non-sensitive stuff)
 *    Set it not visible and expandable by default due to 
 *    problematic shapes which slow down the example.
 *
 */
{
    new BeamPipe (this);
    new SupportTube (this);
    new Solenoid (this);
}

//////////////////////////////////////////////////////////////////////////////////////////
Solenoid::Solenoid (IgTwig *parent) 
    : IgSimpleTwig ( parent, "Solenoid", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (Solenoid *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *greySteel = new SoMaterial;
    greySteel->ambientColor.setValue (0.039, 0.038, 0.038);
    greySteel->diffuseColor.setValue (0.155, 0.150, 0.151);
    greySteel->specularColor.setValue(0.528, 0.524, 0.524);
    greySteel->emissiveColor.setValue(0.314, 0.074, 0.000);
    greySteel->shininess = 0.3;
    greySteel->transparency = 0.0;        
         
    IgSoRotSolid* theG4Cons = new IgSoRotSolid;
    theG4Cons->makeCons (3.10F, 3.40F, 3.10F, 3.40F, 6.50F);
     
    rep->node ()->addChild (greySteel);
    rep->node ()->addChild (theG4Cons);
}

//////////////////////////////////////////////////////////////////////////////////////////
/*
 *    Beampipe:
 *    Set it not visible and expandable by default due to 
 *    problematic shapes which slow down the example.
 *
 */
BeamPipe::BeamPipe (IgTwig *parent) 
    : IgSimpleTwig (parent, "Beam Pipe", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (BeamPipe *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoUnits *theUnits = new SoUnits;
    theUnits->units = SoUnits::METERS; //Meters are the default; CENTIMETERS is a common alternative

    SoMaterial *beryllium = new SoMaterial;
    beryllium->ambientColor.setValue (0.007, 0.002, 0.000);
    beryllium->diffuseColor.setValue (0.314, 0.074, 0.000);
    beryllium->specularColor.setValue(1.000, 0.767, 0.000);
    beryllium->emissiveColor.setValue(0.314, 0.074, 0.000);
    beryllium->shininess = 0.048;
    beryllium->transparency = 0.0;

    std::vector<float> zvals(6);
    std::vector<float> rmin(6);
    std::vector<float> rmax(6);
    
    float z_half       =15.0F;    // rough guess at canonical beampipe
    float z_close      = 2.0F;    // rough guess at canonical beampipe
    float radius_large = 0.150F;  // rough guess at canonical beampipe
    float radius_small = 0.050F;   // rough guess at canonical beampipe
    float thickness     = 0.001F;  // rough guess at canonical beampipe
    zvals[0] = -z_half;                // far from centre...typically fat
    zvals[1] = -z_close;               // now starts to taper down 
    zvals[2] =  zvals[1] + 1.0F;           // has now finished tapering down to become narrow
    zvals[3] = -zvals[2];
    zvals[4] = -zvals[1];
    zvals[5] = -zvals[0];
    rmin[1] = rmin[0] = radius_large;
    rmin[3] = rmin[2] = radius_small;
    rmin[5] = rmin[4] = rmin[0];  
    rmax[1] = rmax[0] = rmin[0] + thickness;
    rmax[3] = rmax[2] = rmin[2] + thickness;
    rmax[5] = rmax[4] = rmax[0];

    IgSoRotSolid* pipe_rep = new IgSoRotSolid();
    pipe_rep->makePcon (zvals, rmin, rmax);
    
    rep->node ()->addChild (theUnits);
    rep->node ()->addChild (beryllium);
    rep->node ()->addChild (pipe_rep);
}

//////////////////////////////////////////////////////////////////////////////////////////
SupportTube::SupportTube (IgTwig *parent) 
    : IgSimpleTwig (parent, "Support Tube", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (SupportTube *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *greySteel = new SoMaterial;
    greySteel->ambientColor.setValue (0.039, 0.038, 0.038);
    greySteel->diffuseColor.setValue (0.155, 0.150, 0.151);
    greySteel->specularColor.setValue(0.528, 0.524, 0.524);
    greySteel->emissiveColor.setValue(0.314, 0.074, 0.000);
    greySteel->shininess = 0.3;
    greySteel->transparency = 0.0;        
   
    IgSoRotSolid *theG4Cons = new IgSoRotSolid ();
    theG4Cons->makeCons (2.950F, 2.970F, 2.950F, 2.970F, 6.50F);
    
    rep->node ()->addChild (greySteel);
    rep->node ()->addChild (theG4Cons);
}

//////////////////////////////////////////////////////////////////////////////////////////
Tracker::Tracker (IgTwig *parent) 
    : IgSimpleTwig (parent, "Tracker", true, true, true)
/**
 *
 *    Tracker  (boxes representing e.g. silicon wafers, or MSGC's)
 *        Barrel:   cylindrically symmetric wheels of boxes
 *        Endcaps:  turbine-blades of boxes (a bit crude)
 */
{
    new InnerBarrel (this);
    new OuterBarrel (this);
    new InnerForward (this);
    new OuterForward (this);
}


//////////////////////////////////////////////////////////////////////////////////////////
InnerBarrel::InnerBarrel (IgTwig *parent) 
    : IgSimpleTwig (parent, "Inner Barrel", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (InnerBarrel *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    // Define materials
    SoMaterial *silicon = new SoMaterial;
    silicon->ambientColor.setValue (0.007, 0.002, 0.000);
    silicon->diffuseColor.setValue (0.314, 0.074, 0.000);
    silicon->specularColor.setValue(1.000, 0.767, 0.000);
    silicon->emissiveColor.setValue(0.867, 0.867, 0.337);
    silicon->shininess = 0.048;
    silicon->transparency = 0.0;

    // Generic parameters defining a wheel of boxes
    int   n_r, n_z = 0;
    float boxHeight, boxWidth, boxDepth, r_min, z_ave, r_gap, z_gap, phi_overlap = 0.F;
            
    // Create representations for tracker barrel (inner)     
    n_r          = 3;
    n_z          = 3;
    boxHeight    = 0.005F;
    boxWidth     = 0.050F;        
    boxDepth     = 0.350F;
    r_min        = 0.140F;        
    z_ave        = 0.000F;        
    r_gap        = 0.080F;        
    z_gap        = 0.005F;
    phi_overlap  = 0.15;
	
    IgGDWheelsOfBoxes *innerBarrel = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
							    r_min, z_ave, r_gap, z_gap, phi_overlap);
    rep->node ()->addChild (silicon);
    rep->node ()->addChild (innerBarrel);
}

//////////////////////////////////////////////////////////////////////////////////////////
OuterBarrel::OuterBarrel (IgTwig *parent) 
    : IgSimpleTwig (parent, "Outer Barrel", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (OuterBarrel *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    // Define materials    
    SoMaterial *aluminium = new SoMaterial;
    aluminium->ambientColor.setValue (0.095, 0.095, 0.095);
    aluminium->diffuseColor.setValue (0.372, 0.371, 0.373);
    aluminium->specularColor.setValue(0.556, 0.554, 0.556);
    aluminium->emissiveColor.setValue(0.500, 0.500, 0.500);
    aluminium->shininess = 0.128;
    aluminium->transparency = 0.0;


    // Generic parameters defining a wheel of boxes
    int   n_r, n_z = 0;
    float boxHeight, boxWidth, boxDepth, r_min, z_ave, r_gap, z_gap, phi_overlap = 0.F;               
        
    // Create representations for tracker barrel (outer) 
    
    n_r          = 4;
    n_z          = 3;
    boxHeight    = 0.030F;
    boxWidth     = 0.200F;        
    boxDepth     = 0.900F;
    r_min        = 0.500F;        
    z_ave        = 0.000F;        
    r_gap        = 0.200F;        
    z_gap        = 0.020F;
    phi_overlap  = 0.100;
	    
    IgGDWheelsOfBoxes *outerBarrel = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
							    r_min, z_ave, r_gap, z_gap, phi_overlap);
    rep->node ()->addChild (aluminium);
    rep->node ()->addChild (outerBarrel);
}

//////////////////////////////////////////////////////////////////////////////////////////
InnerForward::InnerForward (IgTwig *parent) 
    : IgSimpleTwig (parent, "Inner Forward", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (InnerForward *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    // Define materials    
    SoMaterial *silicon = new SoMaterial;
    silicon->ambientColor.setValue (0.007, 0.002, 0.000);
    silicon->diffuseColor.setValue (0.314, 0.074, 0.000);
    silicon->specularColor.setValue(1.000, 0.767, 0.000);
    silicon->emissiveColor.setValue(0.867, 0.867, 0.337);
    silicon->shininess = 0.048;
    silicon->transparency = 0.0;

    // Generic parameters defining a wheel of boxes
    int   n_r, n_z = 0;
    float boxHeight, boxWidth, boxDepth, r_min, z_ave, r_gap, z_gap, phi_overlap = 0.F;
    
        
    // Create representations for tracker endcap (inner) 	
    n_r          = 2;
    n_z          = 3;
    boxHeight    = 0.200F;
    boxWidth     = 0.070F;        
    boxDepth     = 0.020F;
    r_min        = 0.200F;        
    z_ave        = 0.900F;        
    r_gap        = 0.050F;        
    z_gap        = 0.250F;
    phi_overlap  = -0.1;
	
    IgGDWheelsOfBoxes *innerA = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
						       r_min, z_ave, r_gap, z_gap, phi_overlap);
    z_ave = -z_ave;
    IgGDWheelsOfBoxes *innerB = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
						       r_min, z_ave, r_gap, z_gap, phi_overlap);
    rep->node ()->addChild (silicon);
    rep->node ()->addChild (innerA);
    rep->node ()->addChild (innerB);
}

//////////////////////////////////////////////////////////////////////////////////////////
OuterForward::OuterForward (IgTwig *parent) 
    : IgSimpleTwig (parent, "Outer Forward", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (OuterForward *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    // Define materials    
    SoMaterial *aluminium = new SoMaterial;
    aluminium->ambientColor.setValue (0.095, 0.095, 0.095);
    aluminium->diffuseColor.setValue (0.372, 0.371, 0.373);
    aluminium->specularColor.setValue(0.556, 0.554, 0.556);
    aluminium->emissiveColor.setValue(0.500, 0.500, 0.500);
    aluminium->shininess = 0.128;
    aluminium->transparency = 0.0;


    // Generic parameters defining a wheel of boxes
    int   n_r, n_z = 0;
    float boxHeight, boxWidth, boxDepth, r_min, z_ave, r_gap, z_gap, phi_overlap = 0.F;
    
    // Create representations for tracker endcap (outer)        
    n_r          = 2;
    n_z          = 3;
    boxHeight    = 0.500F;
    boxWidth     = 0.150F;        
    boxDepth     = 0.080F;
    r_min        = 0.400F;        
    z_ave        = 2.000F;        
    r_gap        = 0.050F;        
    z_gap        = 0.400F;
    phi_overlap  = -0.1;
	
    IgGDWheelsOfBoxes *outerA = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth, 
						       r_min, z_ave, r_gap, z_gap, phi_overlap);
    z_ave = -z_ave;
    IgGDWheelsOfBoxes *outerB = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
						       r_min, z_ave, r_gap, z_gap, phi_overlap);
    rep->node ()->addChild (aluminium);        
    rep->node ()->addChild (outerA);        
    rep->node ()->addChild (outerB);        
}

//////////////////////////////////////////////////////////////////////////////////////////
Ecal::Ecal(IgTwig *parent) 
    : IgSimpleTwig (parent, "Ecal", true, true, true)
/**
 *
 *    Ecal: Rough baskets of crystals.
 *           Roughly based on Figs 1.2 and 1.3 of CMS Technical Proposal 
 *            
 */
{
    new BarrelECAL (this);
    new EndcapECALZPlus (this);
    new EndcapECALZMinus (this);
}


//////////////////////////////////////////////////////////////////////////////////////////
BarrelECAL::BarrelECAL (IgTwig *parent) 
    : IgSimpleTwig (parent, "Barrel ECAL", true, true, true)
{
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (BarrelECAL *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *crystal = new SoMaterial;  // was "glass.20"
    crystal->ambientColor.setValue (0.704, 0.721, 0.721);
    crystal->diffuseColor.setValue (0.482, 0.481, 0.552);
    crystal->specularColor.setValue(0.834, 0.566, 0.540);
    crystal->emissiveColor.setValue(0.500, 0.500, 1.000);
    crystal->shininess = 0.236;
    crystal->transparency = 0.0;   // 0.216
    
    // define barrel profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_b                 = 18;        // azimuthal divisions
    int nzee_b                 = 4;         // z divisions 
    float phi_b                = 0;         // starting angle
    float del_phi_b            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_b(nzee_b);
    std::vector<float> rmin_b (nzee_b);
    std::vector<float> rmax_b (nzee_b);
    
    zvals_b [0] = -3.760F;
    rmin_b  [0] =  1.500F;
    rmax_b  [0] =  1.840F;

    zvals_b [1] = -3.350F;
    rmin_b  [1] =  1.330F;
    rmax_b  [1] =  rmax_b[0];

    zvals_b [2] = -zvals_b[1];
    rmin_b  [2] =  rmin_b [1];
    rmax_b  [2] =  rmax_b [1];

    zvals_b [3] = -zvals_b[0];
    rmin_b  [3] =  rmin_b [0];
    rmax_b  [3] =  rmax_b [0];

    IgSoRotSolid *barrelEcal = new IgSoRotSolid;
    barrelEcal->makePgon (zvals_b, rmin_b, rmax_b, phi_b, del_phi_b, ndiv_b);
        
    rep->node ()->addChild (crystal);
    rep->node ()->addChild (barrelEcal);
}

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (BarrelECAL *twig, IgLegoRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    typedef IgSbLegoHistogram::value_type ValueType;
    IgSoLegoPlot *lego = new IgSoLegoPlot;
    lego->heightScale = 20;
    IgSbLegoHistogram histo;
    for (int bin = 0; bin < 20; bin++) 
	histo.insert (ValueType (IgSbLegoBin (bin, bin),
				 IgSbLegoValue (0.6 * bin, 0.4 * bin)));
    lego->setData (histo);

    rep->node ()->addChild (lego);
}

//////////////////////////////////////////////////////////////////////////////////////////
EndcapECALZPlus::EndcapECALZPlus (IgTwig *parent) 
    : IgSimpleTwig (parent, "Endcap ECAL (+z)", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (EndcapECALZPlus *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *crystal = new SoMaterial;  // was "glass.20"
    crystal->ambientColor.setValue (0.704, 0.721, 0.721);
    crystal->diffuseColor.setValue (0.482, 0.481, 0.552);
    crystal->specularColor.setValue(0.834, 0.566, 0.540);
    crystal->emissiveColor.setValue(0.500, 0.500, 1.000);
    crystal->shininess = 0.236;
    crystal->transparency = 0.0;   // 0.216
    

    // define endcap profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_e                 = 18;        // azimuthal divisions
    int nzee_e                 = 2;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    std::vector<float> rmin_b (1);
    
    //rmin_b value from BarrelECAL
    rmin_b[0] = 1.500F;
    
    zvals_e [0] = -3.760F;
    rmin_e  [0] =  0.300F;
    rmax_e  [0] =  rmin_b[0] - 0.1;

    zvals_e [1] = -3.250F;      
    rmin_e  [1] =  0.250F;
    rmax_e  [1] =  1.280F;      

    // Endcap on +z side      
    for (int i = 0; i < nzee_e; i++) 
    {	
	zvals_e [i] = - zvals_e [i];
    }

    // now, re-order in increasing z (otherwise normals of PGON are wrong, i.e. inside out )
    std::vector<float> zvals_eB(nzee_e);
    std::vector<float> rmin_eB (nzee_e);
    std::vector<float> rmax_eB (nzee_e);

    for (int i = 0; i < nzee_e; i++) 
    {
	zvals_eB [i] = zvals_e [nzee_e - 1 - i];
	rmin_eB  [i] = rmin_e  [nzee_e - 1 - i];
	rmax_eB  [i] = rmax_e  [nzee_e - 1 - i];
    }
    IgSoRotSolid *endcapEcalB = new IgSoRotSolid;
    endcapEcalB->makePgon (zvals_eB, rmin_eB, rmax_eB, phi_e, del_phi_e, ndiv_e);
        
    rep->node ()->addChild (crystal);
    rep->node ()->addChild (endcapEcalB);
}

//////////////////////////////////////////////////////////////////////////////////////////
EndcapECALZMinus::EndcapECALZMinus (IgTwig *parent) 
    : IgSimpleTwig (parent, "Endcap ECAL (-z)", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (EndcapECALZMinus *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *crystal = new SoMaterial;  // was "glass.20"
    crystal->ambientColor.setValue (0.704, 0.721, 0.721);
    crystal->diffuseColor.setValue (0.482, 0.481, 0.552);
    crystal->specularColor.setValue(0.834, 0.566, 0.540);
    crystal->emissiveColor.setValue(0.500, 0.500, 1.000);
    crystal->shininess = 0.236;
    crystal->transparency = 0.0;   // 0.216
    

    // define endcap profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_e                 = 18;        // azimuthal divisions
    int nzee_e                 = 2;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    std::vector<float> rmin_b (1);
    
    //rmin_b value from BarrelECAL
    rmin_b[0] = 1.500F;
    
    zvals_e [0] = -3.760F;
    rmin_e  [0] =  0.300F;
    rmax_e  [0] =  rmin_b[0] - 0.1;

    zvals_e [1] = -3.250F;      
    rmin_e  [1] =  0.250F;
    rmax_e  [1] =  1.280F;      

    // Endcap on -z side  
    IgSoRotSolid *endcapEcalA = new IgSoRotSolid;
    endcapEcalA->makePgon (zvals_e, rmin_e, rmax_e, phi_e, del_phi_e, ndiv_e);
        
    rep->node ()->addChild (crystal);
    rep->node ()->addChild (endcapEcalA);
}

//////////////////////////////////////////////////////////////////////////////////////////
Hcal::Hcal (IgTwig *parent) 
    : IgSimpleTwig (parent, "Hcal", true, true, true)
/**
 *
 *    Ecal: Rough baskets of crystals.
 *           Roughly based on Figs 1.2 and 1.3 of CMS Technical Proposal 
 *            
 */
{
    new BarrelHCAL (this);
    new EndcapHCALZPlus (this);
    new EndcapHCALZMinus (this);
}


//////////////////////////////////////////////////////////////////////////////////////////
BarrelHCAL::BarrelHCAL (IgTwig *parent) 
    : IgSimpleTwig (parent, "Barrel HCAL", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (BarrelHCAL *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *copper = new SoMaterial;
    copper->ambientColor.setValue (0.007, 0.002, 0.000);
    copper->diffuseColor.setValue (0.414, 0.074, 0.000);
    copper->specularColor.setValue(1.000, 0.767, 0.000);
    copper->emissiveColor.setValue(0.500, 0.500, 0.500);
    copper->shininess = 0.048;
    copper->transparency = 0.0;

    // define barrel profile (i.e. rmin and rmax values) stepping along in z 
    int ndiv_b                 = 18;        // azimuthal divisions
    int nzee_b                 = 4;         // z divisions 
    float phi_b                = 0;         // starting angle
    float del_phi_b            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_b(nzee_b);
    std::vector<float> rmin_b (nzee_b);
    std::vector<float> rmax_b (nzee_b);
    
    zvals_b [0] = -5.000F;
    rmin_b  [0] =  2.600F;
    rmax_b  [0] =  2.850F;

    zvals_b [1] = -3.760F;
    rmin_b  [1] =  1.840F;
    rmax_b  [1] =  rmax_b[0];

    zvals_b [2] = -zvals_b[1];
    rmin_b  [2] =  rmin_b [1];
    rmax_b  [2] =  rmax_b [1];

    zvals_b [3] = -zvals_b[0] ;
    rmin_b  [3] =  rmin_b [0];
    rmax_b  [3] =  rmax_b [0];

    IgSoRotSolid *barrelHcal = new IgSoRotSolid;
    barrelHcal->makePgon (zvals_b, rmin_b, rmax_b, phi_b, del_phi_b, ndiv_b);
    
    rep->node ()->addChild (copper);
    rep->node ()->addChild (barrelHcal);
}

//////////////////////////////////////////////////////////////////////////////////////////
EndcapHCALZPlus::EndcapHCALZPlus (IgTwig *parent) 
    : IgSimpleTwig (parent, "Endcap HCAL (+z)", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (EndcapHCALZPlus *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *copper = new SoMaterial;
    copper->ambientColor.setValue (0.007, 0.002, 0.000);
    copper->diffuseColor.setValue (0.414, 0.074, 0.000);
    copper->specularColor.setValue(1.000, 0.767, 0.000);
    copper->emissiveColor.setValue(0.500, 0.500, 0.500);
    copper->shininess = 0.048;
    copper->transparency = 0.0;

    // define endcap profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_e                 = 18;        // azimuthal divisions
    int nzee_e                 = 3;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    std::vector<float> rmin_b (2);
    std::vector<float> zvals_b(2);
    
    //rmin_b,zvals_b value from BarrelECAL
    rmin_b[0] = 2.600F;
    rmin_b[1] = rmin_b[0];
    zvals_b[0] = -5.000F;
    zvals_b[1] = -3.760F;
    
    zvals_e [0] = -5.600F;
    rmin_e  [0] =  0.500F;
    rmax_e  [0] =  rmin_b[0] - 0.1;

    zvals_e [1] =  zvals_b[0] - 0.1;        // close to barrel with small gap for e.g. cables
    rmin_e  [1] =  0.400F;
    rmax_e  [1] =  rmin_b [0] - 0.1;        // close to barrel with small gap for e.g. cables

    zvals_e [2] =  zvals_b[1] - 0.1;        // close to barrel with small gap for e.g. cables
    rmin_e  [2] =  0.300F;
    rmax_e  [2] =  rmin_b [1] - 0.1;        // close to barrel with small gap for e.g. cables

    // Endcap on +z side      
    for (int i=0; i<nzee_e; i++) 
    {	
	zvals_e [i] = - zvals_e [i];
    }

    // now, re-order in increasing z (otherwise normals of PGON are wrong, i.e. inside out )
    std::vector<float> zvals_eB(nzee_e);
    std::vector<float> rmin_eB (nzee_e);
    std::vector<float> rmax_eB (nzee_e);

    for (int i=0; i<nzee_e; i++) 
    {
	zvals_eB [i] = zvals_e [nzee_e - 1 - i];
	rmin_eB  [i] = rmin_e  [nzee_e - 1 - i];
	rmax_eB  [i] = rmax_e  [nzee_e - 1 - i];
    }
    
    IgSoRotSolid *endcapHcalB = new IgSoRotSolid;
    endcapHcalB->makePgon (zvals_eB, rmin_eB, rmax_eB, phi_e, del_phi_e, ndiv_e);
    
    rep->node ()->addChild (copper);
    rep->node ()->addChild (endcapHcalB);
}

//////////////////////////////////////////////////////////////////////////////////////////
EndcapHCALZMinus::EndcapHCALZMinus (IgTwig *parent) 
    : IgSimpleTwig (parent, "Endcap HCAL (-z)", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (EndcapHCALZMinus *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *copper = new SoMaterial;
    copper->ambientColor.setValue (0.007, 0.002, 0.000);
    copper->diffuseColor.setValue (0.414, 0.074, 0.000);
    copper->specularColor.setValue(1.000, 0.767, 0.000);
    copper->emissiveColor.setValue(0.500, 0.500, 0.500);
    copper->shininess = 0.048;
    copper->transparency = 0.0;

    // define endcap profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_e                 = 18;        // azimuthal divisions
    int nzee_e                 = 3;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    std::vector<float> rmin_b (2);
    std::vector<float> zvals_b(2);
    
    //rmin_b,zvals_b value from BarrelECAL
    rmin_b[0] = 2.600F;
    rmin_b[1] = rmin_b[0];
    zvals_b[0] = -5.000F;
    zvals_b[1] = -3.760F;
    
    zvals_e [0] = -5.600F;
    rmin_e  [0] =  0.500F;
    rmax_e  [0] =  rmin_b [0] - 0.1;

    zvals_e [1] =  zvals_b[0] - 0.1;        // close to barrel with small gap for e.g. cables
    rmin_e  [1] =  0.400F;
    rmax_e  [1] =  rmin_b [0] - 0.1;        // close to barrel with small gap for e.g. cables

    zvals_e [2] =  zvals_b[1] - 0.1;        // close to barrel with small gap for e.g. cables
    rmin_e  [2] =  0.300F;
    rmax_e  [2] =  rmin_b [1] - 0.1;        // close to barrel with small gap for e.g. cables

    // Endcap on -z side  
    IgSoRotSolid *endcapHcalA = new IgSoRotSolid;
    endcapHcalA->makePgon (zvals_e, rmin_e, rmax_e, phi_e, del_phi_e, ndiv_e);
    
    rep->node ()->addChild (copper);
    rep->node ()->addChild (endcapHcalA);
}

//////////////////////////////////////////////////////////////////////////////////////////
ForwardCalorimeter::ForwardCalorimeter (IgTwig *parent) 
    : IgSimpleTwig (parent, "Forward Calorimeters", true, true, true)
/**
 *
 *    ForwardCalorimeter.  Some crude segments.  Roughly based on Figs 1.2 and 1.3 of CMS Technical Proposal 
 *
 */
{
    new ForwardCalorimeterZPlus (this);
    new ForwardCalorimeterZMinus (this);
}


//////////////////////////////////////////////////////////////////////////////////////////
ForwardCalorimeterZMinus::ForwardCalorimeterZMinus (IgTwig *parent) 
    : IgSimpleTwig (parent, "Forward Calorimeter (-z)", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (ForwardCalorimeterZMinus *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *forwardMaterial = new SoMaterial;
    forwardMaterial->ambientColor.setValue (0.007, 0.002, 0.800);
    forwardMaterial->diffuseColor.setValue (0.214, 0.074, 0.800);
    forwardMaterial->specularColor.setValue(0.200, 0.767, 0.800);
    forwardMaterial->emissiveColor.setValue(0.500, 0.500, 0.800);
    forwardMaterial->shininess = 0.5;
    forwardMaterial->transparency = 0.0;

    // define endcap profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_e                 = 8;         // azimuthal divisions
    int nzee_e                 = 2;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    
    zvals_e [0] = -14.100F;
    rmin_e  [0] =  0.2000F;
    rmax_e  [0] =  1.5100F;

    zvals_e [1] = -11.100F;        // close to barrel with small gap for e.g. cables
    rmin_e  [1] =  rmin_e [0];
    rmax_e  [1] =  rmax_e [0];        // close to barrel with small gap for e.g. cables

    // Endcap on -z side  
    
    IgSoRotSolid *solid = new IgSoRotSolid;
    solid->makePgon (zvals_e, rmin_e, rmax_e, phi_e, del_phi_e, ndiv_e);
    
    rep->node ()->addChild (forwardMaterial);
    rep->node ()->addChild (solid);
}

//////////////////////////////////////////////////////////////////////////////////////////
ForwardCalorimeterZPlus::ForwardCalorimeterZPlus (IgTwig *parent) 
    : IgSimpleTwig (parent, "Forward Calorimeter (+z)", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (ForwardCalorimeterZPlus *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoMaterial *forwardMaterial = new SoMaterial;
    forwardMaterial->ambientColor.setValue (0.007, 0.002, 0.800);
    forwardMaterial->diffuseColor.setValue (0.214, 0.074, 0.800);
    forwardMaterial->specularColor.setValue(0.200, 0.767, 0.800);
    forwardMaterial->emissiveColor.setValue(0.500, 0.500, 0.800);
    forwardMaterial->shininess = 0.5;
    forwardMaterial->transparency = 0.0;

    // define endcap profile (i.e. rmin and rmax values) stepping along in z     
    int ndiv_e                 = 8;         // azimuthal divisions
    int nzee_e                 = 2;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    
    zvals_e [0] = -14.100F;
    rmin_e  [0] =  0.2000F;
    rmax_e  [0] =  1.5100F;

    zvals_e [1] = -11.100F;        // close to barrel with small gap for e.g. cables
    rmin_e  [1] =  rmin_e [0];
    rmax_e  [1] =  rmax_e [0];        // close to barrel with small gap for e.g. cables

    // Endcap on +z side      
    for (int i=0; i<nzee_e; i++) 
    {	
	zvals_e [i] = - zvals_e [i];
    }

    // now, re-order in increasing z (otherwise normals of PGON are wrong, i.e. inside out )
    std::vector<float> zvals_eB(nzee_e);
    std::vector<float> rmin_eB (nzee_e);
    std::vector<float> rmax_eB (nzee_e);

    for (int i = 0; i < nzee_e; i++) 
    {
	zvals_eB [i] = zvals_e [nzee_e - 1 - i];
	rmin_eB  [i] = rmin_e  [nzee_e - 1 - i];
	rmax_eB  [i] = rmax_e  [nzee_e - 1 - i];
    }
    
    IgSoRotSolid *solid = new IgSoRotSolid;
    solid->makePgon (zvals_eB, rmin_eB, rmax_eB, phi_e, del_phi_e, ndiv_e);
    
    rep->node ()->addChild (forwardMaterial);
    rep->node ()->addChild (solid);
}

//////////////////////////////////////////////////////////////////////////////////////////
MuonChambers::MuonChambers (IgTwig *parent) 
    : IgSimpleTwig (parent, "Muon Chambers", true, true, true)
/**
 *
 *   A bunch of boxes.
 *
 */
{
    new BarrelChambers (this);
    new EndcapChambers (this);
}


//////////////////////////////////////////////////////////////////////////////////////////
BarrelChambers::BarrelChambers (IgTwig *parent) 
    : IgSimpleTwig (parent, "Barrel Chambers", true, true, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (BarrelChambers *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    // Define materials    
    SoMaterial *muChamber = new SoMaterial;
    muChamber->ambientColor.setValue (0.000, 0.000, 0.000);
    muChamber->diffuseColor.setValue (0.000, 0.000, 0.000);
    muChamber->specularColor.setValue(0.622, 0.622, 0.622);
    muChamber->emissiveColor.setValue(1.000, 0.000, 0.000);
    muChamber->shininess = 0.121;
    muChamber->transparency = 0.0;
         
    // Generic parameters defining a wheel of boxes
    int   n_r, n_z = 0;
    float boxHeight, boxWidth, boxDepth, r_min, z_ave, r_gap, z_gap, phi_overlap = 0.F;
    
    // Create representations for barrel muon chambers     
    n_r          = 2;
    n_z          = 5;
    boxHeight    = 0.350F;
    boxWidth     = 0.800F;        
    boxDepth     = 2.500F;
    r_min        = 3.900F;        
    z_ave        = 0.000F;        
    r_gap        = 0.450F;        
    z_gap        = 0.200F;
    phi_overlap  = -0.05F;
	
    IgGDWheelsOfBoxes *innerChambers = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
							      r_min, z_ave, r_gap, z_gap, phi_overlap);
    
    rep->node ()->addChild (muChamber);
    rep->node ()->addChild (innerChambers);
        
    n_r          = 2;
    n_z          = 5;
    boxHeight    = 0.350F;
    boxWidth     = 1.400F;        
    boxDepth     = 2.500F;
    r_min        = 5.640F ;        
    z_ave        = 0.000F;        
    r_gap        = 1.500F;        
    z_gap        = 0.200F;
    phi_overlap  = -0.05F;
	    
    IgGDWheelsOfBoxes *barrelAbsorber = new IgGDWheelsOfBoxes (n_r, n_z, boxHeight, boxWidth, boxDepth,
							       r_min, z_ave, r_gap, z_gap, phi_overlap);
    
    rep->node ()->addChild (muChamber);
    rep->node ()->addChild (barrelAbsorber);
}

//////////////////////////////////////////////////////////////////////////////////////////
EndcapChambers::EndcapChambers(IgTwig *parent) 
    : IgSimpleTwig (parent, "Endcap Chambers (to do!)", true, true, true)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
IgGDWheelsOfBoxes::IgGDWheelsOfBoxes(
                   int   n_r,
 	           int   n_z,
	           float boxHeight,                
	           float boxWidth,             
	           float boxDepth,                
                   float r_min,
		   float z_ave,
		   float r_gap,
                   float z_gap,
		   float phi_overlap)
{
    SoCube *box = new SoCube;
    box->height = boxHeight;
    box->width  = boxWidth;
    box->depth  = boxDepth;
    	
    SbVec3f z_axis (0.0F, 0.0F, 1.0F);

    SoSeparator *wheel = new SoSeparator;

    // First create a wheel, i.e. all radial layers but a single wafer extent in z
	
    for (int i_r = 0; i_r < n_r; i_r++)
    {
	float r = r_min + i_r * (r_gap + boxHeight);
	int nSegments = (int)((1.0 + phi_overlap) * (2.0 * M_PI ) / (boxWidth / ((r == 0) ? 0.001 : r)));         // no. wafers in azimuth (small angle approx.)
	    			
	SoTransform *localTransform = new SoTransform;
	localTransform->rotation.setValue (SbRotation (z_axis, 2 * M_PI / ((nSegments == 0) ? 1 : nSegments)));  // rotate wafer locally
	localTransform->translation.setValue (SbVec3f (0.0F, r, 0.0F));                // translate to appropriate radius
	    
	SoSeparator *basicBox= new SoSeparator;
	basicBox->addChild (localTransform);
	basicBox->addChild (box);

	SoRotation  *wedgeRotate  = new SoRotation;
	wedgeRotate->rotation.setValue (SbRotation (z_axis, 2 * M_PI / ((nSegments == 0) ? 1 : nSegments)));     // rotate in steps from zero to 0->2PI

	for (int i = 0; i < nSegments; i++)
	{
	    wheel->addChild (basicBox);
	    wheel->addChild (wedgeRotate);
	}
    }

    // now take the basic wheel and replicate along the z-axis
   
    for (int i_z = 0; i_z < n_z; i_z++)
    {	
	float z_min  = z_ave - 0.5 * (n_z-1) * (boxDepth + z_gap);  // Aside:  min z is middle of detector not edge  
	float z_step = boxDepth + z_gap;

	SoTranslation *wheelStep = new SoTranslation;
	wheelStep->translation.setValue (SbVec3f (0.0F, 0.0F, z_min + i_z * z_step));
	    
	SoSeparator *temp = new SoSeparator;  // ensures the translation is "scoped"   
	temp->addChild (wheelStep); 
	temp->addChild (wheel);
	this->addChild (temp);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
static const double WORLD_RADIUS = 10.;
static const double FIELD_SIZE	 = 7.;
static const double FIELD_MAX	 = 3;

class TestField : public IgSbField
{
public:
    virtual void evaluate (const double pt [3], double field []) const {
	// sqrt in distance from origin
	double d = sqrt (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2]);
	if (d > FIELD_SIZE)
	    field [0] = field [1] = 0;
	else
	    field [0] = field [1] = FIELD_MAX * sqrt (1 - d / FIELD_SIZE);

	field [2] = 0;
    }
};

MagneticField::MagneticField (IgTwig *parent, IgState *state) 
    : IgSimpleTwig ( parent, "Magnetic Field", true, false, true),
    m_centre (IgStudioCentre::get (state)->controlCentre ()),
    m_fieldPlanesCategory (0),
    m_plane (new IgSoFieldPlane)
{
    m_fieldPlanesCategory = Ig3DFieldPlanesCategory::get (state);
    if (!m_fieldPlanesCategory )
      m_fieldPlanesCategory = new Ig3DFieldPlanesCategory(state);
    
    //m_centre->addCategory (m_fieldPlanesCategory);
    m_fieldPlanesCategory->registerMe (state);
    // Get the menu as it has already been initialised in the
    // baseClass.
    QPopupMenu *menu = IgQtAppMenuService::get (state)->subMenu
		       (Ig3DBaseBrowser::MENU_ID_3D);

    menu->insertItem
        ("&Field Planes...", this, SLOT (showFieldPlanes ()), 0,
         Ig3DBaseBrowser::MENU_ID_3D_FIELDPLANES, IgQtAppMenuService::index
         (menu, Ig3DBaseBrowser::MENU_ID_3D_FIELDPLANES));
    
    SbBox3f world (-WORLD_RADIUS, -WORLD_RADIUS, -WORLD_RADIUS,
		   WORLD_RADIUS,   WORLD_RADIUS,  WORLD_RADIUS);
    m_plane->colorMap (&IgSbColorMap::jet);
    m_plane->field (new TestField, world);
    m_plane->ref();
    m_fieldPlanesCategory->addOne (m_plane);
}

MagneticField::~MagneticField ()
{ m_plane->unref(); }

void
MagneticField::showFieldPlanes (void)
{
    m_centre->selectCategory (m_fieldPlanesCategory);
    m_centre->show ();
}

IgSoFieldPlane*
MagneticField::getFieldPlane ()
{ return m_plane; }

MMM_DEFUN_FUNC (void,IgBrowserMethods::,doUpdate,
	       (MagneticField *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    rep->node ()->addChild (twig->getFieldPlane ());
}
//////////////////////////////////////////////////////////////////////////////////////////
testCube::testCube (IgTwig *parent) 
    : IgSimpleTwig (parent, "Test Cube", true, false, true)
{
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
	       (testCube *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    // Define materials
    SoMaterial *material = new SoMaterial;
    material->ambientColor.setValue (0.000, 0.000, 0.000);
    material->diffuseColor.setValue (0.000, 0.000, 0.000);
    material->specularColor.setValue(0.622, 0.622, 0.622);
    material->emissiveColor.setValue(1.000, 0.000, 0.000);
    material->shininess = 0.121;
    material->transparency = 0.0;

    rep->node ()->addChild (material);
    rep->node ()->addChild (new SoCube);
}

///////////////////////////////////////////////////////////////////////////////////////////
Event::Event (IgTwig *parent, IgState *state)
    : IgSimpleTwig (parent, "Event", true, false, true)
{
    ASSERT (state);
}

MMM_DEFUN_FUNC (void, IgBrowserMethods::, doUpdate,
		(Event *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
	return;

    SoSeparator *event = new SoSeparator;

    SoMaterial *material = new SoMaterial;
    material->ambientColor.setValue (0.000, 1.000, 0.000);
    material->diffuseColor.setValue (0.000, 1.000, 0.000);
    material->specularColor.setValue(0.000, 1.000, 0.000);
    material->emissiveColor.setValue(0.000, 1.000, 0.000);
    material->shininess = 0.0;
    material->transparency = 0.0;

    event->addChild (material);

    IgSoSimpleTrajectory *track1 = new IgSoSimpleTrajectory;
    IgSoSimpleTrajectory *track2 = new IgSoSimpleTrajectory;

    event->addChild (track1);
    event->addChild (track2);

    SbVec3f points [] = { SbVec3f (  6.25063e-06, 2.0411e-05, -0.0670882),
			  SbVec3f (  6.25063e-06, 2.0411e-05, -0.0670882),
			  SbVec3f ( -0.0127438,  -0.0417177,  -0.0753631),
			  SbVec3f ( -0.0198873,  -0.0663409,  -0.0802327),
			  SbVec3f ( -0.0310742,  -0.106972,   -0.0882219),
			  SbVec3f ( -0.0598742,  -0.226498,   -0.111566),
			  SbVec3f ( -0.0603472,  -0.228691,   -0.111991),
			  SbVec3f ( -0.0828133,  -0.346818,   -0.134919),
			  SbVec3f ( -0.0831764,  -0.349008,   -0.135344),
			  SbVec3f ( -0.0906693,  -0.396957,   -0.144629),
			  SbVec3f ( -0.103941,   -0.502679,   -0.165019),
			  SbVec3f ( -0.111375,   -0.586937,   -0.181154),
			  SbVec3f ( -0.111662,   -0.590975,   -0.181926),
			  SbVec3f ( -0.116895,   -0.695699,   -0.201933),
			  SbVec3f ( -0.117008,   -0.699719,   -0.202702)
    };
    
    track1->controlPoints.setValues (0, 14, points);    
    
    SbVec3f markerPoints [] =  { SbVec3f ( -0.0127438, -0.0417177, -0.0753631),
				 SbVec3f ( -0.0198873, -0.0663409, -0.0802327),
				 SbVec3f ( -0.0310742, -0.106972,  -0.0882219),
				 SbVec3f ( -0.0598742, -0.226498,  -0.111566),
				 SbVec3f ( -0.0603472, -0.228691,  -0.111991),
				 SbVec3f ( -0.0828133, -0.346818,  -0.134919),
				 SbVec3f ( -0.0831764, -0.349008,  -0.135344),
				 SbVec3f ( -0.0906693, -0.396957,  -0.144629),
				 SbVec3f ( -0.103941,  -0.502679,  -0.165019),
				 SbVec3f ( -0.111375,  -0.586937,  -0.181154),
				 SbVec3f ( -0.111662,  -0.590975,  -0.181926),
				 SbVec3f ( -0.116895,  -0.695699,  -0.201933),
				 SbVec3f ( -0.117008,  -0.699719,  -0.202702)
    };

    track1->markerPoints.setValues (0, 12, markerPoints);
    track1->lineWidth = 2;
    track1->pointSize = 3;
    
    SbVec3f points2 [] = { SbVec3f ( 6.25063e-06, 2.0411e-05, -0.0670882),
			   SbVec3f ( 6.25063e-06, 2.0411e-05, -0.0670882),
			   SbVec3f (-0.0327883,   0.0293515,  -0.0406104),
			   SbVec3f (-0.0510188,   0.0464954,  -0.0255492),
			   SbVec3f (-0.0787771,   0.0739177,  -0.00191846),
			   SbVec3f (-0.181202,    0.190784,    0.0922615),
			   SbVec3f (-0.182591,    0.192579,    0.0936367),
			   SbVec3f (-0.236811,    0.26815,     0.149988),
			   SbVec3f (-0.238078,    0.27006,     0.15138),
			   SbVec3f (-0.264752,    0.312075,    0.181596),
			   SbVec3f (-0.302599,    0.37856,     0.228102),
			   SbVec3f (-0.361059,    0.503801,    0.311958),
			   SbVec3f (-0.362621,    0.507672,    0.314486),
			   SbVec3f (-0.38129,     0.55698,     0.346412),
			   SbVec3f (-0.40911,     0.644842,    0.402114),
			   SbVec3f (-0.432223,    0.742349,    0.462494),
			   SbVec3f (-0.446919,    0.834323,    0.518623),
			   SbVec3f (-0.456205,    0.95822,     0.593376)
    };
			  
    track2->controlPoints.setValues (0, 17, points2);
    
    SbVec3f markerPoints2 [] = { SbVec3f (-0.0327883, 0.0293515, -0.0406104),
				 SbVec3f (-0.0510188, 0.0464954, -0.0255492),
				 SbVec3f (-0.0787771, 0.0739177, -0.00191846),
				 SbVec3f (-0.181202,  0.190784,   0.0922615),
				 SbVec3f (-0.182591,  0.192579,   0.0936367),
				 SbVec3f (-0.236811,  0.26815,    0.149988),
				 SbVec3f (-0.238078,  0.27006,    0.15138),
				 SbVec3f (-0.264752,  0.312075,   0.181596),
				 SbVec3f (-0.302599,  0.37856,    0.228102),
				 SbVec3f (-0.361059,  0.503801,   0.311958),
				 SbVec3f (-0.362621,  0.507672,   0.314486),
				 SbVec3f (-0.38129,   0.55698,    0.346412),
				 SbVec3f (-0.40911,   0.644842,   0.402114),
				 SbVec3f (-0.432223,  0.742349,   0.462494),
				 SbVec3f (-0.446919,  0.834323,   0.518623),
				 SbVec3f (-0.456205,  0.95822,    0.593376)
    };
    
    track2->markerPoints.setValues (0, 15, markerPoints2);
    track2->lineWidth = 2;
    track2->pointSize = 3;
  
    rep->node ()->addChild (event);
}

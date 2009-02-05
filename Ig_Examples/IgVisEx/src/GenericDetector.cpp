//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Ig_Examples/IgVisEx/interface/GenericDetector.h"

#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include <cassert>
#include "qobject.h"

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
#include "Inventor/nodes/SoComplexity.h"

// Our own Inventor shapes must be initialised
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPcon.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoRotSolid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCoordinateAxis.h"



Detector::Detector() : IgQtTwig("Detector")
/**
 *
 *    Top-level detector ("container") twig.
 *    This contains ("container") sub-twigs for subdetectors which in
 *    turn contain the actual barrel and endcap elements.
 *
 */
{
	addChild(new Infrastructure);
	addChild(new Tracker);
	addChild(new Ecal);
	addChild(new Hcal);
	addChild(new ForwardCalorimeter);
	addChild(new MuonChambers);
}

Detector::~Detector()
{
}

void Detector::drawMe()
{
	// since "Detector" is just a placeholder, we just let the attached
	// twigs redraw themselves
}





//////////////////////////////////////////////////////////////////////////////////////////

Infrastructure::Infrastructure() : IgQtTwig("Infrastructure")
/**
 *
 *    Infrastructure (some of the large non-sensitive stuff)
 *
 */
{
  // Co-ordinate axes (mostly for debug)
  IgSoCoordinateAxis *axes = new IgSoCoordinateAxis;
  IgQtTwig *axis_twig = new IgQtTwig("Coord Axes");
  axis_twig->addIvNode(axes);
  addChild(axis_twig);
 
  // Add a complexity node so that the cylinders don't look polygonal
  SoComplexity* theComplexity = new SoComplexity;
  theComplexity->type = SoComplexity::OBJECT_SPACE;
  theComplexity->value = 0.9;
  addIvNode(theComplexity);

  // Units
  SoUnits *theUnits = new SoUnits;
  theUnits->units = SoUnits::METERS; //Meters are the default; CENTIMETERS is a common alternative
  addIvNode(theUnits);
 
  SoShapeHints *hints = new SoShapeHints;
  hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  hints->shapeType = SoShapeHints::SOLID;
  hints->faceType = SoShapeHints::CONVEX;


  SoMaterial *beryllium = new SoMaterial;
  beryllium->ambientColor.setValue (0.007, 0.002, 0.000);
  beryllium->diffuseColor.setValue (0.314, 0.074, 0.000);
  beryllium->specularColor.setValue(1.000, 0.767, 0.000);
  beryllium->emissiveColor.setValue(0.314, 0.074, 0.000);
  beryllium->shininess = 0.048;
  beryllium->transparency = 0.0;

  SoMaterial *greySteel = new SoMaterial;
  greySteel->ambientColor.setValue (0.039, 0.038, 0.038);
  greySteel->diffuseColor.setValue (0.155, 0.150, 0.151);
  greySteel->specularColor.setValue(0.528, 0.524, 0.524);
  greySteel->emissiveColor.setValue(0.314, 0.074, 0.000);
  greySteel->shininess = 0.3;
  greySteel->transparency = 0.0;   
    

  
  // beam pipe
  IgSoPcon *pipe_rep = new IgSoPcon; 
  float rmin[6], rmax[6], z[6];
  float deltaAngle    = 2*M_PI;
  double z_half       =15.0F;    // rough guess at canonical beampipe
  double z_close      = 2.0F;    // rough guess at canonical beampipe
  double radius_large = 0.150F;  // rough guess at canonical beampipe
  double radius_small = 0.10F;   // rough guess at canonical beampipe
  float thickness     = 0.001F;  // rough guess at canonical beampipe
  z[0] = -z_half;                // far from centre...typically fat
  z[1] = -z_close;               // now starts to taper down 
  z[2] =  z[1] + 1.0F;           // has now finished tapering down to become narrow
  z[3] = -z[2];
  z[4] = -z[1];
  z[5] = -z[0];
  rmin[1] = rmin[0] = radius_large;
  rmin[3] = rmin[2] = radius_small;
  rmin[5] = rmin[4] = rmin[0];  
  rmax[1] = rmax[0] = rmin[0] + thickness;
  rmax[3] = rmax[2] = rmin[2] + thickness;
  rmax[5] = rmax[4] = rmax[0];
 
  pipe_rep->initialise(6,0,deltaAngle,z,rmin,rmax);

  IgQtTwig *pipe_twig = new IgQtTwig("Beam Pipe");
  pipe_twig->addIvNode(beryllium);
  pipe_twig->addIvNode(pipe_rep);
  addChild(pipe_twig);

  
  // Support (a gratuitous tube between HCAL and coil)
  IgSoPcon *supportTube = new IgSoPcon;
  rmin[1] = rmin[0] = 2.950F;                                
  rmax[1] = rmax[0] = 2.970F;                               
  z[0]              = -6.50F;                              
  z[1]              = -z[0];
  supportTube->initialise(2,0,deltaAngle,z,rmin,rmax);
    
  IgQtTwig *supportTube_twig = new IgQtTwig("Support Tube");
  supportTube_twig->addIvNode(greySteel);
  supportTube_twig->addIvNode(supportTube);
  addChild(supportTube_twig);
  
  
  // solenoid
  IgSoPcon *solenoid_rep = new IgSoPcon;
  rmin[1] = rmin[0] =  3.10F;                                 // roughly CMS
  rmax[1] = rmax[0] =  3.40F;                                 // roughly CMS
  z[0]              = -6.50F;                                 // roughly CMS
  z[1]              = -z[0];
  solenoid_rep->initialise(2,0,deltaAngle,z,rmin,rmax);
        
  IgQtTwig *solenoid_twig = new IgQtTwig("Solenoid");
  solenoid_twig->addIvNode(greySteel);
  solenoid_twig->addIvNode(solenoid_rep);
  addChild(solenoid_twig);


}

Infrastructure::~Infrastructure()
{
}

void Infrastructure::drawMe()
{
}




//////////////////////////////////////////////////////////////////////////////////////////
Tracker::Tracker() : IgQtTwig("Tracker")
/**
 *
 *    Tracker  (boxes representing e.g. silicon wafers, or MSGC's)
 *        Barrel:   cylindrically symmetric wheels of boxes
 *        Endcaps:  turbine-blades of boxes (a bit crude)
 */
{
    // Organise the tracker twig into four distinct sub-twigs 
    
    IgQtTwig * innerBarrelTwig  = new IgQtTwig("Inner Barrel"); 
    IgQtTwig * outerBarrelTwig  = new IgQtTwig("Outer Barrel"); 
    IgQtTwig * innerForwardTwig = new IgQtTwig("Inner Forward");
    IgQtTwig * outerForwardTwig = new IgQtTwig("Outer Forward");

 
    // Define materials
    
    SoMaterial *silicon = new SoMaterial;
    silicon->ambientColor.setValue (0.007, 0.002, 0.000);
    silicon->diffuseColor.setValue (0.314, 0.074, 0.000);
    silicon->specularColor.setValue(1.000, 0.767, 0.000);
    silicon->emissiveColor.setValue(0.867, 0.867, 0.337);
    silicon->shininess = 0.048;
    silicon->transparency = 0.0;

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
	
    wheelsOfBoxes *innerBarrel = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
    innerBarrelTwig->addIvNode (silicon);
    innerBarrelTwig->addIvNode (innerBarrel);
   
        
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
	    
    wheelsOfBoxes *outerBarrel = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
    outerBarrelTwig->addIvNode (aluminium);
    outerBarrelTwig->addIvNode (outerBarrel);
   
          
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
	
    wheelsOfBoxes *innerA = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
    z_ave = -z_ave;
    wheelsOfBoxes *innerB = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);    
    innerForwardTwig->addIvNode (silicon);
    innerForwardTwig->addIvNode (innerA);
    innerForwardTwig->addIvNode (innerB);

             
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
	
    wheelsOfBoxes *outerA = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
    z_ave = -z_ave;
    wheelsOfBoxes *outerB = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
        
    outerForwardTwig->addIvNode (aluminium);
    outerForwardTwig->addIvNode (outerA);
    outerForwardTwig->addIvNode (outerB);
    
    
    // Add all the sub-twigs defined above to the tree
    
    addChild(innerBarrelTwig);
    addChild(outerBarrelTwig);
    addChild(innerForwardTwig);
    addChild(outerForwardTwig);

}
Tracker::~Tracker()
{
}
void Tracker::drawMe()
{
}







//////////////////////////////////////////////////////////////////////////////////////////
Ecal::Ecal() : IgQtTwig("Ecal")
/**
 *
 *    Ecal: Rough baskets of crystals.
*           Roughly based on Figs 1.2 and 1.3 of CMS Technical Proposal 
  *            
 */
{

    // Organise the Ecal twig into distinct sub-twigs 
    
    IgQtTwig * barrelEcalTwig  = new IgQtTwig("Barrel ECAL"); 
    IgQtTwig * endcapEcalATwig = new IgQtTwig("Endcap ECAL (-z)"); 
    IgQtTwig * endcapEcalBTwig = new IgQtTwig("Endcap ECAL (+z)"); 

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

    zvals_b [3] = -zvals_b[0] ;
    rmin_b  [3] =  rmin_b [0];
    rmax_b  [3] =  rmax_b [0];

    IgSoRotSolid * barrelEcal = new IgSoRotSolid;
    barrelEcal->makePgon(zvals_b, rmin_b, rmax_b, phi_b, del_phi_b, ndiv_b);
    
    barrelEcalTwig->addIvNode (crystal);
    barrelEcalTwig->addIvNode (barrelEcal);        
    addChild(barrelEcalTwig);


    // define endcap profile (i.e. rmin and rmax values) stepping along in z 
    
    int ndiv_e                 = 18;        // azimuthal divisions
    int nzee_e                 = 2;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    
    zvals_e [0] = -3.760F;
    rmin_e  [0] =  0.300F;
    rmax_e  [0] =  rmin_b [0] - 0.1;

    zvals_e [1] = -3.250F;      
    rmin_e  [1] =  0.250F;
    rmax_e  [1] =  1.280F;      

    // Endcap on -z side  
    IgSoRotSolid * endcapEcalA = new IgSoRotSolid;
    endcapEcalA->makePgon(zvals_e, rmin_e, rmax_e, phi_e, del_phi_e, ndiv_e);    
    endcapEcalATwig->addIvNode (crystal);
    endcapEcalATwig->addIvNode (endcapEcalA);        
    addChild(endcapEcalATwig);


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
    
    IgSoRotSolid * endcapEcalB = new IgSoRotSolid;
    endcapEcalB->makePgon(zvals_eB, rmin_eB, rmax_eB, phi_e, del_phi_e, ndiv_e);    
    endcapEcalBTwig->addIvNode (crystal);
    endcapEcalBTwig->addIvNode (endcapEcalB);        
    addChild(endcapEcalBTwig);
    
}
Hcal::~Hcal()
{
}
void Hcal::drawMe()
{
}




//////////////////////////////////////////////////////////////////////////////////////////
Hcal::Hcal() : IgQtTwig("Hcal")
/**
 *
 *    Hcal.  Some crude segments.
 *           Roughly based on Figs 1.2 and 1.3 of CMS Technical Proposal 
 *
 */
{

    // Organise the HCAL twig into distinct sub-twigs 
    
    IgQtTwig * barrelHcalTwig  = new IgQtTwig("Barrel HCAL"); 
    IgQtTwig * endcapHcalATwig = new IgQtTwig("Endcap HCAL (-z)"); 
    IgQtTwig * endcapHcalBTwig = new IgQtTwig("Endcap HCAL (+z)"); 

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

    IgSoRotSolid * barrelHcal = new IgSoRotSolid;
    barrelHcal->makePgon(zvals_b, rmin_b, rmax_b, phi_b, del_phi_b, ndiv_b);
    
    barrelHcalTwig->addIvNode (copper);
    barrelHcalTwig->addIvNode (barrelHcal);        
    addChild(barrelHcalTwig);


    // define endcap profile (i.e. rmin and rmax values) stepping along in z 
    
    int ndiv_e                 = 18;        // azimuthal divisions
    int nzee_e                 = 3;         // z divisions 
    float phi_e                = 0;         // starting angle
    float del_phi_e            = 2.*M_PI;   // total phi range    
    std::vector<float> zvals_e(nzee_e);
    std::vector<float> rmin_e (nzee_e);
    std::vector<float> rmax_e (nzee_e);
    
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
    IgSoRotSolid * endcapHcalA = new IgSoRotSolid;
    endcapHcalA->makePgon(zvals_e, rmin_e, rmax_e, phi_e, del_phi_e, ndiv_e);    
    endcapHcalATwig->addIvNode (copper);
    endcapHcalATwig->addIvNode (endcapHcalA);        
    addChild(endcapHcalATwig);


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
    
    IgSoRotSolid * endcapHcalB = new IgSoRotSolid;
    endcapHcalB->makePgon(zvals_eB, rmin_eB, rmax_eB, phi_e, del_phi_e, ndiv_e);    
    endcapHcalBTwig->addIvNode (copper);
    endcapHcalBTwig->addIvNode (endcapHcalB);        
    addChild(endcapHcalBTwig);
    
}
Ecal::~Ecal()
{
}
void Ecal::drawMe()
{
}





//////////////////////////////////////////////////////////////////////////////////////////
ForwardCalorimeter::ForwardCalorimeter() : IgQtTwig("Forward Calorimeters")
/**
 *
 *    ForwardCalorimeter.  Some crude segments.  Roughly based on Figs 1.2 and 1.3 of CMS Technical Proposal 
 *
 */
{

    // Organise the ForwardCalorimeter twig into distinct sub-twigs 
    
    IgQtTwig * ForwardCalorimeterATwig = new IgQtTwig("Forward Calorimeter (-z)"); 
    IgQtTwig * ForwardCalorimeterBTwig = new IgQtTwig("Forward Calorimeter (+z)"); 

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
    IgSoRotSolid * ForwardCalorimeterA = new IgSoRotSolid;
    ForwardCalorimeterA->makePgon(zvals_e, rmin_e, rmax_e, phi_e, del_phi_e, ndiv_e);    
    ForwardCalorimeterATwig->addIvNode (forwardMaterial);
    ForwardCalorimeterATwig->addIvNode (ForwardCalorimeterA);        
    addChild(ForwardCalorimeterATwig);

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
    
    IgSoRotSolid * ForwardCalorimeterB = new IgSoRotSolid;
    ForwardCalorimeterB->makePgon(zvals_eB, rmin_eB, rmax_eB, phi_e, del_phi_e, ndiv_e);    
    ForwardCalorimeterBTwig->addIvNode (forwardMaterial);
    ForwardCalorimeterBTwig->addIvNode (ForwardCalorimeterB);        
    addChild(ForwardCalorimeterBTwig);
    
}
ForwardCalorimeter::~ForwardCalorimeter()
{
}
void ForwardCalorimeter::drawMe()
{
}





//////////////////////////////////////////////////////////////////////////////////////////
MuonChambers::MuonChambers() : IgQtTwig("MuonChambers")
/**
 *
 *   A bunch of boxes.
 *
 */
{
    // Organise the muon detector twig into distinct sub-twigs 
    
    IgQtTwig * barrelChambersTwig  = new IgQtTwig("Barrel Chambers"); 
    IgQtTwig * endcapChambersTwig  = new IgQtTwig("Endcap Chambers (to do!)"); 

 
    // Define materials
    
    SoMaterial *muChamber = new SoMaterial;
    muChamber->ambientColor.setValue (0.000, 0.000, 0.000);
    muChamber->diffuseColor.setValue (0.000, 0.000, 0.000);
    muChamber->specularColor.setValue(0.622, 0.622, 0.622);
    muChamber->emissiveColor.setValue(1.000, 0.000, 0.000);
    muChamber->shininess = 0.121;
    muChamber->transparency = 0.0;
         
    SoMaterial *greySteel = new SoMaterial;
    greySteel->ambientColor.setValue (0.039, 0.038, 0.038);
    greySteel->diffuseColor.setValue (0.155, 0.150, 0.151);
    greySteel->specularColor.setValue(0.528, 0.524, 0.524);
    greySteel->emissiveColor.setValue(0.314, 0.074, 0.000);
    greySteel->shininess = 0.3;
    greySteel->transparency = 0.0;   


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
	
    wheelsOfBoxes *innerChambers = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
    barrelChambersTwig->addIvNode (muChamber);
    barrelChambersTwig->addIvNode (innerChambers);   
        
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
	    
    wheelsOfBoxes *barrelAbsorber = new wheelsOfBoxes(n_r,n_z,boxHeight,boxWidth,boxDepth,r_min,z_ave,r_gap,z_gap,phi_overlap);
    barrelChambersTwig->addIvNode (muChamber);
    barrelChambersTwig->addIvNode (barrelAbsorber);
   
    // Add all the sub-twigs defined above to the tree
    
    addChild(barrelChambersTwig);
    addChild(endcapChambersTwig);
}
MuonChambers::~MuonChambers()
{
}
void MuonChambers::drawMe()
{
}





//////////////////////////////////////////////////////////////////////////////////////////
wheelsOfBoxes::wheelsOfBoxes(
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
    	
    SbVec3f z_axis(0.F,0.F,1.0F);

    SoSeparator *wheel = new SoSeparator;

    // First create a wheel, i.e. all radial layers but a single wafer extent in z
	
    for (int i_r=0; i_r< n_r; i_r++)
	{
	    float r = r_min + i_r * (r_gap + boxHeight);
            int nSegments = int((1.0 + phi_overlap) * (2.*M_PI ) / (boxWidth/r));         // no. wafers in azimuth (small angle approx.)
	    			
            SoTransform *localTransform = new SoTransform;
	    localTransform->rotation.setValue(SbRotation(z_axis,2*M_PI/nSegments));  // rotate wafer locally
            localTransform->translation.setValue(SbVec3f(0.F,r,0.F));                // translate to appropriate radius
	    
	    SoSeparator *basicBox= new SoSeparator;
     	    basicBox->addChild(localTransform);
	    basicBox->addChild(box);

	    SoRotation  *wedgeRotate  = new SoRotation;
	    wedgeRotate->rotation.setValue(SbRotation(z_axis,2*M_PI/nSegments));     // rotate in steps from zero to 0->2PI

	    for (int i=0; i< nSegments; i++)
	    {
		wheel->addChild(basicBox);
		wheel->addChild(wedgeRotate);
	    }
	}

    // now take the basic wheel and replicate along the z-axis
   
    for (int i_z=0; i_z < n_z; i_z++)
        {
            float z_min  = z_ave - 0.5 * (n_z-1) * (boxDepth + z_gap);  // Aside:  min z is middle of detector not edge  
            float z_step = boxDepth + z_gap;

            SoTranslation *wheelStep = new SoTranslation;
            wheelStep->translation.setValue(SbVec3f(0.F,0.F, z_min + i_z * z_step ));
	    
            SoSeparator *temp = new SoSeparator;  // ensures the translation is "scoped"   
	    temp->addChild(wheelStep); 
	    temp->addChild(wheel);
	    addChild(temp);
	}
}





//////////////////////////////////////////////////////////////////////////////////////////
class CalCell : public SoIndexedFaceSet
/** 
 * Use a basic face set to describe each calorimeter cell.
 * This demonstates how to build a quick special-purpose shape
 * from a face set
 * See the documentation on SoIndexedFaceSet for details.
 *
 */
{
public:
	//! must have exactly four sides + top & bottom = 8 vertices
	CalCell(std::vector<SbVec3f>);
protected:
	~CalCell(){};
};
CalCell::CalCell(std::vector<SbVec3f> verts)
{
	const unsigned int nverts = 8;
	const unsigned int nfaces = 6;
	const unsigned int num_indices = nfaces * 5;
	assert ( verts.size() == nverts );
	const unsigned int nper = 4; // number of vertices per face
	std::vector<int> indices(num_indices);
	std::vector<SbVec3f> normals(nfaces);
	SoVertexProperty *vp = new SoVertexProperty;

	// bottom
	for (unsigned int i=0; i < nper; i++) {
		indices[nper - 1 -i] = i;}
	indices[nper] = SO_END_FACE_INDEX;
	normals[0] = (verts[nper-1] - verts[0]).cross(verts[1]-verts[0]); // reverse the order

	// top
	for (unsigned int i=0; i < nper; i++) {
			indices[nper + 1 + i] = nper + i; }
	indices[2*nper + 1] = SO_END_FACE_INDEX;
	normals[1] = (verts[nverts - nper + 1] - verts[nverts - nper]).cross(verts[nverts-1]-verts[nverts - nper]);
	// side faces
	unsigned int ip = 2*nper+2;
	unsigned int iface = 2; 
	unsigned int iv = 0;
	while (iv < nverts - nper) {
		for (unsigned int i = 0; i < nper -1; i++ ) {
			indices[ip++] = iv;
			indices[ip++] = iv + 1;
			indices[ip++] = iv + 1 + nper;
			indices[ip++] = iv + nper;
			indices[ip++] = SO_END_FACE_INDEX;	
		  	normals[iface++] = (verts[iv+1] - verts[iv]).cross(verts[iv+nper]-verts[iv]);
			iv++;}
		// last face wraps vertices back to start
		indices[ip++] = iv;
		indices[ip++] = iv + 1 - nper;
		indices[ip++] = iv + 1;
		indices[ip++] = iv + nper;
		indices[ip++] = SO_END_FACE_INDEX;
		normals[iface++] = (verts[iv+1-nper] - verts[iv]).cross(verts[iv+nper]-verts[iv]);
		iv++;
	}
	std::vector<int> nindices(nfaces); // indices for the normals
	for (unsigned int i=0; i < nfaces; i++) {
		nindices[i] = i; }

	vp->materialBinding.setValue(SoMaterialBinding::OVERALL);
	vp->normalBinding.setValue(SoMaterialBinding::PER_FACE_INDEXED);
	vp->vertex.setValues(0,static_cast<int>(verts.size()),&verts[0]);
	vp->normal.setValues(0,nfaces,&normals[0]);
	this->coordIndex.setValues(0,num_indices,&indices[0]);
	this->normalIndex.setValues(0,nfaces,&nindices[0]);
	this->vertexProperty = vp;
}

























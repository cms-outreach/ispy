//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRectHist.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include "test_main.h"
#include <iostream>
#include <fstream>
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


QString makeTest (SoSeparator *root)
{	

	IgSoShapeKit::initClass ();
	IgSoRectHist::initClass ();

	
/*
	///ECAL 
	float _Rr=1.523;
	float _Rz=2.684;
	float _offsetR=-0.233;
	float _offsetZ=0.486;
	int _nr=120; 
	int _nz=2 * 17 * 6;
	int nOfBins=_nr*2+_nz*2;
 
 
	///HCAL 
	float _Rr=1.930;
	float _Rz=3.675;
	float _offsetR=0;
	float _offsetZ=0.205;
	int _nr=32; 
	int _nz=32;
	int nOfBins=_nr*2+_nz*2;
*/  
	///TEST 
	float _Rr=8;
	float _Rz=10;
	float _offsetR=0;
	float _offsetZ=2;
	int _nr=6; 
	int _nz=10;
	int nOfBins=_nr*2+_nz*2;
    	

	
	std::vector<float> energies (nOfBins);
	
	for (int i = 0; i < nOfBins; i++) {
		energies [i] = 5 * (float) rand () / RAND_MAX;
	}
	
	energies[nOfBins-1]=10;
	energies[nOfBins-2]=-3;
	energies[nOfBins-3]=0;
	
	IgSoRectHist* test = new IgSoRectHist;
	
	test->Rr = _Rr;
	test->Rz = _Rz;
	test->offsetR= _offsetR;
	test->offsetZ = _offsetZ;
	test->energies.setValues (0, energies.size(), &energies [0]);
	test->nr= _nr;
	test->nz= _nz; 
	test->layer = 0.0;
	test->showAnnotations = TRUE;
//	test->logScale = TRUE;    
//	test->scaleFactor = 0.5;
	
	test->setLineColor(1.0,0.,0.);
	
	SoMaterial *mat2 = new SoMaterial;
	mat2->ambientColor.setValue (1.0, 1.0, 0.5);
	mat2->diffuseColor.setValue (1.0, 1.0, 0.5);
	mat2->specularColor.setValue (1.0, 1.0, 0.5);
	mat2->emissiveColor.setValue (1.0, 1.0, 0.5);
	
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->lineWidth.setValue(1.0);
	
	root->addChild (drawStyle);
	root->addChild (mat2);
	root->addChild (test);
		
	return "IGUANA: IgSoRectHist Test";
	    
}

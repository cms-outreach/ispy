//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRectColHist.h"
// #include "Iguana/Inventor/interface/IgSoAxis.h"
// #include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include "test_main.h"
// #include <iostream>
// #include <fstream>
// #include <vector>

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
	IgSoRectColHist::initClass ();

	
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
	float _Rr        = 1.29f;
	float _Rz        = 3.17f;
	float _offsetR   = 5;
	float _offsetZ   = 5;
	unsigned _nr     = 39*2; 
	unsigned _nz     = 85*2;
	unsigned nOfBins = 2 * (_nr + _nz);
	SbVec3f _lineColor (0.5f, 0.5f, 0.5f);
	float _maxDist = 5.f;
	
	std::vector<float> energies (2 * nOfBins);
	
	for (unsigned i = 0; i < nOfBins; i++)
	{
// 		float negative = 2 * (rand () % 2) - 1;
// 		energies [i]           = /*negative */ 10.f * (float) rand () / RAND_MAX ;
// 		energies [nOfBins + i] = /*negative */ 10.f * (float) rand () / RAND_MAX ;
		energies [i]           = 1;
		energies [nOfBins + i] = 1;
	}
// 	energies[10] = -3;
// 	energies[nOfBins + 10] = -3;
// 	energies[13] = 3;
// 	energies[nOfBins + 13] = 3;
// 	energies[nOfBins-1] = 10;
// 	energies[nOfBins-2] = -3;
// 	energies[nOfBins-3] = 0;
	
	IgSoRectColHist* test = new IgSoRectColHist;
	
	test->radiusR = _Rr;
	test->radiusZ = _Rz;
	test->offsetR= _offsetR;
	test->offsetZ = _offsetZ;
	test->energies.setValues (0, energies.size(), &energies [0]);
	test->numR= _nr;
	test->numZ= _nz; 
	test->layer = -5.0;
// 	test->showAnnotations = TRUE;
	test->logScale = false;    
	test->barrelMaxEta = 1.479f;
	test->beamPipeTheta = 5.7f * M_PI / 180.f;
	test->endcapMaxTheta = 25.672f * M_PI / 180.f;
//	test->scaleFactor = 0.5;
// 	test->lineColor = _lineColor;
// 	test->maxDist = _maxDist;
	
	SoMaterial *mat2 = new SoMaterial;
	mat2->diffuseColor.setValue (1.0, 1.0, 0.5);
	
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->lineWidth.setValue (1.0);
	
	root->addChild (drawStyle);
	root->addChild (mat2);
	root->addChild (test);
		
	return "IGUANA: IgSoRectColHist Test";
	    
}

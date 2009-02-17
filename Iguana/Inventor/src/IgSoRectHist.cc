#include "Iguana/Inventor/interface/IgSoRectHist.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoAnnotation.h>

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

enum n_placement {UPPERRIGHTENDCAP, UPPERBARREL, LEFTENDCAP, LOWERBARREL, LOWERRIGHTENDCAP};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoRectHist);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

n_placement 
findBinPlacement(const int nr, 
		 const int nz, 
		 const int binNo) 
{
    //WARNING!! DOESN'T WORK WITH ODD NUMBER OF BINS
    if (binNo  < nr/2) return UPPERRIGHTENDCAP;
    else if (binNo < nr/2 + nz) return UPPERBARREL;
    else if	(binNo < 3./2. * nr + nz) return LEFTENDCAP;
    else if (binNo < 3./2. * nr + 2 * nz) return LOWERBARREL;
    else return LOWERRIGHTENDCAP;
}

void 
logScaleEnergies (std::vector<float> & localEnergies, 
		  std::vector<float> & negativeEnergies) 
{
    for (unsigned int i=0; i<localEnergies.size(); i++) {
	if (localEnergies[i]!=0) localEnergies[i]=log(localEnergies[i]+1);
	if (negativeEnergies[i]!=0) negativeEnergies[i]=-log(-negativeEnergies[i]+1);
    }
}

void 
scaleEnergies (std::vector<float> & localEnergies, 
	       std::vector<float> & negativeEnergies, 
	       const float & scaleFactor) 
{
    for (unsigned int i=0; i<localEnergies.size(); i++) 
    {
	if (localEnergies[i]!=0) localEnergies[i]=localEnergies[i]*scaleFactor;
	if (negativeEnergies[i]!=0) negativeEnergies[i]=negativeEnergies[i]*scaleFactor;
    }
}

int 
findMaxEnergyBin (std::vector<float> & localEnergies) 
{
    float maxEnergy=0;
    unsigned int maxEnergyIndex=0;
	
    for (unsigned int i=0; i<localEnergies.size(); i++) 
    {
	if (localEnergies[i]>maxEnergy) 
	{
	    maxEnergy=localEnergies[i];
	    maxEnergyIndex=i;
	}
    }
	
    return maxEnergyIndex;
}

void resizeByMaxEnergy (const float maxEt, 
			std::vector<float> & localEnergies) 
{
    //getting maximum energy
    std::vector<float>::iterator iter;
    iter=max_element(localEnergies.begin(), localEnergies.end());
    float maxEnergy=*iter;
	
    //checking that maxEnergy is not zero - doing nothing otherwise
    if (maxEnergy==0) 
    {
	std::cout << "Cannot resize by maximum energy - maximum energy of this RecHit collection is zero" << std::endl;
	return;
    }
	
    for (unsigned int i=0; i<localEnergies.size() ; i++) localEnergies[i] = localEnergies[i] * maxEt / maxEnergy;

}

//function calculating coordinates of vertex in barrel
SbVec3f 
IgSoRectHist::CMStoIgBarrel(const float & deltaTheta, 
			    const float & thetaRight, 
			    //thetaIndex is a float to allow easier placement of label
			    const float & thetaIndex, 
			    const float & energy,
			    const n_lowerUpper upperBarrel)
{
    SbVec3f OIcoords;
    OIcoords[1]= upperBarrel ? 
		 Rr.getValue() + offsetR.getValue() + energy : 
		 -(Rr.getValue() + offsetR.getValue() + energy);
    //thetaIndex runs from zero to the number of bins
    //when drawing the 2 lower vertexes of the face, thetaIndex corresponds to the number of the bin
    //when drawing the upper vertexes, thetaIndex is binNo + 1 
    OIcoords[2] = OIcoords[1]* tan (M_PI/2. - (thetaRight + deltaTheta * (float)thetaIndex));
    OIcoords[0] = layer.getValue();
	
    return OIcoords;
}

//function calculating coordinates of vertex in endcap
SbVec3f 
IgSoRectHist::CMStoIgEndcap(const float & deltaTheta, 
			    const float & thetaRight,
			    const float & thetaIndex, 
			    const float & energy,
			    const n_leftRight leftRight)
{
    SbVec3f OIcoords;
	
    OIcoords[2] = leftRight ? 
		  Rz.getValue() + offsetZ.getValue() + energy : 
		  -(Rz.getValue() + offsetZ.getValue() + energy);
    OIcoords[1] = leftRight ? 
		  OIcoords[2] * tan (deltaTheta * (float)thetaIndex) :
		  OIcoords[2] * tan (M_PI - (thetaRight - deltaTheta * (float)thetaIndex) );
    OIcoords[0] = layer.getValue();

    return OIcoords;
}


//FIXME: if there are no negative energies, avoid drawing the shape (return parameter...?)

//function that separates the negative energies from positive energies: 
//they must be drawn in different faceSets
std::vector<float> 
IgSoRectHist::getNegativeEnergies (std::vector<float> & localEnergies) 
{
    float _Rz=Rz.getValue();
    float _Rr=Rr.getValue();
    int _nr=(int)nr.getValue();
    int _nz=(int)nz.getValue();
    float _offsetR=offsetR.getValue();
    float _offsetZ=offsetZ.getValue();

    std::vector<float> negativeEnergies(localEnergies.size());
	
    //TODO: clean the code here
    for (int i=0; i<_nr/2; i++) 
    {
	if (localEnergies[i] < 0) 
	{		
	    negativeEnergies[i]=localEnergies[i];
			
	    //rescale if it has a chance of overlapping the centre
	    if (fabs(negativeEnergies[i]) > _Rz+_offsetZ) 
	    {
		negativeEnergies[i]=-(_Rz+_offsetZ);
	    }
			
	    //set the local energy vector value to zero
	    localEnergies[i]=0;
	}
	else negativeEnergies[i]=0;
    }
	
    for (int i=_nr/2; i<_nr/2+_nz; i++) 
    {
	if (localEnergies[i] < 0) 
	{	
	    negativeEnergies[i]=localEnergies[i];
			
	    //rescale if it has a chance of overlapping the centre
	    if (fabs(negativeEnergies[i]) > _Rr+_offsetR) 
	    {
		negativeEnergies[i]=-(_Rr+_offsetR);
	    }
			
	    //set the local energy vector value to zero
	    localEnergies[i]=0;
	}
		
	else negativeEnergies[i]=0;
    }
	
    for (int i=_nr/2+_nz; i<_nr*3/2+_nz; i++) 
    {
	if (localEnergies[i] < 0) 
	{	
	    negativeEnergies[i]=localEnergies[i];
			
	    //rescale if it has a chance of overlapping the centre
	    if (fabs(negativeEnergies[i]) > _Rz+_offsetZ) 
	    {
		negativeEnergies[i]=-(_Rz+_offsetZ);
	    }
			
	    //set the local energy vector value to zero
	    localEnergies[i]=0;
	}
		
	else negativeEnergies[i]=0;
    }
	
    for (int i=_nr*3/2+_nz; i<_nr*3/2+2*_nz; i++) 
    {
	if (localEnergies[i] < 0) 
	{	
	    negativeEnergies[i]=localEnergies[i];
			
	    //rescale if it has a chance of overlapping the centre
	    if (fabs(negativeEnergies[i]) > _Rr+_offsetR) 
	    {
		negativeEnergies[i]=-(_Rr+_offsetR);
	    }
			
	    //set the local energy vector value to zero
	    localEnergies[i]=0;
	}
		
	else negativeEnergies[i]=0;
    }
	
    for (int i=_nr*3/2+2*_nz; i<2*_nz+2*_nr; i++) 
    {
	if (localEnergies[i] < 0) 
	{		
	    negativeEnergies[i]=localEnergies[i];
			
	    //rescale if it has a chance of overlapping the centre
	    if (fabs(negativeEnergies[i]) > _Rz+_offsetZ) 
	    {
		negativeEnergies[i]=-(_Rz+_offsetZ);
	    }
			
	    //set the local energy vector value to zero
	    localEnergies[i]=0;
	}
	else negativeEnergies[i]=0;
    }
    return negativeEnergies;
}

//generating vertex data: division in bins of equal angle
//TODO: division by deltaeta
std::vector<SbVec3f> 
IgSoRectHist::generateVertexData (std::vector<float> & localEnergies,
				  const int nOfBins, 
				  const float & deltaThetaEndcap, 
				  const float & deltaThetaBarrel, 
				  const float & thetaRightEndcap,
				  const float & thetaRightBarrel, 
				  const float & thetaOffset, 
				  const float & scale) 
{
    float currentEnergy=0, projectedEnergy=0;
    int _nr=(int)nr.getValue();
    int _nz=(int)nz.getValue();
    float _Rr=Rr.getValue();
    float _Rz=Rz.getValue();
    float _offsetRr=offsetR.getValue();
    float _offsetRz=offsetZ.getValue();
    float _layer=layer.getValue();
    SbVec3f vertexBuffer;
    std::vector<SbVec3f> vertexData;

    //numbering of points in face (if CCW):
    //3---------------------2
    //|                     | 
    //|                     |  
    //0---------------------1
    //note: point 3 is shared by the next face
	
    //----------------ENDCAP RIGHT

    //POINT 0
    vertexBuffer.setValue(_layer , 0, _Rz+_offsetRz);
    vertexData.push_back(vertexBuffer);
	
    for (int i=0; i<_nr/2; i++) {

	currentEnergy=localEnergies[i];
	projectedEnergy=currentEnergy*cos(i*deltaThetaEndcap+deltaThetaEndcap/2.);
		
	//POINT 1
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i, projectedEnergy, RIGHT);
	vertexData.push_back(vertexBuffer);
		
	//POINT 2
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i+1, projectedEnergy, RIGHT);
	vertexData.push_back(vertexBuffer);
		
	//POINT 3
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i+1, 0, RIGHT);
	vertexData.push_back(vertexBuffer);
    }

    //----------------UPPER BARREL
	
    //POINT 0
    vertexBuffer.setValue(_layer , _Rr+_offsetRr, _Rz);
    vertexData.push_back(vertexBuffer);
	
    for (int i=_nr/2; i<_nr/2+_nz; i++) {

	currentEnergy=localEnergies[i];
	projectedEnergy=currentEnergy*cos( M_PI/2. - (thetaRightBarrel + (i-(_nr/2))*deltaThetaBarrel + +deltaThetaBarrel/2.));
		
	//POINT 1
	vertexBuffer=CMStoIgBarrel(deltaThetaBarrel, thetaRightBarrel, i-(_nr/2), projectedEnergy, UPPER);
	vertexData.push_back(vertexBuffer);
		
	//POINT 2
	vertexBuffer=CMStoIgBarrel(deltaThetaBarrel, thetaRightBarrel, i+1-(_nr/2), projectedEnergy, UPPER);
	vertexData.push_back(vertexBuffer);
		
	//POINT 3
	vertexBuffer=CMStoIgBarrel(deltaThetaBarrel, thetaRightBarrel, i+1-(_nr/2), 0, UPPER);
	vertexData.push_back(vertexBuffer);

    }
	
    //----------------LEFT ENDCAP

    //POINT 0
    vertexBuffer.setValue(_layer, _Rr, -(_Rz + _offsetRz));
    vertexData.push_back(vertexBuffer);
	
    for (int i=_nr/2+_nz; i<_nr*3/2+_nz; i++) {

	currentEnergy=localEnergies[i];
	projectedEnergy=-currentEnergy*cos(M_PI-thetaRightEndcap+deltaThetaEndcap/2.+(i-(_nr/2+_nz))*deltaThetaEndcap);
		
	//POINT 1
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i-(_nr/2+_nz), projectedEnergy, LEFT);
	vertexData.push_back(vertexBuffer);
		
	//POINT 2
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i-(_nr/2+_nz)+1, projectedEnergy, LEFT);
	vertexData.push_back(vertexBuffer);
		
	//POINT 3
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i-(_nr/2+_nz)+1, 0, LEFT);
	vertexData.push_back(vertexBuffer);
    }
	
    //----------------LOWER BARREL
	
    //POINT 0
    vertexBuffer.setValue(_layer , -(_Rr+_offsetRr), -_Rz);
    vertexData.push_back(vertexBuffer);

    for (int i=_nr*3/2+_nz; i<_nr*3/2+2*_nz; i++) {

	currentEnergy=localEnergies[i];
	projectedEnergy=currentEnergy * cos(M_PI/2. - (thetaRightBarrel + deltaThetaBarrel * (i-(_nr*3/2+_nz)) + deltaThetaBarrel/2.));
		
	//POINT 1
	vertexBuffer=CMStoIgBarrel(deltaThetaBarrel, thetaRightBarrel, i-(_nr*3./2.+_nz), projectedEnergy, LOWER);
	vertexData.push_back(vertexBuffer);
		
	//POINT 2
	vertexBuffer=CMStoIgBarrel(deltaThetaBarrel, thetaRightBarrel, i+1-(_nr*3./2.+_nz), projectedEnergy, LOWER);
	vertexData.push_back(vertexBuffer);
		
	//POINT 3
	vertexBuffer=CMStoIgBarrel(deltaThetaBarrel, thetaRightBarrel, i+1-(_nr*3./2.+_nz), 0, LOWER);
	vertexData.push_back(vertexBuffer);
    }

    //----------------LOWER RIGHT ENDCAP

    //POINT 0
    vertexBuffer.setValue(_layer , -_Rr, _Rz+_offsetRz);
    vertexData.push_back(vertexBuffer);

    for (int i=_nr*3/2+2*_nz; i<2*_nz+2*_nr; i++) {
		
	currentEnergy=localEnergies[i];
	projectedEnergy=currentEnergy*cos(-deltaThetaEndcap*(i-(_nr*3/2+2*_nz)-_nr/2)-deltaThetaEndcap/2.);
		
	//POINT 1
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap,i-(_nr*3./2.+2*_nz)-_nr/2, projectedEnergy, RIGHT);
	vertexData.push_back(vertexBuffer);
		
	//POINT 2
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap,i-(_nr*3./2.+2*_nz)-_nr/2+1, projectedEnergy, RIGHT);
	vertexData.push_back(vertexBuffer);
		
	//POINT 3
	vertexBuffer=CMStoIgEndcap(deltaThetaEndcap, thetaRightEndcap, i-(_nr*3./2.+2*_nz)-_nr/2+1, 0, RIGHT);
	vertexData.push_back(vertexBuffer);
    }

    return vertexData;
}


SbVec3f 
IgSoRectHist::generateLabelCoords (const int binNo,
				   const float & deltaThetaEndcap,
				   const float & deltaThetaBarrel,
				   const float & thetaRightEndcap,
				   const float & thetaRightBarrel,
				   const float & energy)
{
 
    int _nr=(int)nr.getValue();
    int _nz=(int)nz.getValue();
	 
    n_placement placement = findBinPlacement(_nr, _nz, binNo);
    SbVec3f coords;
	
    if (placement == UPPERRIGHTENDCAP || placement == LOWERRIGHTENDCAP) {
	coords = CMStoIgEndcap (deltaThetaEndcap, thetaRightEndcap, binNo + 0.5, energy, RIGHT);
    }
	
    if (placement == UPPERBARREL) {
	coords = CMStoIgBarrel (deltaThetaBarrel, thetaRightBarrel, binNo - _nr/2 + 0.5, energy, UPPER);
    }
	
    if (placement == LEFTENDCAP) {
	coords = CMStoIgEndcap (deltaThetaEndcap, thetaRightEndcap, binNo - (_nr/2 + _nz) + 0.5, energy, LEFT);
    }
	
    if (placement == LOWERBARREL) {
	coords = CMStoIgBarrel (deltaThetaBarrel, thetaRightBarrel, binNo - (_nr * 3./2. + _nz) + 0.5, energy, LOWER);
    }

    if (placement == LOWERRIGHTENDCAP) {
	coords = CMStoIgEndcap (deltaThetaEndcap, thetaRightEndcap, binNo-(_nr*3./2.+2*_nz)-_nr/2+0.5,	 energy, RIGHT);
    }
	
    return coords;


}


std::vector<int> IgSoRectHist::generateFaceIndices (const std::vector<float> & localEnergies, 
						    const int nr, 
						    const int nz) 
{

    //additive parameters for the algorithm of assigning indexes
    int a=0;
    int b=1;
    int c=2;
    int d=3;

    std::vector<int> faceIndices;
	
    //setting first one 
    if (localEnergies[0]!=0) {
	
	faceIndices.push_back(0);
	faceIndices.push_back(1);
	faceIndices.push_back(2);
	faceIndices.push_back(3);
	faceIndices.push_back(0);
	faceIndices.push_back(SO_END_FACE_INDEX);

		
    }
		
    for (int i=1; i < (2*nr + 2*nz); i++) {
		
	if (i==nr/2. || i==(nr/2.+nz) || i==(nr*3./2.+nz) || i==(nr*3./2.+2*nz) ) {
	    a++; b++; c++; d++;
	}
		
	if (localEnergies[i]!=0) {
		
	    faceIndices.push_back(i*3+a);
	    faceIndices.push_back(i*3+b);
	    faceIndices.push_back(i*3+c);
	    faceIndices.push_back(i*3+d);
	    faceIndices.push_back(i*3+a);
	    faceIndices.push_back(SO_END_FACE_INDEX);

	}//end of if (energy was not zero)

    }//end loop on bins

    return faceIndices;
}


//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
void
IgSoRectHist::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoRectHist, IgSoShapeKit, "IgSoShapeKit"); }

//default constructor
IgSoRectHist::IgSoRectHist ()
{

    SO_KIT_CONSTRUCTOR (IgSoRectHist);
	
    // parameters of the constructor
    SO_KIT_ADD_FIELD (Rz,		(10.));
    SO_KIT_ADD_FIELD (Rr,		(5.));
    SO_KIT_ADD_FIELD (offsetR,	(0.));
    SO_KIT_ADD_FIELD (offsetZ,	(0.));
    SO_KIT_ADD_FIELD (nr,		(10.));
    SO_KIT_ADD_FIELD (nz,		(10.));
    SO_KIT_ADD_FIELD (energies,	(0.0));
    SO_KIT_ADD_FIELD (layer,	(0.0));	
	
    SO_KIT_ADD_FIELD (logScale,	(FALSE));
    SO_KIT_ADD_FIELD (scaleFactor,	(1));
    SO_KIT_ADD_FIELD (maxRr,	(0.0));
    SO_KIT_ADD_FIELD (maxRz,	(0.0));
	
    SO_KIT_ADD_FIELD (showAnnotations,	(FALSE));
	
    SO_KIT_ADD_CATALOG_ENTRY (shapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);

    //NEGATIVE ENERGIES	
    SO_KIT_ADD_CATALOG_ENTRY (negativeShapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (negativeMaterial, SoMaterial, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (negativeFaceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);

    //LABELS
    SO_KIT_ADD_CATALOG_ENTRY (soAnnotation, SoAnnotation, FALSE, separator,\x0,TRUE);
	
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
	
    //default colors for the line: black
    lineColor_[0]=0.0;
    lineColor_[1]=0.0;
    lineColor_[2]=0.0;
}

void
IgSoRectHist::refresh (void)
{

    int nOfBins=energies.getNum();
	
    float deltaThetaEndcap = 2*atan(Rr.getValue()/(Rz.getValue()+offsetZ.getValue())) /  nr.getValue();
    float deltaThetaBarrel = 2*atan(Rz.getValue()/(Rr.getValue()+offsetR.getValue())) /  nz.getValue();
	
    float thetaRightEndcap = atan(Rr.getValue() / ( Rz.getValue() + offsetZ.getValue() ) ); 
    float thetaRightBarrel = atan((Rr.getValue()  + offsetR.getValue() )/Rz.getValue() ); 

    //if no energies, don't draw the shape	
    if (nOfBins == 1) {
	
	setPart ("shapeHints",	NULL);
	setPart ("faceSet",	NULL);
	setPart ("negativeShapeHints", NULL);
	setPart ("negativeMaterial", NULL);
	setPart ("negativeFaceSet", NULL);
	setPart ("soAnnotation", NULL);
	return;
		
    }
	
    //for scaling purposes: local copy of energy vector
    std::vector<float> localEnergies(nOfBins);
	
    for (int i=0; i<nOfBins; i++) {
	localEnergies[i]=energies[i];
    }

    //calculating max energy & max energy bin before any scaling is done
    int maxEnergyBin = findMaxEnergyBin(localEnergies);
    std::vector<float>::iterator iter;
    iter=max_element(localEnergies.begin(), localEnergies.end());
    float maxEnergy=*iter;

    //preparing the vector for negative energies
    std::vector<float> negativeEnergies(nOfBins);
    negativeEnergies = getNegativeEnergies (localEnergies);
	
    //scaling
    if (scaleFactor.getValue() !=1) {
	scaleEnergies (localEnergies, negativeEnergies, scaleFactor.getValue());
    }
	
    //log scaling
    if (logScale.getValue()) {
	logScaleEnergies(localEnergies, negativeEnergies);
    }

    //------------SHAPEHINTS
	
    SoShapeHints* shapeHints = new SoShapeHints;
    shapeHints->faceType = SoShapeHints::CONVEX;
    shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	
    SoShapeHints* negativeShapeHints = new SoShapeHints;
    negativeShapeHints->faceType = SoShapeHints::CONVEX;
    negativeShapeHints->vertexOrdering = SoShapeHints::CLOCKWISE;
	
    //------------MATERIALS
	
    SoMaterial * lineMaterial = new SoMaterial;
    SoMaterial * negativeMaterial = new SoMaterial;
    lineMaterial->ambientColor.setValue (lineColor_[0], lineColor_[1] , lineColor_[2] );
    lineMaterial->diffuseColor.setValue (lineColor_[0], lineColor_[1] , lineColor_[2] );
    lineMaterial->specularColor.setValue (lineColor_[0], lineColor_[1] , lineColor_[2] );
    lineMaterial->emissiveColor.setValue (lineColor_[0], lineColor_[1] , lineColor_[2] );
    negativeMaterial ->ambientColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negativeMaterial ->diffuseColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negativeMaterial ->specularColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negativeMaterial ->emissiveColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
	 
    //------------FACESETS
	
    SoIndexedFaceSet* faceSet = new SoIndexedFaceSet;
    SoIndexedFaceSet* negativeFaceSet = new SoIndexedFaceSet;
	
    //-----------VERTEXES	
    std::vector<SbVec3f> vertexData = generateVertexData(localEnergies, nOfBins, deltaThetaEndcap, deltaThetaBarrel, thetaRightEndcap, thetaRightBarrel, 0, 1);
	
    std::vector<SbVec3f> negativeVertexData = generateVertexData(negativeEnergies, nOfBins, deltaThetaEndcap, deltaThetaBarrel, thetaRightEndcap, thetaRightBarrel, 0, 1);
		
    //setting vertexProperty to vertexData
	
    SoVertexProperty* vtx = new SoVertexProperty;
    SoVertexProperty* negativeVtx = new SoVertexProperty;

    vtx->vertex.setValues (0, vertexData.size(), &vertexData[0]);
    vtx->materialBinding = SoMaterialBinding::PER_FACE_INDEXED;
	
    negativeVtx->vertex.setValues (0, negativeVertexData.size(), &negativeVertexData[0]);
    negativeVtx->materialBinding = SoMaterialBinding::PER_FACE_INDEXED;
	
    faceSet->vertexProperty = vtx;
    negativeFaceSet->vertexProperty = negativeVtx;
	
    //-----------FACE INDEXES
    std::vector< int > faceIndices = generateFaceIndices(localEnergies, (int)nr.getValue(), (int)nz.getValue());
    std::vector< int > negativeFaceIndices = generateFaceIndices(negativeEnergies, (int)nr.getValue(), (int)nz.getValue());
	
    faceSet->coordIndex.setValues (0, faceIndices.size(), &faceIndices[0]);
    negativeFaceSet->coordIndex.setValues (0, negativeFaceIndices.size(), &negativeFaceIndices[0]);
	
    //------------LINESET
    SoIndexedLineSet* lineSet = new SoIndexedLineSet;
	
    //-----------VERTEXES	
    lineSet->vertexProperty = vtx;
				
    //-----------FACE INDEXES
    lineSet->coordIndex.setValues (0, faceIndices.size(), &faceIndices[0]);

	
    //-------------ANNOTATIONS
	
    if (showAnnotations.getValue()) {
	
	SoAnnotation * soAnnotation = new SoAnnotation;
	
	char label [50];
	
	SbVec3f labelCoordsVec = generateLabelCoords (maxEnergyBin, deltaThetaEndcap, deltaThetaBarrel, thetaRightEndcap, thetaRightBarrel, maxEnergy);
	
	if (maxEnergy >= 100.0) {
	    sprintf (label, "Emax = %.1f TeV ", maxEnergy / 1000.0);
	}
	
	else {
	    sprintf (label, "Emax = %.1f GeV ", maxEnergy);
	}
	
	SoText2* text = new SoText2;
	text->string = label;
	text->justification = SoText2::CENTER;
	    
	SoFont* labelFont = new SoFont;
	labelFont->size.setValue (14.0);
	labelFont->name.setValue ("Arial");
	
	SoMaterial * textMaterial = new SoMaterial;
	textMaterial->diffuseColor.setValue (0.4, 0.4, 0.4);
	
	SoTranslation* textTranslation = new SoTranslation;
	textTranslation->translation.setValue(labelCoordsVec);
	
	
	soAnnotation->addChild(lineMaterial);
	soAnnotation->addChild(lineSet);
	
	soAnnotation->addChild(textMaterial);
	soAnnotation->addChild(textTranslation);
	soAnnotation->addChild(labelFont);
	soAnnotation->addChild(text);
	
	setPart ("soAnnotation", soAnnotation);
	
    }
	
    else setPart ("soAnnotation", NULL);
	
	
    //------------SETTING PARTS
	
    setPart("shapeHints", shapeHints);
    setPart ("faceSet",    faceSet);
    setPart("lineMaterial", lineMaterial);	

	
    setPart ("negativeShapeHints",    negativeShapeHints);
    setPart ("negativeMaterial",    negativeMaterial);
    setPart ("negativeFaceSet",    negativeFaceSet);	
}

void IgSoRectHist::setLineColor (const float& red,
				 const float& green,
				 const float& blue)
{
    lineColor_[0]=red;
    lineColor_[1]=green;
    lineColor_[2]=blue;
}

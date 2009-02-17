#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
// #include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/SbLinear.h>

#include <math.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
const unsigned IgSoRectColHist::IN = 0;
const unsigned IgSoRectColHist::OUT = 1;
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoRectColHist);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoRectColHist::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoRectColHist, IgSoShapeKit, "IgSoShapeKit"); }

//default constructor
IgSoRectColHist::IgSoRectColHist ()
	: m_barrelDeltaEta (0.f),
	  m_barrelMaxEta (0.f),
	  m_endcapDeltaTheta (0.f),
	  m_endcapMaxTheta (0.f),
	  m_deltasSet (false)
{
	SO_KIT_CONSTRUCTOR (IgSoRectColHist);
	
	// parameters of the constructor
	SO_KIT_ADD_FIELD (radiusZ,    (10.f));
	SO_KIT_ADD_FIELD (radiusR,    (5.f));
	SO_KIT_ADD_FIELD (offsetR,    (0.f));
	SO_KIT_ADD_FIELD (offsetZ,    (0.f));
	SO_KIT_ADD_FIELD (numR,       (10));
	SO_KIT_ADD_FIELD (numZ,       (10));
	SO_KIT_ADD_FIELD (energies,   (0.f));
	SO_KIT_ADD_FIELD (layer,      (0.f));
	SO_KIT_ADD_FIELD (center,     (SbVec3f (layer.getValue (), 0.f, 0.f)));
	SO_KIT_ADD_FIELD (faceColors, (SbColor (0.f, 0.f, 0.f)));
	SO_KIT_ADD_FIELD (lineColor,  (SbColor (0.f, 0.f, 0.f)));
	SO_KIT_ADD_FIELD (barrelMaxEta, (0.f));
	SO_KIT_ADD_FIELD (beamPipeTheta, (0.f));
	SO_KIT_ADD_FIELD (endcapMaxTheta, (0.f));
	
	SO_KIT_ADD_FIELD (logScale,    (FALSE));
	SO_KIT_ADD_FIELD (scaleFactor, (1.f));
	SO_KIT_ADD_FIELD (maxDist,     (0.f));

//     SO_KIT_ADD_FIELD (showAnnotations,	(FALSE));
	
	SO_KIT_ADD_CATALOG_ENTRY (shapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY (faceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
// 	SO_KIT_ADD_CATALOG_ENTRY (inPosFaceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
// 	SO_KIT_ADD_CATALOG_ENTRY (outPosFaceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
// 	SO_KIT_ADD_CATALOG_ENTRY (inNegFaceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
// 	SO_KIT_ADD_CATALOG_ENTRY (outNegFaceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY (lineSet, SoIndexedLineSet, FALSE, separator,\x0, TRUE);

    //LABELS
//     SO_KIT_ADD_CATALOG_ENTRY (soAnnotation, SoAnnotation, FALSE, separator,\x0,TRUE);
	
	SO_KIT_INIT_INSTANCE ();
	setUpConnections (true, true);

	faceColors.set1Value (0, SbVec3f (1.f, 0.f, 0.f));
	faceColors.set1Value (1, SbVec3f (0.f, 1.f, 0.f));
	faceColors.set1Value (2, SbVec3f (0.f, 0.f, 1.f));
	faceColors.set1Value (3, SbVec3f (1.f, 1.f, 0.f));
}

std::vector<int>
IgSoRectColHist::assignIndices (std::vector<std::vector<float> >& localEnergies, std::vector<int>& colorIndices) 
{
	// we push the indices for each face which has to be drawn into the vector
	// and determine the color for each face. We also distinguish between EM
	// and hadronic energy
	std::vector<int> faceIndices;
	unsigned count = 0;
	for (unsigned i = 0; i < localEnergies[IN].size (); i++)
	{
		if (localEnergies[IN][i] != 0.f)
		{
			faceIndices.push_back (count);
			faceIndices.push_back (count + 1);
			faceIndices.push_back (count + 2);
			faceIndices.push_back (count + 3);
			faceIndices.push_back (count);
			faceIndices.push_back (SO_END_FACE_INDEX);
			count += 4;
			colorIndices.push_back ((localEnergies[IN][i] > 0.f) ? (0) : (1));
		}
		if (localEnergies[OUT][i] != 0.f)
		{
			faceIndices.push_back (count);
			faceIndices.push_back (count + 1);
			faceIndices.push_back (count + 2);
			faceIndices.push_back (count + 3);
			faceIndices.push_back (count);
			faceIndices.push_back (SO_END_FACE_INDEX);
			count += 4;
			colorIndices.push_back ( (localEnergies[OUT][i] >= 0.f) ? (2) : (3));
		}
	}
	return faceIndices;
}

IgSoRectColHist::n_placement 
IgSoRectColHist::findBinPlacement (const unsigned binIndex) 
{
	// This method tells you on which side the given bin(number) is located.
	// NOTE: This doesn't work with an odd number of bins, but since, as far
	// as I know, the detector is symmetrical, there cannot be an odd number
	// of bins. CCW. starting in the middle of the right endcap
	unsigned _numZ = numZ.getValue ();
	unsigned _numR = numR.getValue ();
	
	if (binIndex  < _numR/2) return RIGHT;
	else if (binIndex < _numR/2 + _numZ) return TOP;
	else if (binIndex < 3*_numR/2 + _numZ ) return LEFT;
	else if (binIndex < 3*_numR/2 + 2*_numZ ) return BOTTOM;
	else return RIGHT;
}

SbVec3f
IgSoRectColHist::projectPoint (const float energy, const SbVec3f& point, const SbVec3f& center)
{
	// This method returns a point which is projected the given amount of
	// energy in the direction of 'point-center' from 'point' onwards.
	// NOTE: the provided energy equals the length of the rectangle's edge(s) 
	// which point away from the central rectangle. The disadvantage of this 
	// method is that towers with equal energy dont look equally high. Another
	// way to implement the representation of the energy could be to make the 
	// energy equal to the height of the rectangle, but then the towers will
	// appear bigger since they have a larger area.
	
	const float y = point[1] - center[1];
	const float z = point[2] - center[2];
	const float radius = calcLocalRadius (point, center);
	const float angle  = atan2 (y, z); // returns a value in [-pi;pi]
	
	return convertCoordinates (radius + energy, convertAngle (angle), center);
}

// returns x (= layer), y and z coordinates given polar coordinates
// angle supposed to be in rad
SbVec3f
IgSoRectColHist::convertCoordinates (const float radius, const float phi, const SbVec3f& center)
{
	SbVec3f cartesianCoords;
	cartesianCoords[0] =  layer.getValue ();
	cartesianCoords[1] =  radius * sin (phi) + center[2];;
	cartesianCoords[2] =  radius * cos (phi) + center[1];
	return cartesianCoords;
}

int
IgSoRectColHist::findMaxEnergyBin (std::vector<std::vector<float> >& energies)
{
	// look for the bin with maximum summed energy and set the max energy
	m_maxEnergy         = 0.0f;
	int maxEnergyBin    = -1;
	float totalE        = 0.0f;
	const float numBins = energies[IN].size ();
	
	for (unsigned i = 0; i < numBins; i++)
	{
		totalE =  energies[IN][i] + energies[OUT][i];
		if (totalE > m_maxEnergy)
		{
			m_maxEnergy = totalE;
			maxEnergyBin = i;
		}
	}
	return maxEnergyBin;
}

void
IgSoRectColHist::calcLogEnergies (std::vector<std::vector<float> >& logEnergies)
{
	for (unsigned i = 0; i < logEnergies[IN].size (); i++)
	{
		logEnergies[IN][i]  = log ((logEnergies[IN][i]  <= 0.0f) ? (1.0f) : (logEnergies[IN][i]));
		logEnergies[OUT][i] = log ((logEnergies[OUT][i] <= 0.0f) ? (1.0f) : (logEnergies[OUT][i]));
	}
}

void
IgSoRectColHist::scaleEnergies (std::vector<std::vector<float> >& energies)
{
	const float _maxDist = maxDist.getValue ();
	float localScaleFactor = 1.0f;
	
	// maxDist scaling
	if (_maxDist > 0.f)
	{
		if (m_maxEnergy != 0.f)
		{
			localScaleFactor = _maxDist / m_maxEnergy;
		}
		else
		{
			std::cerr << "Maximum energy equals zero, no maxDist scaling is done!" << std::endl;
		}
	}
	
	const float _scaleFactor = scaleFactor.getValue ();
	// multiply the energies by a given factor
	if (_scaleFactor != 1.0f)
	{
		localScaleFactor = localScaleFactor * _scaleFactor;
	}

	for (unsigned i = 0; i < energies[0].size (); i++)
	{
		energies[IN][i]  = localScaleFactor * energies[IN][i];
		energies[OUT][i] = localScaleFactor * energies[OUT][i];
	}
}

void
IgSoRectColHist::setLogEnergies (std::vector<std::vector<float> >& energies)
{
	int maxEnergyBin = -1;
	maxEnergyBin = findMaxEnergyBin (energies);
	
	// if we are using logscale, change to the log values
	if (logScale.getValue () && maxEnergyBin != -1)
	{
		// calculate max energies and determine new maxenergy
		calcLogEnergies (energies);
		m_maxEnergy = energies[IN][maxEnergyBin] + energies[OUT][maxEnergyBin];
	}
}

float
IgSoRectColHist::calcLocalRadius (const SbVec3f& point, const SbVec3f& center) const
{
	// calculate the radius within the local system
	const float y = point[1] - center[1];
	const float z = point[2] - center[2];
	return sqrt (y * y + z * z);
}

SbVec3f
IgSoRectColHist::calcAnglePoint (float angle, float y, float z)
{
	// this method returns a three dimensional point which lies in the
	// intersection of a fixed line and a moving line (and in the x layer).
	// The fixed line is defined by the floats y and z, one of them must be
	// zero which then will be calculated, the other float defines the distance
	// of the line to the corresponding coordinate axis, i.e. it is parallel to
	// it. The "moving" line is defined by the angle and the center of the
	// coordinate system and it is moving with respect to the previous
	// calculation
	const float _layer = layer.getValue ();
	SbLine fixedLine;
	SbVec3f point1 (_layer, y, z);
	
	if (y == 0.f)
	{
		fixedLine.setValue (point1, SbVec3f (_layer, z, z));
	}
	else if (z == 0.f)
	{
		fixedLine.setValue (point1, SbVec3f (_layer, y, y));
	}
	else
	{
		return SbVec3f (_layer, y, z);
	}
	
	SbVec3f test = convertCoordinates (1.f, angleToLeftHanded (angle), SbVec3f (_layer, 0.f, 0.f));
	SbLine movingLine (test, SbVec3f (_layer, 0.f, 0.f));

	SbVec3f result;
	SbVec3f result2;
	// since they are in the same layer, they must intersect
	movingLine.getClosestPoints (fixedLine, result, result2);
	return result;
}

float
IgSoRectColHist::angleToLeftHanded (float rightHandedAngle)
{
	return (2.f * M_PI - rightHandedAngle);
}

void
IgSoRectColHist::determineAngularDelta (void)
{
	// assumption: barrel's deltaEta is const, endcap's deltatheta is (more or
	// less) const
	float _radiusZ = radiusZ.getValue ();
	float _radiusR = radiusR.getValue ();
	unsigned _numZ = numZ.getValue ();
	unsigned _numR = numR.getValue ();
	_radiusZ = (_radiusZ <= 0.f) ? (1.f) : (_radiusZ);
	_radiusR = (_radiusR <= 0.f) ? (1.f) : (_radiusR);
	_numZ = (_numZ == 0) ? (1) : (_numZ);
	_numR = (_numR == 0) ? (1) : (_numR);
	
	// calculate the barrel's delta eta, if no maxEta is given, assume that
	// it's the corner of the detector
	m_barrelMaxEta = barrelMaxEta.getValue ();
	if (m_barrelMaxEta <= 0.f)
	{
		if (m_endcapMaxTheta <= 0.f)
		{
			m_barrelMaxEta = thetaToEta (M_PI/2.f - atan (_radiusZ/_radiusR));
		}
		else
		{
			m_barrelMaxEta = thetaToEta (m_endcapMaxTheta);
		}
	}
	m_barrelDeltaEta = m_barrelMaxEta / (_numZ / 2);
	
	// theta is used for the endcaps, if the maximum is defined, use it,
	// otherwise determine it from maxeta or the corner
	m_endcapMaxTheta = endcapMaxTheta.getValue ();
	if (m_endcapMaxTheta <= 0.f)
	{
		if (m_barrelMaxEta > 0.f)
		{
			m_endcapMaxTheta = etaToTheta (m_barrelMaxEta);
		}
		else
		{
			m_endcapMaxTheta = atan (_radiusR / _radiusZ);
		}
	}
	m_endcapDeltaTheta = (m_endcapMaxTheta - beamPipeTheta.getValue ()) / (_numR / 2);
	m_deltasSet = true;
}

float
IgSoRectColHist::etaToTheta (float eta)
{
	return (2.f * atan (exp (- eta)));
}

float
IgSoRectColHist::thetaToEta (float theta)
{
	return (-log (tan (theta/2.f)));
}

float
IgSoRectColHist::getBinAngle (unsigned _binNum, bool isLeftPoint)
{
	// returns the mathematical angle of the left resp. right lower corner of the bin with the bin number '_binNum'.
	float result = 0.f;
	// determine the delta eta and delta theta first
	if (!m_deltasSet) determineAngularDelta ();
	
	unsigned _numZ = numZ.getValue ();
	unsigned _numR = numR.getValue ();
	_numZ = (_numZ == 0) ? (1) : (_numZ);
	_numR = (_numR == 0) ? (1) : (_numR);
	const unsigned binNum = (isLeftPoint) ? (_binNum + 1) : (_binNum);

	// bin nbr zero starts in the right endcap's center. NOTE that the angle
	// depends on the beam pipe opening (in theta), on the endcaps' maximum
	// theta and on the barrel's maximum eta. There are 8 different sectors,
	// starting at the mathematical angel zero up to 2Pi. Sectors change at the
	// detector's corners. Corners on the other hand, might differ depending on
	// the sector if maxtheta and maxeta aren't equal.
	if (_binNum < _numR/2)
	{
		result = binNum * m_endcapDeltaTheta + beamPipeTheta.getValue ();
	}
	else if (_binNum < (_numZ/2 + _numR/2))
	{
		const float tempEta = m_barrelMaxEta - (binNum - _numR/2) * m_barrelDeltaEta;
		result = ((tempEta < 0.f) ? (M_PI - etaToTheta (fabs (tempEta))) : (etaToTheta (tempEta)));
	}
	else if (_binNum < (_numZ + _numR/2))
	{
		const float tempEta = (binNum - (_numZ/2 + _numR/2)) * m_barrelDeltaEta;
		result = M_PI - etaToTheta (tempEta);
	}
	else if (_binNum < (_numZ + _numR))
	{
		result = (binNum - (_numZ + _numR/2)) * m_endcapDeltaTheta + M_PI - m_endcapMaxTheta;
	}
	else if (_binNum < (_numZ + 3*_numR/2))
	{
		result = (binNum - (_numR + _numZ)) * m_endcapDeltaTheta + beamPipeTheta.getValue () + M_PI;
	}
	else if (_binNum < (3*(_numZ+_numR)/2))
	{
		const float tempEta = m_barrelMaxEta - (binNum - (_numZ + 3*_numR/2)) * m_barrelDeltaEta;
		result =  ((tempEta < 0.f) ? (M_PI - etaToTheta (fabs (tempEta))) : (etaToTheta (tempEta))) + M_PI;
	}
	else if (_binNum < (2*_numZ + 3*_numR/2))
	{
		const float tempEta = (binNum - (3*(_numZ+_numR)/2)) * m_barrelDeltaEta;
		result = 2.f * M_PI - etaToTheta (tempEta);
	}
	else
	{
		result = (binNum - (3*_numR/2 + 2*_numZ)) * m_endcapDeltaTheta + 2.f * M_PI - m_endcapMaxTheta;
	}
	return result;
}

float
IgSoRectColHist::convertAngle (float angle)
{
	// this method takes a value within [-pi;pi] and returns a value [0;2*pi].
	// values from 0 to pi are the same. Returns a "mathematical" angle
	// starting on the positive x-axis.
	float result;
	if (angle > (2.f * M_PI))
	{
		result = fmod (angle, 2 * M_PI);
	}
	else if (angle < 0.f)
	{
		result = (angle + 2 * M_PI);
	}
	else
	{
		result = angle;
	}
	return result;
}

void
IgSoRectColHist::calcBinCorner (unsigned i, SbVec3f& left, SbVec3f& right)
{
	// this method calculates the lower left and right corner of a bin i
	float yCoord, zCoord;
	float _radiusR = radiusR.getValue ();
	float _radiusZ = radiusZ.getValue ();

	switch (findBinPlacement (i)) 
	{
		case TOP:
			zCoord = 0.f;
			yCoord = -_radiusR;
		break;

		case RIGHT:
			zCoord = _radiusZ;
			yCoord = 0.f;
		break;
		
		case BOTTOM:
			zCoord = 0.f;
			yCoord = _radiusR;
		break;

		default: //LEFT 
			zCoord = -_radiusZ;
			yCoord = 0.f;
		break;
	}
	left  = calcAnglePoint (getBinAngle (i, true), yCoord, zCoord);
	right = calcAnglePoint (getBinAngle (i, false), yCoord, zCoord);
}

void
IgSoRectColHist::addOffset (SbVec3f& point, unsigned binNum)
{
	if (offsetZ.getValue () == 0.f && offsetR.getValue () == 0.f) return;
	
	SbVec3f offset (layer.getValue (), 0.f, 0.f);
	switch (findBinPlacement (binNum)) 
	{
		case TOP:
			offset[1] = - offsetR.getValue ();
		break;

		case RIGHT:
			offset[2] = offsetZ.getValue ();
		break;
		
		case BOTTOM:
			offset[1] = offsetR.getValue ();
		break;

		default: //LEFT 
			offset[2] = - offsetZ.getValue ();
		break;
	}
	point = point + offset;
}

void
IgSoRectColHist::refresh (void)
{
	const unsigned numOfBins = energies.getNum () / 2;
	const unsigned _numR = numR.getValue ();
	const unsigned _numZ = numZ.getValue ();
	
	if (2*(_numR + _numZ) == numOfBins && (_numR * _numZ > 0))
	{
		SbVec3f inner[4]; // points of the inner towers
		SbVec3f outer[4]; // points of the outer towers
		
		// local copy of the energies, note that the energy vector is split
		// into 2 vectors for convenience
		std::vector<std::vector<float> > localEnergies (2, std::vector<float> (numOfBins, 0.f));
		for (unsigned i = 0; i < numOfBins; i++) 
		{
			localEnergies[IN][i] = energies[i];
			localEnergies[OUT][i] = energies[numOfBins + i];
    	}
		
		setLogEnergies (localEnergies);
		scaleEnergies (localEnergies);
		
		// the lower corners of a to "i" corresponding bin, note that "left"
		// and "right" are relative
		SbVec3f leftPoint;
		SbVec3f rightPoint;
		
		// calculate the radius of each point from the "center"
		SbVec3f zero (layer.getValue (), 0.f, 0.f);
		float leftRadius  = 0.f;
		float rightRadius = 0.f;
		SbVec3f _center    = center.getValue ();
		SbColor _lineColor = lineColor.getValue ();
		std::vector<SbVec3f> vertexData;
		
		// draw the towers
		for (unsigned i = 0; i < numOfBins; i++)
		{
			// don't display the bin if the energies are zero
			if (localEnergies[0][i] == 0.0f && localEnergies[1][i] == 0.0f) continue;

			calcBinCorner (i, leftPoint, rightPoint);
			
			leftRadius = calcLocalRadius (leftPoint, zero);
			rightRadius = calcLocalRadius (rightPoint, zero);
			
			// the tower's base line must be perpendicular to the normal which
			// comes from the central point, but which side we have to shift
			// for that, is decided below
			if (leftRadius < rightRadius)
			{
				inner[3] = rightPoint + _center;
				inner[0] = projectPoint (rightRadius - leftRadius, leftPoint, zero) + _center;
			}
			else
			{
				inner[0] = leftPoint + _center;
				inner[3] = projectPoint (leftRadius - rightRadius, rightPoint, zero) + _center;
			}

			// inner tower's outer points
			inner[1] = projectPoint (localEnergies[0][i], inner[0], _center);
			inner[2] = projectPoint (localEnergies[0][i], inner[3], _center);			
			// outer tower's inner points
			outer[0] = inner[1];
			outer[3] = inner[2];
			// outer tower's outer points
			outer[1] = projectPoint (localEnergies[1][i], outer[0], _center);
			outer[2] = projectPoint (localEnergies[1][i], outer[3], _center);
			
			// add offsets for each point
			for (unsigned j = 0; j < 4; j++)
			{
				addOffset (inner[j], i);
				addOffset (outer[j], i);
			}
			
			// push the vertex information
			for (unsigned j = 0; j < 4; j++)
			{
				// only display the bin if its energy is not zero
				if (localEnergies[0][i] != 0.f)
				{
					vertexData.push_back (inner[j]);
				}
			}
			
			for (unsigned j = 0; j < 4; j++)
			{
				if (localEnergies[1][i] != 0.f)
				{
					vertexData.push_back (outer[j]);
				}
			}
		}
		
		// declare shapehints
		SoShapeHints* shapeHints = new SoShapeHints;
		shapeHints->faceType = SoShapeHints::CONVEX;
		shapeHints->vertexOrdering = SoShapeHints::CLOCKWISE;

		SoIndexedFaceSet* faceSet = new SoIndexedFaceSet;
		SoIndexedLineSet* lineSet = new SoIndexedLineSet;
		
		// set the properties for the faceset and lineset: vertices and colors
		SoVertexProperty* vtx = new SoVertexProperty;
		vtx->materialBinding = SoMaterialBinding::PER_FACE_INDEXED;
		
		// setting face colors (only diffuse color)
		vtx->orderedRGBA.set1Value (0, faceColors[0].getPackedValue ());
		vtx->orderedRGBA.set1Value (1, faceColors[1].getPackedValue ());
		vtx->orderedRGBA.set1Value (2, faceColors[2].getPackedValue ());
		vtx->orderedRGBA.set1Value (3, faceColors[3].getPackedValue ());
		vtx->orderedRGBA.set1Value (4, _lineColor.getPackedValue ()); // line color
		
		// set the vertices for the towers
		vtx->vertex.setValues (0, vertexData.size (), &vertexData[0]);
		
		std::vector<int> colorIndices;
		std::vector<int> faceIndices = assignIndices (localEnergies, colorIndices);
		
		// assign the face and color indices for the faces and lines
		faceSet->coordIndex.setValues (0, faceIndices.size (), &faceIndices[0]);
		faceSet->materialIndex.setValues (0, colorIndices.size (), &colorIndices[0]);
		lineSet->coordIndex.setValues (0, faceIndices.size (), &faceIndices[0]);
		
		// custom single color for the lines surrounding the faces
		if (!(_lineColor[0] == 0.f && _lineColor[1] == 0.f && _lineColor[2] == 0.f))
		{
			for (unsigned i = 0; i < colorIndices.size (); i++)
			{
				colorIndices[i] = 4;
			}
		}
		lineSet->materialIndex.setValues (0, colorIndices.size (), &colorIndices[0]);
		
		faceSet->vertexProperty = vtx;
		lineSet->vertexProperty = vtx;
		
		setPart ("shapeHints", shapeHints);
		setPart ("faceSet",    faceSet);
		setPart ("lineSet",    lineSet);
	}
}


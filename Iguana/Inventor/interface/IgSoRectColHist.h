#ifndef IG_OPEN_INVENTOR_IG_SO_RECT_COL_HIST_H
# define IG_OPEN_INVENTOR_IG_SO_RECT_COL_HIST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFUInt32.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoMFFloat.h>
# include <Inventor/fields/SoSFVec3f.h>
# include <Inventor/fields/SoMFColor.h>
# include <Inventor/fields/SoSFColor.h>
# include <Inventor/SbVec3f.h>

// # include "Iguana/Utilities/classlib/utils/DebugAids.h"

# include <vector>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoRectColHist: public IgSoShapeKit
{

	SO_KIT_HEADER (IgSoRectColHist);
	SO_KIT_CATALOG_ENTRY_HEADER (shapeHints);
	SO_KIT_CATALOG_ENTRY_HEADER (faceSet);
// 	SO_KIT_CATALOG_ENTRY_HEADER (inPosFaceSet);
// 	SO_KIT_CATALOG_ENTRY_HEADER (outPosFaceSet);
// 	SO_KIT_CATALOG_ENTRY_HEADER (inNegFaceSet);
// 	SO_KIT_CATALOG_ENTRY_HEADER (outNegFaceSet);
	SO_KIT_CATALOG_ENTRY_HEADER (lineSet);

// 	SO_KIT_CATALOG_ENTRY_HEADER (soAnnotation);

public:
	IgSoRectColHist ();
	static void initClass (void);
    
	SoSFFloat  radiusR;
	SoSFFloat  radiusZ;
	SoSFFloat  offsetR;
	SoSFFloat  offsetZ;
	SoSFUInt32 numR; //number of bins (r)
	SoSFUInt32 numZ; //number of bins (z)
	SoMFFloat  energies; // note that we need to pass the 2 * binNum = 2 * (2*numR + 2*numZ) elements, since we have EM and hadronic values
	SoSFFloat  layer;
	SoSFVec3f  center;
	SoMFColor  faceColors;
	SoSFColor  lineColor;
	
	//scaling data members
	SoSFBool  logScale;
	SoSFFloat scaleFactor;
	SoSFFloat maxDist;
	
	SoSFFloat barrelMaxEta;
	SoSFFloat beamPipeTheta;  // half of the opening
	SoSFFloat endcapMaxTheta; // doesnt necessarily have to be the same as barrelMaxEta 
	
	//annotations
// 	SoSFBool 	showAnnotations;

	enum n_placement {TOP, RIGHT, BOTTOM, LEFT};

protected:
	virtual void refresh (void);
    
private:
	float m_maxEnergy;
	float m_barrelDeltaEta;
	float m_barrelMaxEta;
	float m_endcapDeltaTheta;
	float m_endcapMaxTheta;
	bool  m_deltasSet;
	
	const static unsigned IN;
	const static unsigned OUT;
	
	void             addOffset (SbVec3f& point, unsigned binNum);
	float            angleToLeftHanded (float rightHandedAngle);
	std::vector<int> assignIndices (std::vector<std::vector<float> >& localEnergies, std::vector<int>& colorIndices);
	SbVec3f          calcAnglePoint (float angle, float x, float y);
	void             calcBinCorner (unsigned i, SbVec3f& left, SbVec3f& right);
	float            calcLocalRadius (const SbVec3f& point, const SbVec3f& center) const;
	void             calcLogEnergies (std::vector<std::vector<float> >& logEnergies);
	float            convertAngle (float angle);
	SbVec3f          convertCoordinates (const float radius, const float phi, const SbVec3f& center);
	void             determineAngularDelta (void);
	n_placement      findBinPlacement (const unsigned binIndex);
	int              findMaxEnergyBin (std::vector<std::vector<float> >& energies);
	float            getBinAngle (unsigned binNum, bool isLeftPoint);
	SbVec3f          projectPoint (const float energy, const SbVec3f& point, const SbVec3f& center);
	void             scaleEnergies (std::vector<std::vector<float> >& energies);
	void             setLogEnergies (std::vector<std::vector<float> >& energies);
	
	float            etaToTheta (float eta);
	float            thetaToEta (float theta);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif

#endif // IG_OPEN_INVENTOR_IG_SO_RECT_HIST_H


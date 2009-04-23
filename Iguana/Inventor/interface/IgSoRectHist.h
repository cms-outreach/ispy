#ifndef IG_OPEN_INVENTOR_IG_SO_RECT_HIST_H
# define IG_OPEN_INVENTOR_IG_SO_RECT_HIST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFInt32.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoMFFloat.h>
# include <Inventor/SbVec3f.h>

# include "classlib/utils/DebugAids.h"

//TODO: REMOVE THIS!!
# include <iostream>
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

class IG_OPEN_INVENTOR_API IgSoRectHist: public IgSoShapeKit
{

	SO_KIT_HEADER (IgSoRectHist);
	SO_KIT_CATALOG_ENTRY_HEADER (shapeHints);
	SO_KIT_CATALOG_ENTRY_HEADER (faceSet);
	SO_KIT_CATALOG_ENTRY_HEADER (negativeShapeHints);
	SO_KIT_CATALOG_ENTRY_HEADER (negativeMaterial);
	SO_KIT_CATALOG_ENTRY_HEADER (negativeFaceSet);

	SO_KIT_CATALOG_ENTRY_HEADER (soAnnotation);

public:
	IgSoRectHist ();
	static void initClass (void);
    
	SoSFFloat	Rr;
	SoSFFloat	Rz;
	SoSFFloat	offsetR;
	SoSFFloat	offsetZ;
	SoSFFloat	nr; //number of bins (r)
	SoSFFloat	nz; //number of bins (z)
	SoMFFloat	energies;
	SoSFFloat	layer;
	
	//scaling data members
	SoSFBool	logScale;
	SoSFFloat	scaleFactor;
	SoSFFloat	maxRr;
	SoSFFloat	maxRz;
	
	//annotations
	SoSFBool 	showAnnotations;

    	enum n_lowerUpper {LOWER, UPPER};
	enum n_leftRight {LEFT, RIGHT};
	enum n_barrelEndcap {BARREL, ENDCAP};
	
	void setLineColor (	const float & red,
				const float & green,
				const float & blue);

protected:
	virtual void refresh (void);
    
private:

	//array of three colors for the lineSet
	float lineColor_[3];
							
	SbVec3f CMStoIgBarrel (const float & deltaTheta, 
			       const float & thetaRight, 
			       const float & thetaIndex, 
			       const float & energy,
			       const n_lowerUpper upperBarrel);
			
	SbVec3f CMStoIgEndcap (const float & deltaTheta, 
			       const float & thetaRight,
			       const float & thetaIndex, 
			       const float & energy,
			       const n_leftRight leftRight);

	std::vector<float> getNegativeEnergies (std::vector<float> & localEnergies) ;
			
	std::vector<SbVec3f> generateVertexData (std::vector<float> & localEnergies,
						 const int nOfBins, 
						 const float & deltaThetaEndcap, 
						 const float & deltaThetaBarrel, 
						 const float & thetaRightEndcap,
						 const float & thetaRightBarrel, 
						 const float & thetaOffset, 
						 const float & scale);

	SbVec3f generateLabelCoords (const int binNo,
				     const float & deltaThetaEndcap,
				     const float & deltaThetaBarrel,
				     const float & thetaRightEndcap,
				     const float & thetaRightBarrel,
				     const float & energy);
        
	std::vector<int> generateFaceIndices (const std::vector<float> & localEnergies, 
					      const int nr, 
					      const int nz);
	void checkNumberOfBins ();
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif

#endif // IG_OPEN_INVENTOR_IG_SO_RECT_HIST_H


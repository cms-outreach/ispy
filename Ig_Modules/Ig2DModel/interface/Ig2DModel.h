#ifndef IG_2D_MODEL_IG_2D_MODEL_H
# define IG_2D_MODEL_IG_2D_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig2DModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
# include <string>
# include <set>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig2DRep;
class SoTransform;
class SoIndexedFaceSet;
class SoNode;
class SbVec3f;
class SoShapeHints;
class SoGroup;



//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_2D_MODEL_API Ig2DModel : public Ig3DBaseModel
{
public:
    Ig2DModel (IgState *state, Ig3DBaseModel *sourceModel);

    Ig3DBaseModel *	sourceModel (void) const;
    void		setCutTransform (SbVec3f axis, float angle);
    SoNode *		cutTransform (void) const;
    SoNode *		cutPlane (void) const;
    void		setCutPlanePosition (float x, float y);
    void 		setCutPlaneSize (float w, float h);
    
    Ig2DRep * 		getLayer (const std::string &name);   
    bool		isToBeCut (std::string);
    
private:
    Ig2DRep *		createFull (Ig2DRep *parent, const std::string &str);
    Ig2DRep *		fullMatch (Ig3DBaseRep *from, const std::string &str);
    std::string 	closestMatchName (const std::string &str);
    std::string 	getFirst (const std::string &str);
    std::string 	getLast (const std::string &str);
    std::string 	removeFirst (const std::string &str);
    std::string 	removeLast (const std::string &str);    

    Ig3DBaseModel	*m_sourceModel;
    SoIndexedFaceSet	*m_plane;    
    SoTransform		*m_objectTransform;
    SoGroup		*m_cutGroup;
    SoShapeHints	*m_cutHints;
    SoTransform		*m_cutScale;
    SoTransform		*m_cutPosition;    

    // Configuration for the model.
    typedef std::set<std::string> 	NotCutTwigSet;
    NotCutTwigSet			m_cutConf;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_2D_MODEL_IG_2D_MODEL_H

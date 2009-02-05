#ifndef IG_3D_SYSTEM_IG3DSHAPE_MANAGER_H
# define IG_3D_SYSTEM_IG3DSHAPE_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DSystem/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>
# include <list>
# include <string>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_SYSTEM_API Ig3DShapeManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (Ig3DShapeManager);    
public:
    Ig3DShapeManager (IgState *state);
    ~Ig3DShapeManager (void);
    SoNode *	lookup (const std::string &alias);    
    void	add (SoNode *, const std::string &alias , bool save);
    void	add (const std::string &filename, const std::string &alias);
    void	addSearchPath (const std::string &path);
    void 	addAlias (const std::string &oldAlias,
			  const std::string &newAlias);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    typedef std::map <std::string, SoNode *> NodeMap;
    typedef std::list <std::string> PathList;    
    IgState	*m_state;
    NodeMap 	m_nodeMap;    
    PathList	m_pathList;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_SYSTEM_IG3DSHAPE_MANAGER_H

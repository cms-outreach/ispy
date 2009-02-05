#ifndef IG_SOCKET_DRIVER_IG_FILE_MANAGER_H
# define IG_SOCKET_DRIVER_IG_FILE_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgSocketDriver/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>
# include <list>
# include <string>
# include <qcstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgFileManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgFileManager);    
public:
    IgFileManager (IgState *state);
    ~IgFileManager (void);
    QByteArray	lookup (const std::string &alias);    
    void	add (QByteArray array, 
		     const std::string &alias , 
		     bool save);
    void	add (const std::string &filename, const std::string &alias);
    void	addSearchPath (const std::string &path,
                               bool prepend = false);
    void 	addPackagePath (const std::string &packageName,
                                bool prepend = false);    
    void 	addPaths (const std::string &relPath,
                          bool prepend = false);    
    void 	addPaths (const std::string &relPath,
    			  const std::string &envVariable,
                          bool prepend = false);    
    void 	addAlias (const std::string &oldAlias,
			  const std::string &newAlias);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    typedef std::map <std::string, QByteArray> CacheMap;
    typedef std::list <std::string> PathList;    
    IgState	*m_state;
    CacheMap 	m_cacheMap;    
    bool	m_debugFiles;    
    PathList	m_pathList;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>


#endif // IG_SOCKET_DRIVER_IG_FILE_MANAGER_H

#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentableInterface.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfNetworkRepresentable.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"

# include <string>
# include <map>
# include <list>
# include <classlib/utils/Regexp.h>
# include <classlib/iobase/IOSelectEvent.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgOProfConfiguration;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**This is the main Representable class containing all the profiling information about the system.
 */
class IG_QT_PROFILE_SERVICE_API IgOProfRepresentable : public IgOProfRepresentableInterface
{
private:
    IgOProfConfiguration *m_configuration;
    char *m_readBuffer;    
    std::string *m_bufferString;
    lat::Regexp *m_profileInfosRE;    
    bool m_isReading;    
    std::map<int,std::string> m_itoaMap;
    typedef std::list<IgOProfProcessRepresentable *> ProcessesToBeUpdatedContainer;
    ProcessesToBeUpdatedContainer m_processesToBeUpdated;
public:
    IgOProfRepresentable (void);
    void requestUpdateProcessList (void);
    bool updateProcessList (lat::IOSelectEvent *event);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

/** This is to allow the presence of only one OProfRepresentable within the program and to ensure that python and
    C++ share the same one.
 */
class IG_QT_PROFILE_SERVICE_API IgOProfRepresentableSingleton
{
private:
public:
    IgOProfRepresentableSingleton (void);
    static IgOProfRepresentableInterface *instance(void);
    
    IgOProfRepresentableInterface *get(void)
	{
	    return instance();	    
	}    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_H

#ifndef __IG_QT_PROFILE_SERVICE_IG_OPROF_EVENT_MAP_H__
#define __IG_QT_PROFILE_SERVICE_IG_OPROF_EVENT_MAP_H__

# include "Ig_Modules/IgQtProfileService/interface/config.h"
#include <string>
#include <map>

class IgOProfConfiguration;

class IG_QT_PROFILE_SERVICE_API IgOProfEvent
{
public:
    int m_allowedCounter;
    int m_allowedArch;
    int m_ID;
    int m_mask;
    std::string m_name;
    int m_minCount;	
    IgOProfEvent (int allowedCounter, int allowedArch, int id, int mask, std::string name, int minCount);
};	

enum CTR_ALLOWED_ENUM
{	
    CTR_ALL = -1,
    CTR_0 = 1,
    CTR_1 = 1<<1, 
    CTR_2 = 1<<2,
    CTR_3 = 1<<3,
    CTR_4 = 1<<4,
    CTR_5 = 1<<5,
    CTR_6 = 1<<6,
    CTR_7 = 1<<7
};	

class IG_QT_PROFILE_SERVICE_API IgOProfEventMap
{
public:
    /**
     */
    IgOProfEventMap (IgOProfConfiguration *configuration);
    
    IgOProfEvent *byName (const std::string &name)
	{
	    return m_byName[name];	    
	}

    IgOProfEvent *byId (const int id)
	{
	    return m_byId[id];	    
	}

    bool containsId (int id)
	{
    	    return m_byId.find (id) != m_byId.end ();
	}    
private:
    IgOProfConfiguration *m_configuration;
    bool m_dummyEventMap;    
    std::map<std::string, int> m_cpuMapNameId;
    std::map<int, std::string> m_cpuMapIdName;
    std::map<std::string, IgOProfEvent *> m_byName;
    std::map<int, IgOProfEvent *> m_byId;    
};	
#endif // __IG_QT_PROFILE_SERVICE_IG_OPROF_EVENT_MAP_H__

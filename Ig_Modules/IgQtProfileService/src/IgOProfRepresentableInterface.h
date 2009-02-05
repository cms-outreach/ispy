#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_INTERFACE_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_INTERFACE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
# include <string>
# include <map>
# include <iostream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfRepresentableInterface
{
public:
    typedef std::map<std::string, IgOProfProcessRepresentable *> ProcessMap;
protected:
    ProcessMap m_processMap;
public:    
    virtual ProcessMap::iterator begin(void){return m_processMap.begin();}    
    virtual ProcessMap::iterator end(void){return m_processMap.end();}
    virtual IgOProfProcessRepresentable *&operator[](const char *which){return m_processMap[which];}    
    virtual ProcessMap::iterator find(const char *key){return m_processMap.find(key);}    
    virtual IgOProfProcessRepresentable *&operator[](const std::string &which){return m_processMap[which];}    
    virtual ProcessMap::iterator find(const std::string &key){return m_processMap.find(key);}
    virtual IgOProfProcessRepresentable *getProcess(const char *processName);    
    virtual void requestUpdateProcessList(void) = 0;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_INTERFACE_H

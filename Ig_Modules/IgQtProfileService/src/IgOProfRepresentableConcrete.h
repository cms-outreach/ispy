#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_CONCRETE_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_CONCRETE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentableInterface.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentable.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfRepresentableConcrete : public IgOProfRepresentableInterface
{
private:
    IgOProfRepresentableInterface *m_representable;    
public:
    typedef std::map<std::string, IgOProfProcessRepresentable *> ProcessMap;

    IgOProfRepresentableConcrete ()
	:m_representable (IgOProfRepresentableSingleton::instance()) 
	{	    
	}        
    ProcessMap::iterator begin(void){return m_representable->begin();}    
    ProcessMap::iterator end(void) {return m_representable->end();}
    IgOProfProcessRepresentable *&operator[](const char *which){return (*m_representable)[which];}    
    ProcessMap::iterator find(const char *key){return m_representable->find(key);}    

    IgOProfProcessRepresentable *&operator[](const std::string &which){return (*m_representable)[which]; }    
    ProcessMap::iterator find(const std::string &key){return m_representable->find(key);}    
    IgOProfProcessRepresentable *getProcess(const char *name){return m_representable->getProcess(name);}    

    void requestUpdateProcessList(void){m_representable->requestUpdateProcessList();}
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_REPRESENTABLE_CONCRETE_H

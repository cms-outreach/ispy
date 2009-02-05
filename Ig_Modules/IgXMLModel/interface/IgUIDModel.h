#ifndef IG_XML_MODEL_IG_UID_MODEL_H
# define IG_XML_MODEL_IG_UID_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgXMLModel/interface/config.h"
# include "Ig_Modules/IgXMLModel/interface/IgUIDRep.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <set>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_XML_MODEL_API IgUIDModel : public IgModel
{
public:
    IgUIDModel (void);
    ~IgUIDModel (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void 		add (IgUIDRep *rep);
    IgUIDRep *	 	remove (IgUIDRep::IDType id);    
    IgUIDRep * 		lookup (IgUIDRep::IDType id);
    typedef std::set <IgUIDRep::IDType> RepSet;
    RepSet::iterator	begin (void);
    RepSet::iterator	end (void);
    
private:
    RepSet	m_repSet;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_XML_MODEL_IG_UID_MODEL_H

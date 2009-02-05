#ifndef IG_XML_MODEL_IG_UID_REP_H
# define IG_XML_MODEL_IG_UID_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgXMLModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgModel;
class IgUIDModel;
class IgRepContext;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_XML_MODEL_API IgUIDRep : public IgRep
{
public:
    typedef int IDType;
    
    IgUIDRep (IgUIDModel *model);
    ~IgUIDRep (void);
    
    IDType id (void);
    std::string stringId (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;
protected:
    void context (IgRepContext *);    

private:
    IgUIDRep (const IgUIDRep &rep) : IgRep () {};
    
    IgUIDModel *	m_model;    
    IgRepContext *	m_context;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_XML_MODEL_IG_UID_REP_H

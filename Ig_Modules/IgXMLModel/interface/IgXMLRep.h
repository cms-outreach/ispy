#ifndef IG_XML_MODEL_IG_XML_REP_H
# define IG_XML_MODEL_IG_XML_REP_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgXMLModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"
# include <qdom.h>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgXMLModel;
class IgRepContext;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_XML_MODEL_API IgXMLRep : public IgRep,
				  public QDomElement
{
public:
    IgXMLRep (IgXMLModel *model, 
	      IgXMLRep *parent, 
	      IgXMLRep *before, 
	      const std::string &tagName);

    ~IgXMLRep (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;
protected:
    virtual void context (IgRepContext *context);    
private:
    IgXMLModel	*	m_model;
    IgRepContext *	m_context;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_XML_MODEL_IG_XML_REP_H

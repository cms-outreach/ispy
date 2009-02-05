#ifndef IG_XML_MODEL_IG_XML_MODEL_H
# define IG_XML_MODEL_IG_XML_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgXMLModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgDispatcher.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QDomDocument;
class IgXMLRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_XML_MODEL_API IgXMLModel : public IgModel
{
public:
    class IgXMLModelChanged 
    {
    public:
	IgXMLModelChanged (IgXMLModel *source) : m_source(source) {};
	
	IgXMLModel *	       	source (void);	
    private:
	IgXMLModel *		m_source;
    };

    enum EventType {CHANGED};
        
    typedef IgDispatcher <IgXMLModelChanged> Dispatcher;
    
    IgXMLModel (const std::string &documentName,
		const std::string &documentType = "IGUANA-Generic");
    ~IgXMLModel (void);    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    Dispatcher *	dispatcher (enum EventType eventType);
    QDomDocument *	document (void);
    void		trackRep (IgXMLRep *rep);
    
private:
    QDomDocument *	m_document;
    Dispatcher *	m_dispatcher;
    std::list<IgXMLRep *>	m_repList;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_XML_MODEL_IG_XML_MODEL_H

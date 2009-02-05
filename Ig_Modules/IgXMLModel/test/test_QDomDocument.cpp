//<<<<<< INCLUDES                                                       >>>>>>

#include <qdom.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

class MyDocElement : public QDomElement
{
public:
    MyDocElement (QDomDocument *doc);
    ~MyDocElement (void);    
};


MyDocElement::MyDocElement (QDomDocument *doc)
    : QDomElement (doc->createElement ("FOO"))
{
}

MyDocElement::~MyDocElement (void)
{
    std::cerr << "Foo" << std::endl;    
}

QDomDocument * 
create (void)
{
    std::cerr << "In create ()" << std::endl;
    
    QDomDocument *doc = new QDomDocument ();
    MyDocElement element (doc);
    MyDocElement element2 (doc);
    
    doc->appendChild (element);
    element.appendChild (element2);
    return doc;
    
}


int
main (int /*argc*/, char **/*argv*/)
{
    QDomDocument *dom = create ();
    std::cerr << "Out of create" << std::endl;
    std::cerr << dom->toString () << std::endl;
    
    delete dom;    
}


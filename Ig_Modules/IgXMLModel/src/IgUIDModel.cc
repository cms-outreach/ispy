//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDRep.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgUIDModel::IgUIDModel (void)
{
}

IgUIDModel::~IgUIDModel (void)
{
    for (RepSet::iterator i = m_repSet.begin ();
	 i != m_repSet.end ();
	 i++)
    {
	delete this->lookup (*i);	
    }
}

IgUIDRep *
IgUIDModel::lookup (IgUIDRep::IDType id)
{
    // No need for a map<int, IgUIDRep*> if the id is an int! We also
    // get uniqueness for free!!!!!
    return (IgUIDRep *) (id);
}

void
IgUIDModel::add (IgUIDRep *rep)
{
    m_repSet.insert (rep->id ());    
}

IgUIDRep *
IgUIDModel::remove (IgUIDRep::IDType id)
{
    IgUIDRep *rep = this->lookup (id);
    m_repSet.erase (id);
    return rep;    
}

IgUIDModel::RepSet::iterator
IgUIDModel::begin (void)
{
    return m_repSet.begin ();    
}

IgUIDModel::RepSet::iterator
IgUIDModel::end (void)
{
    return m_repSet.end ();    
}


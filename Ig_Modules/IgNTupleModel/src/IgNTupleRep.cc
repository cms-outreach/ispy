//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgNTupleModel/interface/IgNTupleRep.h"
#include "Ig_Modules/IgNTupleModel/interface/IgNTupleModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgNTupleRep::IgNTupleRep (IgNTupleModel *model)
    : IgRep (),
      m_model (model),
      m_context (0)
{
}

void
IgNTupleRep::copy (const IgNTupleRep &source)
{
    m_rows = source.m_rows;    
}


void
IgNTupleRep::addRow (const Row& row)
{    
    m_rows.push_back (row);
}

void
IgNTupleRep::clear (void)
{
    m_rows.resize (0);
}

const IgNTupleRep::Row &
IgNTupleRep::getRow (unsigned int row) const
{
    return m_rows[row];
}

double
IgNTupleRep::valueAt (unsigned int row, 
			unsigned int column) const
{
    return m_rows[row][column];    
}

bool
IgNTupleRep::empty (void) const
{
    if (m_rows.size () > 0)
	return false;
    return true;    
}

IgRepContext *
IgNTupleRep::context (void) const
{
    return m_context;    
}

void
IgNTupleRep::context (IgRepContext *context)
{
    m_context = context;    
}

IgModel *
IgNTupleRep::model (void) const
{
    return m_model;
}

void
IgNTupleRep::columnNames (const ColumnNames &names)
{
    int j = 0;
    m_columnNames.resize (names.size ());
    
    for (ColumnNames::const_iterator i = names.begin ();
	 i != names.end ();	
	 i++)
    {
	m_columnNames[j] = *i;	
	m_columnNamesMap[*i] = j;
	j++;	
    }
}

const IgNTupleRep::ColumnNames &
IgNTupleRep::columnNames (void) const
{
    return m_columnNames;    
}

int
IgNTupleRep::rows (void) const
{
    return m_rows.size ();    
}

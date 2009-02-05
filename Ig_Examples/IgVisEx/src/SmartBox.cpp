// SmartBox.cpp: implementation of the SmartBox class.
//
//////////////////////////////////////////////////////////////////////
#include "Ig_Examples/IgVisEx/interface/SmartBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SmartBox::SmartBox( int data ) : m_data( data )
{
}

void SmartBox::setData( int data )
{
	m_data = data;
}

void SmartBox::print( std::ostream & o ) const
{
	o << "Data = " << m_data << std::endl;
}

SmartBox::~SmartBox()
{
}











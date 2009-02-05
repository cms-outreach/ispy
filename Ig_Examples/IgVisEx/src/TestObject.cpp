// TestObject.cpp: implementation of the TestObject class.
//
//////////////////////////////////////////////////////////////////////
#include "Ig_Examples/IgVisEx/interface/TestObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TestObject::TestObject( int data ) : m_data( data )
{
}

TestObject::~TestObject()
{
}

void TestObject::setData( int data )
{
	m_data = data;
}

int TestObject::getData() const
{
	return m_data;
}

void TestObject::print( std::ostream & o ) const
{
	o << "TestObject data = " << m_data << std::endl;
}







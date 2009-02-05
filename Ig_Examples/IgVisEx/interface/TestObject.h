// TestObject.h: interface for the TestObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTOBJECT_H__9ED60380_5764_4EB9_BFE3_EEA4E60C9C84__INCLUDED_)
#define AFX_TESTOBJECT_H__9ED60380_5764_4EB9_BFE3_EEA4E60C9C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Examples/IgVisEx/interface/config.h"
#include <iostream>

/*!	\class TestObject TestObject.h TestObject.h
 *
 *  \brief An example smart object for use as a test/demo.
 *   \see{SmartBox} for a different approach.
 *   
 *   \author G. Alverson, Northeastern University
 *   \date 08-Sep-2000
 */

class TestObject  
{
public:
	//! c-tor takes a piece of data
	TestObject(int data=0);
	//! set the data
	void setData(int data);
	//! d-tor
	virtual ~TestObject();
	//! returns the data
	int getData(void) const;
	//! extractor for TestObject
    friend std::ostream &	operator<< (std::ostream &os, const TestObject &obj);
protected:
	//! used by the extractor; actually does the output formatting
	virtual void print(std::ostream& o) const;
private:
	//! the data
	int m_data;
};
inline std::ostream &
operator<< (std::ostream &os, const TestObject &obj)
{
    obj.print (os);
    return os;
}

#endif // !defined(AFX_TESTOBJECT_H__9ED60380_5764_4EB9_BFE3_EEA4E60C9C84__INCLUDED_)

#if !defined(AFX_SMARTBOX_H__F901C659_646E_4513_B55A_9DAF6062D7DC__INCLUDED_)
#define AFX_SMARTBOX_H__F901C659_646E_4513_B55A_9DAF6062D7DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*! \class SmartBox SmartBox.h SmartBox.h
 *
 *  \brief A test class consisting of an SoSphere with additional information
 *  and an extractor method (over-ridden from BaseQuery)
 *
 *  \author G. Alverson, Northeastern University  Physics
 *  \date 27-Aug-2000
 */

#include "Ig_Examples/IgVisEx/interface/config.h"
#include <Inventor/nodes/SoSphere.h>
#include "Ig_Examples/IgVisEx/interface/BaseQueryNode.h"

class SmartBox : public SoSphere, public BaseQueryNode  
{
public:
	//! C-tor takes a piece of data to remember
	SmartBox(int data);
	//! d-tor
	virtual ~SmartBox();
	//! retrieve a copy of the stored data
	int getData() const {return m_data;}
	//! set the value of the stored data
	void setData(int data);
protected:
	//! this is the method is over-ridden from BaseQueryNode to supply custom output
	virtual void print(std::ostream& o) const;
private:
	//! the data
	int m_data;
};

#endif // !defined(AFX_SMARTBOX_H__F901C659_646E_4513_B55A_9DAF6062D7DC__INCLUDED_)

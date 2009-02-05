// TestTrack.h: interface for the TestTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTTrack_H__9ED60380_5764_4EB9_BFE3_EEA4E60C9C84__INCLUDED_)
#define AFX_TESTTrack_H__9ED60380_5764_4EB9_BFE3_EEA4E60C9C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Examples/IgVisEx/interface/config.h"
#include <iostream>
#include <functional>

/*!	\class TestTrack TestTrack.h TestTrack.h
 *
 *  \brief An example smart Track for use as a test/demo.
 *   
 *   \author G. Alverson, Northeastern University
 *   \date 24-Feb-2002
 */

class TestTrack  
{
public:
	//! blank c-tor required for containers
	TestTrack(void);
	//! c-tor takes a four-vector; assume origin at zero
	TestTrack(double px, double py, double pz, double E);
	//! d-tor
	virtual ~TestTrack();
	//! extractor for TestTrack
    friend std::ostream &	operator<< (std::ostream &os, const TestTrack &obj);
	//! calculate the invariant mass of a pair of tracks
	static double invMass(const TestTrack& t1, const TestTrack& t2);
	double pX() const {return px;}
	double pY() const {return py;}
	double pZ() const {return pz;}
	double pE() const {return E;}
protected:
	//! used by the extractor; actually does the output formatting
	virtual void print(std::ostream& o) const;
private:
	//! the data
	double px;
	double py;
	double pz;
	double E;
};
inline std::ostream &
operator<< (std::ostream &os, const TestTrack &obj)
{
    obj.print (os);
    return os;
}

#endif // !defined(AFX_TESTTrack_H__9ED60380_5764_4EB9_BFE3_EEA4E60C9C84__INCLUDED_)

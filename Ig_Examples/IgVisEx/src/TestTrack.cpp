// TestTrack.cpp: implementation of the TestTrack class.
//
//////////////////////////////////////////////////////////////////////
#include "Ig_Examples/IgVisEx/interface/TestTrack.h"

#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TestTrack::TestTrack() : px(0.F), py(0.F), pz(0.F), E(0.F)
{
}
TestTrack::~TestTrack()
{
}
TestTrack::TestTrack(double x, double y, double z, double e) : px(x), py(y), pz(z), E(e)
{
}

void TestTrack::print( std::ostream & o ) const
{
	o << "TestTrack: px/py/pz/E = " << px << ", " << py << ", "<< pz << ", "<< E << std::endl;
}
double TestTrack::invMass(const TestTrack& t1, const TestTrack& t2)
{
	double d = (t1.E + t2.E)*(t1.E + t2.E) 
		-(t1.px + t2.px)*(t1.px + t2.px)
		-(t1.py + t2.py)*(t1.py + t2.py)
		-(t1.pz + t2.pz)*(t1.pz + t2.pz);
	if (d>=0)
	{
		return sqrt(d);
	}
	else
	{
		return 0;
	}
}
	






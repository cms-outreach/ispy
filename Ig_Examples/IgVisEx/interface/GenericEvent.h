#if !defined GENERIC_EVENT_H
#define GENERIC_EVENT_H

// This header file contains material to construct a generic event
#include "Ig_Modules/IgVis/interface/IgQtTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtLiveTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtVectorTwig.h"

#include "Ig_Examples/IgVisEx/interface/TestTrack.h"

#include <list>

class Event : public IgQtTwig
{
public:
	Event();
	void drawMe();
	virtual ~Event();
};
/////////////////////////////
class TrackTwig : public IgQtLiveTwig<TestTrack>
{
public:
	explicit TrackTwig (SbString name, bool visibility = 1, bool cascade = 1);
	virtual void drawMe();
	~TrackTwig();
private:
	//! list of internal objects we own
	std::list<TestTrack> *p_list;
};




#endif // GENERIC_EVENT_H

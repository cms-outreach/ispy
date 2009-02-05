//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisEvent.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <sys/time.h> 
#include <qstring.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace edm::service;

VisEvent::VisEvent (const edm::ParameterSet& iPSet)
{}

void 
VisEvent::analyze( const edm::Event& event, const edm::EventSetup& /* eventSetup */)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisEvent requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    const edm::Timestamp time = event.time ();

    timeval eventTime;
    eventTime.tv_sec = time.value () >> 32;
    eventTime.tv_usec = 0xFFFFFFFF & time.value ();

    boost::posix_time::ptime bt0 = boost::posix_time::from_time_t(0);
    boost::posix_time::ptime bt = bt0 + boost::posix_time::seconds(eventTime.tv_sec)
      + boost::posix_time::microseconds(eventTime.tv_usec);

    std::stringstream oss;
    oss << bt;
    std::string contents (oss.str ());

    std::string statusText = (QString ("%1 GMT: Run %2, Event %3, LS %4, Orbit %5, BX %6")
			      .arg (contents.c_str ())
			      .arg (event.id ().run ())
			      .arg (event.id ().event ())
			      .arg (event.luminosityBlock ())
			      .arg (event.orbitNumber ())
			      .arg (event.bunchCrossing ())
			      .latin1 ());

    IgDataStorage *storage = config->storage ();
    std::string text = QString ("Event_V1");
    IgCollection &eventColl = storage->getCollection (text.c_str ());

    IgProperty RUN = eventColl.addProperty ("run", static_cast<int>(0));
    IgProperty EVENT = eventColl.addProperty ("event", static_cast<int>(0));
    IgProperty LS = eventColl.addProperty ("ls", static_cast<int>(0));
    IgProperty ORBIT = eventColl.addProperty ("orbit", static_cast<int>(0));
    IgProperty BX = eventColl.addProperty ("bx", static_cast<int>(0));
    IgProperty TIME = eventColl.addProperty ("time", std::string ());    

    IgCollectionItem eventId = eventColl.create();
    eventId ["run"] = static_cast<int>(event.id ().run ());
    eventId ["event"] = static_cast<int>(event.id ().event ());
    eventId ["ls"] = static_cast<int>(event.luminosityBlock ());
    eventId ["orbit"] = static_cast<int>(event.orbitNumber ());
    eventId ["bx"] = static_cast<int>(event.bunchCrossing ());
    eventId ["time"] = static_cast<std::string>(contents);
}

DEFINE_FWK_MODULE(VisEvent);

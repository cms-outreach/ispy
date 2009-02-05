#ifndef ANALYZER_VIS_EVENT_H
# define ANALYZER_VIS_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "FWCore/Framework/interface/EDAnalyzer.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEvent : public edm::EDAnalyzer
{
public:
    explicit VisEvent (const edm::ParameterSet&);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual ~VisEvent (void) {}

    virtual void analyze( const edm::Event&, const edm::EventSetup& );
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // ANALYZER_VIS_EVENT_H

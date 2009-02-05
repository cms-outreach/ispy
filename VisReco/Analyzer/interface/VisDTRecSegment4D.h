#ifndef ANALYZER_VIS_DTREC_SEGMENT_4D_H
# define ANALYZER_VIS_DTREC_SEGMENT_4D_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "FWCore/Framework/interface/EDAnalyzer.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisDTRecSegment4D : public edm::EDAnalyzer
{
public:
    explicit VisDTRecSegment4D (const edm::ParameterSet&);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual ~VisDTRecSegment4D (void) {}
  
    virtual void analyze (const edm::Event&, const edm::EventSetup&);

private:
    std::string  	friendlyName_;
    std::string  	moduleLabel_;
    std::string  	instanceName_;
    std::string 	processName_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // ANALYZER_VIS_DTREC_SEGMENT_4D_H

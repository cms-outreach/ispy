#ifndef ANALYZER_VIS_SI_PIXEL_REC_HIT_H
# define ANALYZER_VIS_SI_PIXEL_REC_HIT_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "FWCore/Framework/interface/EDAnalyzer.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSiPixelRecHit : public edm::EDAnalyzer
{
public:
    explicit VisSiPixelRecHit (const edm::ParameterSet& iPSet);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual ~VisSiPixelRecHit (void) {}
  
    virtual void analyze( const edm::Event&, const edm::EventSetup& );

private:
    std::string  	friendlyName_;
    std::string  	moduleLabel_;
    std::string  	instanceName_;
    std::string 	processName_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // ANALYZER_VIS_SI_PIXEL_REC_HIT_H

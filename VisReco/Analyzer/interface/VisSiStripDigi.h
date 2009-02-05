#ifndef ANALYZER_VIS_SI_STRIP_DIGI_H
# define ANALYZER_VIS_SI_STRIP_DIGI_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "FWCore/Framework/interface/EDAnalyzer.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSiStripDigi : public edm::EDAnalyzer
{
public:
    explicit VisSiStripDigi (const edm::ParameterSet&);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual ~VisSiStripDigi (void) {}
  
    virtual void analyze( const edm::Event&, const edm::EventSetup& );

private:
    std::string  	friendlyName_;
    std::string  	moduleLabel_;
    std::string  	instanceName_;
    std::string 	processName_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // ANALYZER_VIS_SI_STRIP_DIGI_H

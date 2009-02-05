#ifndef ANALYZER_VIS_PFCLUSTER_H
#define ANALYZER_VIS_PFCLUSTER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"

class VisPFCluster : public edm::EDAnalyzer
{
public:
    VisPFCluster(const edm::ParameterSet&);
    virtual ~VisPFCluster() 
	{}
    
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

private:
    std::string friendlyName_;
    std::string moduleLabel_;
    std::string instanceName_;
    std::string processName_;
};

#endif 

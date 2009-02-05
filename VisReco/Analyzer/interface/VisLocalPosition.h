#ifndef ANALYZER_VIS_LOCAL_POSITION_H
# define ANALYZER_VIS_LOCAL_POSITION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "DataFormats/GeometryVector/interface/LocalPoint.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class TrackingRecHit;
class TrackingGeometry;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisLocalPosition
{
public:
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    static LocalPoint 	localPosition(const TrackingRecHit * rechit, const TrackingGeometry * geometry);

private:    
    static void	     	localPositions(const TrackingRecHit* rechit, const TrackingGeometry* geometry, std::vector<LocalPoint>& points);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // ANALYZER_VIS_LOCAL_POSITION_H

#ifndef VIEW_XTYPEINFO_H
# define VIEW_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"
# include "classlib/utils/XTypeInfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IViewQueuedTwig;
class IViewDTRecSegment4DTwig;
class IViewEventTwig;
class IViewTrackTwig;
class IViewSiStripDigiTwig;
class IViewTrackingRecHitTwig;
class IViewMuonTwig;

class Ig3DModel;
class Ig3DRep;
class IgLegoModel;
class IgLegoRep;
class IgRPhiModel;
class IgRPhiRep;
class IgRZModel;
class IgRZRep;
class IgTextRep;
class IgSimpleTextRep;
class IgTextModel;
class IgSimpleTextModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, IViewQueuedTwig);
XTYPEINFO_DECLARE (, IViewDTRecSegment4DTwig);
XTYPEINFO_DECLARE (, IViewEventTwig);
XTYPEINFO_DECLARE (, IViewTrackTwig);
XTYPEINFO_DECLARE (, IViewSiStripDigiTwig);
XTYPEINFO_DECLARE (, IViewTrackingRecHitTwig);
XTYPEINFO_DECLARE (, IViewMuonTwig);

XTYPEINFO_DECLARE (, Ig3DModel);
XTYPEINFO_DECLARE (, Ig3DRep);
XTYPEINFO_DECLARE (, IgLegoModel);
XTYPEINFO_DECLARE (, IgLegoRep);
XTYPEINFO_DECLARE (, IgRPhiModel);
XTYPEINFO_DECLARE (, IgRPhiRep);
XTYPEINFO_DECLARE (, IgRZModel);
XTYPEINFO_DECLARE (, IgRZRep);
XTYPEINFO_DECLARE (, IgTextRep);
XTYPEINFO_DECLARE (, IgSimpleTextRep);
XTYPEINFO_DECLARE (, IgTextModel);
XTYPEINFO_DECLARE (, IgSimpleTextModel);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_XTYPEINFO_H

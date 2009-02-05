#ifndef VIEW_IVIEW_RECO_CONTENT_H
# define VIEW_IVIEW_RECO_CONTENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/View/interface/IViewContent.h"
# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IViewRecoContent : public IViewContent
{
    IG_DECLARE_STATE_ELEMENT (IViewRecoContent);
public:
    IViewRecoContent (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private:
    void                init (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_RECO_CONTENT_H

#ifndef VIEW_IVIEW_RECO_CONTENT_H
# define VIEW_IVIEW_RECO_CONTENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/ISpyContent.h"
# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API ISpyRecoContent : public ISpyContent
{
    IG_DECLARE_STATE_ELEMENT (ISpyRecoContent);
public:
    ISpyRecoContent (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private:
    void                init (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_RECO_CONTENT_H

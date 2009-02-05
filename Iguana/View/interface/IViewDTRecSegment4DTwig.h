#ifndef VIEW_IVIEW_DTREC_SEGMENT_4D_TWIG_H
# define VIEW_IVIEW_DTREC_SEGMENT_4D_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/View/interface/IViewQueuedTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IViewDTRecSegment4DTwig : public IViewQueuedTwig
{
public:
    IViewDTRecSegment4DTwig (IgState *state, IgTwig *parent, 
			     const std::string &name = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void  onNewEvent (IViewEventMessage& message);

    using IViewQueuedTwig::update;
    virtual void  update (Ig3DRep *rep);
    virtual void  update (IgLegoRep *rep);
    virtual void  update (IgTextRep *rep);
    virtual void  update (IgRPhiRep *rep);
    virtual void  update (IgRZRep *rep);

private:    
    std::string	      	m_text;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_DTREC_SEGMENT_4D_TWIG_H

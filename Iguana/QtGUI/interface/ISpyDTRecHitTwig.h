#ifndef VIEW_IVIEW_DTRECHIT_TWIG_H
#define VIEW_IVIEW_DTRECHIT_TWIG_H

#include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"

class ISpyDTRecHitTwig : public ISpyQueuedTwig
{
public:
    ISpyDTRecHitTwig (IgState *state, IgTwig *parent,const std::string &name = ""); 
    virtual void  onNewEvent (ISpyEventMessage& message);
};

#endif 

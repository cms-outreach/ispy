#ifndef VIEW_IVIEW_DTDIGI_TWIG_H
#define VIEW_IVIEW_DTDIGI_TWIG_H

#include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"

class ISpyDTDigiTwig : public ISpyQueuedTwig
{
public:
    ISpyDTDigiTwig (IgState *state, IgTwig *parent,const std::string &name = ""); 
    virtual void  onNewEvent (ISpyEventMessage& message);
};

#endif 

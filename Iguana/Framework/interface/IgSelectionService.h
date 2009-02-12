#ifndef IGUANA_FRAMEWORK_IG_SELECTION_SERVICE_H
# define IGUANA_FRAMEWORK_IG_SELECTION_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgDispatcher.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSelectionMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A manager for the current object selection in a session group.

  #IgSelectionService manages the current object selection in the
  browsers forming a session group.  All browsers in the group should
  register with the service, and notify it when the selection changes.

  FIXME: management of several independent selections?  */
class IGUANA_FRAMEWORK_API IgSelectionService
    : public IgStateElement, public IgDispatcher<IgSelectionMessage>
{
    IG_DECLARE_STATE_ELEMENT (IgSelectionService);
public:
    IgSelectionService (IgState *state);

private:
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_SELECTION_SERVICE_H

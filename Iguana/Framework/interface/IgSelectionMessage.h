#ifndef IGUANA_FRAMEWORK_IG_SELECTION_MESSAGE_H
# define IGUANA_FRAMEWORK_IG_SELECTION_MESSAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepContext;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A message notifying that an object has been selected in a browser.

  The #IgRepContext carried by the message points to the #IgRep that
  originated the event or null if the current selection was cleared.
  The observers can use it to figure out which object was selected and
  find (or create) their own representations for that object.  Start
  traversal from this #IgRep to look for other representations, or
  create a new one if no suitable one exists.  (Usually it is simplest
  to just pass the context to the appropriate #IgRepSet::lookup()
  method.)  */
class IGUANA_FRAMEWORK_API IgSelectionMessage
{
public:
    // FIXME: selection detail
    // FIXME: multiple selection
    IgSelectionMessage (IgRepContext *selection);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    IgRepContext *		context (void) const;

private:
    /// The #IgRep of the selected object.
    IgRepContext *		m_context;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Get the context of the #IgRep that originates this message. */
inline
IgSelectionMessage::IgSelectionMessage (IgRepContext *context)
    : m_context (context)
{}

/** Get the context of the #IgRep that originates this message. */
inline IgRepContext *
IgSelectionMessage::context (void) const
{ return m_context; }

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_SELECTION_MESSAGE_H

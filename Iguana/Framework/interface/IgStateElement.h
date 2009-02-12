#ifndef IGUANA_FRAMEWORK_IG_STATE_ELEMENT_H
# define IGUANA_FRAMEWORK_IG_STATE_ELEMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateKey.h"
# include "Iguana/Framework/interface/IgState.h"
# include "classlib/utils/DebugAids.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# define IG_DEFINE_STATE_ELEMENT(t,name)		\
  t *t::get (IgState *state) {				\
     ASSERT (state);					\
     if (IgStateElement *e = state->get (s_key)) {	\
        ASSERT (dynamic_cast<t *> (e));			\
        return static_cast<t *> (e);			\
     }							\
     return 0;						\
  }							\
  IgStateKey t::stateKey (void) const { return s_key; }	\
  const char *t::stateLabel (void) { return s_keyLabel;}\
  const char *t::catalogLabel (void){return s_keyLabel;}\
  const char *t::s_keyLabel = name;			\
  IgStateKeyDef t::s_key (name)

# define IG_DECLARE_STATE_ELEMENT(t)			\
 private:						\
  static const char    *s_keyLabel;			\
  static IgStateKeyDef s_key;				\
 public:						\
  static t *get (IgState *state);			\
  static const char *stateLabel (void);			\
  static const char *catalogLabel (void);		\
  virtual IgStateKey stateKey (void) const

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base interface for all element objects stored in #IgState.  */
class IGUANA_FRAMEWORK_API IgStateElement
{
public:
    virtual ~IgStateElement (void);

    /** Return the index key associated with this element.  */
    virtual IgStateKey	stateKey (void) const = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_STATE_ELEMENT_H

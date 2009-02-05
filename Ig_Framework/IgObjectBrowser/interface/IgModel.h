#ifndef IG_OBJECT_BROWSER_IG_MODEL_H
# define IG_OBJECT_BROWSER_IG_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Interface for #IgBrowser data models.

  The only reason for the existence of this interface is the need for
  a polymorphic creation of a #IgRep in specific browser models for
  application domain objects.  In other words, the sole purpose of
  this class is to provide a way to select the type of the #IgRep
  created.  The creation is done using the
  IgBrowserMethods::represent() multi-method.

  There are no requirements on how the model should manage its
  representations.  It should simply inherit from this class and
  provide the methods to interact with representations.  */
class IG_OBJECT_BROWSER_API IgModel
{
public:
    virtual ~IgModel (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_MODEL_H

#ifndef IGUANA_FRAMEWORK_IG_REPRESENTABLE_H
# define IGUANA_FRAMEWORK_IG_REPRESENTABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for all representable application objects.

  This is the base class that all application objects must inherit
  from the participate in the object browser architecture.  It
  specifies no functionality, it merely guarantees one polymorphic
  base class for application objects.

  This class could be replaced with a class from the application
  domain fulfilling the same requirements: a polymorphic base shared
  by all participating objects.  In general it is simpler and more
  flexible to use proxy objects to don these magic capabilities to
  application objects.  In other words, we advice against making
  actual application objects inherit from this class.  */
class IGUANA_FRAMEWORK_API IgRepresentable
{
public:
    virtual ~IgRepresentable (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_REPRESENTABLE_H

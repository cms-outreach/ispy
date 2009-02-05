#ifndef IG_OBJECT_BROWSER_IG_EXTENSION_H
# define IG_OBJECT_BROWSER_IG_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for application extensions.

    This is the base class for application extensions.  It defines no
    functionality for the inheriting class, just to give them a name
    and polymorphic nature.  Extension are normally be created through
    a #IgExtensionFactory, which will give the inheriting constructor
    an #IgState object as an argument.  The general purpose for this
    class is to allow clients to load extensions dynamically with only
    knowing their name but not anything more specific.

    Extensions should have one name for both the plug-in catalog and
    state keys and the constructor should register the extension with
    the state it is given.  This is required for the instantiation
    logic to know not to create multiple copies of the extension.  */
class IG_OBJECT_BROWSER_API IgExtension : public IgStateElement {};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_EXTENSION_H

#ifndef IG_STUDIO_IG_STUDIO_SETUP_EXTENSION_H
# define IG_STUDIO_IG_STUDIO_SETUP_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgStudioSetupExtension : public IgExtension
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool	setup (IgStudioDriver *into);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_STUDIO_SETUP_EXTENSION_H

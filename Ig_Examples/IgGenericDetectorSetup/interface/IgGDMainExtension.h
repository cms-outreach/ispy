#ifndef IG_GENERIC_DETECTOR_SETUP_IG_GD_MAIN_EXTENSION_H
# define IG_GENERIC_DETECTOR_SETUP_IG_GD_MAIN_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgGenericDetectorSetup/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgGDSetup;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_GENERIC_DETECTOR_SETUP_API IgGDMainExtension : public IgExtension
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool	setup (IgGDSetup *from);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_GENERIC_DETECTOR_SETUP_IG_GD_MAIN_EXTENSION_H

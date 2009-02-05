#ifndef IG_NTUPLE_APP_IG_NTUPLE_APP_EXTENSION_H
# define IG_NTUPLE_APP_IG_NTUPLE_APP_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleApp/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgNTupleAppSetup;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_NTUPLE_APP_API IgNTupleAppExtension : public IgExtension
{
public:
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual bool	setup (IgNTupleAppSetup *setup);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_NTUPLE_APP_IG_NTUPLE_APP_EXTENSION_H

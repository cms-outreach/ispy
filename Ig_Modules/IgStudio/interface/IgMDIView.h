#ifndef IG_STUDIO_IG_MDI_VIEW_H
# define IG_STUDIO_IG_MDI_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgMDIView
{
public:
    IgMDIView (IgPage *page, const char *window = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_MDI_VIEW_H

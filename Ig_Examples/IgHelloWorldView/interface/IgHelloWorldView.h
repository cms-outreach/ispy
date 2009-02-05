#ifndef IG_HELLO_WORLD_VIEW_IG_HELLO_WORLD_VIEW_H
# define IG_HELLO_WORLD_VIEW_IG_HELLO_WORLD_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgHelloWorldView/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgView.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_HELLO_WORLD_VIEW_API IgHelloWorldView : public IgView
{
public:
    IgHelloWorldView (IgPage *page);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_HELLO_WORLD_VIEW_IG_HELLO_WORLD_VIEW_H

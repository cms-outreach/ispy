#ifndef IG_3D_BROWSER_IG_3D_BROWSER_H
# define IG_3D_BROWSER_IG_3D_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBrowser/interface/config.h"
# include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BROWSER_API Ig3DBrowser : public Ig3DBaseBrowser
{
public:
    Ig3DBrowser (IgState *state, IgSite *site, Ig3DModel *model = 0);

    static const char *		catalogLabel (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BROWSER_IG_3D_BROWSER_H

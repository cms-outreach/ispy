#ifndef IG_STUDIO_IG_VIEW_DB_H
# define IG_STUDIO_IG_VIEW_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginInfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgView;
class IgPluginInfo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgViewDB : public IgPluginFactory<IgView * (IgPage *)>
{
public:
    static IgViewDB *get (void);

private:
    IgViewDB (void);
    static IgViewDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_SITE_DB_H

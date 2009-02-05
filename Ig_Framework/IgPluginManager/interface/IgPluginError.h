#ifndef IG_PLUGIN_MANAGER_IG_PLUGIN_ERROR_H
# define IG_PLUGIN_MANAGER_IG_PLUGIN_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgPluginManager/interface/config.h"
# include "classlib/utils/Error.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Error object for the plug-in manager.  All lower-level errors
    are chained to this type before being passed upwards.  */
class IgPluginError : public lat::Error
{
public:
    IgPluginError (lat::Error *reason);
    
    virtual std::string	explainSelf (void) const;
    virtual lat::Error *clone (void) const;
    virtual void	rethrow (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_IG_PLUGIN_ERROR_H

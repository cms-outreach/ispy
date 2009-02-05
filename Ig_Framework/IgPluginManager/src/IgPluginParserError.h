#ifndef IG_PLUGIN_MANAGER_IG_PLUGIN_PARSER_ERROR_H
# define IG_PLUGIN_MANAGER_IG_PLUGIN_PARSER_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgPluginManager/interface/config.h"
# include "classlib/utils/Error.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_PLUGIN_MANAGER_API IgPluginParserError : public lat::Error
{
public:
    IgPluginParserError (const std::string &reason);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual std::string	explainSelf (void) const;
    virtual lat::Error *clone (void) const;
    virtual void	rethrow (void);

private:
    std::string		m_reason;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_IG_PLUGIN_PARSER_ERROR_H

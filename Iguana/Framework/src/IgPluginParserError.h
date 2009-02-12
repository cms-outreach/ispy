#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_PARSER_ERROR_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_PARSER_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "classlib/utils/Error.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgPluginParserError : public lat::Error
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

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_PARSER_ERROR_H

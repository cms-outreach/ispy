#ifndef IG_TEXT_MODEL_IG_TEXT_REP_H
# define IG_TEXT_MODEL_IG_TEXT_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTextModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTextModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEXT_MODEL_API IgTextRep : public IgRep
{
public:
    ~IgTextRep (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual const std::string &	text (void) const = 0;
    virtual void		setText (const std::string &text) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEXT_MODEL_IG_TEXT_REP_H

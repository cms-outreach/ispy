#ifndef VIEW_IG_SIMPLE_TEXT_REP_H
# define VIEW_IG_SIMPLE_TEXT_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/View/interface/IgTextRep.h"
# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTextModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgSimpleTextRep : public IgTextRep
{
public:
    IgSimpleTextRep (IgTextModel *model, const std::string &text);
    ~IgSimpleTextRep (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    virtual const std::string &	text (void) const;
    virtual void		setText (const std::string &text);

protected:
    virtual void		context (IgRepContext *context);

private:
    IgTextModel *		m_model;
    IgRepContext *		m_context;
    std::string			m_text;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_SIMPLE_TEXT_REP_H

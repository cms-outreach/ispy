#ifndef IG_NTUPLE_APP_IG_NTUPLE_APP_MAIN_H
# define IG_NTUPLE_APP_IG_NTUPLE_APP_MAIN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleApp/interface/config.h"
# include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppExtension.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgNTupleAppSetup;
class IgStudioDriver;
class IgDocument;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_NTUPLE_APP_API IgNTupleAppMain : public IgNTupleAppExtension
{
    IG_DECLARE_STATE_ELEMENT (IgNTupleAppMain);    
public:
    IgNTupleAppMain (IgState *state);
    ~IgNTupleAppMain (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual bool setup (IgNTupleAppSetup *setup);
private:
    IgState * 			m_state;    
    IgStudioDriver *		m_studio;
    IgDocument *		m_document;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_NTUPLE_APP_IG_NTUPLE_APP_MAIN_H

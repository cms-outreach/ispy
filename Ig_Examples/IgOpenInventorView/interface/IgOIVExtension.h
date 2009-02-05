#ifndef IG_OPEN_INVENTOR_VIEW_IG_OIV_EXTENSION_H
# define IG_OPEN_INVENTOR_VIEW_IG_OIV_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgOpenInventorView/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDocument;
class IgState;
class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_VIEW_API IgOIVExtension : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgOIVExtension);
public:
    IgOIVExtension (IgState *state);
    // implicit copy constructor
    // implicit assignment operator

    virtual bool	setup (IgStudioDriver *into);

private:
    IgState 		*m_state;
    IgDocument		*m_document;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_VIEW_IG_OIV_EXTENSION_H

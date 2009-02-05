#ifndef IG_QT_DEMO_IG_QT_DEMO_EXTENSION_H
# define IG_QT_DEMO_IG_QT_DEMO_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgQTDemo/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;
class IgQTDemo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_DEMO_EXTENSION_API IgQTDemoExtension : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgQTDemoExtension);
public:
    IgQTDemoExtension (IgState *state);
    ~IgQTDemoExtension (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual bool	setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgQTDemo		*m_qtDemo;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_DEMO_IG_QT_DEMO_EXTENSION_H

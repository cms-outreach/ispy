#ifndef IG_OPEN_INVENTOR_DEMO_IG_OIVDEMO_EXTENSION_H
# define IG_OPEN_INVENTOR_DEMO_IG_OIVDEMO_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgOpenInventorDemo/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;
class IgOIVDemo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_DEMO_API IgOIVDemoExtension : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgOIVDemoExtension);
public:
    IgOIVDemoExtension (IgState *state);
    ~IgOIVDemoExtension (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool	setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgOIVDemo		*m_oivDemo;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_DEMO_IG_OIVDEMO_EXTENSION_H

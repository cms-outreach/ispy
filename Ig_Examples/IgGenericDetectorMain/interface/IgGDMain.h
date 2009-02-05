#ifndef IG_GENERIC_DETECTOR_MAIN_IG_GD_MAIN_H
# define IG_GENERIC_DETECTOR_MAIN_IG_GD_MAIN_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgGenericDetectorSetup/interface/IgGDMainExtension.h"


//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;
class IgDocument;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgGDMain : public IgGDMainExtension
{
    IG_DECLARE_STATE_ELEMENT (IgGDMain);
public:
     IgGDMain(IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool	setup (IgGDSetup *from);

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
    IgDocument		*m_document;
 };

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_GENERIC_DETECTOR_MAIN_IG_GD_MAIN_H

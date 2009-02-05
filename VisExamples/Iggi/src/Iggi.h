#ifndef IGGI_IGGI_H
# define IGGI_IGGI_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;
class IgDocument;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class Iggi : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (Iggi);
public:
    Iggi (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual bool	setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
    IgDocument		*m_document;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_H

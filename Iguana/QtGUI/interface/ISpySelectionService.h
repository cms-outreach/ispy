#ifndef QT_GUI_ISPY_SELECTION_SERVICE_H
# define QT_GUI_ISPY_SELECTION_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpySelectionService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (ISpySelectionService);
public:
    ISpySelectionService (IgState *state, SoNode *selection);
    ~ISpySelectionService (void);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual SoNode *		selection (void);

private:
    IgState			*m_state;
    SoNode			*m_selection;

    // undefined semantics
    ISpySelectionService (const ISpySelectionService &);
    ISpySelectionService &operator= (const ISpySelectionService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_SELECTION_SERVICE_H

#ifndef VIEW_IG_DRAW_FUNCTIONS_H
# define VIEW_IG_DRAW_FUNCTIONS_H

//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgDrawFunctions
{
public:
    IgDrawFunctions (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private:    
    void 	init (IgState *state);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_DRAW_FUNCTIONS_H

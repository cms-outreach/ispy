#ifndef IG_HELLO_WORLD_VIEW_IG_HELLO_WORLD_BUTTON_H
# define IG_HELLO_WORLD_VIEW_IG_HELLO_WORLD_BUTTON_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgHelloWorldView/interface/config.h"
# include <qpushbutton.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QWidget;
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_HELLO_WORLD_VIEW_API IgHelloWorldButton : public QPushButton
{
public:
    IgHelloWorldButton (QWidget *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_HELLO_WORLD_VIEW_IG_HELLO_WORLD_BUTTON_H

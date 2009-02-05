#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_SIMPLE_BROWSER_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_SIMPLE_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentable.h"
# include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"

# include <iostream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfSimpleBrowser
{
private:
    IgOProfRepresentable *m_representable;    
public:
    IgOProfSimpleBrowser (IgOProfRepresentable *representable);
    void dump(void);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_SIMPLE_BROWSER_H

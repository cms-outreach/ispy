#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_COUNTER_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_COUNTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgOProfOprofilerProc;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfCounter
{
public:
    /**Name of the counted quantity*/
    std::string m_name;
    /**Description of the counted quantity*/
    std::string m_description;
    /**Overflow for the counter*/
    int m_count;
    /**true if the counter is enable*/
    bool m_enabled;
    /**true if the counter profiles kernel as well*/
    bool m_profileKernel;
    /**true if the counter profiles userspace applications*/
    bool m_profileUser;
    /**Hardware mask for the counter*/
    int m_mask;

    int m_counter;

    IgOProfCounter (std::string name = "0", int count = 0,bool enabled = false, bool profileKernel = true, bool profileUser = true, int mask = 0, int counter= 0);    
    IgOProfCounter (IgOProfOprofilerProc *op, const char *name, int n);
    std::string repr (void);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_COUNTER_H

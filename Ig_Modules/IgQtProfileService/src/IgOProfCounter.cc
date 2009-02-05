//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfCounter.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfOprofilerProc.h"
#include <string>
#include <sstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfCounter::IgOProfCounter (std::string name,
				int count,
				bool enabled,
				bool profileKernel,
				bool profileUser,
				int mask,
				int counter)
    :m_name (name),
     m_count (count),
     m_enabled (enabled),
     m_profileKernel (profileKernel),
     m_profileUser (profileUser),
     m_mask (mask),
     m_counter (counter)
{
}
	
IgOProfCounter::IgOProfCounter (IgOProfOprofilerProc *op, 
				const char *name, 
				int n)
    :m_name (name),
     m_count (op->eventCount(n)),
     m_enabled (op->eventEnabled(n)),
     m_profileKernel (op->eventProfileKernel(n)),
     m_profileUser (op->eventProfileUser(n)),
     m_mask (op->eventMask(n)),
     m_counter (n)
{
}

std::string IgOProfCounter::repr (void)
{
    std::ostringstream t;

    t << "Name:" << m_name << "\n";		
    t << "Count:" << m_count << "\n";
    t << "Enabled:" << m_enabled << "\n";
    t << "Profile Kernel:" << m_profileKernel << "\n";
    t << "Mask" << m_mask << "\n";
    t << "Counter" << m_counter << "\n";
		
    t << std::endl;
		
    return t.str ();
}	


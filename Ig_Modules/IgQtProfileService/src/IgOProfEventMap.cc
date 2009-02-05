//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfEventMap.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfOprofilerProc.h"
#include "classlib/utils/Regexp.h"
#include "classlib/utils/StringOps.h"
#include "classlib/iobase/PipeCmd.h"
#include <iostream>

using namespace lat;

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static std::string
getInput (PipeCmd &cmd)
{
    std::string		result;
    char		buf [1024];
    IOSize		n;
    
    while ((n = cmd.read (buf, sizeof (buf))))
	result.append (buf, n);
    
    return result;
}

static int
myatoi (const std::string &str)
{
    int val = 0xdeadbeef;
    int n = sscanf (str.c_str (), "%d", &val);
    ASSERT (n == 1);
    return val;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfEvent::IgOProfEvent (int allowedCounter, 
			    int allowedArch, 
			    int id, 
			    int mask, 
			    std::string name, 
			    int minCount)
    :m_allowedCounter (allowedCounter),
     m_allowedArch (allowedArch),
     m_ID (id),
     m_mask (mask),
     m_name (name),
     m_minCount (minCount)
{	
}

IgOProfEventMap::IgOProfEventMap (IgOProfConfiguration *configuration)
    :m_configuration (configuration),
     m_dummyEventMap (false)
{
    m_cpuMapNameId["Pentium Pro"] = CPU_PPRO;
    m_cpuMapNameId["PII"] = CPU_PII;
    m_cpuMapNameId["PIII"] = CPU_PIII;
    m_cpuMapNameId["Athlon"] = CPU_ATHLON;
    m_cpuMapNameId["CPU with RTC device"] = CPU_RTC;
    m_cpuMapNameId["P4 / Xeon"] = CPU_P4;
    m_cpuMapNameId["P4 / Xeon with 2 hyper-threads"] = CPU_P4_HT2;
    m_cpuMapNameId["Hammer"] = CPU_HAMMER;
    m_cpuMapNameId["IA64"] = CPU_IA64;
    m_cpuMapNameId["Itanium"] = CPU_IA64_1;
    m_cpuMapNameId["Itanium 2"] = CPU_IA64_2;
    m_cpuMapNameId["Alpha EV4"] = CPU_AXP_EV4;
    m_cpuMapNameId["Alpha EV5"] = CPU_AXP_EV5;
    m_cpuMapNameId["Alpha EV6"] = CPU_AXP_EV6;
    m_cpuMapNameId["Alpha EV67"] = CPU_AXP_EV67;
    
    m_cpuMapIdName[CPU_PPRO] = "Pentium Pro";
    m_cpuMapIdName[CPU_PII] = "PII";
    m_cpuMapIdName[CPU_PIII] = "PIII";	    
    m_cpuMapIdName[CPU_ATHLON] = "Athlon";
    m_cpuMapIdName[CPU_RTC] = "CPU with RTC device";
    m_cpuMapIdName[CPU_P4] = "P4 / Xeon";
    m_cpuMapIdName[CPU_P4_HT2] = "P4 / Xeon with 2 hyper-threads";
    m_cpuMapIdName[CPU_HAMMER] = "Hammer";
    m_cpuMapIdName[CPU_IA64_1] = "Itanium";
    m_cpuMapIdName[CPU_IA64_2] = "Itanium 2";
    m_cpuMapIdName[CPU_AXP_EV4] = "Alpha EV4";
    m_cpuMapIdName[CPU_AXP_EV5] = "Alpha EV5";
    m_cpuMapIdName[CPU_AXP_EV6] = "Alpha EV6";	    
    m_cpuMapIdName[CPU_AXP_EV67] = "Alpha EV67";
        
    ASSERT (m_configuration != 0);
    
    if (m_configuration->m_noProfilerFound == false)
    {	
	// Grab "op_help" output
	PipeCmd		opHelpProc (m_configuration->m_opHelpExec, 
				    lat::IOFlags::OpenRead);
	std::string	opHelpOutput (getInput (opHelpProc));
    
	// Get all counter definitions
	StringList	lines (StringOps::split (opHelpOutput, "\n"));
	Regexp		counter ("^([A-Z][A-Z0-9_]+):[^:]+:\\s*([\\d\\s,]+|all)");
	RegexpMatch	match;
	
	// Get the mapping from events to allowed counters
	for (size_t i = 0; i < lines.size (); ++i)
	{
	    if (counter.match (lines [i], 0, 0, &match) && match.numMatches () == 3)
	    {
		std::string	event = match.matchString (lines [i], 1);
		std::string	counters = match.matchString (lines [i], 2);
		StringList	counterNums (StringOps::split (counters, ", "));
		int		mask = 0;
		int		cpuMask = -1;

		// Make a mask of the counters the event can use
		if (counters == "all")
		    mask = CTR_ALL;
		else
		    for (size_t j = 0; j < counterNums.size (); ++j)
			mask |= 1 << myatoi (counterNums [j]);

		// Get event id for the event
		PipeCmd		eventIdProc (m_configuration->m_opHelpExec
					     + " " + event, 
					     lat::IOFlags::OpenRead);
		int		eventId (myatoi (getInput (eventIdProc)));

		m_byName [event] = m_byId [eventId]
		    = new IgOProfEvent (mask, cpuMask, eventId, 0, event, 6000);
	    }
	}
    }
    else
    {
	m_dummyEventMap = true;	
    }    
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include <classlib/utils/Argz.h>
#include <classlib/iobase/PipeCmd.h>
#include <classlib/utils/Regexp.h>
#include <classlib/utils/RegexpMatch.h>
#include <classlib/iobase/IOSelector.h>
#include <classlib/utils/DebugAids.h>
#include <iostream>
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

// FIXME: the new version of opreport reports all the counter
// together. We need to patch the regular expression to handle this.

IgOProfRepresentable::IgOProfRepresentable (void)
    :m_configuration (IgOProfConfigurationSingleton::instance ()),
     m_bufferString (new std::string ("")),
     m_profileInfosRE (new lat::Regexp ("^\\s*([0-9e.+\\-\\s]+)\\s+(.+)\\n"))
{
    ASSERT (m_configuration != 0);
    ASSERT (m_bufferString != 0);
    m_itoaMap[1]="1"; m_itoaMap[2]="2"; m_itoaMap[3]="3"; m_itoaMap[4]="4";
    m_itoaMap[5]="5"; m_itoaMap[6]="6"; m_itoaMap[7]="7"; m_itoaMap[8]="8"; 
    m_itoaMap[9]="9"; m_itoaMap[0]="0";
}

void 
IgOProfRepresentable::requestUpdateProcessList (void)
{
    lat::IOSelector selector;    
    lat::PipeCmd updateProcess (m_configuration->m_opreportExec
				 + " -f -n | grep \"^[ ]\\{0,8\\}[0-9]\"",
				 lat::IOFlags::OpenRead);

    /*reset the buffer on new read*/
    *m_bufferString = "";

    updateProcess.setBlocking (false);
    
    selector.attach (&updateProcess, lat::IORead, 
		     CreateHook (this, &IgOProfRepresentable::updateProcessList));
    	
    while (selector.dispatch ());
    
    updateProcess.close ();
    updateProcess.wait ();
}


bool 
IgOProfRepresentable::updateProcessList (lat::IOSelectEvent *event)
{
    char buffer[8193];
    int bufferPosition = 0;    
    lat::IOSize size;
    lat::RegexpMatch match;
    
    ASSERT (event != 0);
    
    buffer[0]=0;

    while ((8192-bufferPosition != 0) 
	   && (size = event->source->read (&buffer[bufferPosition], 
					   8192 - bufferPosition)))
    {
	ASSERT (size > 0);
	ASSERT (size + bufferPosition <= 8192);	
	buffer[size+bufferPosition] = 0;
	bufferPosition += size;
    }

    *m_bufferString += buffer;

    if (*m_bufferString != "")
    {
	while (m_profileInfosRE->search (*m_bufferString, 0, 0, &match) != -1)
	{	
	    
	    ASSERT (match.matched () == true);
	    ASSERT (match.matched (1) == true);
	    ASSERT (match.matched (2) == true);
	   
	    std::string hits_string = 
		match.matchString (m_bufferString->c_str (), 1);

	    std::string name = 
		match.matchString (m_bufferString->c_str (), 2);

	    ASSERT (hits_string != "");
	    ASSERT (name != "");
	    
	    std::istringstream hits_stream (hits_string);
	    
	    int hits;
	    float percent;
	    
	    hits_stream >> hits;
	    hits_stream >> percent;

	    if (m_processMap.find (name) == m_processMap.end ())
	    {
		m_processMap[name] = new IgOProfProcessRepresentable (name);
	    }

	    IgOProfProcessRepresentable &currentProcess = *m_processMap[name];
    
	    if (((currentProcess.hits () != hits 
		  || currentProcess.upToDate () == false) 
		 && currentProcess.isExpanded () == true)) 
	    {
		m_processesToBeUpdated.push_back (&currentProcess);
		currentProcess.setUpToDate (true);		
	    }
	    
	    currentProcess.setHits (hits);
	    m_bufferString->erase (match.matchPos (), match.matchLength ());	    
	}
    }
    
    for (ProcessesToBeUpdatedContainer::iterator i = m_processesToBeUpdated.begin (); 
	 i != m_processesToBeUpdated.end (); 
	 i++ )
    {
	(*i)->requestUpdateSymbolList ();
    }
    
    m_processesToBeUpdated.clear ();    

    /*Keep on reading*/
    return (size == 0);    
}

IgOProfRepresentableSingleton::IgOProfRepresentableSingleton (void)
{
}

IgOProfRepresentableInterface *
IgOProfRepresentableSingleton::instance (void)
{
    static IgOProfRepresentableInterface *__instance = 0;

    if (__instance == 0)
    {
	if (IgOProfConfigurationSingleton::instance ()->m_remoteProfiling == true)
	{		    
	    __instance = new IgOProfNetworkRepresentable;
	} else
	{
	    __instance = new IgOProfRepresentable;
	}		
    }
    return __instance;	    
}    


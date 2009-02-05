//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"

#include <iostream>

#include <classlib/utils/Argz.h>
#include <classlib/iobase/PipeCmd.h>
#include <classlib/utils/Regexp.h>
#include <classlib/utils/RegexpMatch.h>
#include <classlib/iobase/IOSelector.h>
#include <classlib/utils/StringOps.h>
#include <classlib/utils/DebugAids.h>

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

// FIXME: the regular expression does not handle spaces in filename...
IgOProfProcessRepresentable::IgOProfProcessRepresentable (const std::string &name)
    :m_configuration (IgOProfConfigurationSingleton::instance ()),
     m_profileInfosRE (new lat::Regexp ("^([\\d.e+\\s\\-]+)\\s+(/[^\\s]+)\\s+([_a-zA-Z\\d.(][^\\n]*)\\n")),
     m_bufferString (new std::string ("")),
     m_bufferPosition (0),
     m_name (name),
     m_hits (0),
     m_isExpanded (false),
     m_upToDate (false)
{
    ASSERT (m_configuration);
    //FIXME: more than 9 counters breaks this workaround
    m_itoaMap[1]="1";m_itoaMap[2]="2";m_itoaMap[3]="3";m_itoaMap[4]="4";
    m_itoaMap[5]="5";m_itoaMap[6]="6";m_itoaMap[7]="7";m_itoaMap[8]="8";
    m_itoaMap[9]="9";m_itoaMap[0]="0";
}

bool 
IgOProfProcessRepresentable::updateSymbolList (lat::IOSelectEvent *event)
{
    char buffer[8193];
    lat::IOSize size;
    lat::RegexpMatch match;
    
    ASSERT(event != 0);

    buffer[0]=0;    
                
    while ((8192-m_bufferPosition) 
	   && (size = event->source->read (&buffer[m_bufferPosition], 
					   8192-m_bufferPosition)))
    {
	ASSERT (size > 0);
	ASSERT (m_bufferPosition+size <= 8192);	
	buffer[m_bufferPosition+size] = 0;
	m_bufferPosition += size;	
    }

    *m_bufferString += buffer;	
    m_bufferPosition = 0;
    
    if (*m_bufferString != "")
    {
	int hits= -1;
	
	while (m_profileInfosRE->search (*m_bufferString, 0, 0, &match) != -1 
	       && !(m_configuration->m_showZeros == true && hits == 0))
	{		 
	    ASSERT (match.matched() == true);	    
	    ASSERT (match.matched(1) == true);
	    ASSERT (match.matched(2) == true);
	    ASSERT (match.matched(3) == true);

	    std::string hitsString 
		= match.matchString(*m_bufferString, 1);
	    std::string library 
		= match.matchString(m_bufferString->c_str(), 2);
	    std::string name 
		= match.matchString(m_bufferString->c_str(), 3);

	    ASSERT (hitsString != "");
	    ASSERT (name != "");
	    	    
	    std::istringstream hitsStream (hitsString);
	    float percent;
	    
	    hitsStream >> hits;
	    hitsStream >> percent;

	    if (m_symbolMap.find (name+library) == m_symbolMap.end ())
	    {
		m_symbolMap[name+library] 
		    = new IgOProfSymbolRepresentable (name, library);
	    }
	    
	    m_symbolMap[name+library]->setHits (hits);
	    
	    m_bufferString->erase (match.matchPos (), match.matchLength ());
	}
    }
    
    return (size == 0);
}

void 
IgOProfProcessRepresentable::requestUpdateSymbolList (void)
{
    lat::IOSelector 	selector;    
    lat::PipeCmd	updateProcess (m_configuration->m_opreportExec
				       + " -l -f -n --demangle=none "
				       + m_name,
				       lat::IOFlags::OpenRead);

    ASSERT (m_configuration->m_currentCounter < 9);    
    
    *m_bufferString="";    
    
    updateProcess.setBlocking (false);
    
    selector.attach (&updateProcess, 
		     lat::IORead, 
		     CreateHook (this,
				 &IgOProfProcessRepresentable::updateSymbolList));
	
    while (selector.dispatch ());
    
    updateProcess.close ();
    updateProcess.wait ();
}

IgOProfSymbolRepresentable *
IgOProfProcessRepresentable::getSymbol (const char *name) 
{
    IgOProfSymbolRepresentable *&returnValue = m_symbolMap[name];

    if (!returnValue)
    {
	IgOProfSymbolRepresentable *tmp;
	tmp = new IgOProfSymbolRepresentable (name);
	m_symbolMap[name] = tmp;
	returnValue = tmp;	
    }

    ASSERT (returnValue);

    return returnValue;
}        

std::string 
IgOProfProcessRepresentable::name (void)
{return m_name;}

int 	
IgOProfProcessRepresentable::hits (void)
{return m_hits;}

int 	
IgOProfProcessRepresentable::offset (void)
{return m_offset;}

bool 	
IgOProfProcessRepresentable::isExpanded (void)
{return m_isExpanded;}

bool	
IgOProfProcessRepresentable::isReading (void)
{return m_isReading;}

bool	
IgOProfProcessRepresentable::upToDate (void)
{return m_upToDate;}

void
IgOProfProcessRepresentable::setHits (int hits)
{m_hits = hits;}

void
IgOProfProcessRepresentable::setOffset (int offset)
{m_offset = offset;}

void
IgOProfProcessRepresentable::setIsExpanded (bool expanded)
{m_isExpanded = expanded;}

void
IgOProfProcessRepresentable::setUpToDate (int upToDate)
{m_upToDate = upToDate;}

IgOProfProcessRepresentable::SymbolMap::iterator 
IgOProfProcessRepresentable::begin (void) 
{return m_symbolMap.begin ();}

IgOProfProcessRepresentable::SymbolMap::iterator 
IgOProfProcessRepresentable::end (void) 
{return m_symbolMap.end ();}

IgOProfProcessRepresentable::SymbolMap::iterator
IgOProfProcessRepresentable::find (const char *key) 
{return m_symbolMap.find (key);}

IgOProfProcessRepresentable::SymbolMap::iterator
IgOProfProcessRepresentable::find (std::string &key) 
{return m_symbolMap.find (key.c_str ());}

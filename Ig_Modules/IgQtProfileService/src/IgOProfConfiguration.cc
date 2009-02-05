//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfOprofilerProc.h"
#include <qsettings.h>
#include <iostream>
#include <classlib/utils/SearchPath.h>
#include <classlib/iobase/Filename.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qregexp.h>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfConfiguration::IgOProfConfiguration (void)
    :m_maskProcess (false),
     m_cfg (new QSettings)
{
    read ();
    checkExec (m_opreportExec, "opreport");
    checkExec (m_opannotateExec, "opannotate");
    checkExec (m_opHelpExec, "op_help");

    m_eventMap = new IgOProfEventMap (this);    
    write ();
}

IgOProfConfiguration::~IgOProfConfiguration (void)
{
    delete m_cfg;
}

bool 
IgOProfConfiguration::checkExec (std::string &exec, const char *programName)
{
    static QString last_path ("/usr/local");
    QRegExp dirnameRE ("\\/.*\\/");
    dirnameRE.search (exec.c_str ());
    
    lat::Filename configPath (dirnameRE.cap ().ascii ());
    lat::Filename configName (programName);
    
    lat::SearchPath searchPath (((std::string) configPath)+":/usr/local/bin:/localscratch/o/oprofile/bin:"+getenv ("PATH"));
    lat::SearchPath::const_iterator fullPathIterator = searchPath.search (configName.asFile ());
    
    if (fullPathIterator != searchPath.end () )
    {
	exec = lat::Filename (*fullPathIterator, configName.asFile ());
	return true;	
    }
        
    return false;
}

void
IgOProfConfiguration::write (void)
{
    int n=0;
    m_cfg->insertSearchPath (QSettings::Windows, "/CERN");
    m_cfg->insertSearchPath (QSettings::Unix, (std::string (getenv ("HOME"))+"/.koprofilerrc").c_str ());
    m_cfg->writeEntry ("/koprofiler/User", m_user.c_str ());
    m_cfg->writeEntry ("/koprofiler/KernelImage", m_vmLinuxPath.c_str ());	
    m_cfg->writeEntry ("/koprofiler/ProcessMask", "");
    m_cfg->writeEntry ("/koprofiler/SymbolMask", "");
    m_cfg->writeEntry ("/koprofiler/ShowZeros", m_showZeros);
    m_cfg->writeEntry ("/koprofiler/ShowLibraries", m_showLibraries);
    m_cfg->writeEntry ("/koprofiler/SeparateSamples", m_separateSamples);	
    m_cfg->writeEntry ("/koprofiler/ProfileKernel", m_profileKernel);
    m_cfg->writeEntry ("/koprofiler/ProfileUser", m_profileUser);
    m_cfg->writeEntry ("/koprofiler/ShowPercent", m_showPercent);
    m_cfg->writeEntry ("/koprofiler/OptimePath", m_opreportExec.c_str ());
    m_cfg->writeEntry ("/koprofiler/OprofiledPath", m_oprofiledExec.c_str ());
    m_cfg->writeEntry ("/koprofiler/OpannotatePath", m_opannotateExec.c_str ());
    m_cfg->writeEntry ("/koprofiler/OphelpPath", m_opHelpExec.c_str ());
    m_cfg->writeEntry ("/koprofiler/OffsetOnStart", m_offsetOnStart);
    m_cfg->writeEntry ("/koprofiler/SortOfTotal", m_sortOnTotal);
    
    std::string counterPrefix;
    for (IgOProfCounterListIterator i = m_counterList.begin () ; i != m_counterList.end () ; i++)
    {
	IgOProfCounter &c = (**i);	
	counterPrefix = std::string ("/koprofiler/Counter")+QString::number (n++, 10).ascii ();
	m_cfg->writeEntry ((counterPrefix+"/Name").c_str (), c.m_name.c_str ());
	m_cfg->writeEntry ((counterPrefix+"/Count").c_str (), c.m_count);
	m_cfg->writeEntry ((counterPrefix+"/Enabled").c_str (), c.m_enabled);
	m_cfg->writeEntry ((counterPrefix+"/Mask").c_str (), c.m_mask);
	m_cfg->writeEntry ((counterPrefix+"/ProfileKernel").c_str (), c.m_profileKernel);
	m_cfg->writeEntry ((counterPrefix+"/ProfileUser").c_str (), c.m_profileUser);
    }
}

void
IgOProfConfiguration::read (void)
{
    std::string cp = "/koprofiler/";

    QSettings &cfg = *m_cfg;
        
    cfg.insertSearchPath (QSettings::Windows, "/CERN");
    cfg.insertSearchPath (QSettings::Unix, QString (getenv ("HOME"))+"/.koprofilerrc");	
    m_showZeros = cfg.readBoolEntry (cp+"ShowZeros", false);	
    m_showLibraries = cfg.readBoolEntry (cp+"ShowLibraries", true);
    m_showPercent = cfg.readBoolEntry (cp+"ShowPercent", false);
    m_separateSamples = cfg.readBoolEntry (cp+"SeparateSamples", true);	    
    m_currentCounter = cfg.readNumEntry (cp+"CurrentCounter", m_oprofiler.enabled () ? firstCounterEnabled () : 0);
    m_processMask = (const char *) cfg.readEntry (cp+"ProcessMask", "");
    m_symbolMask = (const char *) cfg.readEntry (cp+"SymbolMask", "");
    m_vmLinuxPath = (const char *) cfg.readEntry (cp+"KernelImage", "/boot/vmlinux-2.4.18-18.7.x.cernsmp");
    m_user = (const char *) cfg.readEntry (cp+"User", getenv ("USER"));
    m_profileKernel = cfg.readBoolEntry (cp+"ProfileKernel", false);
    m_profileUser = cfg.readBoolEntry (cp+"ProfileUser", true);
    m_opreportExec = (const char *) cfg.readEntry (cp+"OpreportPath", "/localscratch/o/oprofile/bin/opreport");
    m_oprofiledExec = (const char *) cfg.readEntry (cp+"OprofiledPath", "/localscratch/o/oprofile/bin/oprofiled"); 
    m_opannotateExec = (const char *) cfg.readEntry (cp+"OpannotatePath", "/localscratch/o/oprofile/bin/opannotate");  
    m_opHelpExec = (const char *) cfg.readEntry (cp+"OptosourcePath", "/localscratch/o/oprofile/bin/op_help");
    m_offsetOnStart = cfg.readBoolEntry (cp+"OffsetOnStart", false);
    m_sortOnTotal = cfg.readBoolEntry (cp+"SortOnTotal", false);

    if (m_oprofiler.enabled ())
    {
	for (int i = 0 ; i < m_oprofiler.maxCounters () ; i++)
	{
	    IgOProfCounter *c = new IgOProfCounter;
	    std::string prefix = std::string (cp+"Counter/")+QString::number (i, 10);
	    c->m_name = (const char *) cfg.readEntry (prefix+"Name", "CPU_CLK_UNHALTED");
	    c->m_count = cfg.readNumEntry (prefix+"Count", 300000);
	    c->m_mask = cfg.readNumEntry (prefix+"Mask", 0);
	    c->m_enabled = cfg.readBoolEntry (prefix+"Enabled", i == 0);
	    c->m_profileKernel = cfg.readBoolEntry (prefix+"ProfileKernel", true);
	    c->m_profileUser = cfg.readBoolEntry (prefix+"ProfileUser", true);
	    m_counterList.push_back (c);
	}
    }

    m_noProfilerFound = !m_oprofiler.enabled ();    
    
    /**Remote profiler configuration*/
    m_remoteProfiling = cfg.readBoolEntry (cp+"RemoteProfiling", false);
    m_serverAddress = (const char *) cfg.readEntry (cp+"ServerAddress", "lxcmse1.cern.ch");
    m_serverPort = cfg.readNumEntry (cp+"ServerPort", 23456);
}

void 
IgOProfConfiguration::use (void)
{
    IgOProfOprofilerProc &p = m_oprofiler;
    
    p.stop ();
    sleep (1);
    int n = 0;
    for (IgOProfCounterListIterator i = m_counterList.begin ();
	 i != m_counterList.end ();
	 i++)
    {
	IgOProfCounter &c = (**i);	
	p.eventID (n, m_eventMap->byName(c.m_name)->m_ID);
	p.eventCount (n, c.m_count);
	p.eventMask (n, c.m_mask);
	p.eventProfileKernel (n, c.m_profileKernel);
	p.eventProfileUser (n, c.m_profileUser);
	p.eventEnabled (n, c.m_enabled);
	n++;
    }
    p.start (m_vmLinuxPath.c_str (), m_separateSamples);
    sleep (1);
}

void 
IgOProfConfiguration::readCountersFromProc ()
{
    IgOProfOprofilerProc &p = m_oprofiler;
    
    m_counterList.clear ();
    
    for (int i = 0 ;  i < m_oprofiler.maxCounters () ; i++)
    {
	if (p.eventEnabled (i))
	{
	    ASSERT (m_eventMap->containsId (p.eventID (i)));
	    
	    IgOProfCounter *c = 
		new IgOProfCounter (&p, 
				    m_eventMap->byId (p.eventID (i))->m_name.c_str (),
				    i);
	    m_counterList.push_back (c);
	}
    }
}

int 
IgOProfConfiguration::firstCounterEnabled (void)
{
    int i;
    for ( i = 0 ; i < m_oprofiler.maxCounters () && m_oprofiler.eventEnabled (i) == false ; i++);
    if (m_oprofiler.eventEnabled (i)) return i;
    else return -1;
}

IgOProfOprofilerProc &
IgOProfConfiguration::proc (void)
{
    return m_oprofiler;
}

IgOProfConfiguration *
IgOProfConfigurationSingleton::instance (void)
{
    static IgOProfConfiguration *__instance = 0;
    if ( __instance == 0 )
    {
	__instance = new IgOProfConfiguration;
    }
    return __instance;
}    

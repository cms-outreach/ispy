//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfOprofilerProc.h"
#include <dlfcn.h>
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcDevice.h"
#include <qstring.h>
#include <qprocess.h>
#include <iostream>


using namespace std;

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QString IgOProfOprofilerProc::eventStr (const int evt, const char *evtname)
{
    return QString::number (evt, 10)+"/"+evtname;
}

IgOProfOprofilerProc::IgOProfOprofilerProc (void)
    :m_procDevice (new IgOProfProcDevice ("/proc/sys/dev/oprofile/")),
     m_enabled (m_procDevice->enabled ())
{
}

void 
IgOProfOprofilerProc::dump (void)
{
    ASSERT (m_procDevice->writeString ("dump", "1") == true);
}

void
IgOProfOprofilerProc::profileKernel (const bool t)
{
    ASSERT (m_procDevice->writeBool ("kernel", t) == true);
}

void
IgOProfOprofilerProc::profileUser (const bool t)
{
    ASSERT (m_procDevice->writeBool ("user", t) == true);
}

void
IgOProfOprofilerProc::start (const QString &kernel, const bool separateSamples)
{
    QProcess p (QString ("oprofiled"));
    p.addArgument (QString ("--vmlinux=")+kernel);
    p.addArgument (QString ("--separate-samples=")+ ((separateSamples == true) ? "1" : "0"));
    p.start ();
}

void
IgOProfOprofilerProc::stop (void)
{
    ASSERT (m_procDevice->writeString ("dump_stop", "1") == true);
}

void
IgOProfOprofilerProc::eventID (const int evt, const int id)
{
    ASSERT (m_procDevice->writeInt (eventStr (evt, "event"), id) == true);
}

void
IgOProfOprofilerProc::eventCount (const int evt, const int value)
{
    ASSERT (m_procDevice->writeInt (eventStr (evt, "count"), value) == true);
}

void
IgOProfOprofilerProc::eventEnabled (const int evt, const bool enable)
{
    ASSERT (m_procDevice->writeBool (eventStr (evt, "enabled"), enable) == true);
}

void
IgOProfOprofilerProc::eventProfileKernel (const int evt, const bool enable)
{
    m_procDevice->writeBool (eventStr (evt, "kernel"), enable);
}
		
void
IgOProfOprofilerProc::eventProfileUser (const int evt, const bool enable)
{
    m_procDevice->writeBool (eventStr (evt, "user"), enable);
}

void
IgOProfOprofilerProc::eventMask (const int evt, const int mask)
{
    m_procDevice->writeInt (eventStr (evt, +"mask"), mask);
}

int
IgOProfOprofilerProc::eventID (const int evt)
{
    int result;    
    ASSERT (m_procDevice->readInt (eventStr (evt, "event"), result) == true);
    return result;
}

int
IgOProfOprofilerProc::eventCount (const int evt)
{
    int result;
    ASSERT (m_procDevice->readInt (eventStr (evt, "count"), result) == true);
    return result;
}

bool
IgOProfOprofilerProc::eventEnabled (const int evt)
{
    bool result;
    ASSERT (m_procDevice->readBool (eventStr (evt, "enabled"), result) == true);
    return result;    
}

bool
IgOProfOprofilerProc::eventProfileKernel (const int evt)
{
    bool result;
    ASSERT (m_procDevice->readBool (eventStr (evt, "kernel"), result) == true);
    return result;    
}

bool
IgOProfOprofilerProc::eventProfileUser (const int evt)
{
    bool result;
    ASSERT (m_procDevice->readBool (eventStr (evt, "user"), result) == true);
    return result;    
}

int
IgOProfOprofilerProc::eventMask (const int evt)
{
    int result;
    ASSERT (m_procDevice->readInt (eventStr (evt, "unit_mask"), result) == true);
    return result;    
}

int
IgOProfOprofilerProc::cpuType (void)
{
    int result;
    ASSERT (m_procDevice->readInt ("cpu_type", result) == true);
    return result;    
}

int
IgOProfOprofilerProc::maxCounters ()
{
    switch (cpuType ())
    {
    case CPU_P4:
	return 8;
    case CPU_ATHLON:
	return 4;
    default:
	return 2;
    }
}

bool
IgOProfOprofilerProc::canStop (void)
{
    return m_procDevice->canWrite ("dump_stop");
}
 
bool
IgOProfOprofilerProc::canStart (void)
{
    return m_procDevice->canWrite ("0/event");
}

bool
IgOProfOprofilerProc::isRunning (void)
{
    QProcess testIfRunning (QString ("ps"));
    testIfRunning.addArgument ("-C");
    testIfRunning.addArgument ("oprofiled");
    testIfRunning.addArgument ("--no-header");
    
    testIfRunning.start ();
    while (testIfRunning.isRunning ()){}
    
    QString results = testIfRunning.readLineStdout ();
    return results.contains ("oprofiled");
}

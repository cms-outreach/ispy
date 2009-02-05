#ifndef __IG_OPROF_CONFIGURATION_H_
#define __IG_OPROF_CONFIGURATION_H_

#include "Ig_Modules/IgQtProfileService/interface/config.h"

#include <list>
#include <map>
#include <sstream>
#include <string>

#include <qprocess.h>

#include "Ig_Modules/IgQtProfileService/src/IgOProfOprofilerProc.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfigFile.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfEventMap.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfCounter.h"

#include <unistd.h>

class QSettings;

/**Ugly oprofile configuration class...Should be made a singleton, IMHO, and options should be accessed by name...
It reads/writes oprofile settings from /proc 
 */
class IgOProfConfiguration
{
public:
    /**Class describing one of the oprofile CPU counter
     */
    
    
    typedef std::list<IgOProfCounter *> IgOProfCounterList;
    typedef IgOProfCounterList::iterator IgOProfCounterListIterator;
    
    IgOProfCounterList m_counterList;
    std::map<std::string, std::string> m_counterDescription;

    /** true if you want to show symbols with 0 hit, false otherwise.
     */
    bool m_showZeros;
    /** true if you want to show symbols belonging to libraries, false
     * otherwise.
     */
    bool m_showLibraries;
    /** true if you want to show percentage of time spend in a
     * function, false if you want actual hits shown.
     */
    bool m_showPercent;
    /** true if the configuration was read from disk*/
    bool m_isConfigOnDisk;
    /** true if library symbols are counted separatedly for each
     * executable which refers to them*/
    bool m_separateSamples;
    /** true to profile kernel as well*/
    bool m_profileKernel;
    bool m_profileUser;
    bool m_offsetOnStart;	
    bool m_sortOnTotal;	
    bool m_maskProcess;
    /**Current counter being displayed, please, refer to */
    int m_currentCounter;	
    /***/
    std::string m_processMask;
    std::string m_symbolMask;
    std::string m_user;
    std::string m_filename;
    std::string m_vmLinuxPath;
    /**Path for opreport executable*/
    std::string m_opreportExec;
    /**Path for oprofiled executable*/
    std::string m_oprofiledExec;
    /**Path for op_to_source executable*/
    std::string m_opannotateExec;
    /**Path for op_to_source executable*/
    std::string m_opHelpExec;
    /**Wether the profiling informations are remote or not*/
    bool m_remoteProfiling;
    /**Address of the remote machine to be profiled*/
    std::string m_serverAddress;    
    /**Port of the remote machine to be profiled*/
    int m_serverPort;    
    /**Set if /proc/dev/sys/oprofile is not there.*/
    bool m_noProfilerFound;    
    
    IgOProfConfiguration(void);    
    ~IgOProfConfiguration(void);

    bool checkExec(std::string &exec, const char *programName);   
    void write(void);
    void read(void);
    void use(void);
    /**Reads the profiling counters informations from /proc filesystem
     */
    void readCountersFromProc();
    /**\return the first counter enabled.
     */
    int firstCounterEnabled();
    IgOProfOprofilerProc &proc(void);
private:
    QSettings *m_cfg;
    IgOProfOprofilerProc m_oprofiler;
    IgOProfEventMap *m_eventMap;    
};

class IG_QT_PROFILE_SERVICE_API IgOProfConfigurationSingleton
{
private:
public:
    IgOProfConfigurationSingleton (void)
	{	
	}

    static IgOProfConfiguration *instance(void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

#endif

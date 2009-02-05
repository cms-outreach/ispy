#ifndef __IG_OPROF_OPROFILER_PROC_H__
# define __IG_OPROF_OPROFILER_PROC_H__

# include "Ig_Modules/IgQtProfileService/src/IgOProfProcDevice.h"

class IgOProfProcDevice;

typedef enum {
        CPU_NO_GOOD = -1, /**< unsupported CPU type */
        CPU_PPRO, /**< Pentium Pro */
        CPU_PII, /**< Pentium II series */
        CPU_PIII, /**< Pentium III series */
        CPU_ATHLON, /**< AMD P6 series */
        CPU_TIMER_INT, /**< CPU using the timer interrupt */
        CPU_RTC, /**< other CPU to use the RTC */
        CPU_P4,  /**< Pentium 4 / Xeon series */
        CPU_IA64, /**< Generic IA64 */
        CPU_IA64_1, /**< IA64 Merced */
        CPU_IA64_2, /**< IA64 McKinley */
        CPU_HAMMER, /**< AMD Hammer family */
        CPU_P4_HT2, /**< Pentium 4 / Xeon series with 2 hyper-threads */
        CPU_AXP_EV4, /**< Alpha EV4 family */
        CPU_AXP_EV5, /**< Alpha EV5 family */
        CPU_AXP_PCA56, /**< Alpha PCA56 family */
        CPU_AXP_EV6, /**< Alpha EV6 family */
        CPU_AXP_EV67, /**< Alpha EV67 family */
        MAX_CPU_TYPE
} op_cpu;

class QString;

class IgOProfOprofilerProc
{
public:
    IgOProfProcDevice *m_procDevice;    
    QString eventStr (const int evt,const char *evtname);
    IgOProfOprofilerProc (void);
    void dump (void);
    void profileKernel (const bool t);
    void profileUser (const bool t);
    void start (const QString &kernel, bool SeparateSamples);
    void stop (void);
    void eventID (const int evt, int id);
    void eventCount (const int evt, int value);
    void eventEnabled (const int evt, bool enable);
    void eventProfileKernel (const int evt, bool enable);
    void eventProfileUser (const int evt, bool enable);
    void eventMask (const int evt, const int mask);
    int eventID (const int evt);
    int eventCount (const int evt);
    bool eventEnabled (const int evt);
    bool eventProfileKernel (const int evt);
    bool eventProfileUser (const int evt);
    int eventMask (const int evt);
    int cpuType (void);
    int maxCounters (void);
    bool canStop (void);
    bool canStart (void);
    bool isRunning (void);
    bool enabled (void)
	{
	    return m_enabled;	    
	}   	 
private:
    bool m_enabled;    
};

#endif

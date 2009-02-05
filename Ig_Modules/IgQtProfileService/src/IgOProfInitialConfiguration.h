#ifndef __IG_OPROF_INITIAL_CONFIGURATION_H__
#define __IG_OPROF_INITIAL_CONFIGURATION_H__

class IgOProfConfiguration;
class IgOProfApplication;

/**\warning IgInitialConfiguration seems to be useless...Probably just a non deleted file in my old CVS repository. Gonna be removed.
   \todo remove!
 */

class IgOProfInitialConfiguration
{
private:
    bool CanRestart;
public:
    IgOProfInitialConfiguration(IgOProfApplication *parent, IgOProfConfiguration **config);
};	

#endif

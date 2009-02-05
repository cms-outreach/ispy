#ifndef __IG_OPROF_APPLICATION_H__
#define __IG_OPROF_APPLICATION_H__

#include <qapplication.h>

class IgOProfConfiguration;
class IgOProfMainWindow;

/** The application class for the standalone(no IGUANA) GUI application.
    \warning OBSOLETE!
    \todo remove
 */
class IgOProfApplication : public QApplication
{
private:
    IgOProfConfiguration *Config;
    IgOProfMainWindow *MainWin;
public:
    IgOProfApplication(int argc, char **argv);
};

#endif

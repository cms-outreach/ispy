//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfApplication.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfMainWindow.h"
#include <qmainwindow.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfApplication::IgOProfApplication(int argc, char **argv)
    :QApplication(argc,argv)
{
    Config = IgOProfConfigurationSingleton::instance();
    QMainWindow *temp = new QMainWindow();
    MainWin = new IgOProfMainWindow(temp->menuBar(), temp,temp->statusBar(), false);
    setMainWidget(&MainWin->mainWindow());
    connect(this,SIGNAL(lastWindowClosed()),&MainWin->mainWindow(),SLOT(askToSave()));
    MainWin->mainWindow().show();
}

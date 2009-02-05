//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtPython/interface/IgQtPythonShell.h"
#include "Ig_Modules/IgPython/interface/IgPythonStream.h"
#include <classlib/utils/Signal.h>
#include <qmainwindow.h>
#include <qapplication.h>
#include <qwidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int main(int argc, char ** argv)
{
    lat::Signal::handleFatal (argv [0]);
    QApplication *myApp = new QApplication(argc,argv);
    QMainWindow *myWin = new QMainWindow();
    
    QWidget *myWidget = new QWidget(myWin);
    myWin->setCentralWidget(myWidget);    
    
    IgPythonStream *myStream = new IgPythonStream ();
    
    IgQtPythonShell *myShell = new IgQtPythonShell(myWidget, myStream);
    myWidget->show();
    myWin->show();
    myApp->connect( myApp, SIGNAL(lastWindowClosed()), myApp, SLOT(quit()) );
 
    myShell->helpMessage();
    
    myApp->exec();
    
    delete myShell;
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/interface/IgMatEdMainWidget.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdMainMenu.h"
#include <classlib/utils/Signal.h>
#include <qlayout.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int
main(int argc, char ** argv)
{
    // Initializes SoQt library (and implicitly also the Coin and Qt
    // libraries). Returns a top-level / shell Qt window to use.
    lat::Signal::handleFatal (argv [0]);
    QWidget *mainWin = SoQt::init(argc, argv, argv[0]);
    QVBoxLayout *mainLayout = new QVBoxLayout (mainWin);    
    mainLayout->setAutoAdd (true);
    mainLayout->setEnabled (true);
    
    IgMatEdMainMenu *mainMenu = new IgMatEdMainMenu (mainWin);    
    IgMatEdMainWidget *mainWidget = new IgMatEdMainWidget (mainWin,mainMenu);    
    mainMenu->show ();
    
    mainLayout->setMenuBar(mainMenu);

    // Pop up the main window.
    SoQt::show (mainWin);
    // Loop until exit.
    SoQt::mainLoop ();

    return 0;
}

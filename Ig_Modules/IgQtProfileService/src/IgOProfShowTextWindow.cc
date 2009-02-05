//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgQtProfileService/src/IgOProfShowTextWindow.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSyncProcess.h"

#include <qwidget.h>
#include <qstring.h>
#include <qtextedit.h>

#include <unistd.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace std;

IgOProfShowTextWindow::IgOProfShowTextWindow(IgOProfConfiguration *config,const QString &name)
{
    TextBrowser = new QTextEdit(this);
    IgOProfSyncProcess *p = new IgOProfSyncProcess (config->m_opannotateExec.c_str());
    (*p)().addArgument(name);
    cerr << name << endl;
    (*p)().start();
    cerr << p->string()<<endl;
    TextBrowser->setText(p->string());
    TextBrowser->setReadOnly(true);
    TextBrowser->resize(600,400);
    resize(600,400);
    show();
}

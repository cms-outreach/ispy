//<<<<<< INCLUDES                                                       >>>>>>

/*FIXME: This file not needed anymore????*/

#include "Ig_Modules/IgQtProfileService/src/IgOProfSyncProcess.h"
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

IgOProfSyncProcess::IgOProfSyncProcess(const QString &program)
{
    Process = new QProcess(program);
    QObject::connect(Process,SIGNAL(readyReadStdout()), this, SLOT(readOutput()));
}

QProcess &
IgOProfSyncProcess::operator()(void)
{
    return *Process;
}

QString 
IgOProfSyncProcess::string(void)
{
    while(Process->normalExit()==false){}
    return results;
}

void
IgOProfSyncProcess::readOutput(void)
{
    results = QString(Process->readStdout());
}

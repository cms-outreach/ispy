//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgQt/interface/IgQtWorkspace.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtWorkspace::IgQtWorkspace (QWidget *parent, const char *name)
    : QWorkspace (parent, name)
{ }

void
IgQtWorkspace::windowActivationChange  (bool oldActive)
{  
    QWorkspace::windowActivationChange (oldActive);
    if (isActiveWindow())
      emit workspaceActivated ();
    else
      emit workspaceDeactivated ();
}

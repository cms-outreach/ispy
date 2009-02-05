//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgOpenInventorDemo/interface/IgOIVDemoExtension.h"
#include "Ig_Examples/IgOpenInventorDemo/interface/IgOIVDemo.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include <qapplication.h>
#include <qmainwindow.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qdockwindow.h>
#include <qtextedit.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgOIVDemoExtension, "Services/Studio/Session Types/Vis Example--Open Inventor Demo");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOIVDemoExtension::IgOIVDemoExtension (IgState *state)
    : m_state (state)
{ ASSERT (m_state); }

IgOIVDemoExtension::~IgOIVDemoExtension (void)
{ ASSERT (m_state); }

bool
IgOIVDemoExtension::setup (IgStudioDriver *into)
{
    ASSERT (into);

    m_oivDemo = new IgOIVDemo (m_state);
   
    return true;
}

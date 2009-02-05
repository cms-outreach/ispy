//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdMainMenu.h"
#include <classlib/utils/DebugAids.h>
#include <qpopupmenu.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMatEdMainMenu::IgMatEdMainMenu (QWidget *parent)
    :QMenuBar (parent),
     m_parent (parent),
     m_fileMenu (0)
{
    ASSERT (m_parent);
    m_fileMenu = new QPopupMenu(m_parent);
        
    m_fileMenu->insertItem ("Load G4Material Map",LOAD_G4_MATERIAL_MAP);
    m_fileMenu->insertItem ("Save G4Material Map",SAVE_G4_MATERIAL_MAP);
    m_fileMenu->insertItem ("Save Materials",SAVE_MATERIALS);    
    
    this->insertItem ("&File", m_fileMenu);
}

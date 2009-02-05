#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_MENU_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_MENU_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"
# include <qmenubar.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QPopupMenu;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgMatEdMainMenu : public QMenuBar
{
    Q_OBJECT
    QWidget *m_parent;
public:
    enum {
	LOAD_G4_MATERIAL_MAP = 1,
	SAVE_G4_MATERIAL_MAP = 2,
	SAVE_MATERIALS = 3
    };    
    
    IgMatEdMainMenu (QWidget *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    QPopupMenu *m_fileMenu;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_MENU_H

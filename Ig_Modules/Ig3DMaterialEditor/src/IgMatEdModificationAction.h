#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_MODIFICATION_ACTION_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_MODIFICATION_ACTION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"

# include <Inventor/actions/SoAction.h>
# include <Inventor/actions/SoSubAction.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoMaterial;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgMatEdModificationAction : public SoAction
{
    SO_ACTION_HEADER(IgMatEdModificationAction);    
public:
    IgMatEdModificationAction (void);
    ~IgMatEdModificationAction (void);

    static void initClass(void);
    static void modifyMaterial (SoAction *action, SoNode *material);

    static float m_transparency;
    static float m_shininess;
    static float m_diffuseColor[3];
    static float m_ambientColor[3];
    static float m_specularColor[3];
    static float m_emissiveColor[3];    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_MODIFICATION_ACTION_H

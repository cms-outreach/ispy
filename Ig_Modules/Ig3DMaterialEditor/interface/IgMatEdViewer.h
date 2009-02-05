#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_VIEWER_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_VIEWER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QWidget;
class SoSeparator;
class SoOrthographicCamera;
class SoDirectionalLight;
class SoQtRenderArea;
class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgMatEdViewer
{
public:
    IgMatEdViewer (QWidget *parent, SoNode *model);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    QWidget 			*m_parent;    
    SoQtRenderArea 		*m_renderArea;    
    SoDirectionalLight		*m_light;    
    SoOrthographicCamera	*m_camera;    
    SoSeparator			*m_root;
    SoSeparator			*m_userSeparator;
    SoNode			*m_model;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_VIEWER_H

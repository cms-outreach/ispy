#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_COLOR_BOX_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_COLOR_BOX_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"
# include <Inventor/fields/SoMFColor.h> 

#include <qvbox.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QWidget;
class SoNode;
class QLabel;
class QVBox;
class IgMatEdSlider;
class IgMatEdMainWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgMatEdColorBox : public QVBox
{
public:
    IgMatEdColorBox (QWidget *parent, const char *name, float *colorArray, IgMatEdMainWidget *mainWidget);
    void setColor (SoMFColor &color);    
    void updatePosition (SoMFColor &color);    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    QWidget 	*m_parent;
    float	*m_colorArray;
    QLabel 	*m_label;    
    QVBox	*m_sliderBox;    
    IgMatEdMainWidget *m_mainWidget;
    
    IgMatEdSlider	*m_redSlider;
    IgMatEdSlider	*m_greenSlider;
    IgMatEdSlider	*m_blueSlider;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_COLOR_BOX_H

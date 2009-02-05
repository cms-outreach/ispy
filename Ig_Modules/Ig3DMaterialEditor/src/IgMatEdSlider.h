#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_SLIDER_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_SLIDER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"

# include <qhbox.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QLabel;
class QSlider;
class IgMatEdMainWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgMatEdSlider : public QHBox
{
    Q_OBJECT
public:
    IgMatEdSlider (QWidget *parent, const char *name, float *target, IgMatEdMainWidget *mainWidget);
    void setValue (int value);    
    void setValue (float value);    
    void updatePosition (int value);
    void updatePosition (float value);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
public slots:
void updateValue (int newValue);
private:
    QLabel *m_label;
    QSlider *m_slider;
    float *m_target;    
    IgMatEdMainWidget *m_mainWidget;    
    QLabel *m_count;    
    bool m_updatePosition;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_SLIDER_H

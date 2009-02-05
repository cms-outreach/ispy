//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdColorBox.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdSlider.h"

#include <Inventor/nodes/SoNode.h>

#include <qlabel.h>
#include <qvbox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMatEdColorBox::IgMatEdColorBox (QWidget *parent, const char *name, float *colorArray, IgMatEdMainWidget *mainWidget)
    :QVBox (parent),
     m_parent (parent),
     m_colorArray (colorArray),
     m_sliderBox (0),
     m_mainWidget (mainWidget)
{
    ASSERT (m_parent);
    ASSERT (m_colorArray);
    ASSERT (m_mainWidget);    

    this->setSpacing (2);
    this->setMargin (5);    
    
    m_label = new QLabel (name, this);
    
    QHBox *m_guiBox = new QHBox (this);
    QFrame *m_colorFrame = new QFrame (m_guiBox);
    m_colorFrame->setFrameStyle (QFrame::Panel);
    /*QVBox in which to put the sliders*/
    m_sliderBox = new QVBox (m_guiBox);    
    m_sliderBox->setSpacing (2);
    m_sliderBox->setMargin (5);    
    /*Sliders*/
    m_redSlider = new IgMatEdSlider (m_sliderBox, "Red", &m_colorArray[0], m_mainWidget);    
    m_greenSlider = new IgMatEdSlider (m_sliderBox, "Green", &m_colorArray[1], m_mainWidget);    
    m_blueSlider = new IgMatEdSlider (m_sliderBox, "Blue", &m_colorArray[2], m_mainWidget);    
}

void
IgMatEdColorBox::setColor ( SoMFColor &color)
{
    m_redSlider->setValue ((color[0])[0]);
    m_greenSlider->setValue ((color[0])[1]);
    m_blueSlider->setValue ((color[0])[2]);    
}

void
IgMatEdColorBox::updatePosition (SoMFColor &color)
{
    m_redSlider->updatePosition ((color[0])[0]);
    m_greenSlider->updatePosition ((color[0])[1]);
    m_blueSlider->updatePosition ((color[0])[2]);
}

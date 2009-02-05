//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdSlider.h"
#include "Ig_Modules/Ig3DMaterialEditor/interface/IgMatEdMainWidget.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdModificationAction.h"
#include "Ig_Modules/Ig3DMaterialEditor/interface/IgSoMaterialMap.h"
#include <Inventor/nodes/SoNode.h> 
#include <classlib/utils/DebugAids.h>
#include <qlabel.h>
#include <qslider.h>
#include <qcombobox.h>
#include <iostream>
#include <qlistview.h>
#include <qhbox.h>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMatEdSlider::IgMatEdSlider (QWidget *parent, const char *name, float *target, IgMatEdMainWidget *mainWidget)
    :QHBox (parent),
     m_target (target),
     m_mainWidget (mainWidget),
     m_updatePosition (false)
{
    ASSERT (m_target);
    ASSERT (m_mainWidget);

    this->setSpacing (2);
    this->setMargin (2);

    m_label = new QLabel (name,this);
    m_slider = new QSlider (0, 100, 1, 0, Qt::Horizontal, this);
    m_slider->setTickmarks (QSlider::Below);
    m_slider->setValue ((int) ((*target)*100));    
    m_slider->setTickInterval (10);
    
    m_count = new QLabel (QString::number ((*target)*100) + "%",this);
    
    /*Makes sure that labels and count are aligned*/
    m_label->setMinimumWidth (QApplication::fontMetrics ().width ("Green")+10);    
    m_count->setMinimumWidth (QApplication::fontMetrics ().width ("100%")+10);    

    connect (m_slider, SIGNAL (valueChanged (int)), this, SLOT (updateValue (int)));    
}

void
IgMatEdSlider::updateValue (int newValue)
{
	*m_target = ((float) newValue)/100;
	m_count->setText (QString::number (newValue) + "%");    
	IgMatEdModificationAction *action = new IgMatEdModificationAction ();
	//action->apply ((SoNode *) m_mainWidget->m_currentMaterial);
	if (m_updatePosition == false)
	{
	    action->apply ((SoNode *) (*(m_mainWidget->m_soMaterialMap))[m_mainWidget->m_comboBox->currentText ()]);        
	}	
}

void
IgMatEdSlider::setValue (int value)
{
    m_slider->setValue (value);
    updateValue (value);    
}

void
IgMatEdSlider::setValue (float value)
{
    m_slider->setValue ((int)(100*value));    
}

void 
IgMatEdSlider::updatePosition (int newValue)
{
    m_updatePosition = true;    
    *m_target = ((float) newValue)/100;
    m_count->setText (QString::number (newValue) + "%");    
    m_slider->setValue (newValue);    
    m_updatePosition = false;    
}

void
IgMatEdSlider::updatePosition (float newValue)
{
    m_updatePosition = true;    
    *m_target = newValue;
    m_count->setText (QString::number ((int) (100*newValue)) + "%");    
    m_slider->setValue ((int) (100*newValue));    
    m_updatePosition = false;    
}

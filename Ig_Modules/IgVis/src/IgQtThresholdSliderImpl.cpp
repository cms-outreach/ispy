// IgQtThresholdSliderImpl.cpp: implementation of the IgQtThresholdSliderImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtThresholdSliderImpl.h"
#include "qslider.h"
#include "qlineedit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtThresholdSliderImpl::IgQtThresholdSliderImpl( QWidget* parent, const char* name, WFlags fl) :
  IgQtSlider(parent, name, fl), theValue(0), minValue(0), maxValue(50)
{
	Slider->setMinValue(minValue);
	Slider->setMaxValue(maxValue);
	Slider->setValue(theValue);

	Slider->setLineStep(1);
	Slider->setPageStep(10);
	
	QString s;
	theValidator = new QIntValidator(this);
	theValidator->setRange(minValue,maxValue);
	
	SliderTextValue->setValidator(theValidator);
	s.setNum(theValue);
	SliderTextValue->validateAndSet(s,0,0,0);
	
	
}

IgQtThresholdSliderImpl::~IgQtThresholdSliderImpl()
{
	delete theValidator;
}
void IgQtThresholdSliderImpl::textValueChangedSlot()
{
	bool check;
	QString s = SliderTextValue->text();
	theValue = s.toInt(&check);
	Slider->setValue(theValue);

}
void IgQtThresholdSliderImpl::sliderValueChangedSlot(int newValue)
{
	theValue = newValue;
	QString s;
	s.setNum(theValue);
	SliderTextValue->validateAndSet(s,0,0,0);
//	emit 
	newValueSignal(theValue);
}
void IgQtThresholdSliderImpl::setMaxValue(int value)
{
	maxValue = value;
	Slider->setMaxValue(maxValue);
    theValidator->setRange(minValue,maxValue);

}
void IgQtThresholdSliderImpl::setMinValue(int value)
{
	minValue = value;
	Slider->setMinValue(minValue);
    theValidator->setRange(minValue,maxValue);
}

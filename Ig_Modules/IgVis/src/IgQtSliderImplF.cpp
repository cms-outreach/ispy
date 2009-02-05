// IgQtSliderImplF.cpp: implementation of the IgQtSliderImplF class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtSliderImplF.h"
#include <qslider.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <cmath>

#ifdef _WIN32
#include <float.h>
namespace 
{
    double rint(double d) 
    {  return (floor(d+0.5)); } // is not identical to gnu rint if rounding mode is non-standard
}
#endif //_WIN32
#if defined(__KCC) && !defined(__Linux) // for us, means Irix
namespace
{
inline double copysign (double x, double y) {
	return (y==0 ? std::abs(x) : std::abs(x) * y /std::abs(y));
}

int rint(double d)
    {  return const_cast<int>(floor(d+0.5)); }
}
#endif  // Irix  
namespace
{
    int rint_i(double d)
    {  return static_cast<int> (floor(d+0.5)); }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtSliderImplF::IgQtSliderImplF( QWidget* parent, const char* name, WFlags fl) :
IgQtSlider(parent, name, fl), theValue(0), minValue(-1.5), maxValue(1.5), scale_factor(100.F)
{
	Slider->setMinValue(rint_i(minValue*scale_factor));
	Slider->setMaxValue(rint_i(maxValue*scale_factor));
	Slider->setValue(rint_i(theValue*scale_factor));
	
	Slider->setLineStep(1);
	Slider->setPageStep(10);

	QString s;
	theValidator = new QDoubleValidator(this);
	theValidator->setRange(minValue,maxValue);
	
	SliderTextValue->setValidator(theValidator);
	s.setNum(theValue);
	SliderTextValue->validateAndSet(s,0,0,0);
	
}

IgQtSliderImplF::~IgQtSliderImplF()
{
	delete theValidator;
}
void IgQtSliderImplF::textValueChangedSlot()
{
	bool check;
	QString s = SliderTextValue->text();
	theValue = s.toDouble(&check);
	Slider->setValue(rint_i(theValue*scale_factor));
	
}
void IgQtSliderImplF::sliderValueChangedSlot(int newValue)
{
	theValue = newValue/scale_factor;
	QString s;
	s.setNum(theValue);
	SliderTextValue->validateAndSet(s,0,0,0);
	// emit 
	newValueSignal(theValue);
}
void IgQtSliderImplF::setTitle( const QString& tit)
{
	GroupBox1->setTitle( tit );
}
void IgQtSliderImplF::setMaxValue(double value)
{
	maxValue = value;
	Slider->setMaxValue(rint_i(maxValue*scale_factor));
    theValidator->setRange(minValue,maxValue);

}
void IgQtSliderImplF::setMinValue(double value)
{
	minValue = value;
	Slider->setMinValue(rint_i(minValue*scale_factor));
    theValidator->setRange(minValue,maxValue);
}
void IgQtSliderImplF::setValue(double value)
{
    sliderValueChangedSlot(rint_i(value*scale_factor));
}


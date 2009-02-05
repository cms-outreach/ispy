#include "Ig_Modules/IgVis/interface/IgQtXyzScaleImpl.h"

#include "qvalidator.h"
#include "Inventor/nodes/SoGroup.h"
#include "Inventor/nodes/SoScale.h"
#include <cmath>
#include <algorithm>

IgQtXyzScaleImpl::IgQtXyzScaleImpl(SoGroup *topNode, QWidget *parent) : IgQtXyzScale(parent), scaleOffset(50), scaleMin(0),
  scaleMax(100), maxExponent(3.F) 
{
	theScale = new SoScale;
	topNode->addChild(theScale);
	scales.resize(3);
	sliders.resize(3);
	editors.resize(3);
	boxes.resize(3);
	scaleValidator = new QDoubleValidator(parent);
	scaleFactor = 2. * maxExponent/(scaleMax-scaleMin);
	scaleValidator->setTop(sliderToDouble(scaleMax));
	scaleValidator->setBottom(sliderToDouble(scaleMin));
	std::fill<std::vector<double>::iterator,double>(scales.begin(),scales.end(),1.0);
	sliders[0] = x_slider;
	sliders[1] = y_slider;
	sliders[2] = z_slider;
	editors[0] = x_text_input;
	editors[1] = y_text_input;
	editors[2] = z_text_input;
	boxes[0] = xCheckBox;
	boxes[1] = yCheckBox;
	boxes[2] = zCheckBox;
	for (int it = 0; it < 3; it++)
	{
		boxes[it]->setChecked(false);
		sliders[it]->setValue((scaleMax-scaleMin)/2);
		sliders[it]->setEnabled(false);
		editors[it]->setValidator(scaleValidator);
		editors[it]->validateAndSet("1.0",0,0,0);
		editors[it]->setReadOnly(true);
	}
}

IgQtXyzScaleImpl::~IgQtXyzScaleImpl(void)
{
	delete scaleValidator;
}
void IgQtXyzScaleImpl::xboxChecked(int value)
{
	boxChecked(value, xAxis);
}
void IgQtXyzScaleImpl::xsliderValueChanged(int value)
{
	sliderValueChanged(value, xAxis);
}

void IgQtXyzScaleImpl::xtextValueChanged(void)
{
	textValueChanged(xAxis);
}
void IgQtXyzScaleImpl::yboxChecked(int value)
{
	boxChecked(value, yAxis);
}
void IgQtXyzScaleImpl::ysliderValueChanged(int value)
{
	sliderValueChanged(value, yAxis);
}
void IgQtXyzScaleImpl::ytextValueChanged(void)
{
	textValueChanged(yAxis);
}
void IgQtXyzScaleImpl::zboxChecked(int value)
{
	boxChecked(value, zAxis);
}
void IgQtXyzScaleImpl::zsliderValueChanged(int value)
{
	sliderValueChanged(value, zAxis);
}
void IgQtXyzScaleImpl::ztextValueChanged(void)
{
	textValueChanged(zAxis);
}
void IgQtXyzScaleImpl::boxChecked(int value, Axis axis)
{
	if (value == 0) 
	{
		// button is toggle off, set scale to 1.
		sliders[axis]->setValue((scaleMax-scaleMin)/2);
	}
	else if (value==2)
	{
		sliders[axis]->setEnabled(true);
		editors[axis]->setReadOnly(false);
	}

}
void IgQtXyzScaleImpl::sliderValueChanged(int value, Axis axis)
{
	scales[axis] = sliderToDouble(value);
	QString s;
	s.setNum(scales[axis]);
	editors[axis]->validateAndSet(s,0,0,0);
	theScale->scaleFactor.setValue(scales[xAxis], scales[yAxis], scales[zAxis]);

}
void IgQtXyzScaleImpl::textValueChanged(Axis axis)
{
	const QString string = editors[axis]->text();
	bool check;
	double theValue = string.toDouble(&check);
	if (check)
	{
		sliders[axis]->setValue(textToSliderValue(theValue));
	}
}
double IgQtXyzScaleImpl::sliderToDouble(int value)
{
	return std::pow(10.0, scaleFactor * (value - scaleOffset));
}
int IgQtXyzScaleImpl::textToSliderValue(double value)
{
	return static_cast<int>(std::log10(value)/scaleFactor + scaleOffset);
}


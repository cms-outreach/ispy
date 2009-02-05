// IgQtThresholdSliderImpl.h: interface for the IgQtThresholdSliderImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTTHRESHOLDSLIDERIMPL_H__5914CF71_7949_4420_876E_FB30980D830E__INCLUDED_)
#define AFX_IGQTTHRESHOLDSLIDERIMPL_H__5914CF71_7949_4420_876E_FB30980D830E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/IgQtSlider.h"
#include "qvalidator.h"

/*!  \class IgQtThresholdSliderImpl IgQtThresholdSliderImpl.h  Ig_Modules/IgVis/interface/IgQtThresholdSliderImpl.h
 *   \brief Implements the IgQtSlider class (which was machine produced using Qt Designer).
 *
 *   Provides a threshold widget of integer values. Started out as life as an implementation of
 *   (the now abandoned) IgQtThreshold
 *
 *   \author George Alverson, Northeastern University
 *   \date  26 April 2001
 *
 */

class IG_VIS_API IgQtThresholdSliderImpl : public IgQtSlider  
{
	    Q_OBJECT
public:
	IgQtThresholdSliderImpl( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	virtual ~IgQtThresholdSliderImpl();
	//! reset the maximum allowed value and update slider limit
	void setMaxValue(int value);
	//! reset the minimum allowed value and update slider limit
	void setMinValue(int value);
signals:
	//! emitted when a new value is entered in the slider
	virtual void newValueSignal(int); 
public slots:
	//! slot called when the value in the text box changes
    virtual void textValueChangedSlot();
	//! slot called when the slider value changes
    virtual void sliderValueChangedSlot(int newValue);
private:
	int theValue;
	int minValue;
	int maxValue;
	QIntValidator *theValidator;

};

#endif // !defined(AFX_IGQTTHRESHOLDSLIDERIMPL_H__5914CF71_7949_4420_876E_FB30980D830E__INCLUDED_)

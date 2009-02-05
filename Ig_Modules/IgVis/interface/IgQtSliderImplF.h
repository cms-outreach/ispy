// IgQtSliderImplF.h: interface for the IgQtSliderImplF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IgQtSliderImplF_H__5914CF71_7949_4420_876E_FB30980D830E__INCLUDED_)
#define AFX_IgQtSliderImplF_H__5914CF71_7949_4420_876E_FB30980D830E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/IgQtSlider.h"
#include "qvalidator.h"

/*!  \class IgQtSliderImplF IgQtSliderImplF.h  d0scan_qt/d0_specific/IgQtSliderImplF.h
 *   \brief Implements the IgQtSlider class for floats (which was machine produced using Qt Designer).
 *
 *
 *   \author George Alverson, Northeastern University
 *   \date  18 Jan 2002
 */

class IG_VIS_API IgQtSliderImplF : public IgQtSlider  
{
            Q_OBJECT
public:
	IgQtSliderImplF( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	virtual ~IgQtSliderImplF();
	void setTitle( const QString& );
	//! reset the maximum allowed value and update slider limit
	void setMaxValue(double value);
	//! reset the minimum allowed value and update slider limit
	void setMinValue(double value);
signals:
	//! emitted when a new value is entered in the slider
	virtual void newValueSignal(double); 
public slots:
	//! slot called when the value in the text box changes
    virtual void textValueChangedSlot();
	//! slot called when the slider value changes
    virtual void sliderValueChangedSlot(int);
	//! set value
    virtual void setValue(double);
private:
	double theValue;
	double minValue;
	double maxValue;
	double scale_factor;
	QDoubleValidator *theValidator;
	
};

#endif // !defined(AFX_IgQtSliderImplF_H__5914CF71_7949_4420_876E_FB30980D830E__INCLUDED_)

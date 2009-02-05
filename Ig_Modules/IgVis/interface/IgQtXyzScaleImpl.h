#if !defined(IG_QT_XYZ_SCALE_IMPL_H)
#define IG_QT_XYZ_SCALE_IMPL_H
#include "Ig_Modules/IgVis/interface/IgQtXyzScale.h"

/*	\class IgQtXyzScaleImpl
 *	\brief Implementation class for a Qt widget controlling overall scale
 *         factors for the x, y, and z directions.
 *
 *  \author G. Alverson, CMS/D0/Northeastern Software Group
 *  \date 4 August 2002
 *
 */

#include <vector>
#include "qvalidator.h"
#include "qslider.h"
#include "qlineedit.h"
#include "qcheckbox.h"

class SoGroup;
class SoScale;

class IG_VIS_API IgQtXyzScaleImpl :
	public IgQtXyzScale
{
    Q_OBJECT
public:
	IgQtXyzScaleImpl(SoGroup *topNode, QWidget *parent=0);
	~IgQtXyzScaleImpl(void);
	enum Axis {xAxis, yAxis, zAxis}; 
public slots:
    virtual void xboxChecked(int);
    virtual void xsliderValueChanged(int);
    virtual void xtextValueChanged(void);
    virtual void yboxChecked(int);
    virtual void ysliderValueChanged(int);
    virtual void ytextValueChanged(void);
    virtual void zboxChecked(int);
    virtual void zsliderValueChanged(int);
    virtual void ztextValueChanged(void);
protected:
	//! generic method when a check box state is altered
	void boxChecked(int, Axis);
	//! generic method called when a slider is changed
	void sliderValueChanged(int, Axis);
	//! generic method called when a text box value is changed
	void textValueChanged(Axis);
	std::vector<double> scales; //!< scales for the three axes
	//!
	double sliderToDouble(int);
	int textToSliderValue(double);
private:
	double scaleFactor; //!< conversion from slider units to scale units
	int scaleOffset; //!< offset from slider units to scale units
	int scaleMin; //!< minimum value for the slider before scaling
	int scaleMax; //!< maximum value for the slider before scaling
	QDoubleValidator *scaleValidator; //!< for validating the scale values for the sliders
	double maxExponent; //!< maximum (in absolute value) exponent allowed
	std::vector<QSlider*> sliders;
	std::vector<QLineEdit*> editors;
	std::vector<QCheckBox*> boxes;
	SoScale *theScale;
};
#endif // IG_QT_XYZ_SCALE_IMPL_H


/****************************************************************************
** Form interface generated from reading ui file 'Ig_Modules/IgVis/src/IgQtSlider.ui'
**
** Created: Tue Feb 18 14:15:50 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef THRESHOLD_PANEL_H
#define THRESHOLD_PANEL_H

#include "Ig_Modules/IgVis/interface/config.h"
#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QLineEdit;
class QSlider;

class IG_VIS_API IgQtSlider : public QWidget
{ 
    Q_OBJECT

public:
    IgQtSlider( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~IgQtSlider();

    QGroupBox* GroupBox1;
    QSlider* Slider;
    QLabel* Units;
    QLineEdit* SliderTextValue;


public slots:
    virtual void sliderValueChangedSlot(int);
    virtual void textValueChangedSlot();

protected:
    QVBoxLayout* Threshold_PanelLayout;
    QVBoxLayout* GroupBox1Layout;
    QHBoxLayout* Layout1;
};

#endif // THRESHOLD_PANEL_H

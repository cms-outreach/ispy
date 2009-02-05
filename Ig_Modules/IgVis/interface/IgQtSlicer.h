/****************************************************************************
** Form interface generated from reading ui file 'Ig_Modules/IgVis/src/IgQtSlicer.ui'
**
** Created: Tue Feb 18 14:15:47 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef IGQTSLICER_H
#define IGQTSLICER_H

#include "Ig_Modules/IgVis/interface/config.h"
#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class IgSoQtThumbWheel;
class QButtonGroup;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;

class IG_VIS_API IgQtSlicer : public QDialog
{ 
    Q_OBJECT

public:
    IgQtSlicer( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IgQtSlicer();

    IgSoQtThumbWheel* SliceCenterThumbwheel;
    QLabel* SliceCenterLabel;
    QLineEdit* SliceCenterText;
    IgSoQtThumbWheel* SliceThicknessThumbwheel;
    QLabel* SliceThicknessLabel;
    QLineEdit* SliceThicknessText;
    QCheckBox* SliceOnOff;
    QButtonGroup* SliceAxisGroup;
    QLabel* ClipAxisTitle;
    QRadioButton* xAxisButton;
    QRadioButton* yAxisButton;
    QRadioButton* zAxisButton;
    QPushButton* CloseButton;


public slots:
    virtual void centerPositionSettingSlot(float);
    virtual void centerPositionTextChangedSlot();
    virtual void axisChanged(int);
    virtual void sliceOnSlot(bool);
    virtual void thicknessSettingSlot(float);
    virtual void thicknessTextChangedSlot();

protected:
    QVBoxLayout* IgQtSlicerLayout;
    QHBoxLayout* Layout23;
    QHBoxLayout* Layout24;
    QHBoxLayout* Layout40;
    QHBoxLayout* SliceAxisGroupLayout;
};

#endif // IGQTSLICER_H

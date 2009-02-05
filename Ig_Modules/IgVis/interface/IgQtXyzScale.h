/****************************************************************************
** Form interface generated from reading ui file 'c:\Graphics\Projects\IGUANA\Ig_Modules\IgVis\src\IgQtXyzScale.ui'
**
** Created: Tue Feb 18 14:15:49 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SCALE_H
#define SCALE_H

#include "Ig_Modules/IgVis/interface/config.h"
#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QSlider;

class IG_VIS_API IgQtXyzScale : public QDialog
{ 
    Q_OBJECT

public:
    IgQtXyzScale( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IgQtXyzScale();

    QGroupBox* GroupBox8;
    QCheckBox* xCheckBox;
    QSlider* x_slider;
    QLineEdit* x_text_input;
    QCheckBox* yCheckBox;
    QSlider* y_slider;
    QLineEdit* y_text_input;
    QCheckBox* zCheckBox;
    QSlider* z_slider;
    QLineEdit* z_text_input;
    QPushButton* CloseButton;


public slots:
    virtual void xboxChecked(int);
    virtual void xsliderValueChanged(int);
    virtual void xtextValueChanged();
    virtual void yboxChecked(int);
    virtual void ysliderValueChanged(int);
    virtual void ytextValueChanged();
    virtual void zboxChecked(int);
    virtual void zsliderValueChanged(int);
    virtual void ztextValueChanged();

protected:
    QHBoxLayout* ScaleLayout;
    QVBoxLayout* GroupBox8Layout;
    QVBoxLayout* Layout11;
    QHBoxLayout* xLayout;
    QHBoxLayout* yLayout;
    QHBoxLayout* zLayout;
    QHBoxLayout* Layout13;
};

#endif // SCALE_H

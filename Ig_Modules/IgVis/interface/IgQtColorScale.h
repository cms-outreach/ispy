/****************************************************************************
** Form interface generated from reading ui file '../IgQtColorScale.ui'
**
** Created: Thu Aug 2 16:39:37 2001
**      by:  The User Interface Compiler (uic)
**
**
**
**
**      base programing:  	diana bull
**		done by:	vassar college
**				in the month of august 2001
**
**
**
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef IGQTCOLORSCALE_H
#define IGQTCOLORSCALE_H

#include "Ig_Modules/IgVis/interface/config.h"
#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;
class QSlider;
class QPainter;

//!class to allow the user to pick min/max values of a specified property type and colors to corresponding to the min/max values--it then creates a gradient between these colors and applies them to the selected hits or whatever
class IG_VIS_API IgQtColorScale : public QDialog
{ 
    Q_OBJECT

public:
    //!main dialog box constructor
    IgQtColorScale( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IgQtColorScale();
    //!pointers to all the widgets in the dialog box
    QLabel* Property_Type;
    QLabel* Maximum;
    QLabel* Minimum;
    QSlider* min_slider;
    QSlider* max_slider;
    QFrame* gradientcolor;
    QFrame* color2;
    QFrame* color1;
    QFrame* pickvalcolor;
    QLineEdit* pickval;	
    QLineEdit* minval;
    QComboBox* Property_List;
    QPushButton* OK;
    QPushButton* Cancel;
    QLineEdit* maxval;
    QPushButton* Select_Color_1;
    QPushButton* Select_Color_2;
    //!function to get the color corresponding to each value within the min/max range
    QColor& getColorForValue( double colorposition );
   
protected:
    bool event( QEvent* );
    //!this calls the gradation code (gradientColors) and makes it  a paintEvent function so that after an event it can always be updated
    void paintEvent(QPaintEvent *);
    
private:
    //!these are just private members of the class
    bool   colorFlag;
    bool   valueFlag;
    QColor mincolor;
    QColor maxcolor;
    double minValue;
    double maxValue;
    double height;
    double width;
    QColor col;
    double chosenvalue;
    //!this function generates the gradation from the max/mincolors chosen
    void   gradientColors(QPainter *painter);
        
private slots:
    //!this slot connects the chosen color from the QColorDialog box with the mincolor display box 
    void minColorSelectSlot();
    //!connects the chosen color from the QColorDialog box with the maxcolor display box
    void maxColorSelectSlot();
    //!this connection calls the gradient code indirectly through the paintEvent function, updating the gradation each time a part has been invalidated
    void drawGradientBoxSlot();
    //!this connects the value typed in the minimum value box to the program
    void minValueSlot();
    //!this connects the value typed in the maximum value box to the program
    void maxValueSlot();
    //!this connects the value typed in the pickval slot to the program
    void gradientplace();
    
    
signals:
    //!this signal is emitted after both the min and the max colors have been chosen
    void colorChosen();
};

#endif // IGQTCOLORSCALE_H



/****************************************************************************
** Form implementation generated from reading ui file '../IgQtColorScale.ui'
**
** Created: Thu Aug 2 16:39:59 2001
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
#include "Ig_Modules/IgVis/interface/IgQtColorScale.h"

#include <qcombobox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstring.h>
#include <qcolordialog.h>
#include <iostream>
#include <qpainter.h>
#include <cmath>
#include <qmessagebox.h>
#include <qevent.h>

/* 
 *  Constructs a IgQtColorScale which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgQtColorScale::IgQtColorScale( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    colorFlag = false;
    valueFlag = false;
    col = QColor("gray");
    
    if ( !name )
	setName( "IgQtColorScale" );
    resize( 396, 556 ); 
    setCaption( tr( "Color Scale" ) );
    setAcceptDrops( TRUE );
    QToolTip::add(  this, tr( "" ) );

    Property_Type = new QLabel( this, "Property_Type" );
    Property_Type->setGeometry( QRect( 150, 180, 201, 31 ) ); 
    Property_Type->setText( tr( "Property Type" ) );
    Property_Type->setAlignment( int( QLabel::AlignCenter ) );
    QToolTip::add(  Property_Type, tr( "Select property type." ) );

    Maximum = new QLabel( this, "Maximum" );
    Maximum->setGeometry( QRect( 260, 40, 120, 20 ) ); 
    QFont Maximum_font(  Maximum->font() );
    Maximum_font.setFamily( "adobe-helvetica" );
    Maximum->setFont( Maximum_font ); 
    Maximum->setText( tr( "Maximum Value" ) );
    Maximum->setAlignment( int( QLabel::AlignCenter ) );
    QToolTip::add(  Maximum, tr( "Enter maximum value to be evaluated." ) );

    Minimum = new QLabel( this, "Minimum" );
    Minimum->setGeometry( QRect( 260, 400, 120, 20 ) ); 
    Minimum->setText( tr( "Minimum Value" ) );
    Minimum->setAlignment( int( QLabel::AlignCenter ) );
    QToolTip::add(  Minimum, tr( "Enter minimum value to be evaluated." ) );

    //      min_slider = new QSlider( this, "min_slider" );
    //      min_slider->setGeometry( QRect( 80, 380, 24, 71 ) ); 
    //      min_slider->setOrientation( QSlider::Vertical );
    //      QToolTip::add(  min_slider, tr( "Adjust minimum color bound." ) );

    //      max_slider = new QSlider( this, "max_slider" );
    //      max_slider->setGeometry( QRect( 80, 50, 24, 70 ) ); 
    //      max_slider->setOrientation( QSlider::Vertical );
    //      QToolTip::add(  max_slider, tr( "Adjust maximum color bound." ) );

    gradientcolor = new QFrame( this, "gradientcolor" );
    gradientcolor->setGeometry( QRect( 10, 50, 60, 400 ) ); 
    gradientcolor->setAcceptDrops( TRUE );
    gradientcolor->setFrameShape( QFrame::StyledPanel );
    gradientcolor->setFrameShadow( QFrame::Raised );
    gradientcolor->setUpdatesEnabled( TRUE );
    //gradientcolor->repaint();
    
    QToolTip::add(  gradientcolor, tr( "Color gradation between max and min." ) );
    QWhatsThis::add(  gradientcolor, tr( "" ) );
    
    color2 = new QFrame( this, "color2" );
    color2->setGeometry( QRect( 100/*was 140 with maxslider in*/, 50, 71, 31 ) ); 
    color2->setAcceptDrops( FALSE );
    color2->setFrameShape( QFrame::StyledPanel );
    color2->setFrameShadow( QFrame::Raised );
    QToolTip::add(  color2, tr( "Maximum color selected." ) );

    color1 = new QFrame( this, "color1" );
    color1->setGeometry( QRect( 100/*was 140*/, 380, 71, 31 ) ); 
    color1->setAcceptDrops( FALSE );
    color1->setFrameShape( QFrame::StyledPanel );
    color1->setFrameShadow( QFrame::Raised );
    QToolTip::add(  color1, tr( "Minimum color selected." ) );

    minval = new QLineEdit( "0", this, "minval" );
    minval->setGeometry( QRect( 260, 430, 121, 31 ) ); 
    minval->setAcceptDrops( FALSE );

    pickval = new QLineEdit( this, "pickval" );
    pickval->setGeometry( QRect(260, 280, 121, 30) );
    pickval->setAcceptDrops( FALSE );

    pickvalcolor = new QFrame(this, "pickvalcolor" );
    pickvalcolor->setGeometry( QRect( 260, 310, 121, 30 ) );
    pickvalcolor->setAcceptDrops( FALSE );
    pickvalcolor->setFrameShape( QFrame::StyledPanel );
    pickvalcolor->setFrameShadow( QFrame::Raised );    

    Property_List = new QComboBox( FALSE, this, "Property_List" );
    Property_List->insertItem( tr( "Energy (eV)" ) );
    Property_List->setGeometry( QRect( 140, 220, 220, 30 ) ); 
    Property_List->setEditable( FALSE );
    Property_List->setCurrentItem( -1 );
    Property_List->setInsertionPolicy( QComboBox::AtBottom );
    QToolTip::add(  Property_List, tr( "" ) );

    OK = new QPushButton( this, "OK" );
    OK->setGeometry( QRect( 50, 500, 91, 41 ) ); 
    OK->setAcceptDrops( FALSE );
    OK->setText( tr( "OK" ) );
    QToolTip::add(  OK, tr( "Apply color scale to window." ) );

    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setGeometry( QRect( 270, 500, 90, 40 ) ); 
    Cancel->setAcceptDrops( FALSE );
    Cancel->setText( tr( "Cancel" ) );
    QToolTip::add(  Cancel, tr( "Cancel color scale application." ) );

    maxval = new QLineEdit( "0", this, "maxval" );
    maxval->setGeometry( QRect( 260, 70, 120, 30 ) ); 
    maxval->setAcceptDrops( FALSE );

    Select_Color_1 = new QPushButton( this, "Select_Color_1" );
    Select_Color_1->setGeometry( QRect( 80/*was 120*/, 420, 110, 40 ) ); 
    QFont Select_Color_1_font(  Select_Color_1->font() );
    Select_Color_1_font.setFamily( "adobe-helvetica" );
    Select_Color_1->setFont( Select_Color_1_font ); 
    Select_Color_1->setAcceptDrops( FALSE );
    Select_Color_1->setText( tr( "Select Color 1" ) );
    QToolTip::add(  Select_Color_1, tr( "Select color to correspond to minimum property value." ) );

    Select_Color_2 = new QPushButton( this, "Select_Color_2" );
    Select_Color_2->setGeometry( QRect( 80/*was 120*/, 90, 110, 40 ) ); 
    QFont Select_Color_2_font(  Select_Color_2->font() );
    Select_Color_2_font.setFamily( "adobe-helvetica" );
    Select_Color_2->setFont( Select_Color_2_font ); 
    Select_Color_2->setAcceptDrops( FALSE );
    Select_Color_2->setText( tr( "Select Color 2" ) );
    QToolTip::add(  Select_Color_2, tr( "Select color to correspond to maximum property value." ) );

    //////// signals and slots connections

    connect( Select_Color_1, SIGNAL( clicked() ), this, SLOT( minColorSelectSlot() ) );
    //connects the chosen color from the QColorDialog box with the mincolor display box

    connect( Select_Color_2, SIGNAL( clicked() ), this, SLOT(  maxColorSelectSlot() ) );
    //connects the chosen color from the QColorDialog box with the maxcolor display box

    connect( OK, SIGNAL( clicked() ),this, SLOT( accept() ) );
    //applies the color changes to the selected values

    connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    //cancels the dialog box with no changes

    connect( this, SIGNAL( colorChosen() ), this, SLOT( drawGradientBoxSlot() ) );
    //this connection calls the gradient code indirectly through the paintEvent function, updating the gradation each time a part has been invalidated

    connect( pickval, SIGNAL( returnPressed() ), this, SLOT( gradientplace() ) );
    //this connects the value typed in the pickval slot to the program

    connect (maxval, SIGNAL( returnPressed () ), this, SLOT( maxValueSlot() ) );
    //this connects the value typed in the maximum value box to the program, AKA to the getvalue for color slot

    connect (minval, SIGNAL( returnPressed () ), this, SLOT( minValueSlot() ) );   
    //this connects the value typed in the minimum value box to the program, AKA to the getvalue for color slot
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgQtColorScale::~IgQtColorScale()
{
    // no need to delete child widgets, Qt does it all for us
}


/*  
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool IgQtColorScale::event( QEvent* ev )
{
    bool ret = QDialog::event( ev ); 
    if ( ev->type() == QEvent::ApplicationFontChange )
    {
	QFont Maximum_font(  Maximum->font() );
	Maximum_font.setFamily( "adobe-helvetica" );
	Maximum->setFont( Maximum_font ); 
	QFont Select_Color_1_font(  Select_Color_1->font() );
	Select_Color_1_font.setFamily( "adobe-helvetica" );
	Select_Color_1->setFont( Select_Color_1_font ); 
	QFont Select_Color_2_font(  Select_Color_2->font() );
	Select_Color_2_font.setFamily( "adobe-helvetica" );
	Select_Color_2->setFont( Select_Color_2_font ); 
    }
    return ret;
}

void IgQtColorScale::minColorSelectSlot()
{
    //!this connects the newly chosen background color with the display box illustrating the color chosen for the min property type

    colorFlag = false;
    //!resets the bool variable to false each time the slot is utilized so that changes are recognized each time
    
    QColor 	backgroundColor;
    backgroundColor = color1->backgroundColor ();

    QColor	c = QColorDialog::getColor(backgroundColor, this );

    if	(c.isValid())
    {
	color1->setBackgroundColor(QColor ( c.red (), c.green (), c.blue () ) );
	//the if statement allows you to cancel the dialog without an invalid QColor being returned
    }
    mincolor = color1->backgroundColor();
    
    if ((!colorFlag) && (maxcolor.isValid())) 
    {
//	emit 
	colorChosen();
	//!this signal is emitted after both colors are chosen, it is connected to the gradient box slot and is responsible for the gradient appearing (the use of bool variables here allows the user to enter min or max first, and then once both are entered the signal is emitted)
	colorFlag = true;
    }
}

void IgQtColorScale::maxColorSelectSlot()
{
    //!this connects the newly chosen background color with the display box illustrating the color chosen for the max property type

    colorFlag = false;
    //resets the bool variable to false each time the slot is utilized so that updates are recognized
    
    QColor 	backgroundColor;
    backgroundColor = color1->backgroundColor ();
    //!this connects back to the 1st color chosen so the user can make an informed color choice based on the first color
    
    QColor	c = QColorDialog::getColor(backgroundColor, this );

    if	(c.isValid())
    {
	color2->setBackgroundColor(QColor ( c.red (), c.green (), c.blue () ) );
	//the if statement allows you to cancel the dialog without an invalid QColor being returned
    }
    maxcolor = color2->backgroundColor();

    if( (!colorFlag) && (mincolor.isValid()) )
    {
	emit colorChosen();
        //this signal is emitted after both colors are chosen, it is connected to the gradient box and is responsible for the gradient appearing 
	colorFlag = true;
    }
}


void IgQtColorScale::gradientColors(QPainter *painter)
{
    //!this function creates the gradient between the max and min color choices made by the user, the function paintEvent calls this function
    
    //mincolor and maxcolor have been made private members of the IgQtColorScale class so that they are global like variables within the class

    int h, s, v;
    int minHue, minSat, minVal, maxHue, maxSat, maxVal;
        
    if (mincolor.isValid())
    {
	mincolor.hsv( &h,  &s,  &v);
	minHue = h;
	minSat = s;
	minVal = v;
	//here we are obtaining the numeric values of the rgb triplet for the color selected to correspond to the minimum value, making sure that a valid color has been chosen
	
	if (maxcolor.isValid())
	{
	    maxcolor.hsv( &h, &s, &v);
	    maxHue = h;
	    maxSat = s;
	    maxVal = v;
	    //here we are obtaining the numeric values of the rgb triplet for the color selected to correspond to the maximum value
	    
	    width = gradientcolor->width();
	    height = gradientcolor->height(); 
	    //here we obtain the height and width of the gradient box

	    int hue, sat, val;
	    double scaleHue = (double(maxHue - minHue) / height);
	    double scaleSat = (double(maxSat - minSat) / height);
	    double scaleVal = (double(maxVal - minVal) / height);
	    //!these are the sizes of the increment steps that will be used when the colors are looped through
    
	    double   dh = maxHue;
	    double   ds = maxSat;
	    double   dv = maxVal;

	    for( int step = 0; step <= height; step++ ) 
	    {
		dh -= scaleHue;
		ds -= scaleSat;
		dv -= scaleVal;
		//!since dh, ds, and dv are defined as maximums each progressive step must decrement the max values until the minimum values are reached 	
	
		hue = (int) floor(dh + 0.5);
		sat = (int) floor(ds + 0.5);
		val = (int) floor(dv + 0.5);
		//since each line drawn must be in interger steps as demanded by the loop, the double values (dh, ds, and dv) must be converted into interger values
		  
		QColor  gradationColors( hue, sat, val, QColor::Hsv );
		painter->setPen(gradationColors);
		painter->drawLine(0, step, int (width), step);
		//!so for each step, the new color is obtained, the pen is set with that color and a line is drawn at the correct position along the gradient box
	    }
	}
    }
}

void  IgQtColorScale::paintEvent(QPaintEvent *)
{
    //!this function calls the painter in the gradientColors function.  each time colors are chosen the gradation is treated as a paintEvent
    QPainter painter(gradientcolor);
    gradientColors(&painter);
    
}

void  IgQtColorScale::drawGradientBoxSlot()
{
    //!this slot connects to the color chosen signal, and then updates the gradient color function since it is a paintEvent function.....so each time an event happens that invalidates a region of the gradient box it is updated here	
    update();
}
  
QColor& IgQtColorScale::getColorForValue( double colorposition ) 
{
    //!this function will return the corresponding color for a specified energy in the range specified.  this is the function that will be used to connect to the specified hits (or whatever they are) and assign them the correct colors for values
    
    if(colorFlag && valueFlag  && (minValue <= colorposition) && (colorposition <= maxValue) ) 
    {		    
	int hMin, sMin, vMin;
	int hMax, sMax, vMax;
	mincolor.hsv( &hMin, &sMin, &vMin);
	maxcolor.hsv( &hMax, &sMax, &vMax);  
	col.setHsv((int)((( colorposition - minValue)/(maxValue - minValue))*(hMax - hMin) + hMin ),
		   (int)((( colorposition - minValue)/(maxValue - minValue))*(sMax - sMin) + sMin ),
		   (int)((( colorposition - minValue)/(maxValue - minValue))*(vMax - vMin) + vMin ) );    
    }
    //the scale factors in setHsv take into account all the relavent parameters:  so it first finds the normalized value position being considered, and then is multiplied by the difference in hue/sat/val from max to min, and finally it is added to the min hue/sat/val so that for each colorposition from maxValue to minValue the correct step up from min hue/sat/val is being made 
    else
    {
	QMessageBox::warning((QWidget *) this, "Incompleted fields",
			     "All fields must be completed correctly. Please fill in appropriate fields." );
	//!if the necessary fields, the two colors and the min/maxValue, are not both filled in will this error appear, it should also appear if a colorposition outside of the min/maxValue range is asked for
    }
    return col;
}


void IgQtColorScale::minValueSlot()
{
    //!in this slot the min value is read from the qlineedit box
    
    valueFlag = false;
    //resets the bool variable to false each time the slot is utilized so that updates are recognized
    
    QString strmin = minval->text();
    minValue = strmin.toDouble();
    //here the minValue is identified from the QLineEdit input stream

    if ((!valueFlag) && (maxValue <= minValue) ) 
    {
	QMessageBox::warning((QWidget *) this, "", "Maximum must be greater than minimum." );
    }
    else
    {
	valueFlag = true;
    }
    //!the use of bool variables here allows the user to enter min or max first, and then once both are entered the decision is made as to whether or not the max is really greater than the min, if it is not an error box appears
}

void IgQtColorScale::maxValueSlot()
{
    //!in this slot the max value is read from the qlineedit box

    valueFlag = false;
    //resets the bool variable to false each time the slot is utilized so that updates are recognized
    
    QString strmax = maxval->text();
    maxValue = strmax.toDouble();
    //here the maxValue is identified from the QLineEdit input stream

    if ((!valueFlag) && (minValue >= maxValue) ) 
    {
	QMessageBox::warning((QWidget *) this, "", "Maximum must be greater than minimum." );
    }
    else
    {
	valueFlag = true;
    }
    
    //the use of bool variables here allows the user to enter min or max first, and then once both are entered the decision is made as to whether or not the max is really greater than the min, if it is not an error box appears
}

void IgQtColorScale::gradientplace()
{
    //!this function allows the user to enter a value contained in the max/min range, and it then returns the color corresponding to the value entered
    
    valueFlag = false;
    //resets the bool variable to false each time the slot is utilized so that updates are recognized
    
    QString str = pickval->text();
    chosenvalue = str.toDouble();
    //here the pickval is identified from the QLineEdit input stream
    
    if( (!valueFlag) && (minValue <= chosenvalue) && (chosenvalue <= maxValue) ) 
    {
	valueFlag = true;
	QColor realColor = getColorForValue( chosenvalue );
	//!utilize the getColorForValue function in order to obtain the corresponding color for the value entered

	pickvalcolor->setBackgroundColor(realColor);
	//!set the color retrieved from the getColorforValue function to the display box so the user can easily identify it
    }
    else
    {
	QMessageBox::warning((QWidget *) this, "Outside of Range.",
			     "You have chosen a value outside of the min/max range.  Please choose a new value." );
	pickvalcolor->setBackgroundColor("gray");
    }
}



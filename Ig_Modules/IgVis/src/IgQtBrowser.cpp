#include "Ig_Modules/IgVis/interface/IgQtBrowser.h"

#include <qapplication.h>
#include <qnamespace.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qimage.h>
#include <qfiledialog.h>
#include <qgl.h>

#include <iostream> 
#include <fstream>
#include "Ig_Imports/gl2ps/src/gl2ps.h"
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoColorIndex.h>
#include "Ig_Modules/IgVis/interface/x.xpm"
#include "Ig_Modules/IgVis/interface/y.xpm"
#include "Ig_Modules/IgVis/interface/z.xpm"
#include "Ig_Modules/IgVis/interface/Inverse.xpm"
#include "Ig_Extensions/IgQt/interface/IgNetscapeRunner.h"

IgQtBrowser::IgQtBrowser(
    QWidget *parent,
    const char *name,
    SbBool buildInsideParent, 
    SoQtFullViewer::BuildFlag flag,
    SoQtViewer::Type type) :
    SoQtExaminerViewer( parent, name, buildInsideParent, flag, type, FALSE )
{
    setBottomWheelString( "Rot Z" );
    setLeftWheelString( "" );
    setTitle( name );
    if ( isFeedbackVisible() ) //complains on setting to same state
    {
	setFeedbackVisibility( false ); //crashes w/ both visibility and x-axis snap
    }
    if( flag ) {
      QWidget* w = this->buildWidget( this->getParentWidget());
	  this->setBaseWidget( w );
    }
    thePrinter = new QPrinter;
    slave = NULL;
    helpRunner = new IgNetscapeRunner( this );
}

IgQtBrowser::IgQtBrowser(
	QWidget * /*parent*/,
	const char * /*name*/, 
	SbBool /*buildInsideParent*/, 
	SoQtFullViewer::BuildFlag /*flag*/,
	SoQtViewer::Type /*type*/, SbBool /*buildflag*/ )
{
	// need to implement this if we want to sub-class the browser
}
IgQtBrowser::~IgQtBrowser()
{
    delete thePrinter;
    if (slave)
    {
	// FIXME: does this leave windows lying about hidden? goa 2 Jan 2001
	slave->hide();
	delete slave;
    }
    delete helpRunner;
}

// Translate the camera right/left (called by thumb wheel).
//
void
IgQtBrowser::bottomWheelMotion( float val )
{
    this->reorientCamera( SbRotation( SbVec3f( 0.0f, 0.0f, -1.0f ),
				    this->getBottomWheelValue() - val ));
    SoQtFullViewer::bottomWheelMotion( val );
    
}

QWidget *
IgQtBrowser::buildLeftTrim(QWidget * parent)
{
    QWidget *frame = new QWidget(parent);
    frame->setFixedWidth( 30 );

    QButtonGroup *group = new QButtonGroup( frame );
    group->setColumnLayout( 0, Qt::Vertical );
    group->layout()->setSpacing( 0 );
    group->layout()->setMargin( 0 );

    QPushButton	*xBtn   = new QPushButton( QPixmap( const_cast<const char **> ( x_xpm )), QString::null, group );
    QPushButton	*yBtn   = new QPushButton( QPixmap( const_cast<const char **> ( y_xpm )), QString::null, group );
    QPushButton	*zBtn   = new QPushButton( QPixmap( const_cast<const char **> ( z_xpm )), QString::null, group );
    QPushButton	*invBtn = new QPushButton( QPixmap( const_cast<const char **> ( inverse_xpm )), QString::null, group );
    QVBoxLayout *groupLayout = new QVBoxLayout( group->layout());

    // set size by pixmaps of other decorations
    xBtn->setAutoDefault( false );
    xBtn->setFixedSize( QSize( 28, 28 ));
    xBtn->setFocusPolicy( QWidget::NoFocus );
    QToolTip::add( xBtn, "Align camera with the X axis" );
    QWhatsThis::add( xBtn, tr("<b>X Button:</b>\n"
			      "Aligns the camera with the X axis." ));
    QObject::connect( xBtn, SIGNAL( clicked()),
		      this, SLOT( xClicked()));  
    groupLayout->addWidget( xBtn );

    yBtn->setFixedSize( QSize( 28, 28 ));
    yBtn->setFocusPolicy( QWidget::NoFocus );
    QToolTip::add( yBtn, "Align camera with the Y axis" );
    QWhatsThis::add( yBtn, tr("<b>Y Button:</b>\n"
			      "Aligns the camera with the Y axis." ));
    QObject::connect( yBtn, SIGNAL( clicked()),
		      this, SLOT( yClicked()));
    groupLayout->addWidget( yBtn );

    zBtn->setFixedSize( QSize( 28, 28 ));
    zBtn->setFocusPolicy( QWidget::NoFocus );
    QToolTip::add( zBtn, "Align camera with the Z axis" );
    QWhatsThis::add( zBtn, tr("<b>Z Button:</b>\n"
			      "Aligns the camera with the Z axis." ));
    QObject::connect( zBtn, SIGNAL( clicked()),
		      this, SLOT( zClicked()));
    groupLayout->addWidget( zBtn );

    invBtn->setFixedSize( QSize( 28, 28 ));
    invBtn->setFocusPolicy( QWidget::NoFocus );
    QToolTip::add( invBtn, "Invert camera co-ordinates through origin" );
    QWhatsThis::add( invBtn, tr("<b>Invert Button:</b>\n"
				"Inverts the position and view of the camera"
				" through origin." ));
    QObject::connect( invBtn, SIGNAL( clicked()),
		      this, SLOT( invClicked()));
    groupLayout->addWidget( invBtn );

    // Put the button group in a layout.
    QVBoxLayout* frameLayout = new QVBoxLayout( frame );
    frameLayout->setSpacing( 0 );
    frameLayout->setMargin( 0 );
    frameLayout->addWidget( group );
    frameLayout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Fixed,
                                           QSizePolicy::Expanding ));
    return frame;
}

void
IgQtBrowser::xClicked( void )
{
    this->viewPlaneX();
}

void
IgQtBrowser::yClicked( void )
{
    this->viewPlaneY();
}

void
IgQtBrowser::zClicked( void )
{
    this->viewPlaneZ();
}

void
IgQtBrowser::invClicked( void )
{
    this->viewInvPlane();
}

void
IgQtBrowser::viewPlaneX( void ) const
{
    SoCamera* cam = this->getCamera();
    if ( ! cam ) return; // probably a scene-less viewer

#if 0 // an attempt at a version which always points to the point of interest rather than the origin
	SbVec3f norient = SbVec3f(-1,0,0); //correct orientation looking down this axis
	SbVec3f minusZ(0.f,0.f,-1.f);
	SbVec3f d1, d2;
	SbVec3f position = cam->position.getValue();
	float fd = cam->focalDistance.getValue();
	cam->orientation.getValue().multVec(fd*minusZ,d1);
	cam->orientation = SbRotation(SbVec3f(0,0,1),M_PI/2.f)*SbRotation(SbVec3f(0,0,-1),norient);
	cam->orientation.getValue().multVec(fd*minusZ,d2);
	cam->position = position + d1 - d2;
#else
	SbVec3f norient = SbVec3f(-1,0,0); //correct orientation looking down this axis
	cam->position = -norient * cam->position.getValue().length();
	cam->orientation = SbRotation(SbVec3f(0,0,1),M_PI/2.f)*SbRotation(SbVec3f(0,0,-1),norient);
#endif
}

/*!
*/

void
IgQtBrowser::viewPlaneY( void ) const
{
    SoCamera* cam = this->getCamera();
    if ( ! cam ) return; // probably a scene-less viewer

	SbVec3f norient = SbVec3f(0,-1,0); //correct orientation looking down this axis
	cam->position = -norient * cam->position.getValue().length();
	cam->orientation = SbRotation(SbVec3f(0,0,1),-M_PI/2.f)*SbRotation(SbVec3f(0,0,-1),norient);
}

/*!
*/

void
IgQtBrowser::viewPlaneZ( void ) const
{
    SoCamera* cam = this->getCamera();
    if ( ! cam ) return; // probably a scene-less viewer

	SbVec3f norient = SbVec3f(0,0,-1); //correct orientation looking down this axis
	cam->position = -norient * cam->position.getValue().length();
	cam->orientation = SbRotation::identity();
}

void
IgQtBrowser::viewInvPlane( void ) const
{
    SoCamera* cam = this->getCamera();
    if ( ! cam ) return; // probably a scene-less viewer

    float distance = (( cam->position ).getValue()).length();
	SbVec3f up(0.F,1.F,0.F); // nominal up direction
	cam->orientation.getValue().multVec(up,up); // current up direction
    SbVec3f newAxis = cam->position.getValue();
    newAxis.normalize();
    cam->position = -distance*newAxis;
	SbRotation flip = cam->orientation.getValue()*SbRotation(up,M_PI);
    ( cam->orientation ).setValue( flip ); 
}

void
IgQtBrowser::reorientCamera( const SbRotation & rot )
{
    SoCamera * cam = this->getCamera();
    if( cam == NULL ) return;
    
    // Find global coordinates of focal point.
    SbVec3f direction;
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    SbVec3f focalpoint =
	cam->position.getValue() + cam->focalDistance.getValue() * direction;
    
    // Set new orientation value by accumulating the new rotation.
    cam->orientation = rot * cam->orientation.getValue();
    
    // Reposition camera so we are still pointing at the same old focal
    // point.
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    cam->position = focalpoint - cam->focalDistance.getValue() * direction;
}

/*!
  Overloaded from parent class to add the tool tips
*/
void
IgQtBrowser::createViewerButtons( QWidget* parent, SbPList* buttonlist )
{
    inherited::createViewerButtons( parent, buttonlist );

    BtnInfo selectBtn;
    selectBtn.btnToolTipInfo  = "Select/Pick";
    selectBtn.btnWhatThisInfo = "<b>Select/Pick Button:</b>\n"
				"Selects object manipulation or pick mode \n"
				"(and deselects camera or viewer mode).\n"
				"The cursor shape will change to an arrow.\n"
				"In this mode, the user is manipulating objects\n"
				"in the scene graph.";
    
    btnInfoList.append( &selectBtn );

    BtnInfo viewBtn;
    viewBtn.btnToolTipInfo  = "View";
    viewBtn.btnWhatThisInfo = "<b>View Button:</b>\n"
			      "Selects camera or viewer mode (and deselects object \n"
			      "manipulation or pick mode). The cursor shape will\n"
			      "change to a circular arrows icon. In this mode,\n"
			      "the user is moving the camera in 3D space.";
    
    btnInfoList.append( &viewBtn );


    BtnInfo homeBtn;
    homeBtn.btnToolTipInfo  = "Home";
    homeBtn.btnWhatThisInfo = "<b>Home Button:</b>\n"
			      "Returns the camera to its home position\n"
			      "(initial position if not reset).";
    
    btnInfoList.append( &homeBtn );

    BtnInfo setHomeBtn;
    setHomeBtn.btnToolTipInfo  = "Set Home";
    setHomeBtn.btnWhatThisInfo = "<b>Set Home Button:</b>\n"
				 "Resets the home position to the current\n"
				 "camera position.";
    
    btnInfoList.append( &setHomeBtn );

    BtnInfo viewAllBtn;
    viewAllBtn.btnToolTipInfo  = "View All";
    viewAllBtn.btnWhatThisInfo = "<b>View All Button:</b>\n"
				 "Brings the entire scene graph into view.";
   
    btnInfoList.append( &viewAllBtn );

    BtnInfo seekBtn;
    seekBtn.btnToolTipInfo  = "Seek";
    seekBtn.btnWhatThisInfo = "<b>Seek Button:</b>\n"
			      "Allows the user to select a new center\n"
			      "of rotation for the camera. When clicked\n"
			      "on (and in viewer mode) the cursor changes\n"
			      "to a crosshair. The next left mouse\n"
			      "buttonpress causes whatever is underneath\n"
			      "the cursor to be selected as the new center\n"
			      "of rotation. Once the button is released,\n"
			      "the camera either jumps or animates to its\n"
			      "new position depending on the current\n"
			      "setting of the seek time in the preferences\n"
			      "dialog box.";
    
    btnInfoList.append( &seekBtn );

    BtnInfo projectBtn;
    projectBtn.btnToolTipInfo  = "Projection";
    projectBtn.btnWhatThisInfo = "<b>Projection Button:</b>\n"
				 "Selects the type of camera used\n"
				 "by the viewer. It toggles between\n"
				 "the two available camera types -\n"
				 "perspective and orthographic.";

    btnInfoList.append( &projectBtn );

    int nrbuttons = buttonlist->getLength();
    
    if( nrbuttons == 0 ) return;
	
    for( int i = 0; i < nrbuttons; i++ ) 
    {	
	ASSERT( btnInfoList.at(i));
	QToolTip::add(( QWidget* )(( *buttonlist )[i]), btnInfoList.at(i)->btnToolTipInfo );   
	QWhatsThis::add(( QWidget* )(( *buttonlist )[i]), tr( btnInfoList.at(i)->btnWhatThisInfo ));
    }
}

void IgQtBrowser::openViewerHelpCard( void )
{
    helpRunner->netscape( "http://iguana.web.cern.ch/iguana/help/buttons.html" );
}

void IgQtBrowser::printSlot()
{
    const float screenPixelsPerInch = SoOffscreenRenderer::getScreenPixelsPerInch();
    QString vectorEPS("Vector eps (*.eps)");
    QString vectorPDF("Vector pdf (*.pdf)");
    QString lowResEPS("Screen Resolution eps (*.eps)");
    QString medResEPS("300 DPI Resolution eps (*.eps)");
    QString medResJPEG("JPEG, Quality = 0.7 (*.jpg)");
    QString highResJPEG("JPEG, Quality = 1.0 (*.jpg)");
    QString plainJPEG("JPEG (*.jpg");
    QString medResTIFF("TIFF (*.tif)");
    QStringList filters(vectorEPS);
#if TGS_VERSION >= 3000
    filters.append(medResTIFF);
    filters.append(medResJPEG);
    filters.append(highResJPEG);
#elif TGS_VERSION >= 2600  
    filters.append(medResTIFF); 
    filters.append(medResJPEG);    // ok, maybe
    filters.append(highResJPEG);
    filters.append(lowResEPS);     // bad bounding box?
    filters.append(medResEPS);
#elif defined(__COIN__) && (COIN_MAJOR_VERSION==2 && COIN_MINOR_VERSION>1) || COIN_MAJOR_VERSION>2 
// This requires presence of simage and libjpeg to work; other libraries supported by simage are libpng, libtiff, libungif.
// The only formats supported internally are Silicon Graphics RGB and (encapsulated) PostScript

// Create a test renderer to probe the options
    SbViewportRegion outputVR;
    outputVR.setWindowSize(SbVec2s(100,100));
    outputVR.setPixelsPerInch(72);
    SoOffscreenRenderer renderer(outputVR);
    if (renderer.isWriteSupported("jpg")) 
    {
	filters.append(plainJPEG);
    }
#else
    filters.append(medResEPS); 
    filters.append(lowResEPS);
    filters.append(vectorPDF);
#endif
    QFileDialog saveDialog(this->getParentWidget(),
			   "SavePrintFile", true);
    saveDialog.setMode( QFileDialog::AnyFile );
    saveDialog.setFilters(filters);
    if (saveDialog.exec() == QDialog::Accepted)
    {
	QString f = saveDialog.selectedFile();
	QString format;
	float printDPI = 300;
	float quality = 0.7;
	if (saveDialog.selectedFilter()== lowResEPS) {
	    printDPI = screenPixelsPerInch;
	    format = "eps";
	}
	else if (saveDialog.selectedFilter()== medResEPS) {
	    format = "eps";
	    printDPI = 300;
	}
	else if (saveDialog.selectedFilter()== medResJPEG  || saveDialog.selectedFilter()== plainJPEG){
	    printDPI = 300;
	    format = "jpg";
	    quality = 0.7;
	}
	else if (saveDialog.selectedFilter()== highResJPEG) {
	    printDPI = 300;
	    format = "jpg";
	    quality = 1.0;
	}
	else if (saveDialog.selectedFilter()==medResTIFF) {
	    printDPI = 300;
	    format = "tif";
	}
	else if (saveDialog.selectedFilter()==vectorEPS) {
	    format = "eps";
	    quality = 1.0;
	}
	else if (saveDialog.selectedFilter()==vectorPDF) {
	    format = "pdf";
	    quality = 1.0;
	}
	print(f,format,printDPI, quality);
    }
}

void IgQtBrowser::printDefSlot(int i)
{
    QString f("Test_");
    QString axis;
    if (i==0) {
	axis = 'x';}
    else if (i==1) {
	axis = 'y'; }
    else if (i==2) {
	axis = 'z';}
    else {
	axis = "Unknown";
    }
#ifdef __COIN__
#if (COIN_MAJOR_VERSION==2 && COIN_MINOR_VERSION>1) || COIN_MAJOR_VERSION>2 
    // FIXME: don't do this every call GOA 2005 08 02
    // create test renderer
    SbViewportRegion outputVR;
    outputVR.setWindowSize(SbVec2s(100,100));
    outputVR.setPixelsPerInch(72);
    SoOffscreenRenderer renderer(outputVR);
    if (renderer.isWriteSupported("jpg"))
    {
	print(f+axis,"jpg",100,80);
    }
    else
    {
	print(f+axis,"eps",100,80);
    }
#endif
    print(f+axis,"eps",100,80);
#else
    print(f+axis,"jpg",100,80);
#endif
}

void IgQtBrowser::print( const QString filename, const QString format,
			const float printDPI, const float quality )
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    QMainWindow *mw = dynamic_cast<QMainWindow *>(qApp->mainWidget());
    if (mw) mw->statusBar()->message(QString("Printing...")+filename);
    // get and set the background color change
    float redBg;
    float greenBg;
    float blueBg;
    getSceneManager()->getBackgroundColor().getValue( redBg, greenBg, blueBg );

    // Guts of this come from the example in _Inventor Mentor_

    const float screenPixelsPerInch = SoOffscreenRenderer::getScreenPixelsPerInch();
    float dpi = printDPI;
    if (dpi <= 0) dpi = screenPixelsPerInch;
    const SbViewportRegion &screenVR = getViewportRegion();
    const SbVec2s imagePixSize = screenVR.getViewportSizePixels();
    int x = imagePixSize[0];
    int y = imagePixSize[1];
    short xs = (short)( x * dpi/screenPixelsPerInch );
    short ys = (short)( y * dpi/screenPixelsPerInch );
    QString f = filename + QString(".") + format; //add the appropriate extension to the filename

    // want to render from above the SceneGraph so we get what the camera sees
    // SoQtViewer uses the following private code GETPARENTOFNODE(SoNode * root, SoNode * node)
    SoNode *root = static_cast<SoNode*>(SoQtRenderArea::getSceneGraph());
    SoNode *node = static_cast<SoNode*>(this->getCamera());
    SoSearchAction search;
    assert(node);
    search.setNode(node);
    assert(root);
    search.apply(root);
    assert(search.getPath());
    SoGroup *hiddenRoot = 
	static_cast<SoGroup*>( static_cast<SoFullPath *>(search.getPath())->getNodeFromTail(1));
    if ((format=="eps" || format=="pdf") && quality > 0.7 ) // vector eps/pdf
    {
	// Use gl2ps to print vector graphics output.  To be extended to
	// handle other vector graphics formats (SVG, WMF).
	if (FILE *output = fopen (f.utf8 (), "wb+"))
	{
	    int type = GL2PS_EPS;
	    if ( format == "pdf" )
	    {
		type = GL2PS_PDF;
	    }
	    int buffsize = 0, state = GL2PS_OVERFLOW;
	    while (state == GL2PS_OVERFLOW)
	    { 
		buffsize += 1024*1024;
		int gl2psOptions = GL2PS_SILENT | GL2PS_USE_CURRENT_VIEWPORT
		                     | GL2PS_SIMPLE_LINE_OFFSET
				     | GL2PS_BEST_ROOT
				     | GL2PS_OCCLUSION_CULL;
		gl2psBeginPage ("IGUANA Scene Graph", "IGUANA", NULL,
		    type, GL2PS_BSP_SORT,
		    gl2psOptions, 
		    GL_RGBA, 0, NULL,0, 0, 0,
		    buffsize, output, NULL);
		actualRedraw ();
		state = gl2psEndPage();
	    }
	    fclose (output);
	}
    }
    else 
    {
	SbViewportRegion outputVR;
	outputVR.setWindowSize(SbVec2s(xs,ys));
	outputVR.setPixelsPerInch(dpi);
	SoOffscreenRenderer renderer(outputVR);
	renderer.setBackgroundColor( SbColor ( redBg, greenBg, blueBg ));
	SoGLRenderAction *ra = getGLRenderAction();
	if (ra) renderer.setGLRenderAction(ra);
	ra->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND) ;
	ra->setSmoothing(TRUE) ;
	ra->setNumPasses(4) ;
	if (renderer.render(hiddenRoot)) {
	    //TODO: need to kick start the screen renderer to get it going after the render action
	    // can do this as user by starting new viewer and refreshing the original one
	    if (FILE *outFile = fopen(f.latin1(),"w+")) {
		if (format=="eps") {
		    renderer.writeToPostScript(outFile); }
#if TGS_VERSION >= 2600
		else if (format=="jpg") {
		    renderer.writeToJPEG(outFile,quality); }
		else if (format=="tif") {
		    renderer.writeToTIFF(outFile); }
#elif  defined(__COIN__) && ((COIN_MAJOR_VERSION==2 && COIN_MINOR_VERSION>1) || COIN_MAJOR_VERSION>2)
		else if (format=="jpg") {
		    renderer.writeToFile(f.latin1(),"jpg"); }
#endif //TGS_VERSION
	    }
	}
    }
    if (mw) mw->statusBar()->clear();
    QApplication::restoreOverrideCursor();
}

void IgQtBrowser::infoOverlay(bool enable)
{
    //FIXME: debug. goa 01012001
    bool hasOverlays = QGLFormat::hasOpenGLOverlays();
	
    if (enable) 
    {
	// set up for overlay plane
	int color_index = 1;
	SbColor overlayColor(1, 0, 0);
	SoLightModel *lightModel = new SoLightModel;
	lightModel->model.setValue(SoLightModel::BASE_COLOR);
		
	SoColorIndex *colorIndex = new SoColorIndex;
	colorIndex->index = color_index;
		
	SoDrawStyle *lineDrawStyle = new SoDrawStyle;
	lineDrawStyle->style = SoDrawStyle::LINES;

	SoSeparator *topSep = new SoSeparator;
	topSep->addChild(lineDrawStyle);
	topSep->addChild(lightModel);
	topSep->addChild(colorIndex);

	//////// Axes
		
		//make a set of axes
	SoSeparator *axisSep = new SoSeparator;
		
	//the lines for the axes
	SoLineSet *theAxes = new SoLineSet;
	int numVertices[3] = {2,2,2}; // three line segments with 2 vertices each
	const SbVec3f xyz[6] = {
	    SbVec3f(0,0,0),SbVec3f(1,0,0),SbVec3f(0,0,0),
	    SbVec3f(0,1,0),SbVec3f(0,0,0),SbVec3f(0,0,1)
	};
	SoVertexProperty *vertProperty = new SoVertexProperty;
	vertProperty->vertex.setValues(0,6,xyz);
	theAxes->vertexProperty.setValue(vertProperty);
	theAxes->numVertices.setValues(0,3,numVertices);
	axisSep->addChild(theAxes);
		
		//Labels for the axes
	SoFont *axisFont = new SoFont;
	//		axisFont->name.setValue("Times-Roman");
	axisFont->size.setValue(24.0);
	axisSep->addChild(axisFont);
		
	SoText2 *xLabel = new SoText2;
	SoText2 *yLabel = new SoText2;
	SoText2 *zLabel = new SoText2;
	SoTranslation *xTranslation = new SoTranslation;
	SoTranslation *yTranslation = new SoTranslation;
	SoTranslation *zTranslation = new SoTranslation;
	SoSeparator *xSep = new SoSeparator;
	SoSeparator *ySep = new SoSeparator;
	SoSeparator *zSep = new SoSeparator;
		
	xLabel->string = "x";
	yLabel->string = "y";
	zLabel->string = "z";
	xTranslation->translation.setValue(1.1F,0,0);
	yTranslation->translation.setValue(0,1.1F,0);
	zTranslation->translation.setValue(0,0,1.1F);
		
		
	axisSep->addChild(xSep);
	xSep->addChild(xTranslation);
	xSep->addChild(xLabel);
		
	axisSep->addChild(ySep);
	ySep->addChild(yTranslation);
	ySep->addChild(yLabel);
		
	axisSep->addChild(zSep);
	zSep->addChild(zTranslation);
	zSep->addChild(zLabel);
		
	SoTransform *testTransform = new SoTransform;
	testTransform->scaleFactor.setValue(0.2F,0.2F,0.2F);
	testTransform->translation.setValue(-0.7F,-0.6F,0.0F);
		
		
	////////////////////////////////
		
		
		// Must get camera and duplicate inverse rotations
	SoSeparator *testSep = new SoSeparator;
	SoCamera *theCamera = getCamera();
	SoDecomposeRotation * viewCameraRot = new SoDecomposeRotation;
	viewCameraRot->rotation.connectFrom(&theCamera->orientation);
	SoCalculator *rotCalculator = new SoCalculator;
	rotCalculator->expression.set1Value(0,"oa = -a");
	rotCalculator->a.connectFrom(&viewCameraRot->angle);
		
	SoComposeRotation *realCameraRot = new SoComposeRotation;
	realCameraRot->angle.connectFrom(&rotCalculator->oa);
	realCameraRot->axis.connectFrom(&viewCameraRot->axis);
		
	SoRotation *cameraRotation = new SoRotation;
	cameraRotation->rotation.connectFrom(&realCameraRot->rotation);
	testSep->addChild(testTransform);
	testSep->addChild(cameraRotation);
	testSep->addChild(axisSep);

	//scale
	if (theCamera->isOfType(SoOrthographicCamera::getClassTypeId()))
	{	
	    SoSeparator *scaleSep = new SoSeparator;

	    SoFont *scaleFont = new SoFont;
	    scaleFont->name.setValue("Times-Roman");
	    scaleFont->size.setValue(24.0);
	    scaleSep->addChild(scaleFont);

	    SoOrthographicCamera *orthoCam = static_cast<SoOrthographicCamera*>(theCamera);
	    SoText2 *scaleText = new SoText2;
	    scaleText->justification = SoText2::RIGHT;
	    scaleText->string.connectFrom(&orthoCam->height);
	    SoTransform *scaleTransform = new SoTransform;
	    scaleTransform->translation.setValue(0.7F,0.0F,0.0F);
	    scaleSep->addChild(scaleTransform);
	    scaleSep->addChild(scaleText);
	    topSep->addChild(scaleSep);

	}			
		
	topSep->addChild(testSep);
	if (hasOverlays) 
	{
	    setOverlayColorMap(color_index, 1, &overlayColor);
	    setOverlaySceneGraph(topSep);
	}
	else 
	{
            // although it is possible to emulate overlays using direct GL
	    // calls (see the TGS logo emulation in the Ivf browser)
	    // we just pop a new window for now

	    // kill the window... just clutters things up
#if 0
	    slave = new SoQtRenderArea(getWidget(),0,false); //TODO: get this as child of workspace, 16 jan 2001 goa
	    slave->setSceneGraph(topSep);
	    slave->show();
#endif //kill clutter
	}
    }
    else
    {
	if (hasOverlays)
	{
	    setOverlaySceneGraph(NULL);
	}
	else
	{
	    // FIXME: does this leave windows lying about hidden? goa 2 Jan 2001
	    if (slave) 
	    {
		slave->hide();
		delete slave;
	    }

	}
    }
	
}

void IgQtBrowser::toggleCameraType()
{
    infoOverlay(false);
    inherited::toggleCameraType();
    infoOverlay(true);
}


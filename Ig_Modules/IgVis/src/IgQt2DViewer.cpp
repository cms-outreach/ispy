#ifndef QT_CLEAN_NAMESPACE
#define QT_CLEAN_NAMESPACE
#endif //QT_CLEAN_NAMESPACE

#include "Ig_Modules/IgVis/interface/IgQt2DViewer.h"

#include <qapplication.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qimage.h>
#include <qfiledialog.h>
#include <qwhatsthis.h>
#include <qtooltip.h>


#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SoOffscreenRenderer.h>

#include "Ig_Imports/gl2ps/src/gl2ps.h"

IgQt2DViewer::IgQt2DViewer( QWidget *parent, QString title,
						   SbBool embed, SoQtFullViewer::BuildFlag flag,
						   SoQtViewer::Type type) :
SoQtFullViewer( parent, title, embed, flag, type, false )
{
	constructor(true);	
	
}
IgQt2DViewer::IgQt2DViewer( QWidget *parent, QString title,
						   SbBool embed, SoQtFullViewer::BuildFlag flag,
						   SoQtViewer::Type type, bool buildnow) :
SoQtFullViewer( parent, title, embed, flag, type, false )
{
	constructor(buildnow);
}
IgQt2DViewer::~IgQt2DViewer()
{
}
void IgQt2DViewer::constructor(bool /*build*/)
{
    QWidget * viewer = buildWidget( getParentWidget() );
    setBaseWidget( viewer );
    setCameraType(SoOrthographicCamera::getClassTypeId());
    setBottomWheelString("Motion X");
    setLeftWheelString("Motion Y");

    // a headlight will cause an over-bright reflection off planar surfaces and colors should
    // be luminous to be visible in G. Barrand's PostScript renderer
    setHeadlight(false);
}

void IgQt2DViewer::bottomWheelMotion(float value)
{
	float delta =  value - this->getBottomWheelValue();
	SoCamera *camera = getCamera();
	if ( ! camera ) return; // probably a scene-less viewer
	
	SbVec3f dir;
	camera->orientation.getValue().multVec( SbVec3f( 1, 0, 0 ), dir );
	camera->position = camera->position.getValue() - dir * delta *
		camera->focalDistance.getValue() / 2.5f;
	inherited::bottomWheelMotion(value);
}

void IgQt2DViewer::leftWheelMotion(float value) 
{
	float delta =  value - this->getLeftWheelValue();
	SoCamera *camera = getCamera();
	if ( ! camera ) return; // probably a scene-less viewer
	
	SbVec3f dir;
	camera->orientation.getValue().multVec( SbVec3f( 0, 1, 0 ), dir );
	camera->position = camera->position.getValue() + dir * delta *
		camera->focalDistance.getValue() / 2.5f;
	
	inherited::leftWheelMotion( value );
	
}

void IgQt2DViewer::rightWheelMotion(float value)
{
	zoom( this->getRightWheelValue() - value );
	inherited::rightWheelMotion(value);
}

void IgQt2DViewer::zoom( float difference )
{
    SoCamera *camera = getCamera();
    if ( ! camera ) return; // probably a scene-less viewer
	
    SoType cameraType = camera->getTypeId();
    float multiplicator = exp( difference );// in the range of <0, ->>
	
    if ( cameraType.isDerivedFrom( SoOrthographicCamera::getClassTypeId()))
    {
	SoOrthographicCamera* orthocam = ( SoOrthographicCamera* ) camera;
	orthocam->height = orthocam->height.getValue() * multiplicator;
    }
    // ignore if not an orthographic camera
}

void IgQt2DViewer::print( const QString filename, const QString format,
			 const float printDPI, const float quality )
{
    // This routine is an exact copy of the one in IgQtBrowser:
    //   should abstract to a single instance
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
#endif //TGS_VERSION/COIN2
	    }
	}
    }
    if (mw) mw->statusBar()->clear();
    QApplication::restoreOverrideCursor();
}
void IgQt2DViewer::printSlot()
{
    // This routine is an exact copy of the one in IgQtBrowser:
    //   should abstract to a single instance
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
    QStringList filters(medResTIFF); 
    filters.append(medResJPEG);    // ok, maybe
    filters.append(highResJPEG);
    filters.append(lowResEPS);     // bad bounding box?
    filters.append(medResEPS);
#elif defined(__COIN__) && (COIN_MAJOR_VERSION==2 && COIN_MINOR_VERSION>1) || COIN_MAJOR_VERSION>2 
    // create test renderer
    SbViewportRegion outputVR;
    outputVR.setWindowSize(SbVec2s(100,100));
    outputVR.setPixelsPerInch(72);
    SoOffscreenRenderer renderer(outputVR);
    if (renderer.isWriteSupported("jpg"))
    {
	filters.append(plainJPEG);
    }
#else
    filters.append(medResEPS); // best results
    filters.append(lowResEPS);
    filters.append(vectorPDF);
#endif
    QFileDialog saveDialog(this->getParentWidget(),
		"Save Print file as ...", true);
    saveDialog.setMode( QFileDialog::AnyFile );
    saveDialog.setFilters(filters);
    if (saveDialog.exec() == QDialog::Accepted) 
    {
	QString f = saveDialog.selectedFile();
	QString format;
	float printDPI = 300;
	float quality = 0.7F;
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
	    quality = 0.7F;
	}
	else if (saveDialog.selectedFilter()== highResJPEG) {
	    printDPI = 300;
	    format = "jpg";
	    quality = 1.0F;
	}
	else if (saveDialog.selectedFilter()==medResTIFF) {
	    printDPI = 300;
	    format = "tif";
	}
	else if (saveDialog.selectedFilter()==vectorEPS) {
	    printDPI = 72;
	    format = "eps";
	    quality = 1.0F;
	}
	else if (saveDialog.selectedFilter()==vectorPDF) {
	    format = "pdf";
	    quality = 1.0;
	}
	print(f,format,printDPI, quality);
    }
}
void
IgQt2DViewer::createViewerButtons( QWidget* parent, SbPList* buttonlist )
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
			      "for the camera. When clicked on\n"
			      "(and in viewer mode) the cursor changes\n"
			      "to a crosshair. The next left mouse\n"
			      "buttonpress causes whatever is underneath\n"
			      "the cursor to be selected as the new center\n"
			      "of rotation. Once the button is released,\n"
			      "the camera either jumps or animates to its\n"
			      "new position depending on the current\n"
			      "setting of the seek time in the preferences\n"
			      "dialog box.";
    
    btnInfoList.append( &seekBtn );


    int nrbuttons = buttonlist->getLength();
    
    if( nrbuttons == 0 ) return;
	
    for( int i = 0; i < nrbuttons; i++ ) 
    {	
	ASSERT( btnInfoList.at(i));
	QToolTip::add(( QWidget* )(( *buttonlist )[i]), btnInfoList.at(i)->btnToolTipInfo );   
	QWhatsThis::add(( QWidget* )(( *buttonlist )[i]), tr( btnInfoList.at(i)->btnWhatThisInfo ));
    }
}

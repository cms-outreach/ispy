#ifndef IG_QT_BROWSER_H
#define IG_QT_BROWSER_H

#ifndef QT_CLEAN_NAMESPACE
#define QT_CLEAN_NAMESPACE
#endif //QT_CLEAN_NAMESPACE

#include "Ig_Modules/IgVis/interface/config.h"
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <qobject.h>
#include <qlist.h>

// -----------------------------------------------------------------
//  Create a test viewer
// -----------------------------------------------------------------

/*!
 *  \class IgQtBrowser IgQtBrowser.h Ig_Modules/IgVis/interface/IgQtBrowser.h
 *  \brief Create a somewhat modified version of SoQtExaminer viewer
 *
 *  \author G. Alverson
 *  \date 8-Sep-2000
 *
 *  Mods:
 *
 *  16 Oct 2001, I. Osborne
 *
 *  - Added What's This info to the buttons on the left;
 *  - Added Tool tips and What's This info to the buttons
 *    on the right;
 *  - Default focus on the X button has been disabled;
 *  - The buttons are resized to be the same size;
 *  - Cleaned up some warnings.
 *
 */

class IgNetscapeRunner;

class IG_VIS_API IgQtBrowser : public QObject, public SoQtExaminerViewer 
{
	typedef SoQtExaminerViewer inherited;
	Q_OBJECT


public:

    //! c-tor is same as SoQtExaminerViewer
    IgQtBrowser(
	QWidget *parent=NULL,
	const char *name=NULL, 
	SbBool buildInsideParent=TRUE, 
	SoQtFullViewer::BuildFlag flag=SoQtFullViewer::BUILD_ALL,
	SoQtViewer::Type type=BROWSER );
    //! f'n to turn info overlay on or off
    void infoOverlay( bool enable );
    virtual void toggleCameraType();   

public slots:

    //! slot to bring up a print dialog
    void printSlot();
    //! slot for default print (index labels axis)
    void printDefSlot( int i );
	//! snap to x-axis
    void xClicked( void );
	//! snap to y-axis
    void yClicked( void );
	//! snap to z-axis
    void zClicked( void );
	//! invert view relative to origin
    void invClicked( void );


protected:
	//! c-tor with build flag: currently unimplemented
    IgQtBrowser(
	QWidget *parent,
	const char *name, 
	SbBool buildInsideParent, 
	SoQtFullViewer::BuildFlag flag,
	SoQtViewer::Type type, SbBool buildflag );
    //! d-tor
    ~IgQtBrowser();

    //! add additional items (axis controls) on left
    QWidget* buildLeftTrim( QWidget* parent );
    //! use bottom thumbwheel to rotate about z
    virtual void bottomWheelMotion( float val );
    //! re-orient camera (about specified axis)
    virtual void reorientCamera( const SbRotation & rot );
    //! Overloaded from parent class to add the tool tips
    virtual void createViewerButtons( QWidget* parent, SbPList* buttonlist );
    virtual void openViewerHelpCard( void );
    //! internal print routine
    virtual void print( const QString filename, const QString format,
	        const float printDPI, const float quality );

private slots:

private:

    struct BtnInfo;
    
    QList<BtnInfo> btnInfoList;

    struct BtnInfo {
        QString btnToolTipInfo;
        QString btnWhatThisInfo;
    };

    //! keep a printer per window
    QPrinter* thePrinter;
    //! if we don't have overlays, may need a slave window
    SoQtRenderArea* slave;
    //! help web pages shown with netscape
    IgNetscapeRunner* helpRunner;
    void viewPlaneX( void ) const;
    void viewPlaneY( void ) const;
    void viewPlaneZ( void ) const;
    void viewInvPlane( void ) const;
};
#endif //IG_CMSCAN_IG_QT_BROWSER_H











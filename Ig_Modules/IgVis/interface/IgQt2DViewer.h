#if !defined(IGQT2DVIS_INCLUDE)
#define IGQT2DVIS_INCLUDE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#include <qobject.h>
#include <qlist.h>

/*! \class IgQt2DViewer
*
*	\brief A class to generate a modified standard 2d plane viewer
*  (in contrast with SoQtPlaneViewer)
*
*
*/

class IG_VIS_API IgQt2DViewer: public QObject, public SoQtFullViewer 
{
	typedef SoQtFullViewer inherited;
	Q_OBJECT
public:
	explicit IgQt2DViewer(QWidget *parent  = NULL, 
		QString title = NULL, 
		SbBool embed = true, 
		SoQtFullViewer::BuildFlag flag = BUILD_ALL,
		SoQtViewer::Type type = BROWSER);
	~IgQt2DViewer(void);
	virtual void bottomWheelMotion(float value);
	virtual void leftWheelMotion(float value);
	virtual void rightWheelMotion(float value);
	virtual void zoom(float difference);
public slots:
	virtual void printSlot();
protected:	
	IgQt2DViewer(QWidget *parent, 
		const QString title, 
		SbBool embed, 
		SoQtFullViewer::BuildFlag flag,
		SoQtViewer::Type,
		bool buildnow);
	virtual void createViewerButtons( QWidget * parent, SbPList * buttons );
	virtual void print( const QString filename, const QString format= QString("eps"),
			 const float printDPI = 0, const float quality=0 );

private:
	void constructor(bool build);
private:

    struct BtnInfo;
    
    QList<BtnInfo> btnInfoList;

    struct BtnInfo {
        QString btnToolTipInfo;
        QString btnWhatThisInfo;
    };

 
};
#endif //IGQT2DVIS_INCLUDE

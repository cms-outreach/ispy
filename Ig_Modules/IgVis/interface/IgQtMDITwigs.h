#ifndef IG_CMSCAN_IG_QT_MDITWIGS_H
#define IG_CMSCAN_IG_QT_MDITWIGS_H

#include "Ig_Modules/IgVis/interface/config.h"
#include "qhbox.h"
#include "qstring.h"

/*!
 * \class IgQtMDITwigs IgQtMDITwigs.h Ig_Modules/IgVis/interface/IgQtMDITwigs.h
 *
 *  Generate a Qt-type Multiple Document Interface wrapper for
 *  the twig display of an IgQtBrowser
 *
 */

class SoGroup;

class IG_VIS_API IgQtMDITwigs : public QHBox {
	Q_OBJECT
public:
	//! c-tor
	explicit IgQtMDITwigs(QWidget *parent, QString title=NULL);
	//! d-tor
	virtual ~IgQtMDITwigs();
public slots:
private:
	//! class name for unique widget name
	static const QString mdiTwigsName;
	//! serial number
	static int serialNo;
};
#endif // IG_CMSCAN_IG_QT_MDITWIGS_H

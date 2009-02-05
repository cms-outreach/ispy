// IgQtResourceMgr.h: interface for the IgQtResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTRESOURCEMGR_H__3012EB2E_64FF_4D89_A0D7_D993C7501820__INCLUDED_)
#define AFX_IGQTRESOURCEMGR_H__3012EB2E_64FF_4D89_A0D7_D993C7501820__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!   \class IgQtResourceMgr Ig_Modules/IgVis/interface/IgQtResourceMgr.h
*    \brief A singleton class to handle saving and restoring various
*	  adjustable parameters.
*
*    Each managed resource must have a matching pair of toXML/xmlTo methods.
*    This class provides them for basic elements such as widgets and fonts.
*
*    Much of this could be regarded as being superceded by the <<(), ()>> methods supplied
*    by Qt along with the QSettings class, but this class handles non-Qt parameters as well.
*
*    \date 20 Jun 2001
*
*/
#include "Ig_Modules/IgVis/interface/config.h"
#include "qdom.h"
#include "Inventor/SbLinear.h"

class IgQtTwig;
class QWidget;
class IgQtMDIBaseShell;
class IgQtMDI;
class QFont;

class IG_VIS_API IgQtResourceMgr  
{
public:
    //! Get a reference to the one instance of IgQtResourceMgr
    static IgQtResourceMgr& getInstance();
    //! Use the current session to over-write the stored values
    void save();
    //! format a widget into XML
    QDomElement widgetToXML(const QWidget *w);
    //! XML back to widget
    bool xmlToWidget(QWidget *w, const QDomElement& nq) const;
    //! format a font into XML
    QDomElement fontToXML( const QFont &f );
    //! XML back to font
    QFont xmlToFont( const QDomElement& de ) const;
    //! SbVec3f to XML: standalone utility routine
    static bool SbVec3fToXML( const SbVec3f& v, QDomElement& de );
    //! XML back to SbVec3f: standalone utility routine
    static SbVec3f xmlToSbVec3f( const QDomNode& nd );
    //! Supply a hook onto the resource storage area for classes which format their own resource storage
    //! newOld = true implies return a document to save into; false implies restore from
    QDomDocument& getDoc(bool newOld);
    bool isValid( void ) const { return valid; }
protected:
    //! internal c-tor
    IgQtResourceMgr();
    //! internal d-tor
    virtual ~IgQtResourceMgr();
    //! pointer to the one instance
    static IgQtResourceMgr *theResources;
    //! Use the saved resources from the previous session to load our database. true indicates success.
    bool reload();
private:
    //! resources to be restored
    QDomDocument oldDoc;
    //! resources for this session
    QDomDocument newDoc;
    //! indicates old doc was found and is ready to go
    bool valid;
};

#endif // !defined(AFX_IGQTRESOURCEMGR_H__3012EB2E_64FF_4D89_A0D7_D993C7501820__INCLUDED_)

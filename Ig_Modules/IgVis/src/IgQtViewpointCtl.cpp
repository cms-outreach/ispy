// IgQtViewpointCtl.cpp: implementation of the IgQtViewpointCtl class.
//
//////////////////////////////////////////////////////////////////////
#include "Ig_Modules/IgVis/interface/IgQtViewpointCtl.h"
#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"

#include <qinputdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qkeycode.h>
#include <qdom.h>

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/VRMLnodes/SoVRMLViewpoint.h>
#include <Inventor/Qt/viewers/SoQtViewer.h>

#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtViewpointCtl::IgQtViewpointCtl( QWidget* parent, SoQtViewer* viewer ) :
QPopupMenu( parent ), m_viewer( viewer ), idDelObj( 0 )
{

    insertTearOffHandle();
    QString myType( "Viewpoints" );
    setCaption( myType );

    idNewObj = insertItem( "New", this, SLOT( newIgCtlItem()));

    insertSeparator();
}

IgQtViewpointCtl::~IgQtViewpointCtl()
{
}
// we use this private class so we can use for_each. In hindsight,
// it would have been better to use a simple loop and break, especially if 
// we have a large number of entries, although that would not detect duplicate entries
class testNames 
{
private:
    bool status;
    const QString testName;
public:
    bool operator () (std::pair<const int,IgQtViewpoint> obj) 
    {
	status |= ((obj.second).name == testName);
	return status;
    }
    testNames ( const QString name ) : status(false), 
	testName (name)
    {}
    operator bool ()
    {
	return status;
    }
};

bool IgQtViewpointCtl::addIvItem( CtlObj & theItem, const QString & name )
{
    //	 skip re-inserting
    if( std::for_each<itemListIv::iterator,testNames>(ourMap.begin(),ourMap.end(),testNames( name ))) return false;
    int item = insertItem( name );
    ourMap.insert( itemListIv::value_type( item, theItem ));
    connectItem( item, this, SLOT( statusChange( int )));
    setItemEnabled( item, true );

    if( idDelObj == 0 )
    {
	objDelMenu = new QPopupMenu( this, "Delete" );
	idDelObj = insertItem( "Delete", objDelMenu, -1, indexOf( idNewObj ));
    }
    int objDelItem = objDelMenu->insertItem( name, this, SLOT( delIgCtlItem( int ))); // use same id as in main menu
    xrefMap.insert( xRefMap::value_type( objDelItem, item ));
    return true;
}
bool IgQtViewpointCtl::replaceIvItem( CtlObj & theItem, const QString & name )
{
    // assumes only one entry with a given name
    itemListIv::iterator theIter = ourMap.begin();
    for( ; theIter != ourMap.end(); theIter++)
    {
	if (name == (*theIter).second.name) break;
    }
    if (theIter != ourMap.end())
    {
	// now have to find delete item corresponding to this index
	for ( xRefMap::iterator xIter = xrefMap.begin(); xIter != xrefMap.end(); xIter++)
	{
	    if ((*xIter).second == (*theIter).first)
	    {
		delIgCtlItem((*xIter).first);
		break;
	    }
	}
    }
    return addIvItem( theItem, name );
}
void IgQtViewpointCtl::statusChange( int i )
{
    itemListIv::iterator theIter = ourMap.find( i );
    ( *theIter ).second.setViewpoint( m_viewer );
}
void IgQtViewpointCtl::newIgCtlItem(const QString & newName, bool replace)
{
    QString myType( "Viewpoint" );
    QString serialNo( "%1" );
    QString label = myType + QString( "_" ) + serialNo.arg( static_cast<int>(ourMap.size()) + 1 );
    if (newName != QString::null)
    {
	CtlObj newView( m_viewer, newName );
	newView.label = label;
	if (replace) {
	    replaceIvItem( newView, newName );
	}
	else {
	    addIvItem( newView, newName );
	}
    }
}
void IgQtViewpointCtl::newIgCtlItem()
{
    bool ok = false;
    QString myType( "Viewpoint" );
    QString serialNo( "%1" );
    QString label = myType + QString( "_" ) + serialNo.arg( static_cast<int>(ourMap.size()) + 1 );
    QString newName = QInputDialog::getText( tr( "Name" ), QString::null, QLineEdit::Normal, label, &ok, this );
    if( ok && !newName.isEmpty())
    {
	newIgCtlItem(newName, false);
    }
}

void IgQtViewpointCtl::delIgCtlItem( int i )
{
    xRefMap::iterator xIter = xrefMap.find( i );
    if (xIter != xrefMap.end())
    {
	itemListIv::iterator theIter = ourMap.find(( *xIter ).second );
	if (theIter != ourMap.end()) 
	{
	    ourMap.erase( theIter );
	    objDelMenu->removeItem( i );
	    removeItem(( *xIter ).second );
	}
    }
    // if removed last viewpoint, remove "Remove" menu as well
    if( ourMap.empty())
    {
	removeItem( idDelObj );
	idDelObj = 0;
    }
}

void IgQtViewpointCtl::addAllIv( SoNode* topNode )
{
    // Search for ctl's already present. This code from SoQtViewer...
    SoSearchAction search;

#if TGS_VERSION >= 2500  // protect against versions (and COIN) that don't know about SoVRMLViewpoint
    search.setType(SoVRMLViewpoint::getClassTypeId());
    search.setInterest(SoSearchAction::ALL);
    search.apply(topNode);
#endif

    SoGroup* root = NULL;
    if( topNode->getTypeId().isDerivedFrom( SoGroup::getClassTypeId())) 
    {
	root = static_cast< SoGroup* >( topNode );
    }
    SoPathList paths = search.getPaths();
    QString name;
    for( int i = 0; i < paths.getLength(); i++ )
    {
	SoVRMLViewpoint *obj = static_cast< SoVRMLViewpoint* >( paths[ i ]->getTail());
	if( !obj->getName())
	{
	    name = obj->description.getValue().getString();
	}
	else
	{
	    name = obj->getName().getString();
	}
	IgQtViewpoint viewp( obj );
	viewp.label = "Pre-existing";
	addIvItem( viewp, name );
    }
}
QDomElement IgQtViewpointCtl::viewpointToXML( const CtlObj &co ) const
{
    IgQtResourceMgr &theMgr = IgQtResourceMgr::getInstance();
    QDomDocument &theDoc = theMgr.getDoc(true);
    QDomElement v = theDoc.createElement("viewpoint");
    // name
    QDomElement nameDom = theDoc.createElement("name");
    QDomText nameText = theDoc.createTextNode(co.name);
    nameDom.appendChild(nameText);
    v.appendChild(nameDom);
    // label
    QDomElement labelDom = theDoc.createElement("label");
    QDomText labelText = theDoc.createTextNode(co.label);
    labelDom.appendChild(labelText);
    v.appendChild(labelDom);
    // type
    QDomElement typeDom = theDoc.createElement("type");
    QDomText typeText = theDoc.createTextNode(co.type.getName().getString());
    typeDom.appendChild(typeText);
    v.appendChild(typeDom);

    QString sbuf;
    // position
    SbVec3f p = co.position;
    QDomElement positionDom = theDoc.createElement("position");
    QDomElement xDom = theDoc.createElement("x");
    QDomText xText = theDoc.createTextNode(sbuf.setNum(p[0]));
    xDom.appendChild(xText);
    positionDom.appendChild(xDom);
    QDomElement yDom = theDoc.createElement("y");
    QDomText yText = theDoc.createTextNode(sbuf.setNum(p[1]));
    yDom.appendChild(yText);
    positionDom.appendChild(yDom);
    QDomElement zDom = theDoc.createElement("z");
    QDomText zText = theDoc.createTextNode(sbuf.setNum(p[2]));
    zDom.appendChild(zText);
    positionDom.appendChild(zDom);
    v.appendChild(positionDom);

    // orientation
    float q0, q1, q2, q3;
    SbRotation rot = co.orientation;
    rot.getValue(q0,q1,q2,q3);
    QDomElement orientationDom = theDoc.createElement("orientation");
    QDomElement q0Dom = theDoc.createElement("q0");
    QDomText q0Text = theDoc.createTextNode(sbuf.setNum(q0));
    q0Dom.appendChild(q0Text);
    orientationDom.appendChild(q0Dom);
    QDomElement q1Dom = theDoc.createElement("q1");
    QDomText q1Text = theDoc.createTextNode(sbuf.setNum(q1));
    q1Dom.appendChild(q1Text);
    orientationDom.appendChild(q1Dom);
    QDomElement q2Dom = theDoc.createElement("q2");
    QDomText q2Text = theDoc.createTextNode(sbuf.setNum(q2));
    q2Dom.appendChild(q2Text);
    orientationDom.appendChild(q2Dom);
    QDomElement q3Dom = theDoc.createElement("q3");
    QDomText q3Text = theDoc.createTextNode(sbuf.setNum(q3));
    q3Dom.appendChild(q3Text);
    orientationDom.appendChild(q3Dom);
    v.appendChild(orientationDom);

    // heightAngle
    QDomElement ha = theDoc.createElement("heightAngle");
    QDomText haText = theDoc.createTextNode(sbuf.setNum(co.heightAngle));
    ha.appendChild(haText);
    v.appendChild(ha);
    // focalDistance
    QDomElement fd = theDoc.createElement("focalDistance");
    QDomText fdText = theDoc.createTextNode(sbuf.setNum(co.focalDistance));
    fd.appendChild(fdText);
    v.appendChild(fd);
    // aspectRatio
    QDomElement ar = theDoc.createElement("aspectRatio");
    QDomText arText = theDoc.createTextNode(sbuf.setNum(co.aspectRatio));
    ar.appendChild(arText);
    v.appendChild(ar);
    // nearDistance
    QDomElement nd = theDoc.createElement("nearDistance");
    QDomText ndText = theDoc.createTextNode(sbuf.setNum(co.nearDistance));
    nd.appendChild(ndText);
    v.appendChild(nd);
    // farDistance
    QDomElement fard = theDoc.createElement("farDistance");
    QDomText fardText = theDoc.createTextNode(sbuf.setNum(co.farDistance));
    fard.appendChild(fardText);
    v.appendChild(fard);
    return v;
}
IgQtViewpoint IgQtViewpointCtl::xmlToViewpoint( const QDomElement &de )
{
    IgQtViewpoint obj(m_viewer,QString(""));
    if ( de.tagName() != "viewpoint" ) return obj;

    QDomNode nq;
    //name 
    nq = de.namedItem("name");
    if (nq != QDomNode()) 
    {
	obj.name = nq.firstChild().nodeValue();
    }
    // type
    nq = de.namedItem("type");
    if (nq != QDomNode())
    {
	obj.type = SoType::fromName(SbName(nq.firstChild().nodeValue()));
    }
    // position
    nq = de.namedItem("position");
    if (nq != QDomNode())
    {
	QDomNode nx = nq.namedItem("x");
	QDomNode ny = nq.namedItem("y");
	QDomNode nz = nq.namedItem("z");
	obj.position = SbVec3f(nx.firstChild().nodeValue().toFloat(),
	    ny.firstChild().nodeValue().toFloat(),
	    nz.firstChild().nodeValue().toFloat());
    }
    // orientation
    nq = de.namedItem("orientation");
    if (nq != QDomNode())
    {
	QDomNode nq0 = nq.namedItem("q0");
	QDomNode nq1 = nq.namedItem("q1");
	QDomNode nq2 = nq.namedItem("q2");
	QDomNode nq3 = nq.namedItem("q3");
	obj.orientation = SbRotation(nq0.firstChild().nodeValue().toFloat(),
	    nq1.firstChild().nodeValue().toFloat(),
	    nq2.firstChild().nodeValue().toFloat(),
	    nq3.firstChild().nodeValue().toFloat());
    }
    // height angle
    nq = de.namedItem("heightAngle");
    if (nq != QDomNode())
    {
	obj.heightAngle = nq.firstChild().nodeValue().toFloat();
    }
    // focal distance
    nq = de.namedItem("focalDistance");
    if (nq != QDomNode())
    {
	obj.focalDistance = nq.firstChild().nodeValue().toFloat();
    }
    // aspect ratio
    nq = de.namedItem("aspectRatio");
    if (nq != QDomNode())
    {
	obj.aspectRatio = nq.firstChild().nodeValue().toFloat();
    }
    // near distance
    nq = de.namedItem("nearDistance");
    if (!nq.isNull())
    {
	obj.nearDistance = nq.firstChild().nodeValue().toFloat();
    }
    // far distance
    nq = de.namedItem("farDistance");
    if (!nq.isNull())
    {
	obj.farDistance = nq.firstChild().nodeValue().toFloat();
    }
    return obj;
}

bool IgQtViewpointCtl::saveResources(const QString &unique, QDomElement &de) const
{
    IgQtResourceMgr &theMgr = IgQtResourceMgr::getInstance();
    QDomDocument &theDoc = theMgr.getDoc(true);
    // this set of viewpoints
    QDomElement vDom = theDoc.createElement("viewpoints");
    vDom.setAttribute("resourceID",unique);
    de.appendChild(vDom);
    // iterate over objects we control
    for (itemListIv::const_iterator it = ourMap.begin(); it != ourMap.end(); it++)
    {
	vDom.appendChild( viewpointToXML( it->second ) );
    }
    return true;
}
void IgQtViewpointCtl::restoreResources( const QString& /* unique */, const QDomElement &de)
{
    if ( de.tagName() != "viewpoints" ) return;

    QDomNodeList nl = de.elementsByTagName( "viewpoint" );
    for (unsigned int i = 0; i < nl.length(); i++)
    {
	QDomElement qe = nl.item( i ).toElement();
	IgQtViewpoint vp = xmlToViewpoint( qe );
	if ( vp.name != "Last Viewpoint" )
	{
	    addIvItem( vp, vp.name );
	}
	else
	{
	    vp.setViewpoint( m_viewer );
	}
    }    
}

#include "Ig_Modules/IgVis/interface/IgQtVisCtl.h"

#include <Inventor/SbLinear.h>
#include <Inventor/manips/SoJackManip.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/manips/SoDirectionalLightManip.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/manips/SoSpotLightManip.h>

#include <cassert>
#include <algorithm>
#include <functional>

#include <qinputdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qdom.h>

#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"

//DONE, but ugly: This code was originally intended to be a copy of IgQtClipCtl
//      but the model was broken for light since there are a number of different 
//      types of light. The alternatives to the current hack job are to either
//      1) supply an option to create any of the types and test against the list, or
//      2) create a different control for each type. The second option would be unmaintainable.

const QString IgQtVisCtl::showIcons = QString( "Show Icons" );
const QString IgQtVisCtl::hideIcons = QString( "Hide Icons" );


IgQtVisCtl::IgQtVisCtl( QWidget* parent, SoGroup* topNode ) :
    QPopupMenu( parent ), idDelObj( 0 )
{
    insertTearOffHandle();
    QString myType( CtlObj::getClassTypeId().getName().getString());
    setCaption( myType );

    idIconItem = insertItem( "Show Icons", this, SLOT( iconChange()));
    QPopupMenu* newMenu = new QPopupMenu( this, "New Light" );
    idNewObj = insertItem( "New", newMenu );
    idDir = newMenu->insertItem( "Directional Light", this, SLOT( newIgCtlItem( int )));
    idPoint = newMenu->insertItem( "Point Light", this, SLOT( newIgCtlItem( int )));
    idSpot = newMenu->insertItem( "Spot Light", this, SLOT( newIgCtlItem( int )));
  
    insertSeparator();
    theIvGroup = new SoGroup;
    theIvGroup->ref();
    if( topNode != NULL ) {
	makeDefaults();
	topNode->addChild( theIvGroup );
    }
}

IgQtVisCtl::~IgQtVisCtl()
{
    theIvGroup->unref();
}

bool IgQtVisCtl::addIvItem( CtlObj* theItem, const QString & name )
{

    //	 skip re-inserting
    bool found = false;
    for ( itemListIv::iterator it = ourMap.begin(); it != ourMap.end() && !found; it++)
    {
	if ( found = (name == text( it->first ) ) )
	{
	    theIvGroup->replaceChild( it->second, theItem );
	    it->second = theItem;
	    setItemChecked( it->first, theItem->on.getValue() );
	    return true;
	}
    }


    int item = insertItem( name );
    ourMap.insert( itemListIv::value_type( item, theItem ));
    connectItem( item, this, SLOT( statusChange( int )));
    setItemEnabled( item, true );
    setItemChecked( item, theItem->on.getValue());

    if( idDelObj == 0 )
    {
	objDelMenu = new QPopupMenu( this, "Delete" );
	idDelObj = insertItem( "Delete", objDelMenu, -1, indexOf( idNewObj ));
    }
    int objDelItem = objDelMenu->insertItem( name, this, SLOT( delIgCtlItem( int ))); // use same id as in main menu
    xrefMap.insert( xRefMap::value_type( objDelItem, item ));

    return true;
}

void IgQtVisCtl::statusChange( int i )
{
    itemListIv::iterator theIter = ourMap.find( i );
    if( theIter != ourMap.end())
    {
	CtlObj* node = ( *theIter ).second;
	bool itemOn = isItemChecked( i );
	node->on = !itemOn;
	setItemChecked( i, !itemOn );
    }
}

void IgQtVisCtl::iconChange()
{
    SoSearchAction findIt;
    if( text( idIconItem ) == hideIcons )
    {
	changeItem( idIconItem, showIcons );
	for( itemListIv::iterator iter = ourMap.begin(); iter != ourMap.end(); iter++ )
	{
	    CtlObjManip* node = static_cast< CtlObjManip* >(( *iter ).second );
	    if( node != NULL ) // this should be guaranteed to work
	    {
		findIt.setNode( node );
		findIt.apply( theIvGroup );
		SoPath* aPath = findIt.getPath();
		if( aPath != NULL )
		{ // it'll die if the path isn't found
		    if( node->isOfType( SoDirectionalLight::getClassTypeId()))
		    {
			SoDirectionalLight* newObj = new SoDirectionalLight;
			iter->second = newObj;
			SoDirectionalLightManip* nn = static_cast< SoDirectionalLightManip* >( node );
			nn->replaceManip( aPath, newObj );
		    }
		    else if( node->isOfType( SoPointLight::getClassTypeId()))
		    {
			SoPointLight* newObj = new SoPointLight;
			iter->second = newObj;
			SoPointLightManip* nn = static_cast< SoPointLightManip* >( node );
			nn->replaceManip( aPath, newObj );
		    }
		    else if( node->isOfType( SoSpotLight::getClassTypeId()))
		    {
			SoSpotLight* newObj = new SoSpotLight;
			iter->second = newObj;
			SoSpotLightManip* nn = static_cast< SoSpotLightManip* >( node );
			nn->replaceManip( aPath, newObj );
		    }
		}
	    }
	}
    }
    else
    {
	changeItem( idIconItem, hideIcons );
	for( itemListIv::iterator iter = ourMap.begin(); iter != ourMap.end(); iter++ )
	{
	    CtlObj* node = ( *iter ).second;
	    findIt.setNode( node );
	    findIt.apply( theIvGroup );
	    SoPath* aPath = findIt.getPath();
	    if( aPath != NULL )
	    { // it'll die if the path isn't found
		if( node->isOfType( SoDirectionalLight::getClassTypeId()))
		{
		    SoDirectionalLightManip* newObj = new SoDirectionalLightManip;
		    newObj->replaceNode( aPath );
		    iter->second = newObj;
		}
		else if( node->isOfType( SoPointLight::getClassTypeId()))
		{
		    SoPointLightManip* newObj = new SoPointLightManip;
		    newObj->replaceNode( aPath );
		    iter->second = newObj;
		}
		else if( node->isOfType( SoSpotLight::getClassTypeId()))
		{
		    SoSpotLightManip* newObj = new SoSpotLightManip;
		    newObj->replaceNode( aPath );
		    iter->second = newObj;
		}
	    }	
	}
    }
}

void IgQtVisCtl::newIgCtlItem( int i )
{
    bool ok = false;
    QString myType( CtlObj::getClassTypeId().getName().getString());
    QString serialNo( "%1" );
    QString label = myType + QString( "_" ) + serialNo.arg( static_cast<int>(ourMap.size()) + 1 );
    QString newName = QInputDialog::getText( tr( "Name" ), QString::null, QLineEdit::Normal, label, &ok, this );
    if( ok && !newName.isEmpty())
    {
	CtlObjManip* newClip = NULL; 
	if( text( idIconItem ) == hideIcons ) // check if we want manipulator version or plain
	{
	    if( i == idDir ) newClip = new SoDirectionalLightManip;
	    else if( i == idPoint ) newClip = new SoPointLightManip;
	    else if( i == idSpot ) newClip = new SoSpotLightManip;
	}
	else
	{
	    if( i == idDir ) newClip = new SoDirectionalLight;
	    else if( i == idPoint ) newClip = new SoPointLight;
	    else if( i == idSpot ) newClip = new SoSpotLight;
	}
	theIvGroup->addChild( newClip );
	addIvItem( newClip, newName );
    }
}

void IgQtVisCtl::delIgCtlItem( int i )
{
    xRefMap::iterator xIter = xrefMap.find( i );
    itemListIv::iterator theIter = ourMap.find(( *xIter ).second );
    CtlObj* node = ( *theIter ).second;
    theIvGroup->removeChild( node );
    ourMap.erase( theIter );
    objDelMenu->removeItem( i );
    removeItem(( *xIter ).second );

    if( ourMap.empty())
    {
	removeItem( idDelObj );
	idDelObj = 0;
    }
}

SoGroup* IgQtVisCtl::makeDefaults()
{
    //  create default lighting group

    theIvGroup->setName( "Default_Light_Group" );

    // Bright white lights front and top,
    // Slightly dimmer white lights at sides
    // Dimmer blue at bottom/back to enhance depth
    
    SoDirectionalLight* frontFlood = new SoDirectionalLight();
    frontFlood->direction.setValue( 1, 0, 0 );
    frontFlood->intensity.setValue( 1.0f );
    frontFlood->color.setValue( 1.0f, 1.0f, 1.0f );
    theIvGroup->addChild( frontFlood );

    SoDirectionalLight* backFlood = new SoDirectionalLight();
    backFlood->direction.setValue( -1, 0, 0 );
    backFlood->intensity.setValue( 0.5f );
    backFlood->color.setValue( 0.5f, 0.5f, 1.0f );
    theIvGroup->addChild( backFlood );
  
    SoDirectionalLight* topFlood = new SoDirectionalLight;
    topFlood->direction.setValue( 0, -1, 0 );
    topFlood->intensity.setValue( 1.0f );
    topFlood->color.setValue( 1.f, 1.f, 1.f );
    theIvGroup->addChild( topFlood );
  
    SoDirectionalLight* bottomFlood = new SoDirectionalLight();
    bottomFlood->direction.setValue( 0, 1, 0 );
    bottomFlood->intensity.setValue( 0.5f );
    bottomFlood->color.setValue( 0.5f, 0.5f, 1.0f );
    theIvGroup->addChild( bottomFlood );
  
    SoDirectionalLight* leftFlood = new SoDirectionalLight;
    leftFlood->direction.setValue( 1, 0, 0 );
    leftFlood->intensity.setValue( 0.7f );
    leftFlood->color.setValue( 1.f, 1.f, 1.f );
    theIvGroup->addChild( leftFlood );
  
    SoDirectionalLight* rightFlood = new SoDirectionalLight();
    rightFlood->direction.setValue( -1, 0, 0 );
    rightFlood->intensity.setValue( 0.7f );
    rightFlood->color.setValue( 1.0f, 1.0f, 1.0f );
    theIvGroup->addChild( rightFlood );


    SoPointLight* pointLight = new SoPointLight();
    pointLight->location.setValue( SbVec3f( 0.0f, 0.0f, 0.0f ));
    pointLight->intensity.setValue( 0.8f );
    pointLight->color.setValue( SbColor( 1.0f, 0.7f, 0.7f ));
    theIvGroup->addChild( pointLight );

    addIvItem( frontFlood,  QString ("Front flood"));
    addIvItem( backFlood,   QString ("Back flood"));
    addIvItem( topFlood,    QString ("Top flood"));
    addIvItem( bottomFlood, QString ("Bottom flood"));
    addIvItem( leftFlood,   QString ("Left flood"));
    addIvItem( rightFlood,  QString ("Right flood"));

    addIvItem( pointLight, QString ("Centre"));

    return theIvGroup;
}
  
void IgQtVisCtl::addAllIv( SoNode* root )
{
    // Search for ctl's already present. This code from SoQtViewer...
    SoSearchAction search;
    search.setType( CtlObj::getClassTypeId());
    search.setInterest( SoSearchAction::ALL );
    search.apply( root );

    SoPathList paths = search.getPaths();
    QString name;
    for( int i = 0; i < paths.getLength(); i++ )
    {
	CtlObj* obj = static_cast< CtlObj* >( paths[ i ]->getTail());
	if( !obj->getName()){
	    name = "Light";
	}
	else
	{
	    name = obj->getName().getString();
	}

	addIvItem( obj, name );
    }
}
void IgQtVisCtl::allOff(void)
{
    for ( itemListIv::iterator theIter = ourMap.begin(); theIter != ourMap.end(); theIter++)
    {
	CtlObj* node = ( *theIter ).second;
	int i = ( *theIter ).first;
	if (isItemChecked( i ))
	{
	    node->on = false;
	    setItemChecked( i, false );
	}
    }

}
QDomElement IgQtVisCtl::ctlPairToXML( const CtlPair &p ) const
{
  IgQtResourceMgr &theMgr = IgQtResourceMgr::getInstance();
  if ( theMgr.isValid() )
  {
    const CtlObj &co = *p.second;
    QDomDocument &theDoc = theMgr.getDoc(true);
    QDomElement v = theDoc.createElement("light");
    if ( co.on.getValue() ) v.setAttribute( "on", QString::null );
    // label
    QDomElement labelDom = theDoc.createElement("label");
    QDomText labelText = theDoc.createTextNode(p.first);
    labelDom.appendChild(labelText);
    v.appendChild(labelDom);
    // type
    QDomElement typeDom = theDoc.createElement("type");
    QDomText typeText = theDoc.createTextNode(co.getTypeId().getName().getString());
    typeDom.appendChild(typeText);
    v.appendChild(typeDom);

    QString sbuf;
    // color
    SbColor c = co.color.getValue();

    QDomElement cDom = theDoc.createElement("color");
    sbuf = QString("%1").arg(c.getPackedValue(),8,16).upper();
    QDomText cText = theDoc.createTextNode(sbuf);
    cDom.appendChild(cText);
    v.appendChild(cDom);

    QDomElement de = theDoc.createElement("intensity");
    QDomText dt = theDoc.createTextNode(sbuf.setNum(co.intensity.getValue()));
    de.appendChild(dt);
    v.appendChild(de);

    if (co.getTypeId().isDerivedFrom(SoDirectionalLight::getClassTypeId()))
    {
      SoDirectionalLight *l = static_cast<SoDirectionalLight*>(p.second);
      if (l)
      {
	QDomElement dd = theDoc.createElement("direction");
	IgQtResourceMgr::SbVec3fToXML(l->direction.getValue(), dd);
	v.appendChild(dd);
      }
    }
    else if (co.getTypeId().isDerivedFrom(SoPointLight::getClassTypeId()))
    {
      SoPointLight *l = static_cast<SoPointLight*>(p.second);
      if (l)
      {
	QDomElement dd = theDoc.createElement("location");
	IgQtResourceMgr::SbVec3fToXML(l->location.getValue(), dd);
	v.appendChild(dd);
      }

    }
    else if (co.getTypeId().isDerivedFrom(SoSpotLight::getClassTypeId()))
    {
      SoSpotLight *l = static_cast<SoSpotLight*>(p.second);
      if (l)
      {
	QDomElement dd = theDoc.createElement("direction");
	IgQtResourceMgr::SbVec3fToXML(l->direction.getValue(), dd);
	v.appendChild(dd);
	QDomElement dl = theDoc.createElement("location");
	IgQtResourceMgr::SbVec3fToXML(l->location.getValue(), dl);
	v.appendChild(dl);

        QDomElement dor = theDoc.createElement("dropOffRate");
        QDomText dort = theDoc.createTextNode(sbuf.setNum(l->dropOffRate.getValue()));
	dor.appendChild(dort);
	v.appendChild(dor);

        QDomElement dc = theDoc.createElement("cutOffAngle");
        QDomText dct = theDoc.createTextNode(sbuf.setNum(l->cutOffAngle.getValue()));
	dc.appendChild(dct);
	v.appendChild(dc);

      }
    }
    return v;
  }
  else
  {
    QDomElement v;
    return v;
  }
}
IgQtVisCtl::CtlPair* IgQtVisCtl::xmlToCtlPair( const QDomElement &de )
{
    CtlPair *p = 0;
    if ( de.tagName() != "light" )
    {
	return p;
    }

    QString label;
    CtlObj *obj=0;
    QDomNode nq;
    // type
    nq = de.namedItem("type");
    if (nq != QDomNode())
    {
	SoType type = SoType::fromName(SbName(nq.firstChild().nodeValue()));
	obj = static_cast<CtlObj*>(type.createInstance());
	if (type==SoType::fromName(SbName("SoDirectionalLight")))
	{
	    SoDirectionalLight *objj = static_cast<SoDirectionalLight*>(obj);
	    // direction
	    QDomNode nd = de.namedItem("direction");
	    if ( !nd.isNull() )
	    {
	      objj->direction.setValue(IgQtResourceMgr::xmlToSbVec3f(nd));
	    }
	}
	else if(type==SoType::fromName(SbName("SoPointLight")))
	{
	    SoPointLight *objj = static_cast<SoPointLight*>(obj);
	    // location
	    QDomNode nd = de.namedItem("location");
	    if ( !nd.isNull() )
	    {
	        objj->location.setValue(IgQtResourceMgr::xmlToSbVec3f(nd));
	    }
	}
	else if (type==SoType::fromName(SbName("SoSpotLight")))
	{
	    SoSpotLight *objj = static_cast<SoSpotLight*>(obj);
	    // direction
	    QDomNode nd = de.namedItem("direction");
	    if ( !nd.isNull() )
	    {
	        objj->direction.setValue(IgQtResourceMgr::xmlToSbVec3f(nd));
	    }
	    // location
	    nd = de.namedItem("location");
	    if ( !nd.isNull() )
	    {
	        objj->location.setValue(IgQtResourceMgr::xmlToSbVec3f(nd));
	    }
            nd = de.namedItem("cutOffAngle");
	    if ( !nd.isNull() )
	    {
	      bool ok;
	      float f = nd.firstChild().nodeValue().toFloat(&ok);
	      if (ok)
	      {
		objj->cutOffAngle = f;
	      }
	    }
	    nd = de.namedItem("dropOffRate");
	    {
	      bool ok;
	      float f = nd.firstChild().nodeValue().toFloat(&ok);
	      if (ok)
	      {
		objj->cutOffAngle = f;
	      }
	    }
	}
	else
	{
	    return p;
	}

	// on/off
	obj->on = de.hasAttribute("on");

	//label 
	nq = de.namedItem("label");
	if (!nq.isNull()) 
	{
	    label =  nq.firstChild().nodeValue();
	}

	// intensity
	nq = de.namedItem("intensity");
	if (!nq.isNull())
	{
	  bool ok;
	  float intensity = nq.firstChild().nodeValue().toFloat(&ok);
	  if (ok)
	  {
	    obj->intensity = intensity;
	  }
	  else
	  {
	    obj->intensity = 1.0f;
	  }
	}

	// color
	nq = de.namedItem("color");
	if (!nq.isNull())
	{
	    bool ok;
	    float r, g, b;
	    // ignore the lower 8 bits: the "a" value in rgba packed color.
	    unsigned int packed = nq.firstChild().nodeValue().toUInt(&ok, 16);
	    if (ok)
	    {
	      r = static_cast<float>( (packed & 0xFF000000) >> 24 )/0xFF;
	      g = static_cast<float>( (packed & 0x00FF0000) >> 16 )/0xFF;
	      b = static_cast<float>( (packed & 0x0000FF00) >>  8 )/0xFF;
	    }
	    else
	    {
	      r = g = b = 1.0f;
	    }
	    obj->color = SbColor(r,g,b);

	}
    }
    p = new CtlPair(label, obj);
    return p;
}
bool IgQtVisCtl::saveResources(const QString &unique, QDomElement &de) const
{
    IgQtResourceMgr &theMgr = IgQtResourceMgr::getInstance();
    if ( theMgr.isValid() )
    {
	QDomDocument &theDoc = theMgr.getDoc(true);
	// this set of viewpoints
	QDomElement vDom = theDoc.createElement("lights");
	vDom.setAttribute("resourceID",unique);
	de.appendChild(vDom);
	// iterate over objects we control
	for (itemListIv::const_iterator it = ourMap.begin(); it != ourMap.end(); it++)
	{
	    vDom.appendChild( ctlPairToXML( CtlPair(text((it->first)),(it->second)) ) );
	}
    }
    return true;
}
void IgQtVisCtl::restoreResources( const QString& /* unique */, const QDomElement &de)
{
    if ( de.tagName() != "lights" ) return;

    QDomNodeList nl = de.elementsByTagName( "light" );
    for (unsigned int i = 0; i < nl.length(); i++)
    {
	QDomElement qe = nl.item( i ).toElement();
	CtlPair *p = xmlToCtlPair( qe );
	addIvItem( p->second, p->first );
   }    
}
  







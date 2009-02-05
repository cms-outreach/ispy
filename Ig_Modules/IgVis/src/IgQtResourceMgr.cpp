// IgQtResourceMgr.cpp: implementation of the IgQtResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"
#include "Ig_Modules/IgVis/interface/IgQtTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtTwigController.h"
#include "Ig_Modules/IgVis/interface/IgQtTwigListViewItem.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"

#include "qtextstream.h"
#include "qfile.h"
#include "qwidget.h"
#include "qapplication.h"
#include "qregexp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IgQtResourceMgr* IgQtResourceMgr::theResources = 0;
IgQtResourceMgr::IgQtResourceMgr()
{

	// get old doc
	valid = reload();

	// create new doc
	newDoc = QDomDocument("IGUANA");

}

IgQtResourceMgr::~IgQtResourceMgr()
{

}
IgQtResourceMgr& IgQtResourceMgr::getInstance()
{
	if (theResources) 
	{
		return *theResources;
	}
	else
	{
		theResources = new IgQtResourceMgr;
		return *theResources;
	}
}
bool IgQtResourceMgr::reload()
{
	bool result = false;

	QString fileName(".igsave.xml");
	QFile f(fileName);
	if (f.open(IO_ReadOnly)) {
		if (qVersion()>=QString("2.3.0")) // Qt 2.2 has problems reading in medium-complex XML
		{
			oldDoc.setContent(&f);
			f.close();
			result = true;
		}
		else
		{
			oldDoc = QDomDocument("IGUANA");
		}
	}
	else {
		oldDoc = QDomDocument("IGUANA");
	}
	return result;
}
void IgQtResourceMgr::save()
{	// save the current contents of the resource "database"
	QString fileName(".igsave.xml");
	QFile f(fileName);
	if (f.open(IO_WriteOnly)) {
		QTextStream t(&f);
		t << "<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?>" ;

		newDoc.save(t,0);
		f.close();
	}
}
QDomDocument& IgQtResourceMgr::getDoc(bool newOld)
{
	if (newOld)
		return newDoc; 
	else
		return oldDoc;
}
QDomElement IgQtResourceMgr::widgetToXML(const QWidget *w)
{
	QDomElement wDom = newDoc.createElement("widget");
	// minimized? *** NON-WORKING before Qt 3.0.4, ok thereafter
	// min/max
	if (w->isMinimized())
	{
		QDomAttr attr = newDoc.createAttribute("minimized");
		wDom.setAttributeNode(attr);
	}
	else if (w->isMaximized())
	{
		QDomAttr attr = newDoc.createAttribute("maximized");
		wDom.setAttributeNode(attr);
	}
	if (w->isHidden())
	{
		QDomAttr attr = newDoc.createAttribute("hidden");
		wDom.setAttributeNode(attr);
	}
	//   name
	QDomElement nameDom = newDoc.createElement("name");
	QDomText nameText = newDoc.createTextNode(w->name());
	nameDom.appendChild(nameText);
	wDom.appendChild(nameDom);
	//   title  
	QDomElement captionDom = newDoc.createElement("caption");
	QString caption = w->caption();
	if (w->isTopLevel())
	{
		// look for caption of form setCaption( tr("%1 - [%2]"). See qworkspace.cpp
		QRegExp re(" - \\[.*\\]");
		int pos = re.search(caption);
		if ( pos > -1 )
		{
			caption = caption.left( pos );
		}
	}
	QDomText captionText = newDoc.createTextNode(caption);
	captionDom.appendChild(captionText);
	wDom.appendChild(captionDom);

	QString sbuf;
	QWidget *pw = w->parentWidget();

	//    position
	QDomElement positionDom = newDoc.createElement("position");
	QDomElement xDom = newDoc.createElement("x");
	QDomElement yDom = newDoc.createElement("y");
	QDomText xText;
	QDomText yText;
	if (pw) 
	{
		xText = newDoc.createTextNode(sbuf.setNum(pw->pos().x()));
		yText = newDoc.createTextNode(sbuf.setNum(pw->pos().y()));
	}
	else
	{
		xText = newDoc.createTextNode(sbuf.setNum(w->pos().x()));
		yText = newDoc.createTextNode(sbuf.setNum(w->pos().y()));
	}
	xDom.appendChild(xText);
	positionDom.appendChild(xDom);
	yDom.appendChild(yText);
	positionDom.appendChild(yDom);
	wDom.appendChild(positionDom);
	//    size
	QDomElement sizeDom = newDoc.createElement("size");
	QDomElement widthDom = newDoc.createElement("width");
	QDomElement heightDom = newDoc.createElement("height");
	QDomText widthText, heightText;
	if (pw)
	{
		widthText = newDoc.createTextNode(sbuf.setNum(pw->size().width()));
		heightText = newDoc.createTextNode(sbuf.setNum(pw->size().height()));
	}
	else
	{
		widthText = newDoc.createTextNode(sbuf.setNum(w->size().width()));
		heightText = newDoc.createTextNode(sbuf.setNum(w->size().height()));
	}
	widthDom.appendChild(widthText);
	sizeDom.appendChild(widthDom);
	heightDom.appendChild(heightText);
	sizeDom.appendChild(heightDom);
	wDom.appendChild(sizeDom);

	return wDom;

}
bool IgQtResourceMgr::xmlToWidget(QWidget *w, const QDomElement &n) const
{
	if ( n.tagName() != "widget" ) return false;
	QWidget *pw = w->parentWidget();

	// restore the parameters
	bool minimized = n.hasAttribute("minimized") ; // use at end
	bool maximized = n.hasAttribute("maximized");

	//name 
	QDomNode nq = n.namedItem("name");
	if ( !nq.isNull() ) 
	{
		w->setName(nq.firstChild().nodeValue());
	}
	//  caption
	nq = n.namedItem("caption");
	if ( !nq.isNull() )
	{
		w->setCaption(nq.firstChild().nodeValue());
	}

	if (! (minimized || maximized) )
	{
		// position
		nq = n.namedItem("position");
		if ( !nq.isNull() ) 
		{
			QDomNode nx = nq.namedItem("x");
			QDomNode ny = nq.namedItem("y");
			if (pw)
			{
				pw->move(nx.firstChild().nodeValue().toInt(),ny.firstChild().nodeValue().toInt());
			}
			else
			{
				w->move(nx.firstChild().nodeValue().toInt(),ny.firstChild().nodeValue().toInt());
			}
		}
		//  size
		nq = n.namedItem("size");
		if ( !nq.isNull() )
		{
			QDomNode nx = nq.namedItem("width");
			QDomNode ny = nq.namedItem("height");
			if (pw)
			{
				pw->resize(nx.firstChild().nodeValue().toInt(), ny.firstChild().nodeValue().toInt());
			}
			else
			{
				w->resize(nx.firstChild().nodeValue().toInt(), ny.firstChild().nodeValue().toInt());
			}
		}
		w->showNormal();
	}
	else if (minimized) 
	{
		w->showMinimized();
	}
	else if (maximized)
	{
		w->showMaximized();
		w->raise(); // automatically set to top as well
	}
	return true;
}
QDomElement IgQtResourceMgr::fontToXML( const QFont &f )
{
	QDomElement de = newDoc.createElement( "font" );
	de.setAttribute( "family", f.family() );
	de.setAttribute( "pointSize", f.pointSize() );
	de.setAttribute( "weight", f.weight() );
	if ( f.italic() )
	{
		de.setAttribute( "italic", QString::null );
	}
	return de;
}
QFont IgQtResourceMgr::xmlToFont( const QDomElement& de ) const
{
	if ( de.tagName() != "font" ) return QApplication::font();

	QString fontFamily = de.attribute( "family" );
	int pointSize = de.attribute( "pointSize" ).toInt();
	int weight = de.attribute( "weight" ).toInt();

	return QFont(fontFamily, pointSize, weight, de.hasAttribute("italic") );
}
bool IgQtResourceMgr::SbVec3fToXML( const SbVec3f& v, QDomElement& de )
{
	IgQtResourceMgr &theMgr = IgQtResourceMgr::getInstance();
	if ( theMgr.isValid() )
	{
		QString sbuf;
		QDomDocument &theDoc = theMgr.getDoc(true);
		QDomElement xDom = theDoc.createElement("x");
		QDomText xText = theDoc.createTextNode(sbuf.setNum(v[0]));
		xDom.appendChild(xText);
		de.appendChild(xDom);
		QDomElement yDom = theDoc.createElement("y");
		QDomText yText = theDoc.createTextNode(sbuf.setNum(v[1]));
		yDom.appendChild(yText);
		de.appendChild(yDom);
		QDomElement zDom = theDoc.createElement("z");
		QDomText zText = theDoc.createTextNode(sbuf.setNum(v[2]));
		zDom.appendChild(zText);
		de.appendChild(zDom);
		return !( xDom.isNull() || yDom.isNull() || zDom.isNull() );
	}
	return false;
}
SbVec3f IgQtResourceMgr::xmlToSbVec3f( const QDomNode& nd )
{
	if ( !nd.isNull() )
	{
		QDomNode nx = nd.namedItem("x");
		QDomNode ny = nd.namedItem("y");
		QDomNode nz = nd.namedItem("z");

		if (!(nx.isNull() || ny.isNull() || nz.isNull()) )
		{
			return SbVec3f(nx.firstChild().nodeValue().toFloat(),
				ny.firstChild().nodeValue().toFloat(),
				nz.firstChild().nodeValue().toFloat());
		}
		else return SbVec3f(0,0,0);
	}
	return SbVec3f(0,0,0);
}


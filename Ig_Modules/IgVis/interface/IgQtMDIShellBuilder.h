// IgQtMDIShellBuilder.h: interface for the IgQtMDIShellBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDISHELLBUILDER_H__9D5118D7_EA6C_4D71_BFE1_07102D180CD1__INCLUDED_)
#define AFX_IGQTMDISHELLBUILDER_H__9D5118D7_EA6C_4D71_BFE1_07102D180CD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShellBuilder.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIShellFactory.h"
#include <string>
#include <typeinfo>

class IgQtMDIAbsShell;

/*!
 * \class IgQtMDIShellBuilder IgQtMDIShellBuilder.h Ig_Modules/IgVis/interface/IgQtMDIShellBuilder.h
 *
 * \brief Template class to build IgQt MDI browsers
 *
 *  \author G. Alverson, Northeastern University Computing Group
 *  
 */

template <class T> class  IgQtMDIShellBuilder : public IgQtMDIAbsShellBuilder  
{
public:
	IgQtMDIAbsShell *getShell(IgQtMDI *parent=0, SoQtFullViewer *aViewer=0);
	//! currently returns (under MSVC) "class IgQtMDIShellBuilder<class true_class_name>". TBF.
	virtual const QString shellName() const;
	static IgQtMDIAbsShellBuilder *getInstance();
protected:
	IgQtMDIShellBuilder();
	virtual ~IgQtMDIShellBuilder();
	static IgQtMDIAbsShellBuilder *theBuilder;
};

template <class T>
IgQtMDIAbsShellBuilder*  IgQtMDIShellBuilder<T>::theBuilder = 0;

template <class T>  IgQtMDIShellBuilder<T>::IgQtMDIShellBuilder()
{

}

template <class T>  IgQtMDIShellBuilder<T>::~IgQtMDIShellBuilder()
{

}

template <class T>  IgQtMDIAbsShell * IgQtMDIShellBuilder<T>::getShell(IgQtMDI *parent, SoQtFullViewer *aViewer)
{
	return new T(parent,aViewer);
}

template <class T> IgQtMDIAbsShellBuilder* IgQtMDIShellBuilder<T>::getInstance()
{
	if (theBuilder == NULL) {
		theBuilder = new IgQtMDIShellBuilder<T>();
		/* IgQtMDIShellFactory &theFactory = */ IgQtMDIShellFactory::getInstance(); 
	}
	return theBuilder;
}
template <class T> const QString IgQtMDIShellBuilder<T>::shellName() const
{
  return QString ( typeid(*this).name() );
}
#endif // !defined(AFX_IGQTMDISHELLBUILDER_H__9D5118D7_EA6C_4D71_BFE1_07102D180CD1__INCLUDED_)

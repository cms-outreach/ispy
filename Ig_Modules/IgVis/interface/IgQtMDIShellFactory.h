// IgQtMDIShellFactory.h: interface for the IgQtMDIShellFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTMDISHELLFACTORY_H__3D24F4CC_C178_47BD_AA06_7DB56A920776__INCLUDED_)
#define AFX_IGQTMDISHELLFACTORY_H__3D24F4CC_C178_47BD_AA06_7DB56A920776__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "Ig_Modules/IgVis/interface/config.h"
#include <map>
#include <string>
#include <utility>

#include "qstring.h"

class IgQtMDI;
class IgQtMDIAbsShell;
class IgQtMDIAbsShellBuilder;

/*!
 * \class IgQtMDIShellFactory IgQtMDIShellFactory.h Ig_Modules/IgVis/interface/IgQtMDIShellFactory.h
 *
 * \brief This singleton class maintains a registry of all the IgQt MDI browser shells which have made themselves
 *        known. It can return either a browser or a browser builder given the classname of the browser.
 *
 *  \author G. Alverson, Northeastern University Computing Group
 */

class IG_VIS_API IgQtMDIShellFactory  
{
public:
    //! map between \a name and the appropriate \a builder
    typedef std::map<QString, IgQtMDIAbsShellBuilder *> registry_map;
    //! registers an entry for browser class \a name and its builder \a builder, and if supplied, a \a displayName
    void registerBuilder(const QString &name, IgQtMDIAbsShellBuilder *builder, const QString &displayName);
    //! returns an instance of the class
    static IgQtMDIShellFactory& getInstance();
    //! returns a builder for the given browser name
    IgQtMDIAbsShellBuilder * getBuilder( const QString& shellName ) const;
    //! returns an instance of the given browser
    IgQtMDIAbsShell * getShell( const QString& shellName, IgQtMDI* w ) const;
    //! returns the \a displayName (the descriptive name of a shell) given the \a shellName; returns a null string if not found.
    const QString getDisplayName( const QString& shellName) const;
    //! returns the registry map
    registry_map & getMap( void );
protected:
    virtual ~IgQtMDIShellFactory();
    IgQtMDIShellFactory();
    IgQtMDIShellFactory(const IgQtMDIShellFactory&);
private:
    registry_map registry;
    //! map between \a displayName and name
    typedef std::map<QString, QString> displayName_map;
    displayName_map nameMap;
};

class IgQtShellRegisterEntry
{
public:
  IgQtShellRegisterEntry( const QString& shellName, IgQtMDIAbsShellBuilder *builder, const QString& displayName = QString())
  {
    if (!displayName.isNull())
    {
      IgQtMDIShellFactory::getInstance().registerBuilder( shellName, builder, displayName );
    }
    else
    {
      IgQtMDIShellFactory::getInstance().registerBuilder( shellName, builder, shellName );
    }
  }
};
#endif // !defined(AFX_IGQTMDISHELLFACTORY_H__3D24F4CC_C178_47BD_AA06_7DB56A920776__INCLUDED_)

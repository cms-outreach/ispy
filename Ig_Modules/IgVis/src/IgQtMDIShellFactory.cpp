// IgQtMDIShellFactory.cpp: implementation of the IgQtMDIShellFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtMDIShellFactory.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIShellBuilder.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IgQtMDIShellFactory::IgQtMDIShellFactory()
{

}

IgQtMDIShellFactory::~IgQtMDIShellFactory()
{

}
void IgQtMDIShellFactory::registerBuilder(const QString &name, IgQtMDIAbsShellBuilder *builder, const QString &displayName)
{	
  registry.insert( registry_map::value_type( name, builder ) );
  nameMap.insert( displayName_map::value_type( name, displayName ) );
}
IgQtMDIShellFactory& IgQtMDIShellFactory::getInstance()
{
  static IgQtMDIShellFactory theFactory;
  return theFactory;
}
IgQtMDIAbsShellBuilder * IgQtMDIShellFactory::getBuilder( const QString& shellName ) const
{
  registry_map::const_iterator it = registry.find( shellName );
  if ( it != registry.end() )
  {
    return it->second;
  }
  else
  {
    return NULL;
  }
}
IgQtMDIAbsShell * IgQtMDIShellFactory::getShell( const QString& shellName, IgQtMDI* w ) const
{
  registry_map::const_iterator it = registry.find( shellName );
  if ( it != registry.end() )
  {
    IgQtMDIAbsShellBuilder *b = it->second;
    return b->getShell( w );
  }
  else
  {
    return NULL;
  }
}
const QString IgQtMDIShellFactory::getDisplayName( const QString& sn ) const
{
  displayName_map::const_iterator it = nameMap.find( sn );
  if ( it != nameMap.end() )
  {
    return it->second;
  }
  else
  {
    return QString();
  }
}

IgQtMDIShellFactory::registry_map& IgQtMDIShellFactory::getMap( void )
{
  return registry;
}

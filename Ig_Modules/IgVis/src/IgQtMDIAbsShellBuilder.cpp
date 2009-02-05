// IgQtMDIAbsShellBuilder.cpp: implementation of the IgQtMDIAbsShellBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShellBuilder.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtMDIAbsShellBuilder::IgQtMDIAbsShellBuilder()
{

}

IgQtMDIAbsShellBuilder::~IgQtMDIAbsShellBuilder()
{

}
IgQtMDIAbsShellBuilder* IgQtMDIAbsShellBuilder::getInstance()
{
	return 0;
}
IgQtMDIAbsShell* IgQtMDIAbsShellBuilder::getShell(IgQtMDI * /*parent*/, SoQtFullViewer * /*aViewer*/)
{
	return 0;
}
const QString IgQtMDIAbsShellBuilder::shellName() const
{
  return QString( "IgQtMDIAbsShell" );
}


//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtBadSiteError.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QWidget *
IgQtSite::selfFrom (IgSite *site)
{
    if (! site)
	return 0;
    else if (IgQtSite *qsite = dynamic_cast<IgQtSite *> (site))
	return qsite->self ();

    throw new IgQtBadSiteError (site);
}

QWidget *
IgQtSite::hostFrom (IgSite *site)
{
    if (! site)
	return 0;
    else if (IgQtSite *qsite = dynamic_cast<IgQtSite *> (site))
	return qsite->host ();

    throw new IgQtBadSiteError (site);
}

void
IgQtSite::host (IgSite *site, QWidget *child)
{
    if (! site)
	return;
    else if (IgQtSite *qsite = dynamic_cast<IgQtSite *> (site))
	return qsite->host (child);

    throw new IgQtBadSiteError (site);
}

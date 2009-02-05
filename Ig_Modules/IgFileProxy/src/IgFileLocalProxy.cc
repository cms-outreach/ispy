//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgFileLocalProxy.h"
# include <classlib/iobase/Filename.h>
# include <qurl.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgFileLocalProxy::catalogLabel (void)
{ return "file"; }

std::string
IgFileLocalProxy::fetch (const std::string &file)
{
    std::string filename (QUrl (file).path ().latin1 ());
    if (lat::Filename (filename).exists ())
	return filename;
    else
      	// FIXME: throw an exception
	;
    return "";    
}

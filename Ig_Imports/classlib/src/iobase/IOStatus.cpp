//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/IOStatus.h"
#include "classlib/utils/Log.h"
#include "classlib/sysapi/IOStatus.h"

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: CLASSLIB_BASE_API?
logstream &
operator<< (logstream &log, const IOStatus &s)
{
    log << "I/O channel status information:" << indent
	<< "\ncreate time = " << s.m_ctime
	<< "\nmodify time = " << s.m_mtime
	<< "\naccess time = " << s.m_atime
	<< "\ntype        = ";
    switch (s.m_type)
    {
    case IOStatus::TypeFile:		log << "File";			break;
    case IOStatus::TypeDirectory:	log << "Directory";		break;
    case IOStatus::TypeSocket:		log << "Socket";		break;
    case IOStatus::TypeSymlink:		log << "Symlink";		break;
    case IOStatus::TypeNetworkSpecial:	log << "Network special";	break;
    case IOStatus::TypeBlockDev:	log << "Block device";		break;
    case IOStatus::TypeCharDev:		log << "Character device";	break;
    case IOStatus::TypeFIFO:		log << "FIFO";			break;
    case IOStatus::TypeVolumeId:	log << "Volume label";		break;
    default:				log << "Unknown";		break;
    }
    log << "\nacls        = " << s.m_acl.unix () << endl // FIXME: just send acl itself
	<< "\n# links     = " << s.m_links << endl
	<< "\nsize        = " << s.m_size << undent;

    return log;
}

void
IOStatus::clear (void)
{
    m_ctime = m_mtime = m_atime = 0;
    m_type = TypeUnknown;
    m_acl = 0;
    m_flags = 0;
    m_unique [0] = m_unique [1] = m_unique [2] = 0;
    m_links = 0;
    m_size = 0;
}

} // namespace lat

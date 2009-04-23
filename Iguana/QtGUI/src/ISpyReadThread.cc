//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyReadThread.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/zip/ZipMember.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace lat;

ISpyReadThread::ISpyReadThread (QObject *parent)
    : QThread (parent)
{
    m_restart = false;
    m_abort = false;
}

ISpyReadThread::~ISpyReadThread (void) 
{
    m_mutex.lock ();
    m_abort = true;
    m_condition.wakeOne ();
    m_mutex.unlock ();

    wait ();
}

void
ISpyReadThread::read (lat::ZipArchive *fromArchive, lat::ZipArchive::Iterator member, IgDataStorage *toStorage) 
{
    QMutexLocker locker (&m_mutex);
    m_archive = fromArchive;
    m_it = member;
    m_storage = toStorage;
    if (! isRunning ()) 
    {
	start (LowPriority);
    } 
    else 
    {
	m_restart = true;
	m_condition.wakeOne ();
    }
}

void
ISpyReadThread::run (void) 
{
    //forever {
	m_mutex.lock ();
	lat::ZipArchive *archive = m_archive;
	lat::ZipArchive::Iterator member = m_it;
	IgDataStorage *storage = m_storage;
	m_mutex.unlock();
	
	ASSERT (archive);
	InputStream *is = archive->input (*member);
	IOSize length = (*member)->size (ZipMember::UNCOMPRESSED);
	std::vector<char> buffer (length + 1, 0);
	is->xread (&buffer[0], length);
	is->close ();
	delete is;

	IgParser parser (storage);
	parser.parse (&buffer[0]);
	
	emit readMember (QString::fromStdString ((*member)->name ().name ()));      

	m_mutex.lock ();
	if (! m_restart)
	    m_condition.wait (&m_mutex);
	m_restart = false;
        m_mutex.unlock ();
	//    }
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iobase/Storage.h"
#include "classlib/utils/DebugAids.h"

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

StorageInputStream::StorageInputStream (Storage *input)
    : m_storage (input)
{ ASSERT (m_storage); }

// doesn't delete storage but does close it (FIXME: exceptions)
StorageInputStream::~StorageInputStream (void)
{ /* close (); */ }

IOSize
StorageInputStream::available (void)
{ return m_storage->size () - m_storage->position (); }

IOSize
StorageInputStream::read (void *into, IOSize n)
{ return m_storage->read (into, n); }

IOSize
StorageInputStream::readv (IOBuffer *into, IOSize buffers)
{ return m_storage->readv (into, buffers); }

IOSize
StorageInputStream::skip (IOSize n)
{
    IOOffset old = m_storage->position ();
    IOOffset after = m_storage->position (n, Storage::CURRENT);
    return after - old;
}

void
StorageInputStream::close (void)
{ m_storage->close (); }

void
StorageInputStream::reset (void)
{ m_storage->rewind (); }

} // namespace lat

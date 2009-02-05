#include "classlib/iobase/Filename.h"
#include "classlib/iobase/FileNameLock.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>


namespace lat_test { 
  extern std::ostream & out;
}



using namespace lat;

inline void
dump (const char *scope, const Filename &x)
{
    Filename target;
    std::cout << "### " << scope << ": " << x
	      << "\nisAbsolute?     = " << x.isAbsolute ()
	      << "\nisRelative?     = " << x.isRelative ()
	      << "\n"

	      << "\nexists?         = " << x.exists ()
	      << "\nisDirectory?    = " << x.isDirectory ()
	      << "\nisRegular?      = " << x.isRegular ()
	      << "\nisSymlink?      = " << x.isSymlink (&target)
	      << "\n  target        = " << target
	      << "\nisAccessibeDir? = " << x.isAccessibleDirectory ()
	      << "\nisOwnershipPr?  = " << x.isOwnershipPreserved ()
	      << "\nisReadable?     = " << x.isReadable ()
	      << "\nisWritable?     = " << x.isWritable ()
	      << "\nisExecutable?   = " << x.isExecutable ()
	      << "\n"

	// FIXME: status, setStatus, times

	      << "\nasDirectory     = " << x.asDirectory ()
	      << "\nasFile          = " << x.asFile ()
	      << "\n"

	// << "\ndrive           = " << x.drive ()
	// << "\nshare           = " << x.share ()
	      << "\ndirectory       = " << x.directory ()
	      << "\nnondirectory    = " << x.nondirectory ()
	      << "\nextension       = " << x.extension ()
	      << "\n.extension      = " << x.extension (0, true)
	      << "\nsansExtension   = " << x.sansExtension ()
	      << "\n"

	// FIXME: append
	
	      << "\ntruename        = " << x.truename ()
	      << "\nchaseLinks      = " << x.chaseLinks ()
	      << "\n"

	      << "\nautosaveName    = " << x.autosaveName ()
	      << "\nisAutosaveName? = " << x.isAutosaveName ()
	      << "\n"

	// FIXME: findBackupName
	      << "\nbackupName      = " << x.backupName ()
	      << "\nisBackupName?   = " << x.isBackupName ()
	      << "\nsansVersions    = " << x.sansVersions ()
	      << "\n"

	      << "\nexpand          = " << x.expand ()
	      << "\nabbreviate      = " << x.abbreviate ()

	      << "\n";
}

inline void
describe (bool locked, FileNameLock *lock)
{
    std::cout << "\nisLocked?       = ";

    if (locked)
	if (! lock)
	    std::cout << "by myself";
	else
	    std::cout << "user = " << lock->user ()
		      << ", host = " << lock->host ()
		      << ", pid = " << lock->pid ()
		      << ", time = " << lock->time ()
		      << std::endl;
    else
	std::cout << "no\n";
}

inline void
dumpl (const char *scope, const Filename &x)
{
    dump (scope, x);

    if (! x.isDirectory ())
    {
	FileNameLock	*l = 0;
	bool		locked;

	locked = FileNameLock::isLocked (x, &l);
	describe (locked, l);
	delete l;
	l = 0;

	FileNameLock lock;

	lock.lock (x);
	locked = FileNameLock::isLocked (x, &l);
	describe (locked, l);
	delete l;
	l = 0;

	lock.unlock ();
	locked = FileNameLock::isLocked (x, &l);
	describe (locked, l);
	delete l;
	l = 0;
    }
}

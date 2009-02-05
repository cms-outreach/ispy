#include "test/Test.h" 
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/TempFile.h"
#include "classlib/iobase/FileNameLock.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace lat;

void
describel (bool locked, FileNameLock *lock)
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
	std::cout << "no";
}

void
describe (Filename x)
{
    Filename target;

    std::cout << "\nx               = " << x
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

	// FIXME: complete
	// FIXME: glob
	      << "\n";

    if (! x.isDirectory ())
    {
	FileNameLock	*l = 0;
	bool		locked;

	locked = FileNameLock::isLocked (x, &l);
	describel (locked, l);
	delete l;
	l = 0;

	FileNameLock lock;

	lock.lock (x);
	locked = FileNameLock::isLocked (x, &l);
	describel (locked, l);
	delete l;
	l = 0;

	lock.unlock ();
	locked = FileNameLock::isLocked (x, &l);
	describel (locked, l);
	delete l;
	l = 0;
    }
}

void
complete (Filename prefix)
{
    std::vector<Filename>	vec;
    bool			exact;

    prefix.complete (vec, Filename::cwd ());
    std::cout << "\ncomplete (" << prefix << "):\n";
    for (size_t i = 0; i < vec.size (); ++i)
	std::cout << "  " << vec [i] << "\n";

    if (prefix.complete (Filename::cwd (), &exact))
    {
	std::cout << " ==> " << prefix
		  << (exact ? " (exact)\n" : "\n");
    }
    else
	std::cout << " ... no match\n";
}

void
glob (Filename pat)
{
    std::vector<Filename> vec;
    pat.glob (vec, Filename::cwd ());
    std::cout << "\nglob (" << pat << "):\n";
    for (size_t i = 0; i < vec.size (); ++i)
	std::cout << "  " << vec [i] << "\n";
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout <<   "tmpdir          = " << TempFile::tempdir ()
	      << "\ncwd             = " << Filename::cwd ()
	      << "\nnull            = " << Filename::null ()
	      << "\n\n";

    static const char *names [] = {
	"..",
	".",
	"/.",
	"/",
	"/..",
	"/tmp/..",
	"foo.txt",
	"$PWD/bar.txt",
	"../zoo",
	"..//tmp/zoo",
	"$PWD//tmp/$VISUAL//tmp/foobar",
	"$PWD/$VISUAL/goo",
	"$PWD/$VISUAL/../zork",
	0
    };

    ShellEnvironment env;
    for (const char **n = names; *n; ++n)
    {
	std::cout << "##################################################\n";
	std::cout << "### base: " << *n << std::endl;
	describe (*n);
	std::cout << "\n\n";

	std::cout << "### expanded: " << *n << std::endl;
	describe (Filename (*n).expand ());
	std::cout << "\n\n";

	std::cout << "### substituted: " << *n << std::endl;
	describe (Filename (*n).substitute (env));
	std::cout << "\n\n";
    }

    complete ("r");
    complete ("lib");
    // to fix (glob) 
#if 0
    glob ("[a-p]*test*");
#endif

    // FIXME: remove
    // FIXME: rename
    // FIXME: move
    // FIXME: copy

    return 0;
}

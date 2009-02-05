#include "test/Test.h" 
#include "classlib/utils/Regexp.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123", ':'), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123", ':', StringOps::TrimEmpty), "\n")
	   == "abc\n123");
    
    VERIFY(StringOps::join
	   (StringOps::split (":abc:123", ':'), "\n")
	   == "\nabc\n123");

    VERIFY(StringOps::join
	   (StringOps::split (":abc:123", ':', StringOps::TrimEmpty), "\n")
	   == "abc\n123");
    
    VERIFY(StringOps::join
	   (StringOps::split ("abc:123:", ':'), "\n")
	   == "abc\n123\n");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123:", ':', StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("::abc:123:", ':'), "\n")
	   == "\n\nabc\n123\n");

    VERIFY(StringOps::join
	   (StringOps::split ("::abc:123:", ':', StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123", "c:1"), "\n")
	   == "ab\n23");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123", "c:1", StringOps::TrimEmpty), "\n")
	   == "ab\n23");
    
    VERIFY(StringOps::join
	   (StringOps::split (":abc:123", ":"), "\n")
	   == "\nabc\n123");

    VERIFY(StringOps::join
	   (StringOps::split (":abc:123", ":", StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123:", ":"), "\n")
	   == "abc\n123\n");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123:", ":", StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("::abc:123:", ":"), "\n")
	   == "\n\nabc\n123\n");

    VERIFY(StringOps::join
	   (StringOps::split ("::abc:123:", ":", StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split (":abc:123", Regexp (":")), "\n")
	   == "\nabc\n123");

    VERIFY(StringOps::join
	   (StringOps::split (":abc:123", Regexp (":"), StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123:", Regexp (":")), "\n")
	   == "abc\n123\n");

    VERIFY(StringOps::join
	   (StringOps::split ("abc:123:", Regexp (":"), StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("::abc:123:", Regexp (":")), "\n")
	   == "\n\nabc\n123\n");

    VERIFY(StringOps::join
	   (StringOps::split ("::abc:123:", Regexp (":"), StringOps::TrimEmpty), "\n")
	   == "abc\n123");

    VERIFY(StringOps::join
	   (StringOps::split ("/foo/bar", "/", 0, 0, 2), "\n")
	   == "bar");

    VERIFY(StringOps::join
	   (StringOps::split ("/foo/bar/zoo", "/", 0, 0, 2), "\n")
	   == "bar\nzoo");

    return 0;
}

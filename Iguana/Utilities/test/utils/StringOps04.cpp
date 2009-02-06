#include "test/Test.h" 
#include "classlib/utils/Regexp.h"
#include "classlib/utils/RegexpMatch.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    RegexpMatch m;
    std::string t("abcdef");

    VERIFY(Regexp("[def]+").search("abcdef", 0, 0, &m) == 3);
    VERIFY(m.matchPos() == 3);
    VERIFY(m.matchLength() == 3);
    VERIFY(m.matchEnd() == 6);
    
    VERIFY(StringOps::replace("abcdef", Regexp(".*?([def]+)"), "ABC\\1") == "ABCdef");
    VERIFY(StringOps::replace("abcdef", Regexp("([de])"), "<\\1>") == "abc<d><e>f");

    VERIFY(StringOps::replace("a/b", Regexp("^([^/]+)"), "x/\\1/y") == "x/a/y/b");
    VERIFY(StringOps::replace("a/b", Regexp("^([^/]+)"), "x/\\1") == "x/a/b");

    VERIFY(StringOps::replace("a/b", Regexp("^"), "") == "a/b");

    return 0;
}

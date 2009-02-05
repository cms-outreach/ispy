#include "test/Test.h" 
#include "classlib/utils/Range.h"
#include "classlib/utils/Signal.h"
#include <utility>
#include <iostream>
#include <iomanip>

using namespace lat;

#if HAVE_NAMESPACE_STD_REL_OPS
using std::rel_ops::operator>;
#elif HAVE_REL_OPS_IN_STD
using std::operator>;
#endif

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Range<int> r (10, 20);

    Range<int> r1 = MakeRange (10, 11);
    Range<int> r2 = MakeRange (9, 11);
    Range<int> r3 = MakeRange (12, 16);
    Range<int> r4 = MakeRange (20, 21);

    Range<int> i0 = r.intersection (r);
    Range<int> i1 = r.intersection (r1);
    Range<int> i2 = r.intersection (r2);
    Range<int> i3 = r.intersection (r3);
    Range<int> i4 = r.intersection (r4);

    std::cout << "range           = [" << r.low () << ", " << r.high () << ")\n"
	      << "  width         = " << r.width () << "\n\n"
	      << "  empty         = " << r.empty () << "\n\n"

	      << "  contains     10?      "  << r.contains (10) << "\n"
	      << "  contains     14?      "  << r.contains (14) << "\n"
	      << "  contains     20?      "  << r.contains (20) << "\n\n"

	      << "  contains     [10,20)? "  << r.contains (r) << "\n"
	      << "  contains     [10,11)? "  << r.contains (r1) << "\n"
	      << "  contains     [ 9,11)? "  << r.contains (r2) << "\n"
	      << "  contains     [12,16)? "  << r.contains (r3) << "\n"
	      << "  contains     [20,21)? "  << r.contains (r4) << "\n\n"

	      << "  containsStr  [10,20)? "  << r.containsStrictly (r) << "\n"
	      << "  containsStr  [10,11)? "  << r.containsStrictly (r1) << "\n"
	      << "  containsStr  [ 9,11)? "  << r.containsStrictly (r2) << "\n"
	      << "  containsStr  [12,16)? "  << r.containsStrictly (r3) << "\n"
	      << "  containsStr  [20,21)? "  << r.containsStrictly (r4) << "\n\n"

	      << "  intersects   [10,20)? "  << r.intersects (r) << "\n"
	      << "  intersects   [10,11)? "  << r.intersects (r1) << "\n"
	      << "  intersects   [ 9,11)? "  << r.intersects (r2) << "\n"
	      << "  intersects   [12,16)? "  << r.intersects (r3) << "\n"
	      << "  intersects   [20,21)? "  << r.intersects (r4) << "\n\n"

	      << "  intersection [10,20)  [" << i0.low () << ", " << i0.high ()
	      << ") (empty=" << i0.empty () << ")\n"
	      << "  intersection [10,11)  [" << i1.low () << ", " << i1.high ()
	      << ") (empty=" << i1.empty () << ")\n"
	      << "  intersection [ 9,11)  [" << i2.low () << ", " << i2.high ()
	      << ") (empty=" << i2.empty () << ")\n"
	      << "  intersection [12,16)  [" << i3.low () << ", " << i3.high ()
	      << ") (empty=" << i3.empty () << ")\n"
	      << "  intersection [20,21)  [" << i4.low () << ", " << i4.high ()
	      << ") (empty=" << i4.empty () << ")\n\n"

	      << "  ==           [10,20)? "  << (r == r) << "\n"
	      << "  ==           [10,11)? "  << (r == r1) << "\n"
	      << "  ==           [ 9,11)? "  << (r == r2) << "\n"
	      << "  ==           [12,16)? "  << (r == r3) << "\n"
	      << "  ==           [20,21)? "  << (r == r4) << "\n\n"

	      << "  <            [10,20)? "  << (r < r) << "\n"
	      << "  <            [10,11)? "  << (r < r1) << "\n"
	      << "  <            [ 9,11)? "  << (r < r2) << "\n"
	      << "  <            [12,16)? "  << (r < r3) << "\n"
	      << "  <            [20,21)? "  << (r < r4) << "\n\n"

	      << "  >            [10,20)? "  << (r > r) << "\n"
	      << "  >            [10,11)? "  << (r > r1) << "\n"
	      << "  >            [ 9,11)? "  << (r > r2) << "\n"
	      << "  >            [12,16)? "  << (r > r3) << "\n"
	      << "  >            [20,21)? "  << (r > r4) << "\n";

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <new>

static char		*reserve = 0;
static std::new_handler	old = 0;

static void handler (void)
{
    delete [] reserve;
    std::set_new_handler (old);
    fprintf (stdout, "reserve freed\n");
}

int main (int argc, char **argv) {
    int n = 0;
    if (argc < 3 || sscanf (argv[1], "%d", &n) != 1) {
	fprintf (stderr, "%s: usage RESERVE N [N...]\n  "
		 "all chunks are in kilobytes\n", argv[0]);
	return 3;
    }

    try
    {
	reserve = new char [n * 1024 * 1024 / 10];
    }
    catch (std::bad_alloc &)
    {
	fprintf (stderr, "can't get reserve\n");
	return 1;
    }

    old = std::set_new_handler (&handler);

    void *foo = 0;
    for (int i = 2; i < argc; ++i)
    {
	if (sscanf (argv[i], "%d", &n) != 1)
	{
	    fprintf (stderr, "arg %d is not an integer (got <%s>)\n", i-1, argv[i]);
	    return 2;
	}
	try
	{
	    foo = new char [n * 1024 * 1024 / 10];
	}
	catch (std::bad_alloc &)
	{
	    fprintf (stdout, "%s: out of memory on %d\n", argv[0], i);
	    return 1;
	}
	fprintf (stdout, "%p\n", foo);
	memset (foo, 0, n * 1024 * 1024 / 10);
    }
    return 0;
}

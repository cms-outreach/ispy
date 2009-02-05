#include <sys/types.h> // For Mac OS X.
#include <sys/time.h> // For Mac OS X.
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

#ifndef RLIMIT_AS
# define RLIMIT_AS RLIMIT_DATA
#endif

int getlimit (int &rold)
{
    rlimit lold;
    if (getrlimit (RLIMIT_AS, &lold))
    {
	perror ("getrlimit");
	return 255;
    }

    rold = lold.rlim_cur;
    return 0;
}

int bumplimit (int limit, int &rold, int &rnew)
{
    rlimit lold;
    rlimit lnew;
    if (getrlimit (RLIMIT_AS, &lold))
    {
	perror ("getrlimit");
	return 255;
    }

    lnew = lold;
    lnew.rlim_cur = limit;
    if (setrlimit (RLIMIT_AS, &lnew))
    {
	perror ("setrlimit");
	return 255;
    }
    if (getrlimit (RLIMIT_AS, &lnew))
    {
	perror ("getrlimit");
        return 255;
    }

    rold = lold.rlim_cur;
    rnew = lnew.rlim_cur;

    return 0;
}

int main (int argc, char **argv)
{
    int limit = 0;
    if (argc < 3 || sscanf (argv[1], "%d", &limit) != 1) {
        if (getlimit (limit)) limit = 0;
        fprintf (stderr, "usage: %s LIMIT PROGRAM [ARGS...]   "
		 "(current LIMIT = %d)\n", argv[0], limit);
        return 255;
    }
    int rold;
    int rnew;
    int status = bumplimit (limit, rold, rnew);
    if (status) return status;

    fprintf (stdout, "virtual memory limited to %d (was %d)\n", rnew, rold);
    fflush (stdout);

    execvp (argv[2], &argv[2]);
    return 255;
}

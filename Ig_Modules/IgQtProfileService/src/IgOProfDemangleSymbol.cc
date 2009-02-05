/**
 * @file demangle_symbol.cpp
 * Demangle a C++ symbol
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon <moz@compsoc.man.ac.uk>
 */

#include "Ig_Modules/IgQtProfileService/src/IgOProfDemangleSymbol.h"

// from libiberty
/*@{\name demangle option parameter */
#ifndef DMGL_PARAMS
# define DMGL_PARAMS     (1 << 0)        /**< Include function args */
#endif
#ifndef DMGL_ANSI
# define DMGL_ANSI       (1 << 1)        /**< Include const, volatile, etc */
#endif
/*@}*/
extern "C" char * cplus_demangle(char const * mangled, int options);

QString const demangle_symbol(QString const & name)
{
	// Do not try to strip leading underscore, this leads to many
	// C++ demangling failures.
	char * unmangled = cplus_demangle(name.latin1(), DMGL_PARAMS | DMGL_ANSI);

	if (!unmangled)
		return name;

	QString const result(unmangled);
	free(unmangled);

	return result;
}

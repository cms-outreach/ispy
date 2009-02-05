#ifndef __IG_OPROF_HITS_STRING_H__
#define __IG_OPROF_HITS_STRING_H__

#include <qstring.h>

class IgOProfHitsString : public QString
{
public:
    IgOProfHitsString(const int hits, const int hits_offset);
    IgOProfHitsString(const int hits, const int hits_offset, const int parent, const int parent_offset);
};

#endif

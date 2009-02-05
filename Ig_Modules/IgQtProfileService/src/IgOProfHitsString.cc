//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfHitsString.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfHitsString::IgOProfHitsString(const int hits, const int hits_offset)
{
    append(QString::number(hits-hits_offset,10));
    if(hits_offset !=0)append(" ("+QString::number(hits,10)+")");
}

IgOProfHitsString::IgOProfHitsString(const int hits, const int hits_offset, const int parent, const int parent_offset)
{
    double percent;
    if(parent-parent_offset != 0)
    {
	percent = (double)(hits - hits_offset)/(double)(parent - parent_offset)*100;
    }
    else percent = 0;
    append(QString::number(percent,'f',4));
    if(hits_offset !=0)append(" ("+QString::number((double)hits/(double)parent,'f',4)+")");
}	

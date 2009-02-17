#ifndef IG_OPEN_INVENTOR_IG_SO_2D_ARROW_H
# define IG_OPEN_INVENTOR_IG_SO_2D_ARROW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSo2DArrow IgSo2DArrow.h
 *
 * \brief An arrow in the (x,y) plane along the x axis, with total length length. 
 * The arrowhead heigth is headHeight and its length lengthRatio*length
 *
 * \author Laurent Duflot   
 * \date April 2002
 */
class IG_OPEN_INVENTOR_API IgSo2DArrow : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSo2DArrow);
    SO_KIT_CATALOG_ENTRY_HEADER (line);
    SO_KIT_CATALOG_ENTRY_HEADER (head);

public:
    IgSo2DArrow (void);
    static void initClass (void);

    SoSFFloat length;        //< total length including head
    SoSFFloat lengthRatio;   //< fraction of the length for the head
    SoSFFloat headHeight;    //< width of the head

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_2D_ARROW_H

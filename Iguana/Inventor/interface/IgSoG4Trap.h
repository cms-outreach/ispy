#ifndef IG_OPEN_INVENTOR_IG_SO_G4_TRAP_H
# define IG_OPEN_INVENTOR_IG_SO_G4_TRAP_H

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

/*!	\class IgSoG4Trap IgSoG4Trap.h
 *
 *  \brief: The documentation from Geant says:
 * A G4Trap is a general trapezoid: The faces perpendicular to the z planes
 * are tapezia, and their centres are not necessarily on a line parallel to
 * the z axis.
 *
 * Note that of the 11 parameters desribed below, only 9 are really
 * independent - a check for planarity is made in the calculation of the
 * equation for each plane. If the planes are not parallel, a call to
 * G4Exception is made.
 *
 *      pDz     Half-length along the z-axis
 *      pTheta  Polar angle of the line joining the centres of the faces
 *              at -/+pDz
 *      pPhi    Azimuthal angle of the line joing the centre of the face at
 *              -pDz to the centre of the face at +pDz
 *      pDy1    Half-length along y of the face at -pDz
 *      pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
 *      pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
 *      pAlp1   Angle with respect to the y axis from the centre of the side
 *              at y=-pDy1 to the centre at y=+pDy1 of the face at -pDz
 *
 *      pDy2    Half-length along y of the face at +pDz
 *      pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
 *      pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
 *      pAlp2   Angle with respect to the y axis from the centre of the side
 *              at y=-pDy2 to the centre at y=+pDy2 of the face at +pDz
 *
 *
 *	\author Joe Boudreau, HEPVis project
 *  \date 11 Nov 1996
 *
 *  
 */
class IG_OPEN_INVENTOR_API IgSoG4Trap : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoG4Trap);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);

public:
    IgSoG4Trap (void);
    static void initClass (void);

    SoSFFloat dz;	//< Half-length along the z-axis               
    SoSFFloat theta;	//< Polar angle of the line joining the centres of the faces at -/+dz
    SoSFFloat phi;	//< Azimuthal angle of the line joing the centre of the face at -dz to the centre of the face at +pDz
    SoSFFloat dy1;	//< Half-length along y of the face at -dz
    SoSFFloat dx1;	//< Half-length along x of the side at y=-dy1 of the face at -dz
    SoSFFloat dx2;	//< Half-length along x of the side at y=+dy1 of the face at -dz
    SoSFFloat dy2;	//< Half-length along y of the face at +dz
    SoSFFloat dx3;	//< Half-length along x of the side at y=-dy2 of the face at +dz
    SoSFFloat dx4;	//< Half-length along x of the side at y=+dy2 of the face at +dz
    SoSFFloat alp1;	//< Angle with respect to the y axis from the centre of the side at y=-dy1 to the centre at y=+dy1 of the face at -dz
    SoSFFloat alp2;	//< Angle with respect to the y axis from the centre of the side at y=-dy2 to the centre at y=+dy2 of the face at +dz

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_G4_TRAP_H

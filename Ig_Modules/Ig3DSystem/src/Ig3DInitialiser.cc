//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DSystem/interface/Ig3DInitialiser.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DInitialiser::Ig3DInitialiser (lat::Callback action)
{ Ig3DSystem::initAction (action); }

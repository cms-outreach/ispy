#include "Ig_Modules/Ig3DBrowser/interface/Ig3DBrowser.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"

const char *
Ig3DBrowser::catalogLabel (void)
{ return "3D"; }

Ig3DBrowser::Ig3DBrowser (IgState *state, 
			  IgSite *site, 
			  Ig3DModel *model /* = 0 */)
    : Ig3DBaseBrowser (state, 
		       site, 
		       model ? model : new Ig3DModel (state))
{ }

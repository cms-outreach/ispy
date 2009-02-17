//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include "test_main.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QString makeTest (SoSeparator *root)
{
    IgSoShapeKit::initClass ();
    IgSoCrystalHit::initClass ();

    // Bind material per face
    SoMaterialBinding *binding = new SoMaterialBinding;
    binding->value = SoMaterialBinding::PER_FACE;
    root->addChild (binding);
    SbColor c1 [] = { SbColor (1.0, 0.0, 0.0), SbColor (1.0, 0.0, 0.0), SbColor (1.0, 0.0, 0.0),
		      SbColor (1.0, 0.0, 0.0), SbColor (1.0, 0.0, 0.0), SbColor (1.0, 0.0, 0.0) };
    SbColor c2 [] = { SbColor (0.0, 1.0, 0.0), SbColor (0.0, 1.0, 0.0), SbColor (0.0, 1.0, 0.0),
		      SbColor (0.0, 1.0, 0.0), SbColor (0.0, 1.0, 0.0), SbColor (0.0, 1.0, 0.0) };

    SoMaterial	*mat = new SoMaterial;
    mat->diffuseColor.setValues (0, 6, c1);
    mat->diffuseColor.setValues (6, 6, c2);
    
    root->addChild (mat);

    IgSoCrystalHit *obj = new IgSoCrystalHit;
    obj->energy = 1.0;
    root->addChild (obj);

    return "IGUANA: IgSoCrystalHit Test";
}

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/interface/IgSoMaterialMap.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <cstdlib>
#include <iostream>
#include <classlib/iobase/Filename.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


IgSoMaterialMap::IgSoMaterialMap (void)
{
    setDefaultMaterials ();    

    std::string filename;
    
    char *fn = getenv ("SO_MATERIALS_IV");        
    if (!fn)
	filename = "./materials.iv";
    else
	filename = fn;

    SoInput inFile;
    if (lat::Filename (filename).exists ())
    {
	if (inFile.openFile (filename.c_str ()))	
	{
	   this->load (inFile);	   
	}
    }    
}

SoMaterial *
IgSoMaterialMap::operator[] (const std::string &name)
{
    MaterialMap::iterator pos = m_materialMap.find (name);
    return pos != m_materialMap.end () ? pos->second : 0;
}
    
IgSoMaterialMap::MaterialMap::iterator 
IgSoMaterialMap::begin (void)
{
    return m_materialMap.begin ();	    
}

IgSoMaterialMap::MaterialMap::iterator
IgSoMaterialMap::end (void)
{
    return m_materialMap.end ();	    
}


void 
IgSoMaterialMap::load (SoInput &input)
{
    SoNode *node = 0;
    while (SoDB::read (&input, node) && node)
    {
	ASSERT (node->isOfType (SoMaterial::getClassTypeId ()));
	SoMaterial *material = static_cast<SoMaterial *> (node);
	ASSERT (material);
	material->ref ();	    
	m_materialMap [material->getName ().getString ()] = material;
    }    
}


void
IgSoMaterialMap::setDefaultMaterials (void)
{
    std::string defaultMaterials = "";
    defaultMaterials += "#Inventor V2.1 ascii \n";    
    defaultMaterials += "\n";
    defaultMaterials += "\n";
    
    defaultMaterials += "DEF air Material { \n";
    defaultMaterials += "ambientColor 0.38 1 1 \n";
    defaultMaterials += "diffuseColor 0 0.44 0.75\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.1\n";
    defaultMaterials += "transparency 0.49\n";
    defaultMaterials += "}\n";

    defaultMaterials += "DEF aluminium Material {\n";
    defaultMaterials += "ambientColor 0.00714122 0.00495918 0\n";
    defaultMaterials += "diffuseColor 0.68 0.68 0.68\n";
    defaultMaterials += "specularColor 0.729595 0.74 0.72\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.13\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF brass Material {\n";
    defaultMaterials += "ambientColor 0.00714122 0.00495918 0\n";; 
    defaultMaterials += "diffuseColor 1 0.82 0.48\n";
    defaultMaterials += "specularColor 0.44 0.32 0.28\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.16\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF cable Material {\n";
    defaultMaterials += "ambientColor 1 0 0\n";
    defaultMaterials += "diffuseColor 1 0 0\n";
    defaultMaterials += "specularColor 0.44 0.32 0.28\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.06\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF carbon Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.24 0.23 0.23\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.92\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF ceramic Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.93 0.9 0.89\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.92\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF concrete Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.58 0.58 0.59\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.92\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF copper Material {\n";
    defaultMaterials += "ambientColor 0.13 0.23 0\n";
    defaultMaterials += "diffuseColor 0.66 0.45 0\n";
    defaultMaterials += "specularColor 0.76 0.5 0.15\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.11\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF default Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.93 0.89 0.88\n";
    defaultMaterials += "specularColor 0.79 0.84 0.84\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 1\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF defaultGas Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.93 0.9 0.89\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.92\n";
    defaultMaterials += "transparency 0.69\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF foam Material {\n";
    defaultMaterials += "ambientColor 0.00746166 0.00672922 0.00689707\n";
    defaultMaterials += "diffuseColor 0.02916 0.0262913 0.0269627\n";
    defaultMaterials += "specularColor 0.641611 0.651041 0.979599\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.0612192\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF glass Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.93 0.9 0.89\n";
    defaultMaterials += "specularColor 0.42 0.41 0.43\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0\n";
    defaultMaterials += "transparency 0.62\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF gold Material {\n";
    defaultMaterials += "ambientColor 0.00714122 0.00495918 0\n";
    defaultMaterials += "diffuseColor 0.72 0.41 0\n";
    defaultMaterials += "specularColor 1 1 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.15\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF iron Material {\n";
    defaultMaterials += "ambientColor 0 0.0186465 0\n";
    defaultMaterials += "diffuseColor 0.59 0.56 0.56\n";
    defaultMaterials += "specularColor 0.66 0.66 0.66\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.43\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF lead Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.27 0.27 0.27\n";
    defaultMaterials += "specularColor 0.882658 0.9 0.89\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.2\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF silicon Material {\n";
    defaultMaterials += "ambientColor 0.0570077 0.0395514 0\n";
    defaultMaterials += "diffuseColor 0.59 0.36 0\n";
    defaultMaterials += "specularColor 1 1 1\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.03\n";
    defaultMaterials += "transparency 0\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF vacuum Material {\n";
    defaultMaterials += "ambientColor 0 0 0\n";
    defaultMaterials += "diffuseColor 0.93 0.9 0.89\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.92\n";
    defaultMaterials += "transparency 0.66\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF water Material {\n";
    defaultMaterials += "ambientColor 0.00714122 0.00169375 0\n";
    defaultMaterials += "diffuseColor 0.02 0.27 1\n";
    defaultMaterials += "specularColor 0.28 0 0.77\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 0.04\n";
    defaultMaterials += "transparency 0.1\n";
    defaultMaterials += "}\n";
    defaultMaterials += "\n";
    defaultMaterials += "DEF wood Material {\n";
    defaultMaterials += "ambientColor 0.0202792 0 0.0198978\n";
    defaultMaterials += "diffuseColor 0.56 0.41 0.34\n";
    defaultMaterials += "specularColor 0 0 0\n";
    defaultMaterials += "emissiveColor 0 0 0\n";
    defaultMaterials += "shininess 1\n";
    defaultMaterials += "transparency 0\n";    
    defaultMaterials += "}\n";

    SoInput input;
    
    input.setBuffer ((void *) (defaultMaterials.c_str ()), defaultMaterials.size ());
    this->load (input);
}


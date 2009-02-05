#ifndef IG_G4_VOLUMES_IG_SO_MATERIAL_MAP_H
# define IG_G4_VOLUMES_IG_SO_MATERIAL_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"
# include "Inventor/nodes/SoMaterial.h"
# include <map>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoMaterial;
class SoInput;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgSoMaterialMap
{
public:
    typedef std::map<std::string, SoMaterial *> MaterialMap;      


    IgSoMaterialMap (void);
    SoMaterial *operator[] (const std::string &name);    
    MaterialMap::iterator begin (void);
    MaterialMap::iterator end (void);
    SoMaterial *getFullInfos (const std::string &name) 
	{
	    return m_materialMap [name];
	}

    void addEntry (const char *name, SoMaterial *entry)
	{
	    entry->ref ();	    
	    entry->setName (name);	    
	    m_materialMap [name] = entry;	    
	}    
    
    void load (SoInput &input);    

private:
    MaterialMap m_materialMap;    
    void setDefaultMaterials (void);    
};

class IG3DMATERIAL_EDITOR_API IgSoMaterialMapSingleton
{
public:
    static IgSoMaterialMap *instance (void)
	{
	    static IgSoMaterialMap *__instance = 0;
	    if (__instance == 0)
	    {
		__instance = new IgSoMaterialMap ();
	    }	  
	    return __instance;	    
	}    
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_VOLUMES_IG_SO_MATERIAL_MAP_H

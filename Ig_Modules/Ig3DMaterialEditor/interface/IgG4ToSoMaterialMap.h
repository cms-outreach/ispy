#ifndef IG_3D_MATERIAL_EDITOR_IG_G4TOSO_MATERIAL_MAP_H
# define IG_3D_MATERIAL_EDITOR_IG_G4TOSO_MATERIAL_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"
# include <map>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgG4ToSoMaterialMap
{
public:
    typedef std::map <std::string, std::string> MaterialMap;    
    IgG4ToSoMaterialMap (void);
    std::string &operator[] (const char *name);    
    MaterialMap::iterator begin (void);
    MaterialMap::iterator end (void);
    MaterialMap::iterator find (const char *key);
    
    /**Sets up a default mapping table for G4Materials->SoMaterial*/
    void defaultMappings (void);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    MaterialMap m_g4ToSoMap;
};

class IG3DMATERIAL_EDITOR_API IgG4ToSoMaterialMapSingleton
{
public:
    static IgG4ToSoMaterialMap *instance (void)
	{
	    static IgG4ToSoMaterialMap *__instance = 0;
	    if (__instance == 0)
	    {
		__instance = new IgG4ToSoMaterialMap ();
	    }	
	    return __instance;	    
	}   	 
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_VOLUMES_IG_G4MATERIAL_MAP_H

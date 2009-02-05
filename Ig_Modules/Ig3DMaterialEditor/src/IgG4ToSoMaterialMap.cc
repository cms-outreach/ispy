//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/interface/IgG4ToSoMaterialMap.h"
#include <classlib/utils/DebugAids.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgG4ToSoMaterialMap::IgG4ToSoMaterialMap (void)
{
    std::string materialList;
    
    char *tmpMaterialList = getenv ("MATERIAL_CONFIG");
    if ( tmpMaterialList == 0 )
    {	
	materialList = "./materials.map";
    }
    else
    {
	materialList = tmpMaterialList;	
    }       

    defaultMappings ();    
    std::ifstream mlStream (materialList.c_str ());

    if (mlStream)
    {
	while (mlStream.eof () != true)
	{
	    std::stringbuf tmpBuf1;	    
	    std::stringbuf tmpBuf2;
	    
	    std::string g4Mat;
	    std::string soMat;
	    
	    mlStream.get (tmpBuf1,'=');
	    mlStream.ignore (1);
	    mlStream >> std::ws;
	    g4Mat = tmpBuf1.str ();
	    mlStream.get (tmpBuf2,'\n');
	    mlStream.ignore (1);	    
	    soMat = tmpBuf2.str ();
	    
	    int i = soMat.size ();
	    while ( soMat[i-1] == ' ' && i-1 >= 0) 
		i--;		    
	    
	    soMat.erase (i, soMat.size ());	       
	    
	    if ( soMat != "" && soMat[0] !='#')
	    {	    
		std::cerr << g4Mat << "->" << soMat << std::endl;	    
		m_g4ToSoMap[g4Mat] = soMat;
	    }
	}
    }
}


std::string & 
IgG4ToSoMaterialMap::operator[] (const char *name)
{
    return m_g4ToSoMap [name];  
}    

IgG4ToSoMaterialMap::MaterialMap::iterator 
IgG4ToSoMaterialMap::begin (void)
{
    return m_g4ToSoMap.begin ();
}

IgG4ToSoMaterialMap::MaterialMap::iterator 
IgG4ToSoMaterialMap::end (void)
{
    return m_g4ToSoMap.end ();
}

IgG4ToSoMaterialMap::MaterialMap::iterator 
IgG4ToSoMaterialMap::find (const char *key)
{
    return m_g4ToSoMap.find (key);
}

void
IgG4ToSoMaterialMap::defaultMappings (void)
{
    m_g4ToSoMap["80% Argon + 20% CO_2"]= "air";		
    m_g4ToSoMap["AISI-1006-Steel"]= "iron";		
    m_g4ToSoMap["AISI-1045-Steel"]= "iron";		
    m_g4ToSoMap["AQBB borated concre"]= "concrete";		
    m_g4ToSoMap["Air"]= "air";		
    m_g4ToSoMap["Al support"]= "aluminium";		
    m_g4ToSoMap["Al-MMC"]= "aluminium";		
    m_g4ToSoMap["Al-silicate + Zi"]= "aluminium";		
    m_g4ToSoMap["Al_2 O_3"]= "aluminium";		
    m_g4ToSoMap["Alumina"]= "aluminium";		
    m_g4ToSoMap["Aluminium"]= "aluminium";		
    m_g4ToSoMap["Aluminium Pix_b"]= "aluminium";		
    m_g4ToSoMap["Aluminium honeycomb"]= "aluminium";		
    m_g4ToSoMap["Aluminium silicate"]= "aluminium";		
    m_g4ToSoMap["Aluminized Mylar"]= "aluminium";		
    m_g4ToSoMap["Antimony"]= "default";		
    m_g4ToSoMap["Ar CO_2 gas 50:50"]= "default";		
    m_g4ToSoMap["Ar(40%)+Ethane(60%)"]= "default";    
    m_g4ToSoMap["Ar/Cu mixt. for Cors"]= "copper";    
    m_g4ToSoMap["Ar/Cu mixture for EM"]= "copper";    
    m_g4ToSoMap["Ar/Cu mixture for HD"]= "copper";    
    m_g4ToSoMap["Ar/Pb mixture for EM"]= "lead";    
    m_g4ToSoMap["Ar/W Catcher section"]= "aluminium";    
    m_g4ToSoMap["Ar/W Hadron section"]= "aluminium";    
    m_g4ToSoMap["Ar/W mixt. for E.M."]= "aluminium";    
    m_g4ToSoMap["Argon"]= "default";    
    m_g4ToSoMap["Argon 50% CF_4 CO_2"]= "default";    
    m_g4ToSoMap["Argon CF_4 CO_2"]= "default";    
    m_g4ToSoMap["Arsenic"]= "default";    
    m_g4ToSoMap["BET2a concrete +Poly"]= "concrete";    
    m_g4ToSoMap["BET2b concrete +Poly"]= "concrete";    
    m_g4ToSoMap["B_2 O_3"]= "default";    
    m_g4ToSoMap["Ba O"]= "default";    
    m_g4ToSoMap["Bakelite"]= "default";    
    m_g4ToSoMap["Barite"]= "default";    
    m_g4ToSoMap["Barium"]= "default";    
    m_g4ToSoMap["Barium sulfate"]= "default";    
    m_g4ToSoMap["Beryllia"]= "default";    
    m_g4ToSoMap["Beryllium"]= "default";    
    m_g4ToSoMap["Bi_4 Ge_3 O_12"]= "default";		
    m_g4ToSoMap["Bismuth"]= "default";		
    m_g4ToSoMap["Bor 10"]= "default";		
    m_g4ToSoMap["Bor 11"]= "default";		
    m_g4ToSoMap["Borated Polyethyl"]= "default";		
    m_g4ToSoMap["Boron"]= "default";		
    m_g4ToSoMap["Boron-Barite"]= "default";		
    m_g4ToSoMap["Boron_frits-Lumnite"]= "default";		
    m_g4ToSoMap["Bromine"]= "default";		
    m_g4ToSoMap["C F_4"]= "default";		
    m_g4ToSoMap["C O_2"]= "default";		
    m_g4ToSoMap["C6F14"]= "default";		
    m_g4ToSoMap["CF_4 CO_2 50:50"]= "default";		
    m_g4ToSoMap["CO2Ar Freon"]= "defaultGas";		
    m_g4ToSoMap["CSC Electronics"]= "default";		
    m_g4ToSoMap["CT Al cable"]= "aluminium";		
    m_g4ToSoMap["CT Cu cable"]= "copper";		
    m_g4ToSoMap["C_New Air"]= "air";		
    m_g4ToSoMap["Ca C O_3"]= "default";		
    m_g4ToSoMap["Ca O"]= "default";		
    m_g4ToSoMap["Cable_0"]= "cable";		
    m_g4ToSoMap["Cable_MSGC_2"]= "cable";		
    m_g4ToSoMap["Cable_MSGC_3"]= "cable";		
    m_g4ToSoMap["Cable_Si_1"]= "cable";		
    m_g4ToSoMap["Cables"]= "cable";		
    m_g4ToSoMap["Cadmium"]= "default";		
    m_g4ToSoMap["Calcium"]= "default";		
    m_g4ToSoMap["Carbon"]= "carbon";		
    m_g4ToSoMap["Carbon fib.str."]= "default";		
    m_g4ToSoMap["Carbon fibre st_b"]= "default";		
    m_g4ToSoMap["Carbon fibre str."]= "default";		
    m_g4ToSoMap["Ce F_3"]= "default";		
    m_g4ToSoMap["Ceramic"]= "default";		
    m_g4ToSoMap["Cerium"]= "default";		
    m_g4ToSoMap["Cesium"]= "default";		
    m_g4ToSoMap["Chlorine"]= "default";		
    m_g4ToSoMap["Chromium"]= "default";		
    m_g4ToSoMap["Cobalt"]= "default";		
    m_g4ToSoMap["Coil average"]= "aluminium";		
    m_g4ToSoMap["Colmanite Barite"]= "default";		
    m_g4ToSoMap["Connector"]= "aluminium";		
    m_g4ToSoMap["Coolant"]= "water";		
    m_g4ToSoMap["Copper"]= "copper";		
    m_g4ToSoMap["Crack average"]= "default";		
    m_g4ToSoMap["Cu/Quartz"]= "copper";		
    m_g4ToSoMap["Cu/Sci spaghetti mix"]= "copper";		
    m_g4ToSoMap["Cu/Scintillator/PolB"]= "copper";		
    m_g4ToSoMap["DTBX gas"]= "air";		
    m_g4ToSoMap["Dead_Argon CF_4 CO_2"]= "default";		
    m_g4ToSoMap["Deuterium"]= "default";		
    m_g4ToSoMap["Doped Quartz"]= "default";		
    m_g4ToSoMap["Double-sided MSGC el"]= "default";		
    m_g4ToSoMap["Double-sided MSGCsub"]= "default";		
    m_g4ToSoMap["Double-sided Si elec"]= "default";		
    m_g4ToSoMap["DropsPolyethylene"]= "default";		
    m_g4ToSoMap["EAPD_Silicon"]= "default";		
    m_g4ToSoMap["E_Air"]= "air";		
    m_g4ToSoMap["E_Aluminium"]= "aluminium";		
    m_g4ToSoMap["E_CablPP1"]= "cable";		
    m_g4ToSoMap["E_CablPP2"]= "cable";		
    m_g4ToSoMap["E_CablPP3"]= "cable";		
    m_g4ToSoMap["E_CablPP4"]= "cable";		
    m_g4ToSoMap["E_Cables"]= "cable";		
    m_g4ToSoMap["E_Carbon Fibre"]= "default";		
    m_g4ToSoMap["E_Copper"]= "copper";		
    m_g4ToSoMap["E_Epoxy"]= "default";		
    m_g4ToSoMap["E_G10"]= "default";		
    m_g4ToSoMap["E_Iron"]= "iron";		
    m_g4ToSoMap["E_Lead"]= "lead";		
    m_g4ToSoMap["E_PbWO4"]= "lead";		
    m_g4ToSoMap["E_Polythene"]= "default";		
    m_g4ToSoMap["E_Rohacell"]= "default";		
    m_g4ToSoMap["E_Silicon"]= "silicon";		
    m_g4ToSoMap["E_Water"]= "water";		
    m_g4ToSoMap["Ec_Cable_1"]= "cable";		
    m_g4ToSoMap["Epoxy"]= "default";		
    m_g4ToSoMap["Ethane"]= "default";		
    m_g4ToSoMap["Fe_2 O_3"]= "iron";		
    m_g4ToSoMap["Fibre_Ribbon"]= "default";		
    m_g4ToSoMap["Fibre_connector"]= "default";		
    m_g4ToSoMap["Fluorine"]= "default";		
    m_g4ToSoMap["Flushing gas"]= "default";		
    m_g4ToSoMap["Foam"]= "default";		
    m_g4ToSoMap["Freon-12"]= "default";		
    m_g4ToSoMap["FrontEnd Electronics"]= "default";		
    m_g4ToSoMap["G10"]= "default";		
    m_g4ToSoMap["G_conntr"]= "default";		
    m_g4ToSoMap["Gallium"]= "default";		
    m_g4ToSoMap["Gas"]= "air";		
    m_g4ToSoMap["Germanium"]= "default";		
    m_g4ToSoMap["Glass"]= "default";		
    m_g4ToSoMap["Gold"]= "gold";		
    m_g4ToSoMap["Graph.Epoxy Sup."]= "default";		
    m_g4ToSoMap["Graphite Epoxy suprt"]= "default";		
    m_g4ToSoMap["HFE"]= "default";		
    m_g4ToSoMap["HV Light Guides"]= "default";		
    m_g4ToSoMap["HV_Cu/QFib_mx."]= "default";		
    m_g4ToSoMap["H_Air"]= "air";		
    m_g4ToSoMap["H_Aluminium"]= "aluminium";		
    m_g4ToSoMap["H_Brass"]= "brass";		
    m_g4ToSoMap["H_Iron"]= "iron";		
    m_g4ToSoMap["H_Polystyrene"]= "default";		
    m_g4ToSoMap["H_Scintillator"]= "default";		
    m_g4ToSoMap["Hcal average"]= "default";		
    m_g4ToSoMap["Hcal sci"]= "default";		
    m_g4ToSoMap["Helium"]= "air";		
    m_g4ToSoMap["Hexel for CSC"]= "default";		
    m_g4ToSoMap["High Tension cables"]= "cable";		
    m_g4ToSoMap["Honeycomb"]= "default";		
    m_g4ToSoMap["Hybrids"]= "default";		
    m_g4ToSoMap["Hydrogen"]= "default";		
    m_g4ToSoMap["ICB"]= "default";		
    m_g4ToSoMap["Indium"]= "default";		
    m_g4ToSoMap["Insulation"]= "default";		
    m_g4ToSoMap["Iodine"]= "default";		
    m_g4ToSoMap["Iron"]= "iron";		
    m_g4ToSoMap["Isobutane"]= "default";		
    m_g4ToSoMap["K_2 O"]= "default";		
    m_g4ToSoMap["Kapton"]= "default";		
    m_g4ToSoMap["Kr/Cu mixture for HD"]= "copper";		
    m_g4ToSoMap["Kr/Pb mixture for EM"]= "lead";		
    m_g4ToSoMap["Kr/Pb mixture for HD"]= "lead";		
    m_g4ToSoMap["Krypton"]= "default";		
    m_g4ToSoMap["Lanthanum"]= "default";		
    m_g4ToSoMap["Lead"]= "lead";		
    m_g4ToSoMap["LeadBPolymer"]= "lead";		
    m_g4ToSoMap["LeadLoadedPolymerCon"]= "lead";		
    m_g4ToSoMap["Limonite"]= "default";		
    m_g4ToSoMap["Limonite Iron"]= "default";		
    m_g4ToSoMap["Limonite magetite"]= "default";		
    m_g4ToSoMap["Liquid Ar Detector"]= "default";		
    m_g4ToSoMap["Liquid Argon"]= "default";		
    m_g4ToSoMap["Liquid Kr Detector"]= "default";		
    m_g4ToSoMap["Liquid Krypton"]= "default";		
    m_g4ToSoMap["Lithium"]= "default";		
    m_g4ToSoMap["Lithium Polyethyl."]= "default";		
    m_g4ToSoMap["Low Tension cables"]= "default";		
    m_g4ToSoMap["Lucite"]= "default";		
    m_g4ToSoMap["ME_free_space"]= "default";		
    m_g4ToSoMap["MSGC cooling pipe"]= "default";		
    m_g4ToSoMap["MSGC-Average"]= "default";		
    m_g4ToSoMap["MS_Al36"]= "aluminium";		
    m_g4ToSoMap["MS_Al48"]= "aluminium";		
    m_g4ToSoMap["MS_Al60"]= "aluminium";		
    m_g4ToSoMap["MS_Al_cable"]= "aluminium";		
    m_g4ToSoMap["MS_Cu60"]= "copper";		
    m_g4ToSoMap["MS_Cu_cable"]= "copper";		
    m_g4ToSoMap["MS_cntr"]= "default";		
    m_g4ToSoMap["MS_cntrl_cable"]= "default";		
    m_g4ToSoMap["M_Aluminium"]= "aluminium";		
    m_g4ToSoMap["M_Argon 50% CF_4 CO_2"]= "default";		
    m_g4ToSoMap["M_Argon CF_4 CO_2"]= "default";		
    m_g4ToSoMap["M_B_Air"]= "air";		
    m_g4ToSoMap["M_Cables"]= "cable";		
    m_g4ToSoMap["M_Copper"]= "copper";		
    m_g4ToSoMap["M_DTBX Gas"]= "default";		
    m_g4ToSoMap["M_Electronics averag"]= "default";		
    m_g4ToSoMap["M_F_Air"]= "air";		
    m_g4ToSoMap["M_NEMA FR4 plate"]= "default";		
    m_g4ToSoMap["M_NEMA G10 plate"]= "default";		
    m_g4ToSoMap["M_RPC_Bakelite"]= "default";		
    m_g4ToSoMap["M_RPC_Gas"]= "default";		
    m_g4ToSoMap["M_Steel-008"]= "iron";		
    m_g4ToSoMap["M_Thick_Steel-008"]= "iron";		
    m_g4ToSoMap["M_YokeSteel"]= "iron";		
    m_g4ToSoMap["M_honeycomb"]= "default";		
    m_g4ToSoMap["Magetite"]= "default";		
    m_g4ToSoMap["Magnesium"]= "default";		
    m_g4ToSoMap["Magnet Conductor"]= "default";		
    m_g4ToSoMap["MagnetiteBoron"]= "default";		
    m_g4ToSoMap["MagnetiteConc"]= "default";		
    m_g4ToSoMap["Manganese"]= "default";		
    m_g4ToSoMap["Marble"]= "default";		
    m_g4ToSoMap["Methane"]= "default";		
    m_g4ToSoMap["Mg O"]= "default";		
    m_g4ToSoMap["Mg-MMC"]= "default";		
    m_g4ToSoMap["Mn O"]= "default";		
    m_g4ToSoMap["Molybdenum"]= "default";		
    m_g4ToSoMap["Muon Al"]= "aluminium";		
    m_g4ToSoMap["Muon average"]= "default";		
    m_g4ToSoMap["Mylar"]= "default";		
    m_g4ToSoMap["NEMA FR4 plate"]= "default";		
    m_g4ToSoMap["NEMA G10 plate"]= "default";		
    m_g4ToSoMap["Na_2 O"]= "default";		
    m_g4ToSoMap["Ne30_DME70"]= "default";		
    m_g4ToSoMap["Neodymium"]= "default";		
    m_g4ToSoMap["Neon"]= "default";		
    m_g4ToSoMap["Ni_2 O_3"]= "iron";		
    m_g4ToSoMap["Nickel"]= "iron";		
    m_g4ToSoMap["Niobium"]= "default";		
    m_g4ToSoMap["Nitrogen"]= "default";		
    m_g4ToSoMap["Nomex"]= "default";		
    m_g4ToSoMap["Nomex for CSC"]= "default";		
    m_g4ToSoMap["Noryl"]= "default";		
    m_g4ToSoMap["O_Hybrid"]= "air";		
    m_g4ToSoMap["Optical fibre"]= "default";		
    m_g4ToSoMap["Outer_pipes"]= "default";		
    m_g4ToSoMap["Oxygen"]= "air";		
    m_g4ToSoMap["P_pipes"]= "aluminium";		
    m_g4ToSoMap["Palladium"]= "default";		
    m_g4ToSoMap["Pb W O_4"]= "lead";		
    m_g4ToSoMap["Pb/Sci spaghetti mix"]= "lead";		
    m_g4ToSoMap["Peek"]= "default";		
    m_g4ToSoMap["Phosphor"]= "default";		
    m_g4ToSoMap["PhotoCathode"]= "default";		
    m_g4ToSoMap["Pigtails"]= "default";		
    m_g4ToSoMap["Pipe with Water"]= "water";		
    m_g4ToSoMap["Pipe with gas"]= "air";		
    m_g4ToSoMap["Pix_Bar_Baseplate"]= "default";		
    m_g4ToSoMap["Pix_Bar_Cable"]= "cable";		
    m_g4ToSoMap["Pix_Bar_Cable1"]= "cable";		
    m_g4ToSoMap["Pix_Bar_Cable2"]= "cable";		
    m_g4ToSoMap["Pix_Bar_Cable3"]= "cable";		
    m_g4ToSoMap["Pix_Bar_Conn1"]= "aluminium";		
    m_g4ToSoMap["Pix_Bar_Conn2"]= "aluminium";		
    m_g4ToSoMap["Pix_Bar_Conn3"]= "aluminium";		
    m_g4ToSoMap["Pix_Bar_Cool"]= "default";		
    m_g4ToSoMap["Pix_Bar_Ring_CF"]= "default";		
    m_g4ToSoMap["Pix_Bar_Ring_HC"]= "default";		
    m_g4ToSoMap["Pix_Bar_Services"]= "default";		
    m_g4ToSoMap["Pix_Bar_Water"]= "water";		
    m_g4ToSoMap["Pix_Fwd_Base"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Blade"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Bypass"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Cool"]= "default";		
    m_g4ToSoMap["Pix_Fwd_HDI"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Iring"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Oring"]= "default";		
    m_g4ToSoMap["Pix_Fwd_PortCard"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Ring"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Services_1"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Services_2"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Services_3"]= "default";		
    m_g4ToSoMap["Pix_Fwd_Strut"]= "default";		
    m_g4ToSoMap["Pix_Fwd_VHDI"]= "default";		
    m_g4ToSoMap["Plexiglas"]= "default";		
    m_g4ToSoMap["Polycarbonate"]= "default";		
    m_g4ToSoMap["Polyethylene"]= "default";		
    m_g4ToSoMap["Polymer Concrete"]= "default";		
    m_g4ToSoMap["Polystyrene"]= "default";		
    m_g4ToSoMap["Polyvinylchloride"]= "default";		
    m_g4ToSoMap["Potassium"]= "default";		
    m_g4ToSoMap["Praseodymium"]= "default";		
    m_g4ToSoMap["Px_cool_pipe"]= "default";		
    m_g4ToSoMap["Quartz"]= "default";		
    m_g4ToSoMap["Quartz support"]= "default";		
    m_g4ToSoMap["QuartzBundle"]= "default";		
    m_g4ToSoMap["QuartzFibers"]= "default";		
    m_g4ToSoMap["RPC Gas"]= "default";		
    m_g4ToSoMap["Rhodium"]= "default";		
    m_g4ToSoMap["Rohacell"]= "default";		
    m_g4ToSoMap["Rubidium"]= "default";		
    m_g4ToSoMap["Ruthenium"]= "default";		
    m_g4ToSoMap["SITRA-Average"]= "default";		
    m_g4ToSoMap["SMD_metal"]= "default";		
    m_g4ToSoMap["S_2 O_3"]= "default";		
    m_g4ToSoMap["Scandium"]= "default";		
    m_g4ToSoMap["Scintillator"]= "default";		
    m_g4ToSoMap["Selenium"]= "default";		
    m_g4ToSoMap["Serpentine - Iron"]= "iron";		
    m_g4ToSoMap["Serpentine 2"]= "iron";		
    m_g4ToSoMap["Si O_2"]= "silicon";		
    m_g4ToSoMap["Si cooling pipe"]= "aluminium";		
    m_g4ToSoMap["Silica"]= "silicon";		
    m_g4ToSoMap["Silicon"]= "silicon";		
    m_g4ToSoMap["Silicon Detector"]= "silicon";		
    m_g4ToSoMap["Siliecal"]= "default";		
    m_g4ToSoMap["Silver"]= "iron";		
    m_g4ToSoMap["Single-sided MSGC el"]= "default";		
    m_g4ToSoMap["Single-sided MSGCsub"]= "default";		
    m_g4ToSoMap["Single-sided Si elec"]= "silicon";		
    m_g4ToSoMap["Sodium"]= "default";		
    m_g4ToSoMap["StainlessSteel"]= "iron";		
    m_g4ToSoMap["Stand.Concrete"]= "default";		
    m_g4ToSoMap["Standard Serpentine"]= "aluminium";		
    m_g4ToSoMap["Steel-008"]= "iron";		
    m_g4ToSoMap["Steel-light"]= "iron";		
    m_g4ToSoMap["Strontium"]= "default";		
    m_g4ToSoMap["Sulfur"]= "default";		
    m_g4ToSoMap["Super Conductor"]= "default";		
    m_g4ToSoMap["TEC_Cool"]= "default";		
    m_g4ToSoMap["TEC_FixServ"]= "default";		
    m_g4ToSoMap["TEC_Fixframe"]= "default";		
    m_g4ToSoMap["TEC_ICC11"]= "default";		
    m_g4ToSoMap["TEC_ICC12"]= "default";		
    m_g4ToSoMap["TEC_ICC13"]= "default";		
    m_g4ToSoMap["TEC_ICC14"]= "default";		
    m_g4ToSoMap["TEC_ICC15"]= "default";		
    m_g4ToSoMap["TEC_ICC16"]= "default";		
    m_g4ToSoMap["TEC_ICC17"]= "default";		
    m_g4ToSoMap["TEC_ICC1B1"]= "default";		
    m_g4ToSoMap["TEC_ICC1B2"]= "default";		
    m_g4ToSoMap["TEC_ICC1B3"]= "default";		
    m_g4ToSoMap["TEC_ICC1B4"]= "default";		
    m_g4ToSoMap["TEC_ICC21"]= "default";		
    m_g4ToSoMap["TEC_ICC22"]= "default";		
    m_g4ToSoMap["TEC_ICC23"]= "default";		
    m_g4ToSoMap["TEC_ICC24"]= "default";		
    m_g4ToSoMap["TEC_ICC25"]= "default";		
    m_g4ToSoMap["TEC_ICC26"]= "default";		
    m_g4ToSoMap["TEC_ICC27"]= "default";		
    m_g4ToSoMap["TEC_ICC2B1"]= "default";		
    m_g4ToSoMap["TEC_ICC2B2"]= "default";		
    m_g4ToSoMap["TEC_ICC2B3"]= "default";		
    m_g4ToSoMap["TEC_ICC2B4"]= "default";		
    m_g4ToSoMap["TEC_Nomex"]= "default";		
    m_g4ToSoMap["TEC_OptoCon"]= "default";		
    m_g4ToSoMap["TEC_OptoH"]= "default";		
    m_g4ToSoMap["TEC_PetalCon"]= "default";		
    m_g4ToSoMap["TEC_PitchAdapter"]= "default";		
    m_g4ToSoMap["TEC_Side_Wafer"]= "default";		
    m_g4ToSoMap["TEC_Wafer"]= "default";		
    m_g4ToSoMap["TEC_ax_cable"]= "default";		
    m_g4ToSoMap["TEC_ele1"]= "default";		
    m_g4ToSoMap["TEC_ele2"]= "default";		
    m_g4ToSoMap["TEC_ele3"]= "default";		
    m_g4ToSoMap["TEC_ele4"]= "default";		
    m_g4ToSoMap["TEC_ele5"]= "default";		
    m_g4ToSoMap["TEC_ele6"]= "default";		
    m_g4ToSoMap["TEC_ele7"]= "default";		
    m_g4ToSoMap["TEC_frame_side"]= "default";		
    m_g4ToSoMap["TEC_frame_top"]= "default";		
    m_g4ToSoMap["TEC_periscopes"]= "default";		
    m_g4ToSoMap["TEC_petal"]= "default";		
    m_g4ToSoMap["TEC_rad_cable"]= "default";		
    m_g4ToSoMap["TEC_rods"]= "default";		
    m_g4ToSoMap["TEC_tiltmeter"]= "default";		
    m_g4ToSoMap["TEC_wheel_CF"]= "default";		
    m_g4ToSoMap["TEC_wheel_Nomex"]= "default";		
    m_g4ToSoMap["TEC_wheelinsert"]= "default";		
    m_g4ToSoMap["TIB_CCUM"]= "default";		
    m_g4ToSoMap["TIB_CF"]= "default";		
    m_g4ToSoMap["TIB_ICC1"]= "default";		
    m_g4ToSoMap["TIB_ICC2"]= "default";		
    m_g4ToSoMap["TIB_ICC_ledge1"]= "default";		
    m_g4ToSoMap["TIB_ICC_ledge2"]= "default";		
    m_g4ToSoMap["TIB_PA_rphi"]= "default";		
    m_g4ToSoMap["TIB_PA_ster"]= "default";		
    m_g4ToSoMap["TIB_Rib_in1"]= "default";		
    m_g4ToSoMap["TIB_Rib_in2"]= "default";		
    m_g4ToSoMap["TIB_Rib_in3"]= "default";		
    m_g4ToSoMap["TIB_Rib_in4"]= "default";		
    m_g4ToSoMap["TIB_Silicon"]= "silicon";		
    m_g4ToSoMap["TIB_Wafer"]= "default";		
    m_g4ToSoMap["TIB_cab_rad1"]= "cable";		
    m_g4ToSoMap["TIB_cab_rad2"]= "cable";		
    m_g4ToSoMap["TIB_cab_rad3"]= "cable";		
    m_g4ToSoMap["TIB_cab_rad4"]= "cable";		
    m_g4ToSoMap["TIB_cables_ax"]= "cable";		
    m_g4ToSoMap["TIB_cables_ax_out"]= "cable";		
    m_g4ToSoMap["TIB_cent_supp"]= "default";		
    m_g4ToSoMap["TIB_connector"]= "aluminium";		
    m_g4ToSoMap["TIB_cylinder"]= "default";		
    m_g4ToSoMap["TIB_disk"]= "default";		
    m_g4ToSoMap["TIB_ele1"]= "default";		
    m_g4ToSoMap["TIB_ele2"]= "default";		
    m_g4ToSoMap["TIB_ele3"]= "default";		
    m_g4ToSoMap["TIB_ele4"]= "default";		
    m_g4ToSoMap["TIB_ledge_bds"]= "default";		
    m_g4ToSoMap["TIB_ledge_bss"]= "default";		
    m_g4ToSoMap["TIB_ledge_hybrid"]= "default";		
    m_g4ToSoMap["TIB_ledge_side"]= "default";		
    m_g4ToSoMap["TIB_mod_cool"]= "default";		
    m_g4ToSoMap["TIB_mod_spacer1"]= "default";		
    m_g4ToSoMap["TIB_mod_spacer2"]= "default";		
    m_g4ToSoMap["TIB_mother_cable"]= "default";		
    m_g4ToSoMap["TIB_outer_supp"]= "default";		
    m_g4ToSoMap["TIB_rail"]= "default";		
    m_g4ToSoMap["TIB_rib0"]= "default";		
    m_g4ToSoMap["TIB_rib1"]= "default";		
    m_g4ToSoMap["TIB_rib2"]= "default";		
    m_g4ToSoMap["TIB_rib3"]= "default";		
    m_g4ToSoMap["TIB_rib4"]= "default";		
    m_g4ToSoMap["TIB_sid_rail1"]= "default";		
    m_g4ToSoMap["TIB_sid_rail2"]= "default";		
    m_g4ToSoMap["TID_CF"]= "default";		
    m_g4ToSoMap["TID_Connector"]= "aluminium";		
    m_g4ToSoMap["TID_Cool_Manifold"]= "default";		
    m_g4ToSoMap["TID_Cool_insert"]= "default";		
    m_g4ToSoMap["TID_Cool_ring"]= "default";		
    m_g4ToSoMap["TID_ICB1"]= "default";		
    m_g4ToSoMap["TID_ICB2"]= "default";		
    m_g4ToSoMap["TID_ICB3"]= "default";		
    m_g4ToSoMap["TID_ICC1"]= "default";		
    m_g4ToSoMap["TID_ICC2"]= "default";		
    m_g4ToSoMap["TID_Nomex"]= "default";		
    m_g4ToSoMap["TID_PitchAdapter"]= "default";		
    m_g4ToSoMap["TID_Side_Wafer"]= "default";		
    m_g4ToSoMap["TID_Wafer"]= "default";		
    m_g4ToSoMap["TID_Wheel_Fixation"]= "default";		
    m_g4ToSoMap["TID_ax_cable"]= "cable";		
    m_g4ToSoMap["TID_ele1"]= "default";		
    m_g4ToSoMap["TID_ele2"]= "default";		
    m_g4ToSoMap["TID_ele3"]= "default";		
    m_g4ToSoMap["TID_frame_side"]= "default";		
    m_g4ToSoMap["TID_frame_top"]= "default";		
    m_g4ToSoMap["TID_in_cable"]= "cable";		
    m_g4ToSoMap["TID_mech"]= "default";		
    m_g4ToSoMap["TIE_Connector"]= "aluminium";		
    m_g4ToSoMap["TIE_Fixframe"]= "default";		
    m_g4ToSoMap["TIE_ICC1"]= "default";		
    m_g4ToSoMap["TIE_ICC2"]= "default";		
    m_g4ToSoMap["TIE_ICC3"]= "default";		
    m_g4ToSoMap["TIE_PitchAdapter"]= "default";		
    m_g4ToSoMap["TIE_Side_Wafer"]= "default";		
    m_g4ToSoMap["TIE_Wafer"]= "default";		
    m_g4ToSoMap["TIE_ax_cable"]= "cable";		
    m_g4ToSoMap["TIE_ele1"]= "default";		
    m_g4ToSoMap["TIE_ele2"]= "default";		
    m_g4ToSoMap["TIE_ele3"]= "default";		
    m_g4ToSoMap["TIE_frame_light"]= "default";		
    m_g4ToSoMap["TIE_frame_light_back"]= "default";		
    m_g4ToSoMap["TIE_in_cable"]= "default";		
    m_g4ToSoMap["TIE_mech"]= "default";		
    m_g4ToSoMap["TIE_petal"]= "default";		
    m_g4ToSoMap["TIE_rad_cable"]= "default";		
    m_g4ToSoMap["TOB_Aluminium"]= "aluminium";		
    m_g4ToSoMap["TOB_Brass"]= "brass";		
    m_g4ToSoMap["TOB_CCUM1"]= "default";		
    m_g4ToSoMap["TOB_CCUM2"]= "default";		
    m_g4ToSoMap["TOB_CCUM3"]= "default";		
    m_g4ToSoMap["TOB_CF_Str"]= "default";		
    m_g4ToSoMap["TOB_CONN1"]= "default";		
    m_g4ToSoMap["TOB_CONN2"]= "default";		
    m_g4ToSoMap["TOB_CONN3"]= "default";		
    m_g4ToSoMap["TOB_CuNi"]= "copper";		
    m_g4ToSoMap["TOB_Epoxy"]= "default";		
    m_g4ToSoMap["TOB_ICB"]= "default";		
    m_g4ToSoMap["TOB_ICC1"]= "default";		
    m_g4ToSoMap["TOB_ICC2"]= "default";		
    m_g4ToSoMap["TOB_ICC3"]= "default";		
    m_g4ToSoMap["TOB_ICC4"]= "default";		
    m_g4ToSoMap["TOB_ICC5"]= "default";		
    m_g4ToSoMap["TOB_ICC6"]= "default";		
    m_g4ToSoMap["TOB_Nomex"]= "default";		
    m_g4ToSoMap["TOB_PA_rphi"]= "default";		
    m_g4ToSoMap["TOB_PA_ster"]= "default";		
    m_g4ToSoMap["TOB_Silicon"]= "silicon";		
    m_g4ToSoMap["TOB_Wafer"]= "default";		
    m_g4ToSoMap["TOB_ax_cable"]= "cable";		
    m_g4ToSoMap["TOB_cool_DS"]= "default";		
    m_g4ToSoMap["TOB_cool_SS"]= "default";		
    m_g4ToSoMap["TOB_cool_solder"]= "default";		
    m_g4ToSoMap["TOB_coolmanifold1"]= "default";		
    m_g4ToSoMap["TOB_coolmanifold2"]= "default";		
    m_g4ToSoMap["TOB_coolmanifold3"]= "default";		
    m_g4ToSoMap["TOB_coolmanifold4"]= "default";		
    m_g4ToSoMap["TOB_coolmanifold5"]= "default";		
    m_g4ToSoMap["TOB_coolmanifold6"]= "default";		
    m_g4ToSoMap["TOB_ele10"]= "default";		
    m_g4ToSoMap["TOB_ele5"]= "default";		
    m_g4ToSoMap["TOB_ele6"]= "default";		
    m_g4ToSoMap["TOB_ele7"]= "default";		
    m_g4ToSoMap["TOB_ele8"]= "default";		
    m_g4ToSoMap["TOB_ele9"]= "default";		
    m_g4ToSoMap["TOB_frame_ele"]= "default";		
    m_g4ToSoMap["TOB_hybrid_supp"]= "default";		
    m_g4ToSoMap["TOB_middle_ribs"]= "default";		
    m_g4ToSoMap["TOB_mod_cool_1"]= "default";		
    m_g4ToSoMap["TOB_mod_cool_2"]= "default";		
    m_g4ToSoMap["TOB_mod_cool_3"]= "default";		
    m_g4ToSoMap["TOB_mod_cool_4"]= "default";		
    m_g4ToSoMap["TOB_optfib_DS"]= "default";		
    m_g4ToSoMap["TOB_optfib_SS"]= "default";		
    m_g4ToSoMap["TOB_plate_A"]= "default";		
    m_g4ToSoMap["TOB_plate_B"]= "default";		
    m_g4ToSoMap["TOB_plate_C"]= "default";		
    m_g4ToSoMap["TOB_rad_cable1"]= "cable";		
    m_g4ToSoMap["TOB_rad_cable2"]= "cable";		
    m_g4ToSoMap["TOB_rad_cable3"]= "cable";		
    m_g4ToSoMap["TOB_rad_cable4"]= "cable";		
    m_g4ToSoMap["TOB_rad_cable5"]= "cable";		
    m_g4ToSoMap["TOB_rad_cable6"]= "cable";		
    m_g4ToSoMap["TOB_rail"]= "iron";		
    m_g4ToSoMap["TOB_rib1"]= "default";		
    m_g4ToSoMap["TOB_rib2"]= "default";		
    m_g4ToSoMap["TOB_rib3"]= "default";		
    m_g4ToSoMap["TOB_rib4"]= "default";		
    m_g4ToSoMap["TOB_rib5"]= "default";		
    m_g4ToSoMap["TOB_rib6"]= "default";		
    m_g4ToSoMap["TOB_rod"]= "default";		
    m_g4ToSoMap["TOB_sid_rail1"]= "iron";		
    m_g4ToSoMap["TOB_sid_rail2"]= "iron";		
    m_g4ToSoMap["TRAK_Average"]= "default";		
    m_g4ToSoMap["T_Air"]= "air";		
    m_g4ToSoMap["T_Argon CF_4 CO_2"]= "air";		
    m_g4ToSoMap["T_Bronze"]= "copper";		
    m_g4ToSoMap["T_Carbon fibre str."]= "default";		
    m_g4ToSoMap["T_FR4"]= "default";		
    m_g4ToSoMap["T_Foam"]= "default";		
    m_g4ToSoMap["T_Kapton"]= "default";		
    m_g4ToSoMap["T_MSGC-Average"]= "default";		
    m_g4ToSoMap["T_OPC"]= "default";		
    m_g4ToSoMap["T_OP_cable"]= "cable";		
    m_g4ToSoMap["T_PIXEL-Average"]= "default";		
    m_g4ToSoMap["T_Pix_Bar_Det"]= "default";		
    m_g4ToSoMap["T_Pix_Bar_Hybrid"]= "default";		
    m_g4ToSoMap["T_Pix_Bar_Readout"]= "default";		
    m_g4ToSoMap["T_Pix_Fwd_Det"]= "default";		
    m_g4ToSoMap["T_Pix_Fwd_Readout"]= "default";		
    m_g4ToSoMap["T_SITRA-Average"]= "default";		
    m_g4ToSoMap["T_Silicon"]= "silicon";		
    m_g4ToSoMap["T_Silicon Detector"]= "silicon";		
    m_g4ToSoMap["Technetium"]= "default";		
    m_g4ToSoMap["Teflon"]= "default";		
    m_g4ToSoMap["Tellurium"]= "default";		
    m_g4ToSoMap["Thick_Copper"]= "copper";		
    m_g4ToSoMap["Thick_MagnetiteConc"]= "concrete";		
    m_g4ToSoMap["Thick_Stand.Concrete"]= "concrete";		
    m_g4ToSoMap["Ti_2 O_3"]= "aluminium";		
    m_g4ToSoMap["Tin"]= "aluminium";		
    m_g4ToSoMap["Titanium"]= "aluminium";		
    m_g4ToSoMap["Tk_CCU_board"]= "default";		
    m_g4ToSoMap["Tk_CF_supp"]= "default";		
    m_g4ToSoMap["Tk_CF_supp_sili"]= "default";		
    m_g4ToSoMap["Tk_Cable_1"]= "cable";		
    m_g4ToSoMap["Tk_IO_board"]= "default";		
    m_g4ToSoMap["Tk_Opt_Ribbon"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_A"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_B"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_C"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_D"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_E"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_F"]= "default";		
    m_g4ToSoMap["Tk_SO_FC_G"]= "default";		
    m_g4ToSoMap["Tk_cool_pipe"]= "aluminium";		
    m_g4ToSoMap["Tk_op_boxes"]= "default";		
    m_g4ToSoMap["Tk_panels_in"]= "default";		
    m_g4ToSoMap["Tk_panels_mid1"]= "default";		
    m_g4ToSoMap["Tk_panels_mid2"]= "default";		
    m_g4ToSoMap["Tk_panels_mid3"]= "default";		
    m_g4ToSoMap["Tk_panels_out"]= "default";		
    m_g4ToSoMap["Tk_panels_up"]= "default";		
    m_g4ToSoMap["Tk_square_bundles"]= "default";		
    m_g4ToSoMap["Tk_support"]= "default";		
    m_g4ToSoMap["Transmission line"]= "default";		
    m_g4ToSoMap["Tungsten"]= "default";		
    m_g4ToSoMap["Uranium"]= "default";		
    m_g4ToSoMap["V_Air"]= "air";		
    m_g4ToSoMap["V_Iron"]= "iron";		
    m_g4ToSoMap["V_Quartz"]= "glass";		
    m_g4ToSoMap["Vacuum"]= "vacuum";		
    m_g4ToSoMap["Vanadium"]= "default";		
    m_g4ToSoMap["Vcal C4H10"]= "default";		
    m_g4ToSoMap["Vcal CO2"]= "air";		
    m_g4ToSoMap["Vcal average"]= "default";		
    m_g4ToSoMap["W/Sci spaghetti mix"]= "default";		
    m_g4ToSoMap["Water"]= "water";		
    m_g4ToSoMap["Wood"]= "wood";		
    m_g4ToSoMap["Xenon"]= "defaultGas";		
    m_g4ToSoMap["YokeSteel"]= "iron";		
    m_g4ToSoMap["Yttrium"]= "default";		
    m_g4ToSoMap["Zinc"]= "default";		
    m_g4ToSoMap["Zirconium"]= "default";		
    m_g4ToSoMap["active_screen"]= "default";		
    m_g4ToSoMap["c_Peek"]= "default";			       	
}    

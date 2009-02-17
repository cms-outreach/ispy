#ifndef IG_OPEN_INVENTOR_IG_SO_LEGO_PLOT_H
# define IG_OPEN_INVENTOR_IG_SO_LEGO_PLOT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFInt32.h>
# include <Inventor/fields/SoSFBool.h>
# include <map>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** An (almost private) helper class for IgSoLegoPlot. Stores
    two energies so it is really only a struct with an operator.  */
class IgSbLegoValue
{
public:
    IgSbLegoValue (const float emEnergy, const float Energy) : emE (emEnergy), E (Energy) {}
    IgSbLegoValue &operator+= (const IgSbLegoValue &x) { emE += x.emE; E += x.E; return *this; }

    float emE;
    float E;
};

/** An (almost private) helper class for constructing a legoHist
    stores the x/y indices.  */
class IgSbLegoBin
{
public:
    IgSbLegoBin (const int i, const int j) : ix (i), iy (j) {}
    int ix;
    int iy;
};

struct IgSbLegoBinCompare
{
    bool operator() (const IgSbLegoBin &lb1, const IgSbLegoBin &lb2) const 
	{
	    return lb1.ix < lb2.ix || (lb1.ix == lb2.ix && lb1.iy < lb2.iy);
	}
};

typedef std::map<IgSbLegoBin, IgSbLegoValue, IgSbLegoBinCompare> IgSbLegoHistogram;

/*!	\class IgSoLegoPlot IgSoLegoPlot.h Iguana/Inventor/interface/IgSoLegoPlot.h
 *	\brief Provides a basic lego plot in eta-phi for calorimetry
 */

class IG_OPEN_INVENTOR_API IgSoLegoPlot : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoLegoPlot);
    SO_KIT_CATALOG_ENTRY_HEADER (phiLabels);
    SO_KIT_CATALOG_ENTRY_HEADER (etaLabels);
    SO_KIT_CATALOG_ENTRY_HEADER (grid);
    SO_KIT_CATALOG_ENTRY_HEADER (font);
    SO_KIT_CATALOG_ENTRY_HEADER (labels);
    SO_KIT_CATALOG_ENTRY_HEADER (labelScale);
    SO_KIT_CATALOG_ENTRY_HEADER (labelScaleFont);
    SO_KIT_CATALOG_ENTRY_HEADER (labelScaleOffset);
    SO_KIT_CATALOG_ENTRY_HEADER (labelScaleText);
    SO_KIT_CATALOG_ENTRY_HEADER (labelX);
    SO_KIT_CATALOG_ENTRY_HEADER (labelXOffset);
    SO_KIT_CATALOG_ENTRY_HEADER (labelXText);
    SO_KIT_CATALOG_ENTRY_HEADER (labelZ);
    SO_KIT_CATALOG_ENTRY_HEADER (labelZOffset);
    SO_KIT_CATALOG_ENTRY_HEADER (labelZText);
    SO_KIT_CATALOG_ENTRY_HEADER (legos);

public:
    IgSoLegoPlot (void);
    static void initClass (void);

    SoSFInt32	divsPhi;
    SoSFInt32	divsEta;
    SoSFFloat	heightScale;
    SoSFFloat	maxEta;
    SoSFFloat	minEnergy;		//< sets the minimum enery to be displayed
    SoSFBool	displayNegTowers;	//< switch ON/OFF the display of negative towers

    void setData (const IgSbLegoHistogram &histo);

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_LEGO_PLOT_H

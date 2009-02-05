#ifndef IG_3D_BASE_WINDOW_IG_3D_COMMON_VIEW_PROPERTIES_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_3D_COMMON_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <vector>
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgCommonViewCategory;
class Ig3DBaseBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgCommonViewPropertiesCategory
    : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgCommonViewPropertiesCategory);    

public:
    IgCommonViewPropertiesCategory (IgState *state);
    ~IgCommonViewPropertiesCategory ();

    IgState* state (void) { return m_state; }
    IgCommonViewCategory* getCategory (unsigned int index) const;
    IgCommonViewCategory* getCategory (const QString& name) const;
    
    void		registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser);
    
private:
    std::vector <IgCommonViewCategory*> m_categories;
    IgState	*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_3D_COMMON_VIEW_PROPERTIES_CATEGORY_H

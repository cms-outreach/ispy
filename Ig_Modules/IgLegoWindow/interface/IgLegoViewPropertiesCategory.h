#ifndef IG_LEGO_WINDOW_IG_LEGO_VIEW_PROPERTIES_CATEGORY_H
# define IG_LEGO_WINDOW_IG_LEGO_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgLegoWindow/interface/config.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgLegoBrowser;
class IgQtAxisAlignment;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_LEGO_WINDOW_API IgLegoViewPropertiesCategory 
    : public IgControlCategory,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgLegoViewPropertiesCategory);
    
public:
    IgLegoViewPropertiesCategory (IgState *state,
				  IgLegoBrowser *source);
    ~IgLegoViewPropertiesCategory (void);

    virtual void	browserChanged  (IgLegoBrowser *source);
    virtual void	registerBrowser (IgState *state,
                                         IgLegoBrowser *source);

public slots:
    virtual void	alignAxis (void);

private:
    IgLegoBrowser	*m_source;

    IgQtAxisAlignment	*m_axisAlignment; // 

    // undefined semantics
    IgLegoViewPropertiesCategory (const IgLegoViewPropertiesCategory &);
    IgLegoViewPropertiesCategory &operator = (const IgLegoViewPropertiesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_LEGO_WINDOW_IG_LEGO_VIEW_PROPERTIES_CATEGORY_H

#ifndef IG_RZ_WINDOW_IG_RZ_VIEW_PROPERTIES_CATEGORY_H
# define IG_RZ_WINDOW_IG_RZ_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRZWindow/interface/config.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRZBrowser;
class IgQtAxisAlignment;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RZ_WINDOW_API IgRZViewPropertiesCategory 
    : public IgControlCategory,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgRZViewPropertiesCategory);
    
public:
    IgRZViewPropertiesCategory (IgState *state,
				IgRZBrowser *source);
    ~IgRZViewPropertiesCategory (void);

    virtual void	browserChanged  (IgRZBrowser *source);
    virtual void	registerBrowser (IgState *state,
                                         IgRZBrowser *source);

public slots:
    virtual void	alignAxis (void);

private:
    IgRZBrowser	*m_source;

    IgQtAxisAlignment	*m_axisAlignment; // 

    // undefined semantics
    IgRZViewPropertiesCategory (const IgRZViewPropertiesCategory &);
    IgRZViewPropertiesCategory &operator = (const IgRZViewPropertiesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RZ_WINDOW_IG_RZ_VIEW_PROPERTIES_CATEGORY_H

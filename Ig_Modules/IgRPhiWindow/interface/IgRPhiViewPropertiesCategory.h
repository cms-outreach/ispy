#ifndef IG_RPHI_WINDOW_IG_RPHI_VIEW_PROPERTIES_CATEGORY_H
# define IG_RPHI_WINDOW_IG_RPHI_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRPhiWindow/interface/config.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRPhiBrowser;
class IgQtAxisAlignment;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RPHI_WINDOW_API IgRPhiViewPropertiesCategory 
    : public IgControlCategory,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgRPhiViewPropertiesCategory);
    
public:
    IgRPhiViewPropertiesCategory (IgState *state,
				  IgRPhiBrowser *source);
    ~IgRPhiViewPropertiesCategory (void);

    virtual void	browserChanged  (IgRPhiBrowser *source);
    virtual void	registerBrowser (IgState *state,
                                         IgRPhiBrowser *source);

public slots:
    virtual void	alignAxis (void);

private:
    IgRPhiBrowser	*m_source;

    IgQtAxisAlignment	*m_axisAlignment; // 

    // undefined semantics
    IgRPhiViewPropertiesCategory (const IgRPhiViewPropertiesCategory &);
    IgRPhiViewPropertiesCategory &operator = (const IgRPhiViewPropertiesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RPHI_WINDOW_IG_RPHI_VIEW_PROPERTIES_CATEGORY_H

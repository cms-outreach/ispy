#ifndef IG_3D_BASE_WINDOW_IG_QTAPPLICATION_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_QTAPPLICATION_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/IgCommonViewCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgQtApplicationCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtApplicationCategory);    
    Q_OBJECT
public:
    IgQtApplicationCategory (IgState *state);
    ~IgQtApplicationCategory ();

private slots:
    virtual void		setWindowStyle		(const QString & style);
    virtual void		updateWindowStyle  	(const QString & style);

private:
    void			addQApplicationTab 	(void);
    
    QComboBox			*m_windowStyle;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_QTAPPLICATION_CATEGORY_H

#ifndef IG_2D_WINDOW_IG_2D_WINDOW_H
# define IG_2D_WINDOW_IG_2D_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig2DWindow/interface/config.h"
# include "Ig_Modules/Ig3DBaseWindow/interface/Ig3DBaseWindow.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgPage;
class Ig2DViewPropertiesCategory;
class Ig2DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_2D_WINDOW_API Ig2DWindow : public Ig3DBaseWindow
{
    Q_OBJECT
public:
    Ig2DWindow (IgPage *page);
    ~Ig2DWindow (void);
    // implicit copy constructor
    // implicit assignment operator
protected:
    virtual void initMenu (void);
    virtual void initCategories (void);    
    virtual void initToolBar (void);
    
    Ig2DViewPropertiesCategory	*m_2DViewPropertiesCategory;
private slots:
virtual void show2DViewProperties (void);    
private:
    Ig2DModel	*m_2dmodel;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_2D_WINDOW_IG_2D_WINDOW_H

#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_WIDGET_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_WIDGET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DMaterialEditor/interface/config.h"
# include "Ig_Modules/Ig3DMaterialEditor/interface/IgSoMaterialMap.h"
# include <qwidget.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QHBox;
class QVBox;
class QListView;
class QListViewItem;
class QVBoxLayout;
class QComboBox;
class QPushButton;
class SoSeparator;
class SoMaterial;
class IgG4ToSoMaterialMap;
class IgMatEdSlider;
class IgMatEdColorBox;
class IgMatEdMainMenu;
class IgMatEdViewer;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG3DMATERIAL_EDITOR_API IgMatEdMainWidget : public QWidget
{
private:    
    Q_OBJECT   
    QWidget 		*m_parent;
public:
    IgMatEdMainWidget (QWidget *parent, IgMatEdMainMenu *menu);
    ~IgMatEdMainWidget (void);

    void saveSoMaterials (void);
    void saveMaterialMap (void);
    
    // implicit copy constructor
    // implicit assignment operator
   // implicit destructor
    IgSoMaterialMap	*m_soMaterialMap;
    QComboBox		*m_comboBox;    
    QListView 		*m_g4ListView;
public slots:
void setCurrentG4Material (const QString &name);    
    void newMaterial (const QString &text);    
    void newG4Material (QListViewItem *item);
    void fileMenuActivated (int item);    
    void createSoMaterial (void);    
private:
    IgMatEdMainMenu	*m_menu;    
    QVBoxLayout		*m_layout;    
    QHBox	 	*m_mainHBox;
    IgMatEdViewer	*m_viewer;    
    SoSeparator		*m_rootNode;
    QVBox		*m_sliderBox;    
    QHBox		*m_colorBox;    
    QHBox		*m_soMaterialBox;
    QPushButton		*m_createMaterialButton;    
    IgG4ToSoMaterialMap	*m_g4ToSoMap;
    IgMatEdSlider	*m_shininessSlider;
    IgMatEdSlider	*m_transparencySlider;

    IgMatEdColorBox 	*m_diffuseColorBox;
    IgMatEdColorBox 	*m_ambientColorBox;
    IgMatEdColorBox 	*m_specularColorBox;
    IgMatEdColorBox 	*m_emissiveColorBox;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_WIDGET_H

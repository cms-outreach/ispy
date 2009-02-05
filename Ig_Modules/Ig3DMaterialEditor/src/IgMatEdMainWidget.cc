//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/interface/IgMatEdMainWidget.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdSlider.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdColorBox.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdModificationAction.h"
#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdMainMenu.h"
#include "Ig_Modules/Ig3DMaterialEditor/interface/IgMatEdViewer.h"
#include "Ig_Modules/Ig3DMaterialEditor/interface/IgSoMaterialMap.h"
#include "Ig_Modules/Ig3DMaterialEditor/interface/IgG4ToSoMaterialMap.h"

#include <iostream>
#include <fstream>

#include <qhbox.h>
#include <qvbox.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qslider.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qframe.h>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>

#include <classlib/iobase/Filename.h>
#include <classlib/utils/FileAcl.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMatEdMainWidget::IgMatEdMainWidget (QWidget *parent, IgMatEdMainMenu *menu)
    :QWidget (parent),
     m_parent (parent),
     m_g4ListView (0),
     m_menu (menu),
     m_layout (0),
     m_mainHBox (0),
     m_viewer (0),
     m_rootNode (0),
     m_sliderBox (0)
{    
    ASSERT (m_parent);

    m_layout = new QVBoxLayout (this);    
    m_layout->setAutoAdd (TRUE);    

    m_mainHBox = new QHBox (this);
    m_mainHBox->setMargin(5);
    m_mainHBox->setSpacing(20);
    m_mainHBox->setEnabled (true);
    
    // Create a listview with all the materials
    m_g4ListView = new QListView (m_mainHBox);
    m_g4ListView->addColumn ("G4 material");    
    m_g4ListView->addColumn ("OIV material");    

    m_g4ToSoMap = IgG4ToSoMaterialMapSingleton::instance ();

    for (IgG4ToSoMaterialMap::MaterialMap::iterator i = m_g4ToSoMap->begin ();i != m_g4ToSoMap->end () ; i++)
    {
	QListViewItem *item = new QListViewItem (m_g4ListView);
	item->setText (0, i->first);
	item->setText (1, i->second);	
    }        

    m_soMaterialBox = new QHBox (this);
    m_soMaterialBox->setMargin (10);
    m_soMaterialBox->setSpacing (5);

    m_sliderBox = new QVBox (this);

    m_sliderBox->setMargin (10);
    m_sliderBox->setSpacing (5);    
    
    //Makes the sphere scenegraph.
    m_rootNode = new SoSeparator;
    m_rootNode->ref ();

    m_soMaterialMap = IgSoMaterialMapSingleton::instance ();
    
    ASSERT (m_soMaterialMap);
    ASSERT ((*m_soMaterialMap)["default"]);
    
    m_rootNode->insertChild ((*m_soMaterialMap)["default"], 0);
    m_rootNode->insertChild (new SoSphere, 1);
    
    m_colorBox = new QHBox (this);    
    
    m_createMaterialButton = new QPushButton ("Create material", m_soMaterialBox);    
    connect (m_createMaterialButton, SIGNAL (clicked()), this, SLOT (createSoMaterial()));
    
    m_comboBox = new QComboBox (m_soMaterialBox);

    for (IgSoMaterialMap::MaterialMap::iterator i = m_soMaterialMap->begin (); 
	 i != m_soMaterialMap->end (); 
	 i++)
    {
	m_comboBox->insertItem (i->first);	
    }

    connect (m_comboBox, SIGNAL (activated (const QString &)), this, SLOT (newMaterial (const QString &)));
    connect (m_comboBox, SIGNAL (highlighted (const QString &)), this, SLOT (newMaterial (const QString &)));
    connect (m_g4ListView, SIGNAL (selectionChanged (QListViewItem *)), this, SLOT (newG4Material (QListViewItem *)));
    
    if (m_menu != 0)
    {
	QPopupMenu *tmpMenu = m_menu->m_fileMenu;
	ASSERT (tmpMenu);    
	connect (tmpMenu, SIGNAL (activated (int)), this, SLOT (fileMenuActivated (int)));
    }
    
    m_shininessSlider = new IgMatEdSlider (m_sliderBox, "Shininess", &IgMatEdModificationAction::m_shininess, this);
    m_transparencySlider = new IgMatEdSlider (m_sliderBox, "Transparency", &IgMatEdModificationAction::m_transparency, this);

    m_diffuseColorBox = new IgMatEdColorBox (m_colorBox, "Diffuse", IgMatEdModificationAction::m_diffuseColor, this);
    m_ambientColorBox = new IgMatEdColorBox (m_colorBox, "Ambient", IgMatEdModificationAction::m_ambientColor, this);
    m_specularColorBox = new IgMatEdColorBox (m_colorBox, "Specular", IgMatEdModificationAction::m_specularColor, this);
    m_emissiveColorBox = new IgMatEdColorBox (m_colorBox, "Emissive", IgMatEdModificationAction::m_emissiveColor, this);
    
    // Use one of the convenient SoQt viewer classes.
    QHBox *dummyBox = new QHBox (m_mainHBox);
    QFrame *dummyFrame = new QFrame (dummyBox);
    dummyFrame->setMinimumSize (200,200);
    
    m_viewer = new IgMatEdViewer (dummyFrame, m_rootNode);
    m_g4ListView->show ();
    m_mainHBox->show ();
    m_g4ListView->setSelected (m_g4ListView->firstChild (), true);    
}

IgMatEdMainWidget::~IgMatEdMainWidget (void)
{
    ASSERT (m_viewer);
    ASSERT (m_rootNode);
    ASSERT (m_g4ListView);
    
    m_rootNode->unref (); 
    
    delete m_viewer;    
    delete m_g4ListView;    
}

void
IgMatEdMainWidget::setCurrentG4Material (const QString &name)
{
    QListViewItem *item = m_g4ListView->findItem (name,0);
    //If the material name matches one of the list, then select it
    //in the list, otherwise create and select a new entry for it.
    if (!item)
    {
	item = new QListViewItem (m_g4ListView, name, "default");	
	m_g4ListView->insertItem (item);
	(*m_g4ToSoMap)[name.ascii ()] = "default";
    }    
    m_g4ListView->setCurrentItem (item);    
    m_g4ListView->ensureItemVisible (item);    
}

void
IgMatEdMainWidget::newG4Material (QListViewItem *item)
{    
    std::cerr << "newG4Material called" << std::endl;    
    newMaterial (item->text (1));    
    m_comboBox->setCurrentText (item->text (1));    
}

void 
IgMatEdMainWidget::newMaterial (const QString &text)
{
    std::cerr << "newMaterial called with argument " << text << std::endl;    
    ASSERT (m_soMaterialMap);

    if (text != QString::null)
    {
	/*Set the new name for the SoMaterial*/
	ASSERT (m_g4ListView);	
	ASSERT (m_comboBox);
	QListViewItem *item = m_g4ListView->selectedItem ();
	item->setText (1, text);	

	/*Updates the G4->So mapping map*/
	(*m_g4ToSoMap) [item->text (0).ascii ()] = text.ascii ();		

	/*Updates the display material using the selected one*/
	SoMaterial *m_selectedMaterial = (*m_soMaterialMap) [text.ascii ()];

	if (m_selectedMaterial == 0)
	{
	    QMessageBox::warning (0, "Warning!", "Open Inventor material "+text+" not found. Using a default one for it.", "OK");
	    m_selectedMaterial = new SoMaterial;
	    m_soMaterialMap->addEntry (text.ascii (), m_selectedMaterial);
	}	

	ASSERT (m_selectedMaterial);
	m_rootNode->removeChild (0);
	m_rootNode->insertChild (m_selectedMaterial, 0);
	std::cerr << m_selectedMaterial->ambientColor[0][0] << " " << m_selectedMaterial->ambientColor[0][1] << " " << m_selectedMaterial->ambientColor[0][2] << std::endl;
	
	/*Updates the position of the sliders*/
	ASSERT (m_diffuseColorBox);
	ASSERT (m_ambientColorBox);
	ASSERT (m_specularColorBox);
	ASSERT (m_emissiveColorBox);
	ASSERT (m_transparencySlider);
	ASSERT (m_shininessSlider);
	
	m_diffuseColorBox->updatePosition (m_selectedMaterial->diffuseColor);    
	m_ambientColorBox->updatePosition (m_selectedMaterial->ambientColor);
	m_specularColorBox->updatePosition (m_selectedMaterial->specularColor);
	m_emissiveColorBox->updatePosition (m_selectedMaterial->emissiveColor);
	m_transparencySlider->updatePosition ((int) (m_selectedMaterial->transparency[0]*100));
	m_shininessSlider->updatePosition ((int) (m_selectedMaterial->shininess[0]*100));
    }
}

void 
IgMatEdMainWidget::saveMaterialMap (void)
{
    //Get material map name, use default one "material.map" if not set.
    std::string filename;
    char *fname = getenv ("MATERIAL_CONFIG");    
        
    if (fname == 0)
    {
	filename = "./materials.map";	
    }
    else
    {
	filename = fname;    
    }
    
    std::ofstream output;
    output.open (filename.c_str ());

    if (output)
    {
	QListViewItem *item = m_g4ListView->firstChild ();
    
	while (item!=0)
	{
	    output << item->text (0) << "= " << item->text (1) << std::endl;
	    item = item->nextSibling ();	
	}
	output.close();    
    }
}

void
IgMatEdMainWidget::saveSoMaterials (void)
{
    SoOutput out;
    std::string filename;
    
    char *fn = getenv ("SO_MATERIALS_IV");        
    if (!fn)
	filename = "./materials.iv";
    else
	filename = fn;
    
    out.openFile (filename.c_str ());
    SoWriteAction writer (&out);
    for (IgSoMaterialMap::MaterialMap::iterator i = m_soMaterialMap->begin (); 
	 i != m_soMaterialMap->end (); 
	 i++)
    {
	writer.apply (i->second);
    }
}

void
IgMatEdMainWidget::fileMenuActivated (int item)
{
    switch (item)
    {
    case IgMatEdMainMenu::SAVE_G4_MATERIAL_MAP:
	saveMaterialMap ();
	break;
    case IgMatEdMainMenu::LOAD_G4_MATERIAL_MAP:
	break;
    case IgMatEdMainMenu::SAVE_MATERIALS:
	saveSoMaterials ();	
	break;	
    }    
}

void
IgMatEdMainWidget::createSoMaterial (void)
{
    bool ok;    
    QString text = QInputDialog::getText (
	"Create new material", "Enter material name:", QLineEdit::Normal,
	QString::null, &ok, this );
    if (text != QString::null)
    {
	SoMaterial *entry= new SoMaterial;
	entry->ref ();
	entry->setName (text.ascii ());

	m_soMaterialMap->addEntry (text.ascii (), entry);	
	m_comboBox->insertItem (text);
    }    
}

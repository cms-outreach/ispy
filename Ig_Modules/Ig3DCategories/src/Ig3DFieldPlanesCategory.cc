//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DFieldPlanesCategory.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DPlaneControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DEnumControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DUInt32Control.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSbColorMap.h"
#include <qcheckbox.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <qcombobox.h>
#include <climits>
#undef emit
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Ig3DFieldPlanesCategory, "/Control Center/Categories/Field Plane");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DFieldPlanesCategory::Ig3DFieldPlanesCategory (IgState *state)
    : Ig3DObjectCategory<IgSoFieldPlane> (state, "Field Planes", MultiItem | MonitorChanges),
      m_plane	(new Ig3DPlaneControl (this, "Field Planes")),
      m_mapDensityX (new Ig3DUInt32Control (this, "Density X",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_mapDensityZ (new Ig3DUInt32Control (this, "Density Z",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_segRatioX   (new Ig3DUInt32Control (this, "Segment Ratio X",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_segRatioZ   (new Ig3DUInt32Control (this, "Segment Ratio Z",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_maxvalue    (new Ig3DFloatControl (this, "Maximum Value",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_invisible   (new Ig3DUInt32Control (this, "Invisible Alpha",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_alpha       (new Ig3DUInt32Control (this, "Default Alpha",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_component   (new Ig3DEnumControl (this, "Component")),
      m_colorMap    (0),
      m_show	    (0),
      m_showMap	    (0),
      m_showSegments(0),
      m_manip	    (0),
      m_gradientScale(0),
      m_colorMapDirty (false)
{     
    QWidget	*area = bodyArea ();
    int		row   = bodyLayout ()->numRows ();
    ASSERT (row >= 0);
    
    // Add a new row to the grid, label on the left and the value on the right
    QWidget 	*vlab = makeIndentedLabel ("Color Map", area);
    bodyLayout ()->addWidget (vlab, row, 0);

    m_colorMap = new QComboBox (area);
    m_colorMap->setEditable(false);
    bodyLayout ()->addWidget (makeSpacedBox (area, m_colorMap), row, 1);
    
    m_show = new Ig3DBoolControl  (this, "Show/Hide Field");
    m_showMap = new Ig3DBoolControl  (this, "Show/Hide Field Color Map");
    m_showSegments = new Ig3DBoolControl  (this, "Show/Hide Field Segments");
    m_manip = new Ig3DBoolControl  (this, "Show/Hide Manipulator");
    m_gradientScale = new Ig3DBoolControl  (this, "Show/Hide Gradient scale");
    
    QWhatsThis::add (panel (), "3D Field Plane Control Category");
    QWhatsThis::add (m_plane->directionControl ()->xControl ()->widget (),
                     "Rotate 3D field plane around X-axis.");
    QWhatsThis::add (m_plane->directionControl ()->yControl ()->widget (),
                     "Rotate 3D field plane around Y-axis.");
    QWhatsThis::add (m_plane->directionControl ()->zControl ()->widget (),
                     "Rotate 3D field plane around Z-axis.");
    QWhatsThis::add (m_mapDensityX->control ()->widget (),"Set density X");
    QWhatsThis::add (m_mapDensityZ->control ()->widget (),"Set density Z");
    QWhatsThis::add (m_segRatioX->control ()->widget (),"Set segment ratio X");
    QWhatsThis::add (m_segRatioZ->control ()->widget (),"Set segment ratio Z");
    QWhatsThis::add (m_maxvalue->control ()->widget (),"Set maximum value");
    QWhatsThis::add (m_invisible->control ()->widget (),"Set invisible alpha value");
    QWhatsThis::add (m_alpha->control ()->widget (),"Set default alpha value");
    QWhatsThis::add (m_component->widget (),"Set magnitude type");
    QWhatsThis::add (m_colorMap,"Set color map of the field");
    QWhatsThis::add (m_show->widget (), "Show/Hide the 3D field plane");
    QWhatsThis::add (m_showMap->widget (), "Show/Hide the 3D field color map");
    QWhatsThis::add (m_showSegments->widget (), "Show/Hide the 3D field segments");
    QWhatsThis::add (m_manip->widget (), "Show/Hide the field manipulator");
    QWhatsThis::add (m_gradientScale->widget (), "Show/Hide the gradient scale");
    
    m_colorMap->insertItem ("Autumn");
    m_colorMap->insertItem ("Bone");
    m_colorMap->insertItem ("Cool");
    m_colorMap->insertItem ("Copper");
    m_colorMap->insertItem ("Flag");
    m_colorMap->insertItem ("Gray");
    m_colorMap->insertItem ("Hot");
    m_colorMap->insertItem ("HSV");
    m_colorMap->insertItem ("Jet");
    m_colorMap->insertItem ("Pink");
    m_colorMap->insertItem ("Prism");
    m_colorMap->insertItem ("Spring");
    m_colorMap->insertItem ("Summer");
    m_colorMap->insertItem ("VGA");
    m_colorMap->insertItem ("Winter");
    
    for (int i = 0; i <m_colorMap->count (); i++)
    {
      if (m_colorMap->text (i) == "Jet")
      {
        m_colorMap->setCurrentItem (i);
	break;
      }
    }
    
    //set up some min/max values of different controls
    m_mapDensityX->control()->setRange (1, 1000);
    m_mapDensityZ->control()->setRange (1, 1000);
    m_segRatioX->control()->setRange (1, 100);
    m_segRatioZ->control()->setRange (1, 100);
    m_invisible->control()->setRange (0, 255);
    m_alpha->control()->setRange (0, 255);
    
    // Space it up 
    bottomLayout ()->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Minimum, QSizePolicy::Expanding));
	
    connect (m_colorMap, SIGNAL (activated (int)),
             this,       SLOT   (colorMapChanged(int)));
    connect (this, SIGNAL (tracking (bool)),
             this, SLOT   (trackingChanged(bool)));
	     
    state->put (s_key, this);
}

void
Ig3DFieldPlanesCategory::colorMapChanged (int /*index*/)
{
    m_colorMapDirty = true;
    if (isTracking ())
      applyColorMap ();
    else
      setDirty (true);
}

void
Ig3DFieldPlanesCategory::trackingChanged (bool mode)
{
    if (mode)
      applyColorMap ();
}

void
Ig3DFieldPlanesCategory::applyColorMap (void)
{
    if ( m_colorMapDirty )
    {
      int index = m_colorMap->currentItem ();
      IgSoFieldPlane *obj = object (currentItem ());
      obj->colorMap (IgSbColorMap::getColorMap ((IgSbColorMap::ColorMap) index));
      m_colorMapDirty = false;
    }
}

void
Ig3DFieldPlanesCategory::attach (void)
{
    IgSoFieldPlane *obj = object (currentItem ());
    m_plane->setField (&obj->plane);
    m_mapDensityX->setField (&obj->mapDensityX);
    m_mapDensityZ->setField (&obj->mapDensityZ);
    m_segRatioX->setField (&obj->segRatioX);
    m_segRatioZ->setField (&obj->segRatioZ);
    m_maxvalue->setField (&obj->maxvalue);
    m_invisible->setField (&obj->invisible);
    m_alpha->setField (&obj->alpha);
    m_component->setField (&obj->component);
    m_show->setField (&obj->show);
    m_showMap->setField (&obj->showMap);
    m_showSegments->setField (&obj->showSegments);
    m_manip->setField (&obj->manip);
    m_gradientScale->setField (&obj->gradientScale);
}

void
Ig3DFieldPlanesCategory::detach (void)
{
    m_plane->setField (0);
    m_mapDensityX->setField (0);
    m_mapDensityZ->setField (0);
    m_segRatioX->setField (0);
    m_segRatioZ->setField (0);
    m_maxvalue->setField (0);
    m_invisible->setField (0);
    m_alpha->setField (0);
    m_component->setField (0);
    m_show->setField (0);
    m_showMap->setField (0);
    m_showSegments->setField (0);
    m_manip->setField (0);
    m_gradientScale->setField (0);
}

void
Ig3DFieldPlanesCategory::update (void)
{
    m_plane->update ();
    m_mapDensityX->update ();
    m_mapDensityZ->update ();
    m_segRatioX->update ();
    m_segRatioZ->update ();
    m_maxvalue->update ();
    m_invisible->update ();
    m_alpha->update ();
    m_component->update ();
    m_show->update ();
    m_showMap->update ();
    m_showSegments->update ();
    m_manip->update ();
    m_gradientScale->update ();
}

void
Ig3DFieldPlanesCategory::apply (void)
{
    m_plane->apply ();
    m_mapDensityX->apply ();
    m_mapDensityZ->apply ();
    m_segRatioX->apply ();
    m_segRatioZ->apply ();
    m_maxvalue->apply ();
    m_invisible->apply ();
    m_alpha->apply ();
    m_component->apply ();
    m_show->apply ();
    m_showMap->apply ();
    m_showSegments->apply ();
    m_manip->apply ();
    m_gradientScale->apply ();
    applyColorMap ();
}

//////////////////////////////////////////////////////////////////////
void
Ig3DFieldPlanesCategory::addOne (IgSoFieldPlane *fieldPlane)
{
  QString name (Ig3DBaseModel::decode (fieldPlane->getName ()).c_str ());
  if (name.isEmpty ())
    name = QString ("Field Plane #%1").arg (items() + 1);
  fieldPlane->setName (Ig3DBaseModel::encode (name.utf8 ().data ()));
  add (fieldPlane, true);
  setCurrentItem (items ()-1);
  itemChanged (-1);
}

void
Ig3DFieldPlanesCategory::deleteOne (IgSoFieldPlane *fieldPlane)
{
  int	index = find (fieldPlane);
  int	current = currentItem ();
  
  if (index == current)
    detach ();
  remove (index);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DFieldPlanesCategory::renameItem (const QString &newName)
{
    int current		= currentItem ();
    QString oldName (Ig3DBaseModel::decode(object (current)->getName ()).c_str ());
    if (!newName.isEmpty() && newName != oldName)
    {
      IgControlCategory:: renameItem(newName);
      rename (current, currentText ());
    }
}

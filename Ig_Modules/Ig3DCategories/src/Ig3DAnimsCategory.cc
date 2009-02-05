//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DAnimsCategory.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DUnitVec3fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DShortControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DInt32Control.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include <qinputdialog.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <Inventor/nodes/SoGroup.h>
#undef emit
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Ig3DAnimsCategory, "/Control Centre/Categories/Animators");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DAnimsCategory::Ig3DAnimsCategory (IgState *state,
				      Ig3DBaseBrowser *source)
    : Ig3DObjectCategory<IgSoAnimator> (state, "Animators"),
      m_source	(source),
      m_axis	(new Ig3DUnitVec3fControl (this, "Axis")),
      m_timeMax (new Ig3DShortControl (this, "Max Time",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_timeStep(new Ig3DShortControl (this, "Time Step",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_timeFreq(new Ig3DFloatControl (this, "Frequency",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),      
      m_state	(new Ig3DInt32Control (this, "State",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_newAxis	(new Ig3DInt32Control (this, "New Axis",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),      
      m_complete(new Ig3DBoolControl  (this, "Complate")),
      m_on	(new Ig3DBoolControl  (this, "Start/Stop")),
      m_reset	(new Ig3DBoolControl  (this, "Reset animation"))
{     
    m_complete->setEditable (false);
    m_newAxis->setEditable (false);
    m_timeMax->control ()->setMinValue (1);
    m_timeStep->control ()->setMinValue (1);
    m_newAxis->control ()->setRange(0, 2);
    m_state->control ()->setRange(0, 2);
    m_timeFreq->control ()->setMinValue (0.);
    
    QWhatsThis::add (panel (), "3D Animatation Control Category");
    QWhatsThis::add (m_axis->xControl ()->widget (), "X-axis of animation");
    QWhatsThis::add (m_axis->yControl ()->widget (), "Y-axis of animation");
    QWhatsThis::add (m_axis->zControl ()->widget (), "Z-axis of animation");
    QWhatsThis::add (m_timeMax->control ()->widget (),"Maximum counter value");
    QWhatsThis::add (m_timeStep->control ()->widget (),"Counter step size"); 
    QWhatsThis::add (m_timeFreq->control ()->widget (),
                      "Number of complete cycles from the\n"
                      "min value to the max value per second");
    QWhatsThis::add (m_on->widget (), "Start/Stop animation");
    QWhatsThis::add (m_complete->widget (), "Animation Completed");
    QWhatsThis::add (m_newAxis->control ()->widget (), 
                                         "Current Axis of animation\n"
                                         " 0 - Along X-axis\n"
			                 " 1 - Along Y-axis\n"
			                 " 2 - Along Z-axis");
    QWhatsThis::add (m_state->control ()->widget (),
                                       "State of animation\n"
                                       " 0 - Along X-axis\n"
			               " 1 - Along Y-axis\n"
			               " 2 - Along Z-axis");

    // Space it up 
    bottomLayout ()->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Minimum, QSizePolicy::Expanding));
    state->put (s_key, this);    
}

Ig3DAnimsCategory::~Ig3DAnimsCategory (void)
{ 
    state()->detach (s_key);    
}

void
Ig3DAnimsCategory::attach (void)
{
    IgSoAnimator *obj = object (currentItem ());
    m_axis->setField (&obj->axis);
    m_timeMax->setField (&obj->timeMax);
    m_timeStep->setField (&obj->timeStep);
    m_timeFreq->setField (&obj->timeFreq);
    m_newAxis->setField (&obj->newAxis);
    m_state->setField (&obj->state);
    m_on->setField (&obj->on);
    m_reset->setField (&obj->reset);
    m_complete->setField (&obj->complete);
}

void
Ig3DAnimsCategory::detach (void)
{
    m_axis->setField (0);
    m_timeMax->setField (0);
    m_timeStep->setField (0);
    m_timeFreq->setField (0);
    m_newAxis->setField (0);
    m_state->setField (0);
    m_on->setField (0);
    m_reset->setField (0);
    m_complete->setField (0);
}

void
Ig3DAnimsCategory::update (void)
{
    m_axis->update ();
    m_timeMax->update ();
    m_timeStep->update ();
    m_timeFreq->update ();
    m_newAxis->update ();
    m_state->update ();
    m_on->update ();
    m_reset->update ();
    m_complete->update ();
}

void
Ig3DAnimsCategory::apply (void)
{
    m_axis->apply ();
    m_timeMax->apply ();
    m_timeStep->apply ();
    m_timeFreq->apply ();
    m_newAxis->apply ();
    m_state->apply ();
    m_on->apply ();
    m_reset->apply ();
    m_complete->apply ();
}

void
Ig3DAnimsCategory::setEditable (bool value)
{
    IgControlItem::setEditable (value);
    m_complete->setEditable (false);
    m_newAxis->setEditable (false);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DAnimsCategory::reconstruct (void)
{
    ASSERT (m_source);
    SoGroup *group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	   Ig3DBaseModel::encode ("Default Anims Group")));
    clear ();
    if (group)
      reconstruct (group);
    else
      buildDefaults ();
}

void
Ig3DAnimsCategory::reconstruct (SoGroup *from)
{
    for (int i = 0, n = from->getNumChildren (); i < n; ++i)
    {
	ASSERT (from->getChild (i)->isOfType (IgSoAnimator::getClassTypeId ()));
	IgSoAnimator *node = static_cast<IgSoAnimator *> (from->getChild (i));
	add (node, true);
    }
    detach ();
    if (object (currentItem ()))
    {
        attach ();
        update ();
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DAnimsCategory::createNewItem (void)
{ 
    makeOne ();
    setCurrentItem (items ()-1);
    itemChanged (-1);
}

void
Ig3DAnimsCategory::deleteItem (void)
{ deleteOne (); }

void
Ig3DAnimsCategory::renameItem (const QString &newName)
{
    int current		= currentItem ();
    QString oldName (Ig3DBaseModel::decode(object (current)->getName ()).c_str ());
    if (!newName.isEmpty() && newName != oldName)
    {
      IgControlCategory:: renameItem(newName);
      rename (current, currentText ());
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DAnimsCategory::makeOne (QString name /* = "" */)
{
    if (name.isEmpty ())
    {
      bool	ok = false;
      QString	base ("Animator #%1");
      
      name = QInputDialog::getText (tr("Name"), QString::null,
				    QLineEdit::Normal,
				    base.arg (items () + 1),
				    &ok, panel ());

      if (! ok || name.isEmpty ())
	return;
    }
    
    IgSoAnimator *anim = new IgSoAnimator;
    anim->setName (Ig3DBaseModel::encode (name.utf8 ().data ()));

    SoGroup *group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	   Ig3DBaseModel::encode ("Default Anims Group")));
    ASSERT (group);
    group->addChild (anim);
    add (anim, true);
}

void
Ig3DAnimsCategory::deleteOne (void)
{
    int 		current = currentItem ();
    IgSoAnimator	*obj    = object (current);

    detach ();
    SoGroup *group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	   Ig3DBaseModel::encode ("Default Anims Group")));
    ASSERT (group);
    group->removeChild (obj);
    remove (current);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DAnimsCategory::buildDefaults (void)
{
    SoGroup *group = new SoGroup;
    group->setName (Ig3DBaseModel::encode ("Default Anims Group"));
    m_source->model ()->attachPoint ()->magic ()->addChild (group);
    makeOne ("Default Animator");
}

//////////////////////////////////////////////////////////////////////
void
Ig3DAnimsCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);    
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DAnimsCategory::browserChanged,
					   browser));    
    IgControlCategory::registerMe (state);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DAnimsCategory::browserChanged (Ig3DBaseBrowser *browser)
{
    m_source = browser;
    reconstruct ();
}

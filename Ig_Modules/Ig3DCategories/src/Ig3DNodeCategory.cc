//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DNodeCategory.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMInt32Control.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMUInt32Control.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMShortControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMUShortControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DInt32Control.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DUInt32Control.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DShortControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DUShortControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMColorControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DColorControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMBoolControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DStringControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DMStringControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVec3fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVec2fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVec4fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DRotationFControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DPlaneControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSFBool.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <string>
#undef emit
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define RANGE_FIELD_LIMIT_KEY							\
	QString key (m_currentNode->getTypeId ().getName ().getString ());	\
	key += ":";								\
	key += fieldName.getString ();

#define FIELD_DEFAULT_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      case SET:									\
	{CONTROL_TYPE *cfield = dynamic_cast<CONTROL_TYPE *> (m_fields [field]);\
	cfield->setField (dynamic_cast<FIELD_TYPE *>(field));			\
        break;}									\
      default:									\
        ASSERT (0);

#define SMRANGE_FIELD_DELETE_OPERATIONS(CONTROL_TYPE)				\
      case DELETE:								\
	{SbName  fieldName;							\
	CONTROL_TYPE* cfield = 	dynamic_cast<CONTROL_TYPE *> (m_fields [field]);\
	disconnect (cfield, SIGNAL (settingsChanged (SoField*)), 		\
	            this,   SLOT (saveSettings (SoField*)));			\
	delete cfield;								\
	m_fields.erase (field);							\
	break;}

#define RANGE_FIELD_SAVE_LIMIT_OPERATIONS(CONTROL_TYPE)				\
      case SAVE_LIMITS:								\
	{SbName  fieldName;							\
	CONTROL_TYPE* cfield = dynamic_cast<CONTROL_TYPE *> (m_fields [field]);	\
	m_currentNode->getFieldName (field, fieldName);				\
	RANGE_FIELD_LIMIT_KEY							\
	m_fieldLimitsMap[key] = cfield->getSettingString ();			\
	break;}

#define FIELD_SPEC_OPERATIONS(CONTROL_TYPE)					\
      case SAVE_LIMITS:								\
      	break;									\
      case DELETE:								\
	{delete dynamic_cast<CONTROL_TYPE *> (m_fields [field]);		\
	m_fields.erase (field);							\
	break;}									\
      case CREATE:								\
	{SbName  fieldName;							\
	m_currentNode->getFieldName (field, fieldName);				\
	CONTROL_TYPE* cfield = new CONTROL_TYPE (this, fieldName.getString ());	\
	m_fields [field] = cfield;						\
        break;}

#define RANGE_FIELD_SET_LIMITS							\
	RANGE_FIELD_LIMIT_KEY							\
	if (m_fieldLimitsMap.find(key) != m_fieldLimitsMap.end ())		\
	{QString s(m_fieldLimitsMap[key]); cfield->setSettingString (s);}	\
	connect (cfield, SIGNAL (settingsChanged (SoField*)), 			\
	         this,   SLOT (saveSettings (SoField*)));
	

#define SRANGE_FIELD_SPEC_OPERATIONS(CONTROL_TYPE)				\
      RANGE_FIELD_SAVE_LIMIT_OPERATIONS(CONTROL_TYPE)				\
      SMRANGE_FIELD_DELETE_OPERATIONS(CONTROL_TYPE)				\
      case CREATE:								\
	{SbName  fieldName;							\
	m_currentNode->getFieldName (field, fieldName);				\
	CONTROL_TYPE* cfield = new CONTROL_TYPE (this,				\
	                           fieldName.getString (),			\
	                           IgQtRangeControl::StepperPart		\
		                   | IgQtRangeControl::ValuePart		\
			           | IgQtRangeControl::SliderPart);		\
	m_fields [field] = cfield;						\
	RANGE_FIELD_SET_LIMITS							\
        break;}

#define MRANGE_FIELD_SPEC_OPERATIONS(CONTROL_TYPE)				\
      RANGE_FIELD_SAVE_LIMIT_OPERATIONS(CONTROL_TYPE)				\
      SMRANGE_FIELD_DELETE_OPERATIONS(CONTROL_TYPE)				\
      case CREATE:								\
	{SbName  fieldName;							\
	m_currentNode->getFieldName (field, fieldName);				\
	CONTROL_TYPE* cfield = new CONTROL_TYPE (this,				\
	                           fieldName.getString (),			\
	                           IgQtRangeMControl::StepperPart		\
		                   | IgQtRangeMControl::ValuePart		\
			           | IgQtRangeMControl::SliderPart		\
				   | IgQtRangeMControl::IndexPart);		\
	m_fields [field] = cfield;						\
	RANGE_FIELD_SET_LIMITS							\
        break;}

#define SVEC_FIELD_SPEC_OPERATIONS(CONTROL_TYPE)				\
      RANGE_FIELD_SAVE_LIMIT_OPERATIONS(CONTROL_TYPE)				\
      SMRANGE_FIELD_DELETE_OPERATIONS(CONTROL_TYPE)				\
      case CREATE:								\
	{SbName  fieldName;							\
	m_currentNode->getFieldName (field, fieldName);				\
	CONTROL_TYPE* cfield = new CONTROL_TYPE (this,				\
	                           fieldName.getString ());			\
	m_fields [field] = cfield;						\
	RANGE_FIELD_SET_LIMITS							\
        break;}

#define FIELD_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)				\
      switch (type)								\
      {										\
        FIELD_SPEC_OPERATIONS(CONTROL_TYPE)					\
        FIELD_DEFAULT_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      }

#define SRANGE_FIELD_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      switch (type)								\
      {										\
        SRANGE_FIELD_SPEC_OPERATIONS(CONTROL_TYPE)				\
        FIELD_DEFAULT_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      }

#define MRANGE_FIELD_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      switch (type)								\
      {										\
        MRANGE_FIELD_SPEC_OPERATIONS(CONTROL_TYPE)				\
        FIELD_DEFAULT_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      }
	  
#define SVEC_FIELD_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)				\
      switch (type)								\
      {										\
        SVEC_FIELD_SPEC_OPERATIONS(CONTROL_TYPE)				\
        FIELD_DEFAULT_OPERATIONS(CONTROL_TYPE, FIELD_TYPE)			\
      }

static std::vector<std::map<SoType, lat::Callback2<SoField*,
                     Ig3DNodeCategory::FieldOptions> > > controllableFields;

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Ig3DNodeCategory,
                         "/Control Center/Categories/Node Manipulator");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DNodeCategory::Ig3DNodeCategory (IgState * state,
                                    Ig3DBaseBrowser *source)
    : IgControlCategory (state, "Node Manipulator", MonitorChanges | SplitterArea),
      m_source (source),
      m_currentNode (0),
      m_nodeTree(0)
{
  state->put (s_key, this);
  if (controllableFields.size () == 0){
    std::map<SoType, lat::Callback2<SoField*,
                     Ig3DNodeCategory::FieldOptions> > types;
    types [SoSFVec2f::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSVec2f);
    types [SoSFVec3f::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSVec3f);
    types [SoSFVec4f::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSVec4f);
    types [SoSFRotation::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSRotation);
    types [SoSFPlane::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSPlane);
    controllableFields.push_back (types);
    types.clear ();
    
    types [SoMFFloat::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMFloat);
    types [SoMFInt32::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMInt32);
    types [SoMFUInt32::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMUInt32);
    types [SoMFShort::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMShort);
    types [SoMFUShort::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMUShort);
    types [SoSFFloat::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSFloat);
    types [SoSFInt32::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSInt32);
    types [SoSFUInt32::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSUInt32);
    types [SoSFShort::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSShort);
    types [SoSFUShort::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSUShort);
    controllableFields.push_back (types);
    types.clear ();

    types [SoMFString::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMString);
    types [SoSFString::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSString);
    types [SoMFColor::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMColor);
    types [SoSFColor::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSColor);
    controllableFields.push_back (types);
    types.clear ();

    types [SoMFBool::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldMBool);
    types [SoSFBool::getClassTypeId ()] =
        lat::CreateCallback (this, &Ig3DNodeCategory::fieldSBool);
    controllableFields.push_back (types);
    types.clear ();
  }
}

Ig3DNodeCategory::~Ig3DNodeCategory (void)
{ clearCategory (); }

void
Ig3DNodeCategory::show (void)
{
    IgControlCategory::show ();
    focusIn ();
}

void
Ig3DNodeCategory::hide (void)
{
    IgControlCategory::hide ();
    clearBodyArea ();
}

void
Ig3DNodeCategory::focusIn (void)
{
    SoNode *oldnode=m_currentNode;
    if (m_source && m_source->getSelection ())
      manipulate (m_source->getSelection ()->node ());
    else
      manipulate (0);
      
    for(nodeMap::iterator itr = m_nodeMap.begin ();
        itr != m_nodeMap.end (); itr++)
    {
      if (itr->second == oldnode)
      {
	QListViewItem *top = itr->first->parent ();
	while (top)
	{
	  top->setOpen (true);
	  top = top->parent ();
	}
	m_nodeTree->setSelected (itr->first, true);
	break;
      }
    }
}

void
Ig3DNodeCategory::saveSettings (SoField* field)
{
    ASSERT (m_fields.find (field) != m_fields.end ());

    std::map<SoType, lat::Callback2<SoField*,
      Ig3DNodeCategory::FieldOptions> >::iterator itr;
    
    for (unsigned int i=0; i < controllableFields.size (); i++)
    {
      for(itr = controllableFields[i].begin ();
          itr != controllableFields[i].end (); itr++)
      {
	if (field->isOfType (itr->first))
	{
	  (itr->second)(field, SAVE_LIMITS);
	  return;
        }
      }
    }
}

void
Ig3DNodeCategory::attach (void)
{ itrateOverFields (SET); }

void
Ig3DNodeCategory::detach (void)
{
    fieldControlMap::iterator item;
    for (item = m_fields.begin(); item != m_fields.end(); item++) 
      (*item).second->clearField ();
}

void
Ig3DNodeCategory::update (void)
{
    fieldControlMap::iterator item;
    for (item = m_fields.begin(); item != m_fields.end(); item++) 
      (*item).second->update ();
}

void
Ig3DNodeCategory::apply (void)
{
    fieldControlMap::iterator item;
    for (item = m_fields.begin() ; item != m_fields.end() ; item++) 
      (*item).second->apply ();
}

//////////////////////////////////////////////////////////////////////

void
Ig3DNodeCategory::manipulateItem (QListViewItem *item)
{
    if(!item) return;
    if (m_nodeMap.find (item) != m_nodeMap.end ())
    {
      bool oldTracking = isTracking ();
      clearBodyArea ();
      addFields (m_nodeMap [item], item->text (0));
      
      if (m_fields.size () == 0)
      {
	QWidget	*area = bodyArea ();
        int	row   = bodyLayout ()->numRows ();
      
        QWidget *vlab = makeIndentedLabel (
	                "This node either has no fields to manilupate\n"
	                "or the implementation is not yet ready.\n\n",
	                area);
        bodyLayout ()->addWidget (vlab, row, 0);
      }

      bodyArea()->show ();      
      IgControlCategory::show ();
      setTracking (oldTracking);

      // Space it up 
      bottomLayout ()->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    else
      ASSERT (0);
}

void
Ig3DNodeCategory::manipulate (SoNode *node)
{      
    clearBodyArea ();
    if (!node)
    { 
      if (m_nodeTree)
      {
        m_nodeTree->clear ();
	m_nodeMap.clear ();
	m_nodeTree->hide ();
      }
      QWidget	*area = bodyArea ();
      int	row   = bodyLayout ()->numRows ();
      
      QWidget *vlab = makeIndentedLabel (
	               "No selected node on the scenegraph.\n"
		       "If you want to manipulate a node then please\n"
		       "change to the pick mode and then click on\n"
		       "the node to manipuate in the 3D browser.\n\n",
	               area);
      bodyLayout ()->addWidget (vlab, row, 0);
      IgControlCategory::show ();
      return;
    }
    
    if (!m_nodeTree)
    {
      m_nodeTree = new QListView (leftArea (), "Node Manipulator List");
      m_nodeTree->addColumn ("Node Type/Name");
      m_nodeTree->setSelectionMode (QListView::Single);
      leftLayout ()->addWidget (m_nodeTree);
      connect (m_nodeTree, SIGNAL ( selectionChanged (QListViewItem *)),
	       this,       SLOT   ( manipulateItem (QListViewItem *)));
    }
    m_nodeTree->clear ();
    m_nodeMap.clear ();
    m_nodeTree->show ();
    
    QListViewItem *topItem = new QListViewItem (m_nodeTree, "Nodes List");
    topItem->setSelectable (false);
    addNode (node, topItem);
    topItem->setOpen (true);
    if (topItem->childCount () == 0)
      manipulate (0);
    else if (topItem->firstChild ())
      m_nodeTree->setSelected (topItem->firstChild (), true);
}

bool
Ig3DNodeCategory::addNode (SoNode *node, QListViewItem *parent /* = 0 */ )
{
    std::string name("");
    QListViewItem *item = 0;
    
    //Check for the already existance of the node in map    
    for(nodeMap::iterator itr = m_nodeMap.begin ();
        itr != m_nodeMap.end (); itr++)
      if (itr->second == node)
	return false;

    bool isGroup = node->isOfType(SoGroup::getClassTypeId ());
    
    if (!(isGroup || hasControllableFields(node)))
      return false;
    
    if (node->getName ().getLength ())
      name = Ig3DBaseModel::decode(node->getName ());
    else
      name = Ig3DBaseModel::decode(node->getTypeId ().getName ());

    if (parent == 0)
      item = new QListViewItem (m_nodeTree, name.c_str ());
    else
      item = new QListViewItem (parent, name.c_str ());
    
    if (isGroup)
    {
      SoGroup *group = dynamic_cast<SoGroup*>(node);
      int added = 0;
      for(int i = 0; i < group->getNumChildren (); i++)
        if (addNode (group->getChild (i), item))
	  added++;
      if(!added)
      {
        delete item;
	return false;
      }
    }
    
    m_nodeMap [item] = node;
    return true;
}

void
Ig3DNodeCategory::clearCategory (void)
{
    IgControlCategory::clearCategory ();
    detach ();
    fieldControlMap::iterator item;
    for(item = m_fields.begin (); item != m_fields.end (); item++)
      disconnect (item->second, SIGNAL (settingsChanged (SoField*)),
                  this,         SLOT (saveSettings (SoField*)));
    m_fields.clear ();
}

void
Ig3DNodeCategory::clearBodyArea (void)
{
    IgControlCategory::clearBodyArea ();
    detach ();
    fieldControlMap::iterator item;
    for(item = m_fields.begin (); item != m_fields.end (); item++)
      disconnect (item->second, SIGNAL (settingsChanged (SoField*)),
                  this,         SLOT (saveSettings (SoField*)));
    m_fields.clear ();
}

bool
Ig3DNodeCategory::hasControllableFields (SoNode *node)
{
  SoFieldList fields;
  SoField *field = 0;
  int num = node->getFields (fields);
  std::map<SoType, lat::Callback2<SoField*,
    Ig3DNodeCategory::FieldOptions> >::iterator item;

  for (int i = 0; i < num; i++ )
  {
    field = fields[i];
    for(unsigned int j=0; j < controllableFields.size (); j++)
    {
      for(item = controllableFields[j].begin ();
          item != controllableFields[j].end (); item++)
      {
        if (field->isOfType (item->first))
          return true;
      }
    }
  }
  return false;
}

/** itrate over all the controllable fields in order i.e.
So*F(Float/Int32/Long...) first and then So*FColor and then
So*FBool etc.
**/
void
Ig3DNodeCategory::itrateOverFields (SoNode* node, FieldOptions type)
{
    SoFieldList fields;
    SoField *field = 0;
    int num = node->getFields (fields);
    
    int done = 0;
    std::map<SoType, lat::Callback2<SoField*,
      Ig3DNodeCategory::FieldOptions> >::iterator item;
    for (unsigned int i=0; i < controllableFields.size (); i++)
    {
      for (int j = 0; j < num; j++)
      {
        field = fields[j];
	for(item = controllableFields[i].begin ();
	  item != controllableFields[i].end (); item++)
	{
	  if (field->isOfType (item->first))
	  {
	    (item->second)(field, type);
	    done++;
	    if (done == num)
	      return;
	    break;
	  }
	}
      }
    }
}

void
Ig3DNodeCategory::itrateOverFields (FieldOptions type)
{
    SoField *field = 0;
    fieldControlMap::iterator item;
    std::map<SoType, lat::Callback2<SoField*,
      Ig3DNodeCategory::FieldOptions> >::iterator itr;
    
    for(item = m_fields.begin (); item != m_fields.end (); item++)
    {
      field = item->first;
      for (unsigned int i=0; i < controllableFields.size (); i++)
      {
        for(itr = controllableFields[i].begin ();
	  itr != controllableFields[i].end (); itr++)
	{
	  if (field->isOfType (itr->first))
	  {
	    (itr->second)(field, type);
	    break;
	  }
	}
      }
    }
}

void
Ig3DNodeCategory::addFields (SoNode *node, const char *name)
{
    if (node){
      m_currentNode = node;
      QWidget	*area = bodyArea ();
      int	row   = bodyLayout ()->numRows ();
      
      std::string name1 ("");
      if (name == 0){
        if (node->getName ().getLength ())
          name1 = Ig3DBaseModel::decode(node->getName ());
	else
	  name1 = Ig3DBaseModel::decode(node->getTypeId ().getName ());
      }
      else
        name1 = name;
      
      //alignment widget
      if (!name1.empty ()){
        QWidget 	*vlab = makeIndentedLabel (name1.c_str (),area);
        bodyLayout ()->addWidget (vlab, row, 0);
	pushIndent ();
      }
      
      itrateOverFields (m_currentNode, CREATE);
      
      if (!name1.empty ())
        popIndent ();
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DNodeCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DNodeCategory::browserFocusIn,
					   browser));    
    IgControlCategory::registerMe (state);
    m_source = browser;
}

void
Ig3DNodeCategory::browserFocusIn (Ig3DBaseBrowser *browser)
{ m_source = browser; }

//////////////////////////////////////////////////////////////////////
void
Ig3DNodeCategory::fieldMFloat (SoField* field, FieldOptions type)
{
      MRANGE_FIELD_OPERATIONS(Ig3DMFloatControl, SoMFFloat);
}

void
Ig3DNodeCategory::fieldMInt32 (SoField* field, FieldOptions type)
{
      MRANGE_FIELD_OPERATIONS(Ig3DMInt32Control, SoMFInt32);
}

void
Ig3DNodeCategory::fieldMUInt32 (SoField* field, FieldOptions type)
{
      MRANGE_FIELD_OPERATIONS(Ig3DMUInt32Control, SoMFUInt32);
}

void
Ig3DNodeCategory::fieldMShort (SoField* field, FieldOptions type)
{
      MRANGE_FIELD_OPERATIONS(Ig3DMShortControl, SoMFShort);
}

void
Ig3DNodeCategory::fieldMUShort (SoField* field, FieldOptions type)
{
      MRANGE_FIELD_OPERATIONS(Ig3DMUShortControl, SoMFUShort);
}

void
Ig3DNodeCategory::fieldSFloat (SoField* field, FieldOptions type)
{
      SRANGE_FIELD_OPERATIONS(Ig3DFloatControl, SoSFFloat);
}

void
Ig3DNodeCategory::fieldSInt32 (SoField* field, FieldOptions type)
{
      SRANGE_FIELD_OPERATIONS(Ig3DInt32Control, SoSFInt32);
}

void
Ig3DNodeCategory::fieldSUInt32 (SoField* field, FieldOptions type)
{
      SRANGE_FIELD_OPERATIONS(Ig3DUInt32Control, SoSFUInt32);
}

void
Ig3DNodeCategory::fieldSShort (SoField* field, FieldOptions type)
{
      SRANGE_FIELD_OPERATIONS(Ig3DShortControl, SoSFShort);
}

void
Ig3DNodeCategory::fieldSUShort (SoField* field, FieldOptions type)
{
      SRANGE_FIELD_OPERATIONS(Ig3DUShortControl, SoSFUShort);
}

void
Ig3DNodeCategory::fieldSColor (SoField* field, FieldOptions type)
{
      FIELD_OPERATIONS(Ig3DColorControl, SoSFColor);
}

void
Ig3DNodeCategory::fieldMColor (SoField* field, FieldOptions type)
{
      FIELD_OPERATIONS(Ig3DMColorControl, SoMFColor);
}

void
Ig3DNodeCategory::fieldSBool (SoField* field, FieldOptions type)
{
      FIELD_OPERATIONS(Ig3DBoolControl, SoSFBool);
}

void
Ig3DNodeCategory::fieldMBool (SoField* field, FieldOptions type)
{
      FIELD_OPERATIONS(Ig3DMBoolControl, SoMFBool);
}

void
Ig3DNodeCategory::fieldSVec2f (SoField* field, FieldOptions type)
{
      SVEC_FIELD_OPERATIONS(Ig3DVec2fControl, SoSFVec2f);
}

void
Ig3DNodeCategory::fieldSVec3f (SoField* field, FieldOptions type)
{
      SVEC_FIELD_OPERATIONS(Ig3DVec3fControl, SoSFVec3f);
}

void
Ig3DNodeCategory::fieldSVec4f (SoField* field, FieldOptions type)
{
      SVEC_FIELD_OPERATIONS(Ig3DVec4fControl, SoSFVec4f);
}

void
Ig3DNodeCategory::fieldSRotation (SoField* field, FieldOptions type)
{
      SVEC_FIELD_OPERATIONS(Ig3DRotationFControl, SoSFRotation);
}

void
Ig3DNodeCategory::fieldSPlane (SoField* field, FieldOptions type)
{
      SVEC_FIELD_OPERATIONS(Ig3DPlaneControl, SoSFPlane);
}

void
Ig3DNodeCategory::fieldSString (SoField* field, FieldOptions type)
{
      FIELD_OPERATIONS(Ig3DStringControl, SoSFString);
}

void
Ig3DNodeCategory::fieldMString (SoField* field, FieldOptions type)
{
      FIELD_OPERATIONS(Ig3DMStringControl, SoMFString);
}

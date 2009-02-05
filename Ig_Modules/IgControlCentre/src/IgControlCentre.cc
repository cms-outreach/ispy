/****************************************************************************
** Form implementation generated from reading ui file 'Ig_Modules/IgControlCentre/src/IgControlCentre.ui'
**
** Created: Fri Oct 31 14:27:53 2003
**      by: The User Interface Compiler ($Id: IgControlCentre.cc,v 1.24 2005/07/07 13:37:15 muzaffar Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppHelp.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qiconview.h>
#include <qpoint.h>
#include <qwhatsthis.h>
#include <qsizepolicy.h>
#include <qevent.h>
/*
 *  Constructs a IgControlCentre as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgControlCentre::IgControlCentre(QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
      IgControlCentreLayout (0),
      m_layout1 (0),
      m_layout2 (0),
      m_current (0),
      m_iconView (0),
      m_updateOnWindowActive (false),
      m_focusChanged (false)
{
    if ( !name )
      setName( m_name );
    
    m_name = "IGUANA Control Centre";
    m_baseName = "";
    
    setFocusPolicy (QWidget::StrongFocus);
    IgControlCentreLayout = new QGridLayout( this, 1, 1, 11, 6, "IgControlCentreLayout");

    m_layout2 = new QGridLayout( 0, 1, 1, 0, 6, "m_layout2"); 

    m_iconView = new QIconView (0, "m_iconView");
    m_iconView->setSelectionMode (QIconView::Single);
    m_iconView->setAutoArrange (true);
    m_iconView->setResizeMode (QIconView::Adjust);
    
    m_layout2->addWidget(m_iconView , 0, 0 );

    m_layout1 = new QHBoxLayout( 0, 0, 6, "m_layout1"); 

    m_back = new QPushButton( this, "m_back" );
    m_back->setAutoDefault( false );
    m_back->setEnabled ( false );
    m_back->setFixedSize (QSize(45, 22));
    m_layout1->addWidget( m_back );
    QSpacerItem* spacer0 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_layout1->addItem( spacer0 );

    m_close = new QPushButton( this, "m_close" );
    m_close->setAutoDefault( false );
    m_close->setFixedSize (QSize(45, 22));
    m_layout1->addWidget( m_close );
    QSpacerItem* spacer = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_layout1->addItem( spacer );

    m_revert = new QPushButton( this, "m_revert" );
    m_revert->setAutoDefault( false );
    m_revert->setEnabled( false );
    m_revert->setFixedSize (QSize(45, 22));
    m_layout1->addWidget( m_revert );
    QSpacerItem* spacer_2 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_layout1->addItem( spacer_2 );

    m_apply = new QPushButton( this, "m_apply" );
    m_apply->setAutoDefault( false );
    m_apply->setEnabled( false );
    m_apply->setFixedSize (QSize(45, 22));
    m_layout1->addWidget( m_apply );
    QSpacerItem* spacer_3 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_layout1->addItem( spacer_3 );

    m_help = new QPushButton( this, "m_help" );
    m_help->setAutoDefault( false );
    m_help->setFixedSize (QSize(45, 22));
    m_layout1->addWidget( m_help );

    m_layout2->addLayout( m_layout1, 1, 0 );

    IgControlCentreLayout->addLayout( m_layout2, 0, 0 );
    languageChange();
    clearWState( WState_Polished );
    setSizeGripEnabled (true);

    // signals and slots connections
    connect (m_back, SIGNAL(clicked ()), this, SLOT(back ()));
    connect (m_close, SIGNAL(clicked ()), this, SLOT(close ()));
    connect (m_apply, SIGNAL(clicked ()), this, SLOT(apply ()));
    connect (m_revert, SIGNAL(clicked ()), this, SLOT(revert ()));
    connect (m_help, SIGNAL(clicked ()), this, SLOT(help ()));
    connect (m_iconView, SIGNAL (doubleClicked (QIconViewItem *)),
             this,       SLOT (selectIconItem (QIconViewItem *)));
    connect (m_iconView, SIGNAL (returnPressed (QIconViewItem *)),
             this,       SLOT (selectIconItem (QIconViewItem *)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
IgControlCentre::~IgControlCentre()
{
    // no need to delete child widgets, Qt does it all for us
    while (! m_categories.empty ())
    {
	delete m_categories.begin ()->second.first;
	m_categories.erase (m_categories.begin ());
    }
    deleteIconViewArch (&m_iconViewArch);
}

void
IgControlCentre::deleteIconViewArch (IconViewArch *toplevel)
{
    while (!toplevel->empty ()){
       IconViewArch::iterator itr = toplevel->begin ();
       if (itr->second.first != 0)
       {
         deleteIconViewArch (static_cast<IconViewArch *>(itr->second.first));
         delete static_cast<IconViewArch *>(itr->second.first);
       }
       toplevel->erase (itr);
    }
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void IgControlCentre::languageChange()
{
    setCaption( tr( m_name ) );
    m_back->setText( tr( "&Back" ) );
    m_back->setAccel( QKeySequence( tr( "Alt+P" ) ) );
    QWhatsThis::add( m_back, tr( "Go to the previous panel of the control centre." ) );
    m_close->setText( tr( "&Close" ) );
    m_close->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    QWhatsThis::add( m_close, tr( "Close the Control Centre." ) );
    m_revert->setText( tr( "&Revert" ) );
    m_revert->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    QWhatsThis::add( m_revert, tr( "Revert the changes made via GUI which are not yet applied. <br>\n"
"When track changes option is not available or if available and <br>\n"
"enabled then changes made via GUI will automatically be applied <br>\n"
"and one can not revert those changes." ) );
    m_apply->setText( tr( "&Apply" ) );
    m_apply->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    QWhatsThis::add( m_apply, tr( "Apply the changes made via GUI which are not yet applied.<br>\n"
"When track changes option is not available or if available and<br>\n"
"enabled then changes made via GUI will automatically be applied." ) );
    m_help->setText( tr( "&Help" ) );
    m_help->setAccel( QKeySequence( tr( "Alt+H" ) ) );
    QWhatsThis::add( m_help, tr( "Get the help.\n"));
}

/** Show the control centre dialog.  */
void
IgControlCentre::show (void)
{
    static bool firstTime=true;
    if(!isShown())
    {
      QString old (m_baseName);
      m_baseName = "qqqqqq";      
      updateControlCentre (old);
      m_updateOnWindowActive=false;
      m_focusChanged = false;
      if (firstTime)
      {
        resize(QSize(200, 200).expandedTo (minimumSizeHint ()));
	firstTime = false;
      }
      QWidget::show ();
    }
    else
    {
      setActiveWindow ();
      raise();    
    }
}

/** Hide the control centre dialog.  */
void
IgControlCentre::hide (void)
{
    if (m_current)
    {
      QString name = m_baseName;
      back ();
      m_baseName =  name;
    }
    QWidget::hide ();
}

/** Respond to "Apply" button press.  Directs the call to the
    current category.  */
void 
IgControlCentre::apply (void)
{
    ASSERT (m_current);
    m_current->apply ();
}

/** Respond to "Revert" button press.  Directs the call to the
    current category.  */
void 
IgControlCentre::revert (void)
{
    ASSERT (m_current);
    m_current->revert ();
}

/** Get help. */
void 
IgControlCentre::help (void)
{
    QString url ("");
    if (m_current)
      url = m_current->getHelpURL ();
    if (url.isEmpty ())
      url = "http://iguana.web.cern.ch/iguana/snapshot/help/control_centre.html";
    IgQtAppHelp::netscape (url);
}

/** Go to the back. 
**/
void 
IgControlCentre::back (void)
{ 
    int index = m_baseName.findRev ("/");
    if (index != -1)
      showIconView (m_baseName.left(index));
    else
      showIconView ("");
}

void
IgControlCentre::updateControlCentre (QString name)
{
    if (name.length ()
        && m_categories.find (name) != m_categories.end ())
    {
      if (m_categories [name].second == true)
        showCategory (m_categories [name].first);
      else
      {
        m_baseName = name;
	back ();
      }
      return;
    }
    showIconView (name);
}

void 
IgControlCentre::hideCategory (void)
{
    if(!m_current) return;
    disconnect (m_current, SIGNAL(dirty()), 
	        this, SLOT(dirtyCategory()));
    disconnect (m_current, SIGNAL(clean()), 
	       this, SLOT(cleanCategory()));
    m_current->hide ();
    m_current->panel ()->reparent ((QWidget*)(0), QPoint(0,0));
    m_layout2->remove (m_current->panel ());
    m_current = 0;
}

void
IgControlCentre::showCategory (IgControlCategory *category)
{
    m_baseName = category->name ();
    if (m_current == category) return;
    hideCategory ();
    hideIconView ();
    m_current = category;
    connect (m_current, SIGNAL(dirty()), 
	     this, SLOT(dirtyCategory()));
    connect (m_current, SIGNAL(clean()), 
             this, SLOT(cleanCategory()));
    m_current->panel ()->reparent (this, QPoint(0,0));
    m_layout2->addWidget(m_current->panel (), 0, 0);
    m_current->panel ()->show();
    m_back->setEnabled (true);
    if (m_baseName.length () > 20)
      setCaption(tr(m_name + " [..." + m_baseName.right (17) + "]"));
    else
      setCaption(tr(m_name + " [" + m_baseName + "]"));
    m_current->show ();
}

void
IgControlCentre::addIconViewItems (QString name)
{
  if (m_baseName == name && m_iconView->count() != 0) return;
  QString newBase ("");
  IconViewArch *toplevel = &m_iconViewArch;
 
  if (name != "")
  {  
    int index = name.find ("/");
    while (index != -1)
    {
      unsigned int length = name.length ();
      std::pair<void*, bool> item = (*toplevel)[name.left (index)];
      if (item.second == true)
      {
	toplevel=static_cast<IconViewArch *>(item.first);
        if(newBase == "")
	  newBase = name.left (index);
	else
	  newBase = newBase+ "/" + name.left (index);

	name = name.right (length - index - 1);
        index = name.find ("/");
      }
      else
      {
        index = -1;
	name = name.left (index);
      }
    }
    if ((*toplevel)[name].second == true)
    {
      toplevel=static_cast<IconViewArch *>((*toplevel)[name].first);
      
      if (newBase == "")
        newBase = name;
      else
        newBase = newBase+ "/" + name;
    }
  }
  
  if (newBase == "")
    m_back->setEnabled (false);
  else
    m_back->setEnabled (true);

  m_baseName = newBase;
  m_iconViewList.clear ();
  m_iconView->clear ();
  IconViewArch::iterator itr;
  
  for(itr = toplevel->begin (); itr != toplevel->end (); itr++)
  {
    if (itr->second.second == true)
    {
      if (itr->second.first)
        m_iconViewList.push_back (new QIconViewItem (m_iconView,
                                                     itr->first));
      else
      {
        QString cat (itr->first);
	if (!m_baseName.isEmpty ())
	  cat = m_baseName + "/" + cat;
	QPixmap icon (m_categories[cat].first->getIcon ());
	if (icon.isNull ())
	  m_iconViewList.push_back (new QIconViewItem (m_iconView, itr->first));
	else
	  m_iconViewList.push_back (new QIconViewItem (m_iconView,
                                                       itr->first,
						       icon));
      }
    }
  }
  m_iconView->arrangeItemsInGrid ();

  if (m_baseName.length ())
  {
    if (m_baseName.length () > 20)
      setCaption(tr(m_name + " [..." + m_baseName.right (17) + "]"));
    else
      setCaption(tr(m_name + " [" + m_baseName + "]"));
  }
}

void
IgControlCentre::showIconView (QString name)
{
    addIconViewItems (name);
    if (!m_iconView->isShown ())
    {
      hideCategory ();
      m_iconView->reparent (this, QPoint(0,0));
      m_layout2->addWidget (m_iconView , 0, 0 );
      cleanCategory ();
      setCaption(tr(m_name));
      m_iconView->show ();
      m_iconView->arrangeItemsInGrid ();
    }
}

void
IgControlCentre::hideIconView (void)
{
    if (!m_iconView->isShown ()) return;
    m_iconView->reparent (0, QPoint(0,0));
    m_layout2->remove (m_iconView);
    m_iconView->hide ();
}

/** Find the Category and returns the QIconViewItem
**/

bool
IgControlCentre::hasCategory (const IgControlCategory *category)
{
    CategoryMap::iterator itr;
    for(itr = m_categories.begin (); itr != m_categories.end (); itr++)
    {
      if (itr->second.first == category)
        return true;
    }
    return false;
}

/** Add @a category to the control centre.  It must be a non-null
    pointer to a category not previously added to the control centre.
    An item by the category's name will be added to the list and to
    the dialog's widget stack for the actual body content area.  It
    will not be made the current selection.  */
void
IgControlCentre::addCategory (IgControlCategory *category)
{ 
    ASSERT (category);
    ASSERT (!hasCategory (category));
    
    QString name = category->name ();
    unsigned int length = name.length ();
    
    ASSERT (length);
    ASSERT (m_categories.find (name) == m_categories.end ());
    ASSERT ((name.left(1) != "/") && (name.right(1) != "/"));
    
    std::pair<IgControlCategory *, bool> c(category, true);
    m_categories[name]=c;
    
    int index = name.find ("/");
    IconViewArch *toplevel = &m_iconViewArch;
    while (index != -1)
    {
      length = length -1;
      
      QString sname = name.left(index);
      length = length-index;
      name = name.right (length);
      if (toplevel->find(sname) != toplevel->end ())
        toplevel = static_cast<IconViewArch *>((*toplevel)[sname].first);
      else
      {
	IconViewArch *child = new IconViewArch;
	std::pair<void *, bool> c(child, true);
	(*toplevel)[sname] = c;
	toplevel=child;
      }
      index = name.find ("/");
    }
    std::pair<void *, bool> c1(0, true);
    (*toplevel)[name] = c1;
}

void
IgControlCentre::removeCategory (QString name, IconViewArch *toplevel)
{
    int index = name.find ("/");
    if (index != -1)
    {
      QString sname = name.left(index);
      name = name.right (name.length() - index - 1);
      removeCategory (name, static_cast<IconViewArch *>((*toplevel)[sname].first));
      name = sname;
    }
    if ((*toplevel)[name].first == 0)
      toplevel->erase (name);
    else if (static_cast<IconViewArch *>((*toplevel)[name].first)->size () == 0)
    {
        delete static_cast<IconViewArch *>((*toplevel)[name].first);
	toplevel->erase (name);
    }
}

/** Remove @a category to the control centre.  It must be a non-null
    pointer to a category previously added to the control centre.  The
    category will be removed from the list and from the body content
    area widget stack, but will not be deleted.  If it is the
    currently selected category, selection will be reset to null.  */
void
IgControlCentre::removeCategory (IgControlCategory *category)
{    
    ASSERT (category);
    ASSERT (hasCategory (category));
    if (m_current == category)
      back ();
    m_categories.erase (category->name ());
    removeCategory (category->name (), &m_iconViewArch);
}

/** Make @a category the current selection and show its control area
    in the body content area.  If @a category is a non-null pointer,
    it must have been previously added with #addCategory(), and will
    be made the current selection.  If @a category is null, the
    current selection is cleared.  */
void
IgControlCentre::selectCategory (IgControlCategory *category)
{
    if (category)
    {
      ASSERT (hasCategory (category));
      if (m_categories [category->name ()].second == true)
      {
        showCategory (category);
	return;
      }
    }
    back ();
}

void
IgControlCentre::selectCategory (const QString& name)
{
    updateControlCentre (name);
}

void
IgControlCentre::enableCategory (QString name, IconViewArch *toplevel, bool state /*= true */)
{
    int index = name.find ("/");
    if (index != -1)
    {
      QString sname = name.left(index);
      name = name.right (name.length() - index - 1);
      enableCategory (name, static_cast<IconViewArch *>((*toplevel)[sname].first), state);
      name = sname;
    }
    m_focusChanged = true;
    
    if ((*toplevel)[name].first == 0 || state)
      (*toplevel)[name].second = state;
    else
    {
      ASSERT (toplevel->size ());
      IconViewArch::iterator itr;
  
      IconViewArch *tmpmap = static_cast<IconViewArch *>((*toplevel)[name].first);
      for(itr = tmpmap->begin (); itr != tmpmap->end (); itr++)
      {
	if (itr->second.second == true)
	{
          (*toplevel)[name].second = true;
	  return;
	}
      }
      (*toplevel)[name].second = false;
    }
}

void
IgControlCentre::enableCategory (IgControlCategory *category, bool state /*= true */)
{
    ASSERT (m_categories.size ());
    ASSERT (category && hasCategory (category));
    ASSERT (m_categories.find (category->name ()) != m_categories.end ());
    m_categories[category->name ()].second = state;
    enableCategory (category->name (), &m_iconViewArch, state);
}

void
IgControlCentre::selectIconItem (QIconViewItem* item)
{
    ASSERT (item);
    if (m_baseName == "")
      updateControlCentre (item->text ());
    else
      updateControlCentre (m_baseName + "/" + item->text ());
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Mark the current category dirty, enabling the "Apply" and
    "Revert" buttons.  */
void
IgControlCentre::dirtyCategory (void)
{
    m_apply->setEnabled (true);
    m_revert->setEnabled (true);
}

/** Mark the current category clean, disabling the "Apply" and
    "Revert" buttons.  */
void
IgControlCentre::cleanCategory (void)
{
    m_apply->setEnabled (false);
    m_revert->setEnabled (false);
}

QIconView *
IgControlCentre::iconView (void)
{
    return m_iconView;    
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgControlCentre::windowActivationChange  (bool oldActive)
{  
    QDialog::windowActivationChange (oldActive);
    if (isActiveWindow() && m_updateOnWindowActive)
    {
      if(m_current && m_categories [m_current->name ()].second == true)
      {
        m_current->focusIn ();
      }
      else if (m_focusChanged)
      {
        m_focusChanged = false;
	QString old (m_baseName);
	m_baseName = "qqqqqq";
        updateControlCentre (old);
      }
      else if(m_current)
      {
        m_current->focusIn ();
      }
      m_updateOnWindowActive = false;
    }
    else if (!isActiveWindow())
    {
      m_updateOnWindowActive = false;
      if ( m_current && m_categories [m_current->name ()].second == true)
        m_current->focusOut ();
    }
    else
      m_updateOnWindowActive = false;
}

void
IgControlCentre::updateOnWindowActive (void)
{ m_updateOnWindowActive = true; }

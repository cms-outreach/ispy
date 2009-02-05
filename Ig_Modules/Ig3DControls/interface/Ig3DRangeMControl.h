#ifndef IG_3D_CONTROLS_IG_3DRANGE_MCONTROL_H
# define IG_3D_CONTROLS_IG_3DRANGE_MCONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
# include <classlib/utils/DebugAids.h>
# include <qlayout.h>
# include <qlabel.h>
# include <qwhatsthis.h>
# include <qtooltip.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class C1, class C2, class C3>
class IG_3D_CONTROLS_API Ig3DRangeMControl : public Ig3DFieldControl
{
public:
    typedef Ig3DRangeMControl<C1, C2, C3> inherited;

    Ig3DRangeMControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    C1 *		field (void) const
    { return static_cast<C1 *> (Ig3DFieldControl::field ()); }
    
    void		setField (C1 *field)
    {
      Ig3DFieldControl::setField (field);
      if (field){
        m_value->resize (field->getNum ());
        m_value->setDirty (false);
      }
    }
    
    virtual const C2*	fieldValue (void) const
    { ASSERT (field ()); return field ()->getValues (0); }

    C3 *		control (void) const
    { return m_value; }
    
    virtual const C2*	value (void) const
    { return &(m_value->value ()[0]); }

    virtual void	setValue (unsigned int size, const C2* newval)
    { m_value->setValue (size, newval); setDirty (true); }

    virtual void	setValue (const std::vector<C2>& newval)
    { m_value->setValue (newval); setDirty (true); }

    QString	getSettingString (void) const
    { return control ()->getSettingString (); }
    
    void	setSettingString (QString &setting)
    { control ()->setSettingString (setting); }
    
protected:
    virtual void	updateValue (void)
    { setValue (m_value->size (), fieldValue ()); }
    
    virtual void	applyValue (void)
    { field ()->setValues (0, m_value->size (), value ()); }

private:
    C3	*m_value;
};

template <class C1, class C2, class C3>
Ig3DRangeMControl<C1, C2, C3>::Ig3DRangeMControl (IgControlCategory *pane,
                                    QString label,
				    int parts /* = IgQtRangeControl::ValuePart */)
    : Ig3DFieldControl (pane)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());

    QWidget	*area = pane->bodyArea ();
    int		row = gridRows ();

    ASSERT (row >= 0);

    // Add a new row to the grid, label on the left and the value on the right
    QWidget 	*vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, row, 0);
    QString tips ("Set the \"");
    tips += label + "\" of the item";
    QToolTip::add (vlab, tips.latin1 ());
    QWhatsThis::add (vlab, tips.latin1 ());

    m_value = new C3 (area, parts);
    addGridWidget (makeSpacedBox (area, m_value->widget ()), row, 1);

    // Always reflect the state of the real value control and vice versa.
    connect (m_value,            SIGNAL (dirty    ()), 
             this,               SLOT   (setDirty ()));
    connect (this,               SIGNAL (editable   (bool)), 
             m_value->widget (), SLOT   (setEnabled (bool)));
    connect (this,               SIGNAL (editable   (bool)), 
             vlab,               SLOT   (setEnabled (bool)));
    connect (this,               SIGNAL (clean    ()),
             m_value,            SLOT   (setClean ()));
    connect (pane,               SIGNAL (tracking          (bool)), 
             m_value,            SLOT   (resetChangedValue (bool)));
    connect (m_value,            SIGNAL (settingsChanged (void)),
             this,		 SLOT   (fieldSettingsChanged (void)));
}
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DRANGE_MCONTROL_H

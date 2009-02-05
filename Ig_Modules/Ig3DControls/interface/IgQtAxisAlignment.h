#ifndef IG_3D_CONTROLS_IG_QT_AXIS_ALIGNMENT_H
# define IG_3D_CONTROLS_IG_QT_AXIS_ALIGNMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgControlItem.h"
# include "Ig_Modules/Ig3DControls/interface/config.h"
# include <Inventor/SbLinear.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QButtonGroup;
class QPushButton;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
class IG_3D_CONTROLS_API IgQtAxisAlignment
    : public IgControlItem
{
    Q_OBJECT
public:
    IgQtAxisAlignment (QWidget *parent, float min = 0., float max = 1.,
		      const char *name = 0);

    SbVec3f		value		(void) const;
    QWidget*		widget		()     const;

    float		minAxisAlign	(void) const;
    float		maxAxisAlign	(void) const;

    bool		xAlign		(void) const;
    bool		yAlign		(void) const;
    bool		zAlign		(void) const;

public slots:
    virtual void	setMinAxisAlign	(float newval);
    virtual void	setMaxAxisAlign	(float newval);
    virtual void	setAlign	(unsigned short axis);
    virtual void	setXAlign	(void);
    virtual void	setYAlign	(void);
    virtual void	setZAlign	(void);
        
private:
    QButtonGroup	*m_widget;
    QPushButton		*m_xalign;
    QPushButton		*m_yalign;
    QPushButton		*m_zalign;
    unsigned short	m_alignAxis;
    float		m_min;
    float		m_max;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_QT_AXIS_ALIGNMENT_H

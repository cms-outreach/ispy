#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlCommon.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMControl : public IgQtRangeControlCommon
{
    Q_OBJECT
public:
    IgQtRangeMControl (QWidget *parent, int parts,
                       unsigned int size = 1,
		       const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    int			currentIndex (void) const;
    
    virtual void	resizeIndex (unsigned int size);

public slots:
    virtual void	newIndex (int index);

protected slots:
    virtual void	updateTextValue (void);

protected:

    QComboBox		*m_index;
    bool		m_indexPart;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_H

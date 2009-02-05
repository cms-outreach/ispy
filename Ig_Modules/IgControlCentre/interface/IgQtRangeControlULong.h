#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_ULONG_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_ULONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IG_CONTROL_CENTRE_API IgQtRangeControlULong : public IgQtRangeControlImp<unsigned long>
{
    Q_OBJECT
public:
    IgQtRangeControlULong (QWidget *parent, int parts, unsigned long value = 0,
		         const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    unsigned long	stringToNumber (const QString& val) const
    { return val.toULong (); }
    
signals:
    void		valueChanged (unsigned long delta);
    void		newValue (unsigned long newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_ULONG_H

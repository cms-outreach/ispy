#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_LONG_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_LONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IG_CONTROL_CENTRE_API IgQtRangeControlLong : public IgQtRangeControlImp<long>
{
    Q_OBJECT
public:
    IgQtRangeControlLong (QWidget *parent, int parts, long value = 0,
		         const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    long		stringToNumber (const QString& val) const
    { return val.toLong (); }
    
signals:
    void		valueChanged (long delta);
    void		newValue (long newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_LONG_H

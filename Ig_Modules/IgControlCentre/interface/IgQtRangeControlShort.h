#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_SHORT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_SHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IG_CONTROL_CENTRE_API IgQtRangeControlShort : public IgQtRangeControlImp<short>
{
    Q_OBJECT
public:
    IgQtRangeControlShort (QWidget *parent, int parts, short value = 0,
		           const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    short		stringToNumber (const QString& val) const
    { return val.toShort (); }
    
signals:
    void		valueChanged (short delta);
    void		newValue (short newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_SHORT_H

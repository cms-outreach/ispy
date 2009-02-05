#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_INT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_INT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IG_CONTROL_CENTRE_API IgQtRangeControlInt : public IgQtRangeControlImp<int>
{
    Q_OBJECT
public:
    IgQtRangeControlInt (QWidget *parent, int parts, int value = 0,
		         const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    int		stringToNumber (const QString& val) const
    { return val.toInt (); }
    
signals:
    void		valueChanged (int delta);
    void		newValue (int newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_INT_H

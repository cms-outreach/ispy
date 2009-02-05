#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_USHORT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_USHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeControlUShort : public IgQtRangeControlImp<unsigned short>
{
    Q_OBJECT
public:
    IgQtRangeControlUShort (QWidget *parent, int parts, unsigned short value = 0,
		          const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    unsigned short	stringToNumber (const QString& val) const
    { return val.toUShort (); }
    
signals:
    void		valueChanged (unsigned short delta);
    void		newValue (unsigned short newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_USHORT_H

#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_ULONG_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_ULONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IG_CONTROL_CENTRE_API IgQtRangeMControlULong : public IgQtRangeMControlImp<unsigned long>
{
    Q_OBJECT
public:
    IgQtRangeMControlULong (QWidget *parent, int parts,
                          unsigned long value = 0,
		          const char *name = 0);

    IgQtRangeMControlULong (QWidget *parent, int parts,
                          unsigned int size,
		          const unsigned long* values,
		          const char *name = 0);

    IgQtRangeMControlULong (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    unsigned long	stringToNumber (const QString& val) const
    { return val.toULong(); }
    
signals:
    void		valueChanged (std::vector<unsigned long> delta);
    void		newValue (std::vector<unsigned long> newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_ULONG_H

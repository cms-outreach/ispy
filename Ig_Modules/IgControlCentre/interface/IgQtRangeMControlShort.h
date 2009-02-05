#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_SHORT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_SHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IG_CONTROL_CENTRE_API IgQtRangeMControlShort : public IgQtRangeMControlImp<short>
{
    Q_OBJECT
public:
    IgQtRangeMControlShort (QWidget *parent, int parts,
                          short value = 0,
		          const char *name = 0);

    IgQtRangeMControlShort (QWidget *parent, int parts,
                          unsigned int size,
		          const short* values,
		          const char *name = 0);

    IgQtRangeMControlShort (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    short		stringToNumber (const QString& val) const
    { return val.toShort(); }
    
signals:
    void		valueChanged (std::vector<short> delta);
    void		newValue (std::vector<short> newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_SHORT_H

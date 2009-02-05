#ifndef IG_TABLE_MODEL_IG_SIMPLE_TABLE_MODEL_H
# define IG_TABLE_MODEL_IG_SIMPLE_TABLE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTableModel/interface/IgTableModel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TABLE_MODEL_API IgSimpleTableModel : public IgTableModel
{
public:
    IgSimpleTableModel (/* FIXME */);
    // default copy constructor
    // default destructor
    // default assignment operator

    // FIXME: model change listeners?  qt signals and slots?
    virtual void		listen (EventType event, const Listener &listener);
    virtual void		unlisten (EventType event, const Listener &listener);
    virtual void		changed (unsigned row, unsigned column);

private:
    // FIXME: a matrix of IgCell's?
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TABLE_MODEL_IG_SIMPLE_TABLE_MODEL_H

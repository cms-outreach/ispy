#ifndef IG_CONTROL_CENTRE_IG_CONTROL_ITEM_H
# define IG_CONTROL_CENTRE_IG_CONTROL_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/config.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgControlItem : public QObject
{
    Q_OBJECT
public:
    IgControlItem (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static QWidget *	makeSpacedBox	(QWidget *parent, QWidget *content);
    static QWidget *	makeBox       	(QWidget *parent, QWidget *content);

    virtual bool	isDirty (void) const;
    virtual bool	isEditable (void) const;

public slots:
    virtual void	setDirty (bool value);
    virtual void	setDirty (void);
    virtual void	setClean (void);
    virtual void	setEditable (bool value);

signals:
    void		dirty (void);
    void		clean (void);
    void		editable (bool value);

private:
    bool		m_editable;
    bool		m_dirty;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_CONTROL_ITEM_H

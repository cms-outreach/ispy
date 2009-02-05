#ifndef IG_STUDIO_IG_VIEW_CREATOR_H
# define IG_STUDIO_IG_VIEW_CREATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgViewDB.h"
# include <qobject.h>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgPage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: this can probably be done with callbacks.

class IG_STUDIO_API IgViewCreator : public QObject
{
    Q_OBJECT
public:
    IgViewCreator (IgViewDB::Iterator &i, IgPage *page);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
public slots:
void createView (void);
private:
    IgPage 		*m_page;
    IgViewDB::Iterator	m_iterator;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_VIEW_CREATOR_H

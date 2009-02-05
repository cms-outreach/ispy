#ifndef IG_QT_PROFILE_SERVICE_IG_QT_PROFILE_EXTENSION_H
# define IG_QT_PROFILE_SERVICE_IG_QT_PROFILE_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgQtProfileInfo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**This class gets loaded at startup time and adds a "Profile GUI" menu item to the main menubar.
 */

class IG_QT_PROFILE_SERVICE_API IgQtProfileExtension : public QObject, public IgExtension
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgQtProfileExtension);
public:
    IgQtProfileExtension (IgState *state);
    ~IgQtProfileExtension (void);
public slots:
/** Shows a popup window with the profiling GUI in it.
 */
    void showProfiler(void);
    // implicit copy constructor
    // implicit assignment operator
private:
    IgState 		*m_state;
    IgQtProfileInfo	*m_profileInfo;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_QT_PROFILE_EXTENSION_H

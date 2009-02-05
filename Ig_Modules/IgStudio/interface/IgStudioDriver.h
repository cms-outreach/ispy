#ifndef IG_STUDIO_IG_STUDIO_DRIVER_H
# define IG_STUDIO_IG_STUDIO_DRIVER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgDriver.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioSetupExtension;
class IgSite;
class QString;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgStudioDriver : public IgDriver
{
public:
    IgStudioDriver (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual int		run (void);
    virtual IgSite *	mainSite (void) const;
    virtual QWidget *	mainWindow (void) const;
    virtual IgState *	state (void) const;

    static const char *	catalogLabel (void);

protected:
    virtual void	loadSettings (void);
    virtual void	setupMainWindow (void);
    virtual bool	setupSession (QString sessionType);

private:
    std::vector<std::string> getAliases (const std::string& name,
                                         const std::string& type);
    IgState		*m_state;
    IgStudioSetupExtension *m_setup;
    IgSite		*m_mainSite;
    QWidget		*m_mainWindow;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_STUDIO_DRIVER_H

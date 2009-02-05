#ifndef IGGI_IG_EVENT_MENU_H
# define IGGI_IG_EVENT_MENU_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgExtension.h"
# include <qobject.h>
# include "classlib/zip/ZipArchive.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>

class IgTwig;

//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgEventMenu : public QObject, public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (IgEventMenu);
    Q_OBJECT
public:
    static const int MENU_ID_EVENT	= 7900;

    static const int MENU_INIT_APP	= 7902;
    static const int MENU_RE_INIT_APP	= 7903;
    static const int MENU_EVENT_NEXT	= 7904;
    static const int MENU_EVENT_PREVIOUS = 7905;
    static const int MENU_EVENT_AUTO	= 7906;
    static const int MENU_EVENT_NUM	= 7907;
    static const int MENU_EVENT_REWIND	= 7908;
    static const int MENU_EVENT_SKIP	= 7909;

    IgEventMenu (IgState *state);
    ~IgEventMenu (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		openEventFile (void);
    void		nextEvent (void);
    void		previousEvent (void);
    void		rewind (void);
    void		processEvent (unsigned int runNum, unsigned int eventNum);
    void		processEventDialog (void);
    void		skipEvent (long num);
    void		skipEventDialog (void);
    void 		readEventFile (const std::string &file);

private:
    void 		readZipMember (lat::ZipArchive::Iterator it);
    
    IgState		*m_state;
    lat::ZipArchive 	*m_archive;
    lat::ZipArchive::Iterator m_it;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_EVENT_MENU_H

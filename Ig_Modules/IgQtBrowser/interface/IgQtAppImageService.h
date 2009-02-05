#ifndef IG_QT_BROWSER_IG_QT_APP_IMAGE_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_IMAGE_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class QPixmap;
class QString;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppImageService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppImageService);    
public:
    IgQtAppImageService (IgState *state);
    ~IgQtAppImageService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    void	registerImage (const char *data[],
			       const int id);    
    void	registerImage (const QString &filename,
			       const int id);
    void	registerImage (const QPixmap *,
			       const int id);
    QPixmap	*pixmapById (const int id);
private:
    IgState	*m_state;    
    typedef std::map<int, QPixmap *> ImageMap;
    ImageMap	m_images;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_IMAGE_SERVICE_H

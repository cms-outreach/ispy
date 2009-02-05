//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppImageService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <classlib/utils/DebugAids.h>
#include <qpixmap.h>
#include <qstring.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppImageService, "Services/Qt/Images");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppImageService::IgQtAppImageService (IgState *state)
    :m_state (state)
{
    ASSERT (state);
    state->put (s_key, this);    
}

IgQtAppImageService::~IgQtAppImageService (void)
{
    m_state->detach (s_key);    
}

void
IgQtAppImageService::registerImage (const char *data[], const int id)
{
    QPixmap *pixmap = m_images[id];
    
    if (!pixmap)
    {	
	pixmap = new QPixmap (data);
	m_images[id] = pixmap;	
    }
}

void
IgQtAppImageService::registerImage (const QPixmap *map, const int id)
{
    QPixmap *pixmap = m_images[id];
    
    if (!pixmap)
    {	
	pixmap = new QPixmap (*map);
	m_images[id] = pixmap;	
    }
}

void
IgQtAppImageService::registerImage (const QString &name, const int id)
{
    QPixmap *pixmap = m_images[id];
    
    if (!pixmap)
    {	
	pixmap = new QPixmap (name);
	m_images[id] = pixmap;	
    }
}

QPixmap *
IgQtAppImageService::pixmapById (const int id)
{
    return m_images[id];    
}

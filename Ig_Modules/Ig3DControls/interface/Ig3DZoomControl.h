#ifndef IG_3D_CONTROLS_IG_3DZOOM_CONTROL_H
# define IG_3D_CONTROLS_IG_3DZOOM_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/config.h"
# include <qwidget.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DZoomControl : public QWidget
{
    Q_OBJECT
public:
    Ig3DZoomControl (QWidget *parent = 0, const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    unsigned int	zoomValue (void);

public slots:
    virtual void	zoomIn (void);
    virtual void	zoomOut (void);

signals:
    void		zoom (float);

private slots:
    virtual void	setZoom (const QString &item);

private:
    unsigned int	m_zoomValue;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DZOOM_CONTROL_H

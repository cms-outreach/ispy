#ifndef IG_TEST_IG_RZ_COORDSYSTEM_H
#define IG_TEST_IG_RZ_COORDSYSTEM_H

//<<<<<< INCLUDES                                                       >>>>>>

// # include <QAbstractGraphicsShapeItem>
# include "ISpyItem.h"
# include <QPainter>
# include <QStyleOptionGraphicsItem>
# include <QPointF>
# include <iostream>
# include <sstream>
# include <math.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// class IgRZCoordSystem : public QAbstractGraphicsShapeItem
class IgRZCoordSystem : public ISpyItem
{

public:
    IgRZCoordSystem (ISpyItem * parent = 0);
    ~IgRZCoordSystem (void);
	
    virtual QRectF boundingRect (void) const;
    virtual void   paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    virtual void   zoomIn (void);
    virtual void   zoomOut (void);

    void setCenter (const QPointF& value);
    void setColor (const QColor& value);
    void setDeltaEta (const float& value);
    void setDeltaHeight (const float& value);
    void setDeltaWidth (const float& value);
    void setMaxEta (const float& value);
    void setMaxHeight (const float& value);
    void setMaxWidth (const float& value);
    void setScaleFactor (const float& value);
    void setViewportSize (const float width, const float height);
	
    void showMeterLines (bool value = true);
    void showCentimeterLines (bool value = true);
    void showMillimeterLines (bool value = true);
    void showEtaLabels (bool value = true);
    void showEtaLines (bool value = true);

private:

    // lines which represent the border of the whole area
    QLineF horizontal;
    QLineF vertical;
    // some constant attributes used for the labels
    static const QPointF constCorrection;
    static const QSizeF labelBoxSize;
	
    void drawAxes (QPainter* painter, const QPen& pen);
    void drawEtaLines (QPainter* painter, const QPen& pen);
    void drawEtaLabels (QPainter* painter, const QPen& pen);
    void drawHeightLines (QPainter* painter, const QPen& pen, float delta);
    void drawWidthLines (QPainter* painter, const QPen& pen, float delta);
    void drawMeterLines (QPainter* painter, const QPen& pen);
    void drawCentimeterLines (QPainter* painter, const QPen& pen);
    void drawMillimeterLines (QPainter* painter, const QPen& pen);
	
    QPointF     convertCoordinates (const float radius, const float phi, const QPointF& center = QPointF (0.f,0.f)) const;
    float       etaToTheta (const float& eta) const;
    std::string floatToString (float a) const;
    QPointF     getPointOnRectangle (const float& eta, const int& signX, const int& signY, const QLineF& orientedLine) const;
    bool        isUpperEdgeEta (const float &eta) const;
    float       thetaToEta (const float& theta) const;
    void        updateVariables ();
	
    float   m_maxEta;
    float   m_maxWidth;
    float   m_maxHeight;
    float   m_deltaEta;
    bool    m_showMeterLines;
    bool    m_showCentimeterLines;
    bool    m_showMillimeterLines;
    QPointF m_center;
    QColor  m_color;
    bool    m_needVariablesUpdate;
    float   m_marginEta; // the boarder rectangle's eta value
    float   m_scaleFactor;
    bool    m_showEtaLabels;
    bool    m_showEtaLines;
    float   m_viewportHeight;
    float   m_viewportWidth;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

inline void
IgRZCoordSystem::setMaxEta (const float& value)
{
    if (value > 0.f)
    {
	m_maxEta = value;
    }
    else
    {
	m_maxEta = 5.f;
	std::cerr << value << " is an invalid value for maxEta. It has now been set to " << m_maxEta << std::endl;
    }
    update ();
}

inline void
IgRZCoordSystem::setMaxWidth (const float& value)
{
    if (value > 0.f)
    {
	m_maxWidth = value;
    }
    else
    {
	m_maxWidth = 12.f;
	std::cerr << value << " is an invalid value for maxWidth. It has now been set to " << m_maxWidth << std::endl;
    }
    m_needVariablesUpdate = true;
    update ();
}

inline void
IgRZCoordSystem::setMaxHeight (const float& value)
{
    if (value > 0.f)
    {
	m_maxHeight = value;
    }
    else
    {
	m_maxHeight = 6.f;
	std::cerr << value << " is an invalid value for maxHeight. It has now been set to " << m_maxHeight << std::endl;
    }
    m_needVariablesUpdate = true;
    update ();
}

inline void
IgRZCoordSystem::setDeltaEta (const float& value)
{
    if (value > 0.f)
    {
	m_deltaEta = value;
    }
    else
    {
	m_deltaEta = 0.1f;
	std::cerr << value << " is an invalid value for deltaEta. It has now been set to " << m_deltaEta << std::endl;
    }
    update ();
}

inline void
IgRZCoordSystem::setScaleFactor (const float& value)
{
    if (value > 0.f)
    {
	m_scaleFactor = value;
    }
    else
    {
	m_scaleFactor = 1.f;
	std::cerr << value << " is an invalid value for scaleFactor. It has now been set to " << m_scaleFactor << std::endl;
    }
    m_needVariablesUpdate = true;
    update ();
}

inline void
IgRZCoordSystem::setViewportSize (const float width, const float height)
{
    m_viewportHeight = height;
    m_viewportWidth = width;
    m_needVariablesUpdate = true;
    update ();
}

inline void
IgRZCoordSystem::showMeterLines (bool value /*= true*/)
{
    m_showMeterLines = value;
    update ();
}

inline void
IgRZCoordSystem::showCentimeterLines (bool value /*= true*/)
{
    m_showCentimeterLines = value;
    update ();
}

inline void
IgRZCoordSystem::showMillimeterLines (bool value /*= true*/)
{
    m_showMillimeterLines = value;
    update ();
}

inline void 
IgRZCoordSystem::showEtaLabels (bool value /*= true*/)
{
    m_showEtaLabels = value;
    update ();
}

inline void 
IgRZCoordSystem::showEtaLines (bool value /*= true*/)
{
    m_showEtaLines = value;
    update ();
}

inline void
IgRZCoordSystem::setColor (const QColor& value)
{
    m_color = value;
    update ();
}

inline void
IgRZCoordSystem::setCenter (const QPointF& value)
{
    m_center = value;
    update ();
}

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline float
IgRZCoordSystem::etaToTheta (const float& eta) const
{
    return (2.f * atan (exp (- eta)));
}

inline float
IgRZCoordSystem::thetaToEta (const float& theta) const
{
    return (-log (tan (theta/2.f)));
}

inline bool
IgRZCoordSystem::isUpperEdgeEta (const float& eta) const
{
    return (eta <= m_marginEta);
}



#endif // IG_TEST_IG_RZ_COORDSYSTEM_H

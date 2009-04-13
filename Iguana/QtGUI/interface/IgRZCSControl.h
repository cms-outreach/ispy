#ifndef IG_TEST_IG_RZ_CS_CONTROL_H
#define IG_TEST_IG_RZ_CS_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "IgRZCoordSystem.h"
#include "ui_IgRZCSControl.h"
#include "ISpyController.h"
#include <QDialog>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgRZCSControl : public ISpyController
{
    Q_OBJECT
public:
    IgRZCSControl (IgRZCoordSystem* cs = 0, ISpyController* parent = 0);
    ~IgRZCSControl ();
	
    virtual void setControlledItem (ISpyItem* item);

public slots:
void setShowMeter (int value);
    void setShowCentimeter (int value);
    void setShowMillimeter (int value);
    void setShowLabels (int value);
    void setShowEtaLines (int value);
    void setMaxEta (double value);
    void setDeltaEta (double value);

private:
    IgRZCoordSystem* m_ctrdItem;
	
    Ui::IgRZCSControl ui;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IG_TEST_IG_RZ_CS_CONTROL_H

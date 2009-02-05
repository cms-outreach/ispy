#ifndef IG_TEST_IG_POLAR_CS_CONTROL_H
#define IG_TEST_IG_POLAR_CS_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "IgPolarCoordSystem.h"
#include "ui_IgPolarCSControl.h"
#include "IggiController.h"
// #include <QDialog>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgPolarCSControl : public IggiController
{
	Q_OBJECT
public:
	IgPolarCSControl (IgPolarCoordSystem* cs = 0, IggiController* parent = 0);
	~IgPolarCSControl ();
	
// 	void setCoordSystem (IgPolarCoordSystem* cs);
	virtual void setControlledItem (IggiItem* item);

public slots:
	void setDeltaPhi (double value);
	void setEndRadius (double value);
	void setEnergyUnit (double value);
	void setShowAngleLabels (int value);
	void setShowMinorAngles (int value);
	void setShowUnitLabel (int value);
	void setStartRadius (double value);
	void setUnitLabel (const QString& value);

private:
	IgPolarCoordSystem* m_ctrdItem;
	
	Ui::IgPolarCSControl ui;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IG_TEST_IG_POLAR_CS_CONTROL_H

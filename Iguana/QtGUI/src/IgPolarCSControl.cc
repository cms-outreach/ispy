//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/QtGUI/interface/IgPolarCSControl.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLabel>
#include <math.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPolarCSControl::IgPolarCSControl (IgPolarCoordSystem* cs /*= 0*/, ISpyController* parent /*= 0*/) 
	: ISpyController (parent),
	  m_ctrdItem (cs)
{
	ui.setupUi (this);
// 	m_unitLabel = new QCheckBox ("Unit label", this);
// 	m_unitText = new QLineEdit ("GeV", this);
// 	m_angleLabels = new QCheckBox ("Angle labels", this);
// 	m_minorAngles = new QCheckBox ("Minor angles", this);
// 	m_deltaPhi = new QDoubleSpinBox (this);
// 	m_energyUnit = new QDoubleSpinBox (this);
// 	
// 	m_unitLabel->setCheckState (Qt::Checked);
// 	m_angleLabels->setCheckState (Qt::Checked);
// 	m_minorAngles->setCheckState (Qt::Checked);
// 
// 	m_deltaPhi->setRange (5., 360.);
// 	m_deltaPhi->setValue (22.5);
// 	m_deltaPhi->setDecimals (2);
// 
// 	m_energyUnit->setRange (0.001, 1000.);
// 	m_energyUnit->setValue (10.);
// 	m_energyUnit->setDecimals (3);
// 	
// 	// no need to delete these variables since we pass them to a parent layout
// 	QHBoxLayout* unitLabelBox = new QHBoxLayout;
// 	unitLabelBox->addWidget (m_unitLabel);
// 	unitLabelBox->addWidget (m_unitText);
// 	
// 	QHBoxLayout* phiBox = new QHBoxLayout;
// 	phiBox->addWidget (m_minorAngles);
// 	phiBox->addWidget (m_deltaPhi);
// 	
// 	QHBoxLayout* unitBox = new QHBoxLayout;
// 	unitBox->addWidget (new QLabel ("Unit Distance", this));
// 	unitBox->addWidget (m_energyUnit);
// 	
// 	QVBoxLayout *layout = new QVBoxLayout;
// 	layout->addLayout (unitLabelBox);
// 	layout->addWidget (m_angleLabels);
// 	layout->addWidget (m_minorAngles);
// 	layout->addLayout (phiBox);
// 	layout->addLayout (unitBox);
// 	setLayout (layout);
// 	
// 	connectBoxes ();
}

IgPolarCSControl::~IgPolarCSControl ()
{}

// void
// IgPolarCSControl::setCoordSystem (IgPolarCoordSystem* cs)
// {
// 	m_ctrdItem = cs;
// }

void 
IgPolarCSControl::setControlledItem (ISpyItem* item)
{
	m_ctrdItem = dynamic_cast<IgPolarCoordSystem*>(item);
}

void
IgPolarCSControl::setShowUnitLabel (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showUnitLabel (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showUnitLabel (true);
		}
	}
}

void
IgPolarCSControl::setShowAngleLabels (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showAngleLabels (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showAngleLabels (true);
		}
	}
}

void
IgPolarCSControl::setShowMinorAngles (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showMinorAngles (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showMinorAngles (true);
		}
	}
}

void
IgPolarCSControl::setDeltaPhi (double value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setLineAngle (value * M_PI / 180.);
}

void
IgPolarCSControl::setEnergyUnit (double value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setE2Uratio (value);
}

void
IgPolarCSControl::setUnitLabel (const QString& value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setUnitLabel (value);
}

void
IgPolarCSControl::setEndRadius (double value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setEndRadius (value);
}

void
IgPolarCSControl::setStartRadius (double value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setStartRadius (value);
}


//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/Iggi/interface/IgRZCSControl.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRZCSControl::IgRZCSControl (IgRZCoordSystem* cs /*= 0*/, IggiController* parent /*= 0*/) 
	: IggiController (parent),
	m_ctrdItem (cs)
{
	ui.setupUi (this);
}

IgRZCSControl::~IgRZCSControl ()
{}

void 
IgRZCSControl::setControlledItem (IggiItem* item)
{
	m_ctrdItem = dynamic_cast<IgRZCoordSystem*>(item);
}

void
IgRZCSControl::setShowMeter (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showMeterLines (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showMeterLines (true);
		}
	}
}

void
IgRZCSControl::setShowCentimeter (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showCentimeterLines (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showCentimeterLines (true);
		}
	}
}

void
IgRZCSControl::setShowMillimeter (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showMillimeterLines (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showMillimeterLines (true);
		}
	}
}

void
IgRZCSControl::setShowLabels (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showEtaLabels (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showEtaLabels (true);
		}
	}
}

void
IgRZCSControl::setShowEtaLines (int value)
{
	if (m_ctrdItem != 0)
	{
		if (value == Qt::Unchecked)
		{
			m_ctrdItem->showEtaLines (false);
		}
		else if (value ==  Qt::Checked)
		{
			m_ctrdItem->showEtaLines (true);
		}
	}
}

void
IgRZCSControl::setMaxEta (double value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setMaxEta (value);
}

void
IgRZCSControl::setDeltaEta (double value)
{
	if (m_ctrdItem != 0)
		m_ctrdItem->setDeltaEta (value);
}

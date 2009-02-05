//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgWizardPage.h"
#include "Ig_Modules/IgStudio/interface/IgWizard.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgWizardPage::IgWizardPage (IgWizard *owner, const char *name, WFlags f)
    : QVBox (owner, name, f),
      m_wizard (owner)
{
    setMargin (11);
    setSpacing (6);
}

void
IgWizardPage::enter (void)
{}

void
IgWizardPage::leave (void)
{}

void
IgWizardPage::commit (void)
{}

void
IgWizardPage::help (void)
{}

QString
IgWizardPage::title (void) const
{ return ""; }

bool
IgWizardPage::isAppropriate (void) const
{ return true; }

bool
IgWizardPage::isBackAvailable (void) const
{ return true; }

bool
IgWizardPage::isNextAvailable (void) const
{ return true; }

bool
IgWizardPage::isFinishAvailable (void) const
{ return false; }

bool
IgWizardPage::isHelpAvailable (void) const
{ return false; }

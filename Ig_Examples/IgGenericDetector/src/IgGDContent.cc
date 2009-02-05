//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgGenericDetector/interface/IgGDContent.h"
#include "Ig_Examples/IgGenericDetector/interface/IgGDTwig.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/IgCommonOps/interface/IgTwigOps.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuMessage.h"
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgGDContent, "Data/Generic/Detector");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgGDContent::IgGDContent (IgState *state)
    : m_state (state)
{
    ASSERT (m_state);
    qApp->lock ();
    IgTwig *root = IgDocumentData::get (m_state)->root ();
    
    new Detector(root, m_state);
    new	Event (root, m_state);
    new	testCube (root);
    
    ASSERT (IgQtObjectMenuService::get (state));
    IgQtObjectMenuService::get (state)
        ->add (lat::CreateCallback (new IgTwigOps, &IgTwigOps::menuAction));    
    qApp->unlock (false);
}

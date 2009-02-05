//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgViewCreator.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Modules/IgStudio/interface/IgView.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgViewCreator::IgViewCreator (IgViewDB::Iterator &i, IgPage *page)
    : m_page (page),
      m_iterator (i)
{
}

void
IgViewCreator::createView (void)
{
    // FIXME: IgView API is pretty small so that we have to explicitly
    // call show () in the Ig*View constructor in order to be able to
    // show the MDI window. It would be nice to have IgView::show() so
    // that view writers don't have to call it... But maybe
    // not...Let's wait for Lassi to see what he thinks...
    IgView* view = (*m_iterator)->create (m_page);
    if (!view->state ())
      delete view;
}

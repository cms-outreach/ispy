//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgSimpleTextRep.h"
#include "Iguana/QtGUI/interface/IgTextModel.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSimpleTextRep::IgSimpleTextRep (IgTextModel *model, const std::string &text)
    : m_model (model),
      m_context (0),
      m_text (text)
{
}

IgSimpleTextRep::~IgSimpleTextRep (void)
{
    // The destruction may come upon me in two ways: the object is
    // going away and the context is deleting me, or someone else
    // killed me.  In the former case the caller will have done the
    // right thing of getting rid of references to me.  However if my
    // context *was* deleted, I must not delete it again; if so, it
    // has already set its rep to null, which we use as a clue.

    ASSERT (m_context);
    if (m_context->rep ())
    {
	ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;
    }
}

IgRepContext *
IgSimpleTextRep::context (void) const
{ return m_context; }

void
IgSimpleTextRep::context (IgRepContext *context)
{ ASSERT (! m_context); ASSERT (context); m_context = context; }

IgModel *
IgSimpleTextRep::model (void) const
{ return m_model; }

const std::string &
IgSimpleTextRep::text (void) const
{ return m_text; }

void
IgSimpleTextRep::setText (const std::string &text)
{ m_text = text; if (m_model->text () == this) m_model->changed (); }

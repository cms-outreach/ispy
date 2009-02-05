//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTreeModel/interface/IgTreeModel.h"
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

IgTreeModel::IgTreeModel (IgTreeNode * /* root = 0 */)
{}

IgTreeModel::~IgTreeModel (void)
{}

void
IgTreeModel::listen (EventType /* event*/, const Listener & /*listener*/)
{ ASSERT (false); }

void
IgTreeModel::unlisten (EventType /*event*/, const Listener & /*listener*/)
{ ASSERT (false); }

void
IgTreeModel::changed (const IgTreePath * /*path*/, IgTreeNode * /*value*/)
{ ASSERT (false); }


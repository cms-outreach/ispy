//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgAjaxTreeModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgXMLModel.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include <iostream>
#include <qapplication.h>
#include <qdom.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int 
main (int argc, char **argv)
{
    new QApplication (argc, argv);
    IgSimpleTwig *t1 = new IgSimpleTwig ("foo");
    IgSimpleTwig *t2 = new IgSimpleTwig (t1,"bar");
    IgSimpleTwig *t3 = new IgSimpleTwig (t1,"pippo");
    new IgSimpleTwig (t2,"bar1");
    new IgSimpleTwig (t2,"bar2");
    new IgSimpleTwig (t2,"bar3");
    new IgSimpleTwig (t3,"pippo1");
    new IgSimpleTwig (t3,"pippo2");
    new IgSimpleTwig (t3,"pippo3");
    IgAjaxTreeModel *model = new IgAjaxTreeModel (new IgUIDModel ());
    //    IgRepSet::lookup (t1, model, true);
    IgRepSet::lookup (t1->lookup ("bar"), model, true);
    IgRepSet::update (t1, model, IgTwig::SELF_MASK| IgTwig::STRUCTURE_MASK);    
    
    std::cerr << model->document ()->toString () << std::endl;    
}


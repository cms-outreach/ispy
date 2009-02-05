//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfPopupMenu.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfPopupMenu::IgOProfPopupMenu (void)
{
    insertItem("Mask out",POPUP_MASK_OUT);
    insertItem("Show source",POPUP_SHOW_SOURCE);
    insertItem("Print",POPUP_PRINT);
    insertItem("Save to file",POPUP_SAVE_TO_FILE);
    insertItem("Set offset",POPUP_SET_OFFSET);
    insertItem("Show only this one", POPUP_SET_MASK);		
    setItemEnabled(POPUP_SHOW_SOURCE,false);
    setItemEnabled(POPUP_MASK_OUT,false);
    setItemEnabled(POPUP_PRINT,false);
}

#ifndef __IG_OPROF_POPUP_MENU__
#define __IG_OPROF_POPUP_MENU__

#include <qpopupmenu.h>

class IgOProfPopupMenu : public QPopupMenu
{
public:
    enum POPUP_ENUM
    {
	POPUP_MASK_OUT,
	POPUP_SHOW_SOURCE,
	POPUP_PRINT,
	POPUP_SAVE_TO_FILE,
	POPUP_SET_OFFSET,
	POPUP_SET_MASK
    };
    
    IgOProfPopupMenu(void);
};

#endif

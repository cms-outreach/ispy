// For the time being this is just for keeping track of buttons.
window.PopupMenuService = function (state)
{
	if (arguments.length > 0)
	{
		this.init (state);	
	}
}

PopupMenuService = window.PopupMenuService;

PopupMenuService.method ('init', function (state) {
	this.state = state;
	state.services["PopupMenuService"] = this;
	this.availableMenus = {};
});

PopupMenuService.method ('registerMenu', function (menu, label) {
	this.availableMenus[label] = menu;
});

PopupMenuService.method ('currentMenu', function (menuLabel, targetItem, x, y) {
	var menu = this.availableMenus[menuLabel];
	if (menu)
	{
    	menu.targetItem = targetItem;
    	document.documentElement.currentPopupMenu = menu;
        menu.div.style["left"] = x + "px";
        menu.div.style["top"] = y + "px";
    	menu.show ();
    	Evt.addEventListener (document.documentElement, 
    				"click", 
    				PopupMenuService.prototype.hideMenu,
    				true);
    }
	return false;
});

PopupMenuService.method ('hideMenu', function (event) {
	document.documentElement.currentPopupMenu.hide ();
	Evt.removeEventListener (document.documentElement, 
				"click", 
				PopupMenuService.prototype.hideMenu);
	return false;
});

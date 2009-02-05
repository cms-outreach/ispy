window.PopupMenuItem = function (parent, label, callback)
{
	if (arguments.length > 0)
	{
		this.init (parent, label, callback);
	}
}

PopupMenuItem = window.PopupMenuItem;

importClass ("Studio.Widget");

PopupMenuItem.inherits (Widget);

PopupMenuItem.method ('init', function (parent, label, callback){
	this.uber ('init', parent, 1);
	this.label = document.createTextNode (label);
    //FIXME: only works for 1 level menus.
	this.div.menu = parent;
	this.div.appendChild (this.label);
	this.div.clickCallback = callback;
    this.div.style["fontFamily"] = "sans-serif";
    this.div.style["fontSize"] = "80%";
    this.div.style["paddingTop"] = "2px";
    this.div.style["paddingBottom"] = "1px";
    this.div.style["paddingLeft"] = "2px";
	Evt.addEventListener (this.div, "click", PopupMenuItem.prototype.onClick, true);
	Evt.addEventListener (this.div, "mouseover", PopupMenuItem.prototype.onMouseOver, true);
	Evt.addEventListener (this.div, "mouseout", PopupMenuItem.prototype.onMouseOut, true);
});

PopupMenuItem.method ('onMouseOver', function (event)
{
	this.oldColor = this.style["backgroundColor"];
	this.style["backgroundColor"] = "#FF0000";
	this.style["color"] = "#FFFFFF";
});

PopupMenuItem.method ('onMouseOut', function (event)
{
	this.style["backgroundColor"] = this.oldColor;
	this.style["color"] = "#000000";
});

PopupMenuItem.method ('onClick', function (event){
	this.clickCallback.call (this.menu.state, this.menu.targetItem);
});

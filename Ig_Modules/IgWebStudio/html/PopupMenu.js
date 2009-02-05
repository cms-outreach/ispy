// For the time being this is just for keeping track of buttons.
window.PopupMenu = function (title, state)
{
	if (arguments.length > 0)
	{
		this.init (title, state);	
	}
}

PopupMenu = window.PopupMenu;
importClass ("Studio.Widget");

PopupMenu.inherits (Widget);

PopupMenu.method ('init', function (title, state) {
	this.uber ('init', document.documentElement, 1);
	this.state = state;
	this.oddChild = true;
	this.title = document.createElement ("DIV");
	this.title.style["backgroundColor"] = "#0140CA";	
	this.title.style["color"] = "#FFFFFF";	
	this.title.style ["fontFamily"] = "sans-serif";
	this.title.style ["fontSize"] = "90%";
	this.title.appendChild (document.createTextNode (title));

	this.div.style["border"] = "solid";
	this.div.style["borderWidth"] = "1px";

	this.div.appendChild (this.title);	
	this.div.style.position = 'absolute';
	this.div.style.zIndex = '1';
	this.hide ();
});

PopupMenu.method ('setupChild', function (child)
{
	this.uber ('setupChild', child, 1);
	if (this.oddChild == true)
	{
		child.div.style["backgroundColor"] = "#E5ECF9";
		this.oddChild = false;
	}
	else
	{
		child.div.style["backgroundColor"] = "#D5DCE8";
		this.oddChild = true;
	}

});


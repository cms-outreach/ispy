window.Toolbar3D = function (state, parent)
{
	if (arguments.length > 0)
	{
		this.init (state, parent);
	}
}

Toolbar3D = window.Toolbar3D;

importClass ("Studio.Toolbar");
importClass ("Studio.ToolTip");
importClass ("Studio.ToolbarButton");

Toolbar3D.inherits (Toolbar);

Toolbar3D.method ('init', function (state, parent)
{
	this.uber ('init', state, parent, "Toolbar3D", "3D Toolbar");
	this.state = state;
	this.parent = parent;
	var toolbarService = state.lookup ("Toolbar Manager");

	var parentDiv = this.contents;
	var baseUrl = "../Studio/getPic?name=";
	var button = null;

	button = new ToolbarButton (parentDiv, baseUrl + "stock_draw-sphere.png", "View all");
    new ToolTip (button, "View all");
	this.addButton ("VIEW_ALL", button);

	button = new ToolbarButton (parentDiv, baseUrl + "stock_up.png", "Up");
    new ToolTip (button, "Move up");
	this.addButton ("UP", button);

	button = new ToolbarButton (parentDiv, baseUrl + "stock_down.png", "Down");
    new ToolTip (button, "Move down");
	this.addButton ("DOWN", button);

	button = new ToolbarButton (parentDiv, baseUrl + "stock_left.png", "Left");
    new ToolTip (button, "Move left");
	this.addButton ("LEFT", button);

	button = new ToolbarButton (parentDiv, baseUrl + "stock_right.png", "Right");
    new ToolTip (button, "Move right");
	this.addButton ("RIGHT", button);

	button = new ToolbarButton (parentDiv, baseUrl + "stock_zoom-in.png", "Zoom in");
    new ToolTip (button, "Zoom in");
	this.addButton ("ZOOM_IN", button);

	button = new ToolbarButton (parentDiv, baseUrl + "stock_zoom-out.png", "Zoom out");
    new ToolTip (button, "Zoom out");
	this.addButton ("ZOOM_OUT", button);
});

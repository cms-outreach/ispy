importClass ("Studio.Widget");

window.Workspace = function (state, parent)
{
	if (arguments.length > 0)
	{
		this.init (state, parent);
	}
}

Workspace = window.Workspace;

Workspace.inherits (Widget);

Workspace.method ('init', function (state, parent) {

	this.uber ('init', parent, 1);
    this.windows = {};
	this.state = state;
    
    this.div.style ["paddingTop"] = "6px";
    this.div.style ["padding"] = "10px";
    this.div.style ["verticalAlign"] = "top";
    this.div.style ["width"] = "100%";
    this.div.style ["height"] = "100%";
});


Workspace.method ('setupChild', function (child) {
	child = this.uber ('setupChild', child, 1);
	child.style["position"] = "relative";		
	child.style["cssFloat"] = "left";
    return child;
});

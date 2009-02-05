importClass ("Studio.Widget");

window.Row = function (parent, oldParent)
{
	if (arguments.length > 0)
	{
	    // FIXME: hack required for compatibility with old API.
	    // FIXME: put an ASSERT (!oldParent); at some point.
	    if (oldParent)
	    {
	        parent = oldParent;
	    }
		this.init (parent);
	}	
}

Row = window.Row;

Row.inherits (Widget);

Row.method ('init', function (parent) {    
	this.uber ('init', parent, 1);

    this.div.style["padding"] = "2px";
    this.div.style["width"] = "100%";
});

Row.method ("setupChild", function (child)
{
    child = this.uber ("setupChild", child, 1);
    
    child.style["position"] = "static";
	child.style["cssFloat"] = "left";
	
	return child;
});

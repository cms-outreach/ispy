importClass ("Studio.Widget");

window.Column = function (parent, oldParent)
{
	if (arguments.length > 0)
	{
	    //FIXME: hack required for old API requiring state
	    //FIXME: put an ASSERT (!oldParent) at some point.
	    if (oldParent)
	    {
	        parent = oldParent;
	    }
	        
		this.init (parent);
	}
}

Column = window.Column;

Column.inherits (Widget);

Column.method ('init', function (parent) {
	this.uber ('init', parent, 1);

	this.div.style["padding"] = "2px";
	this.div.style.height = "100%"; 
});

Column.method ('setupChild', function (child) {
	child = this.uber ('setupChild', child, 1);
	child.style["position"] = "static";
//	child.style["cssFloat"] = "none";
//    var br = document.createElement ("br")
//    NodeUtils.insertAfter (br, child);
	return child; 
});

//return Column;
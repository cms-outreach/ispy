window.ToolbarButton = function (parent, iconUrl, label, callback)
{
    if (arguments.length > 0)
    {
        this.init (parent, iconUrl, label, callback);
    }
}

ToolbarButton = window.ToolbarButton;

ToolbarButton.method ("init", function (parent, iconUrl, label, callback)
{
    //this.setOnClickCallback = ToolbarButton.setOnClickCallback;
	//this.onClick = ToolbarButton.onClick;
	this.onClickCallback = null;
	this.icon = document.createElement ("img");
 	this.icon.onClickCallback = new EmptyCallback_1 ();
	var a = document.createElement ("a");
 	this.icon.foo = "foo";
	Evt.addEventListener (a, "mousedown", ToolbarButton.prototype.onMouseDown, false);
	this.icon.src = iconUrl;
 	this.icon.setAttribute ("alt", label); 
 	a.appendChild (this.icon);
 	parent.appendChild (a);    
});

ToolbarButton.method ("setOnClickCallback", function (callback)
{
	this.icon.onClickCallback = callback;
});

ToolbarButton.method ("onClick", function (e) {
    var evt = new Evt (e);
    var source = evt.getSource ();
	source.onClickCallback.call ();
});

ToolbarButton.method ("onMouseDown", function (e) {
    var evt = new Evt (e);
    var source = evt.getSource ();
	source.onClickCallback.call ();
});

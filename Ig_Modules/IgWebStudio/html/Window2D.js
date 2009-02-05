window.Window2D = function (state, parent, id, title)
{
    if (arguments.length > 0)
    {
        this.init (state, parent, id, title);
    }
}

Window2D = window.Window2D;
importClass ("Studio.Window")
Window2D.inherits (Window);

Window2D.method ('init', function (state, parent, id, title)
{
    this.uber ('init', state, parent, id, title, "Window2D.prototype.onloadCallback (this, '" + id + "')");
});

Window2D.method ('onloadCallback',  function (placeHolder, id) {
	placeHolder.setAttribute ("onload", "");
	if (placeHolder == null)
		return;
	var parent = placeHolder.parentNode;
	new Browser2D (parent, id, 156, 156);
	parent.removeChild (placeHolder);
});

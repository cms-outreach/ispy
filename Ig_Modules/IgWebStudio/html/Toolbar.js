window.Toolbar = function (state, parent, id, title)
{
	if (arguments.length > 0)
	{
		this.init (state, parent, id, title);
	}
}

Toolbar = window.Toolbar;

Toolbar.method ('init', function (state, parent, id, title)
{
	state.services ["Toolbars/" + id] = this;
	this.panel = new Panel (state, parent, id, title, "Toolbar.prototype.onCreation (this,'" + id + "')");
	this.contents = this.panel.contentsDiv;
	this.buttons = new Function ();
});

Toolbar.method ('addButton', function (buttonId, button)
{
	if (! this.buttons [buttonId])
	{
		this.buttons [buttonId] = button;
	}
	
	return this.buttons [buttonId];
});

Toolbar.method ('onCreation', function (placeHolder, id)
{
	placeHolder.setAttribute ("onload", "");
	if (placeHolder == null)
		return;
	var parent = placeHolder.parentNode;
	parent.removeChild (placeHolder);
});

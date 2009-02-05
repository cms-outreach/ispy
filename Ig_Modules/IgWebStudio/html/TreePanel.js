importClass ("Studio.Panel")

window.TreePanel =  function (state, parent, id, title)
{
    if (arguments.length > 0)
    {
        this.init (state, parent, id, title);
    }
}

TreePanel = window.TreePanel;

TreePanel.inherits (Panel);

TreePanel.method ('init', function (state, parent, id, title) {
	this.uber ('init', state, parent, id, title, "TreePanel.prototype.onloadCallback (this, '" + id + "')");
    importClass ("Studio.TreeBrowser")

	this.browser = new TreeBrowser (this, id, "/", state);
});

TreePanel.method ('onloadCallback', function (placeHolder) {
	placeHolder.setAttribute ("onload", "");
	if (placeHolder == null)
		return;
	var parent = placeHolder.parentNode;
	parent.removeChild (placeHolder);
});
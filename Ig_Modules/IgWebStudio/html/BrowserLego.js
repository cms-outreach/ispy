window.BrowserLego = function (parent, id, width, height)
{
	if (arguments.length > 0)
	{
		this.init (parent, id, width, height);
	}
}

BrowserLego = window.BrowserLego;
importClass ("Studio.Browser3DBase");

BrowserLego.inherits (Browser3DBase);

BrowserLego.method ('init', function (parent, id, width, height) {
	this.uber ('init', parent, id, width, height, "Lego");
});

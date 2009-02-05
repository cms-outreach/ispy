window.BrowserRPhi = function (parent, id, width, height)
{
	if (arguments.length > 0)
	{
		this.init (parent, id, width, height);
	}
}

BrowserRPhi = window.BrowserRPhi;

importClass ("Studio.Browser3DBase");

BrowserRPhi.inherits (Browser3DBase);

BrowserRPhi.method ('init', function (parent, id, width, height) {
	this.uber ('init', parent, id, width, height, "RPhi");
});

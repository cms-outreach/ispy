window.BrowserRZ = function (parent, id, width, height)
{
	if (arguments.length > 0)
	{
		this.init (parent, id, width, height);
	}
}

BrowserRZ = window.BrowserRZ;

importClass ("Studio.Browser3DBase");

BrowserRZ.inherits (Browser3DBase);

BrowserRZ.method ('init', function (parent, id, width, height) {
	this.uber ('init', parent, id, width, height, "RZ");
});

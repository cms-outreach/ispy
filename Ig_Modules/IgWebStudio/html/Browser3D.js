window.Browser3D = function (parent, id, width, height)
{
	if (arguments.length > 0)
	{
		this.init (parent, id, width, height);
	}
};

Browser3D = window.Browser3D;
importClass ("Studio.Browser3DBase");

Browser3D.inherits (Browser3DBase);

Browser3D.method ('init', function (parent, id, width, height)
{
	this.uber (parent, id, width, height, "3D");
});

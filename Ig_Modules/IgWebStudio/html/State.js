window.State = function (parent)
{
	if (parent)
	{
		parent.children.push (this);
		this.parent = parent;
	}
	this.services = new Function ();
	this.children = new Array ();	
}

State = window.State;

State.method ("lookup", function (serviceName) {
	if (this.services[serviceName])
	{	
		return this.services[serviceName];
	}
	if (this.parent)
	{
		return this.parent.lookup (serviceName);
	}
	return null;
});

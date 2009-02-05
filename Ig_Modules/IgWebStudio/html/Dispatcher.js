window.Dispatcher = function ()
{
	this.observerList = new Array ();
}

Dispatcher = window.Dispatcher;

Dispatcher.method ('addObserver', function (observerCallback)
{
	this.observerList.push (observerCallback);
});

Dispatcher.method ('broadcasts', function ()
{
	for (var i = 0; i < this.observerList.length; i++)
	{
		this.observerList[i].call ();
	}
});

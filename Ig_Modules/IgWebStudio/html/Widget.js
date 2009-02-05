window.Widget = function (parent)
{
	if (arguments.length > 0)
	{
		this.init (parent);
	}
}

Widget = window.Widget;

Widget.method ('init', function (parent)
{	
	this.div = document.createElement ("div");
	if (parent)
	{
		if (parent.setupChild != undefined)
		{
			parent.setupChild (this);
		}
		else
		{
			// Parent it's a div...
			parent.appendChild (this.div);
		}
	}
});

window.WidgetsUtils =  function ()
{    
}

WidgetsUtils = window.WidgetsUtils;

WidgetsUtils.extractDiv = function (wannabe) {
    if (wannabe.div) 
        return wannabe.div;
    else
        return wannabe;
};

Widget.method ('setupChild', function (child)
{	
    child = WidgetsUtils.extractDiv (child);
//    child.style ["cssFloat"] = "none";

	this.div.appendChild (child);
	return child;
});

Widget.method ('hide', function () {
    this.div.style.display = 'none';    
});

Widget.method ('show', function () {
    this.div.style.display = 'block';    
});

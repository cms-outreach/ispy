window.TwigOps = function ()
{    
}

TwigOps = window.TwigOps;

TwigOps.childWithName = function (obj, name)
{
    var realObj = obj;

    for (var i = 0; i < realObj.childNodes.length; i++)
    {
        var child = realObj.childNodes.item (i);
        if (child.nodeName == "div")
        {
            if (child.getAttribute ("NAME") == name)
            {
                return child;
            }
        }
    }
    return null;
}

TwigOps.childWithRPID  = function (obj, rpid)
{
    var children = obj;

    if (obj["childNodes"])
    {
        children = obj.childNodes;
    }

    for (var i = 0; i < children.length; i++)
    {
        var child = children.item (i);
        if (child.getAttribute)
        {
            if (child.getAttribute ("rpid") == rpid)
            {
                return child;
            }
            else if (child.getAttribute ("id") == rpid)
            {
                return child;
            }
        }
    }
    
    return null;
}

TwigOps.getTwigName  = function (obj, prevPath)
{
	if (obj.getAttribute ("NAME") == "/")
	{
		return "/" + prevPath;
	}
	else
	{
		var parent = obj.parentNode
		if (prevPath != "")
		{
			prevPath = "/" + prevPath
		}
		if (obj.getAttribute ("NAME") == null)
		{
			obj = parent;
			parent = parent.parentNode;
		}
		return TwigOps.getTwigName (parent, obj.getAttribute ("NAME") + prevPath)
	}
}

TwigOps.findStateInParents = function (startDiv)
{
    if (startDiv.parentNode)
    {
        var parent = startDiv.parentNode;
        if (parent.state)
        {
            return parent.state;
        }
        else
        {
            return TwigOps.findStateInParents (parent);
        }
    }
    return null;
}
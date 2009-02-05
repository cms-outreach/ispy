function Evt(evt)
{
	this.evt = evt ? evt : window.event; 
	this.source = evt.target ? evt.target : evt.srcElement;
	this.x = evt.pageX ? evt.pageX : evt.clientX;
	this.y = evt.pageY ? evt.pageY : evt.clientY;
//	this.relX = this.x - this.source.parentNode.offsetLeft;
//	this.relY = this.y - this.source.parentNode.offsetTop;
    this.relX = this.evt.layerX;
    this.relY = - this.evt.layerY;    
}

Evt.prototype.getSource = function ()
{
    var targ;
    if (this.evt.originalTarget) targ = this.evt.originalTarget;
	else if (this.evt.target) targ = this.evt.target;
	else if (this.evt.srcElement) targ = this.evt.srcElement;
	if (targ.nodeType == 3) // defeat Safari bug
		targ = targ.parentNode;
    return targ;
}

		
Evt.prototype.toString = function ()
{
	return "Evt [ x = " + this.x + ", y = " + this.y + " ]";
}
		
Evt.prototype.consume = function ()
{
	if (this.evt.stopPropagation)
	{
		this.evt.stopPropagation();
		this.evt.preventDefault();
	}
	else if (this.evt.cancelBubble)
	{
		this.evt.cancelBubble = true;
		this.evt.returnValue  = false;
	}
}
		
Evt.addEventListener = function (target,type,func,bubbles)
{
    if (target.addEventListener)
    {
    	target.addEventListener (type,func,bubbles);
    }
    else if (target.attachEvent)
    {
		target.attachEvent("on"+type,func,bubbles);	        
    }
	else if (document.addEventListener)
	{
	    if (target == window)
	    {
	        document.addEventListener (type, func, bubbles);
        }
        else
        {
           alert ("Unsupported browser");
        }   
	}
	else if (document.attachEvent)
	{
	    if (target == window)
        {
		    document.attachEvent("on"+type,func,bubbles);	
	    }
        else
        {
           alert ("Unsupported browser");
        }   
	}
	else
	{
	    target["on"+type] = func;
	}
}
	
Evt.removeEventListener = function (target,type,func,bubbles)
{
	if (document.removeEventListener)
	{
		target.removeEventListener(type,func,bubbles);
	}
	else if (document.detachEvent)
	{
		target.detachEvent("on"+type,func,bubbles);
	}
	else
	{
		target["on"+type] = null;
	}
}

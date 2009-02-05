function Browser2D (parent, id, width, height)
{
	this.createEventLayer = Browser2D.createEventLayer;
	this.dragMove = Browser2D.dragMove;
	this.dragRelease = Browser2D.dragRelease;
	var req = new HTTPRequest ();
	req.id = id;
	req.parent = parent;
	req.browser = this;

	req.http.onreadystatechange = function ()
	{	
		if (parent != null)
		{
			if (req.http.readyState == 4)
			{
				var img = document.createElement ("img");
				var div = document.createElement ("div");
				div.setAttribute ("id", id);
				div.setAttribute ("isLoading", false);
				img.width = width;
				img.height = height;
				parent.appendChild (div);
				div.appendChild (img);
				Evt.addEventListener (img, "load", req.browser.createEventLayer, false);
				img.src = "2D/browse?browserName=" + id + "&foo=" + new Date().getTime ();
			}
		}
	}
	req.http.open("GET", "2D/create?name=" + id, true);
	req.http.setRequestHeader("content-type", "text/xml");
	req.http.send ("");
}

Browser2D.createEventLayer = function (evt)
{
	this.removeEventListener ("load", Browser2D.createEventLayer, false);
	id = this.parentNode.getAttribute ("id");
	target = this;
	layer = new EventLayer (target, id);
	layer.onMouseDown (Browser2D.dragPress); 
}

var currentBrowser;
var currentTarget;

Browser2D.dragPress = function (evt)
{
	this.parentNode.setAttribute ("isLoading", false);
	evt = new Evt(evt);
	targetBrowser = evt.source.getAttribute ("target");
	currentBrowser = targetBrowser;	
	// The target img is always the previous element.
	var img = evt.source.previousSibling;
	var currentTarget = img;
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;
	img.src = "2D/browse?browserName=" + targetBrowser + "&spin=" + x + "," + y + "&" + new Date ().getTime ();
	Evt.addEventListener (evt.source, "mousemove", Browser2D.dragMove, false);
	Evt.addEventListener (evt.source, "mouseup", Browser2D.dragRelease, false);
	Evt.addEventListener (evt.source, "mouseout", Browser2D.dragOut, false);
	return false;
}

Browser2D.isLoading = false;

var isReallyLoading = false;

Browser2D.imageUpdated = function (target)
{
	//Here *this* is the img element.
	this.parentNode.setAttribute("isLoading", false);
	isReallyLoading = false;
}

Browser2D.dragMove = function (e)
{	
	// here *this* is the event catcher div. 
	var evt = new Evt (e);
	var isLoading = this.parentNode.getAttribute ("isLoading");
	var targetBrowser = currentBrowser;
	img = this.previousSibling;
	if (isReallyLoading != true)
	{
		isReallyLoading = true;
		this.parentNode.setAttribute ("isLoading", true);
		img.addEventListener ("load", Browser2D.imageUpdated, false);
		var x = evt.relX / img.clientWidth;
		var y = evt.relY / img.clientHeight;		
		imgUrl = "2D/browse?browserName=" + targetBrowser + "&spin=" + x + "," + y + "&" + new Date ().getTime ();
		img.src = imgUrl;
		evt.stopPropagation = true;
		evt.consume ();
		return false;
	}
}

Browser2D.dragRelease = function (e)
{
	var evt = new Evt (e);
	isReallyLoading = false;
	targetBrowser = currentBrowser;
	img = this.previousSibling; 
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;		
	img.src = "2D/browse?browserName=" + targetBrowser + "&lastSpin=" + x + "," + y + "&" + new Date ().getTime ();
	Evt.removeEventListener (this, "mousemove", Browser2D.dragMove, false);
	Evt.removeEventListener (this, "mouseup", Browser2D.dragRelease, false);
	Evt.removeEventListener (this, "mouseout", Browser2D.dragOut, false);
	return false;
}

Browser2D.dragOut = function (evt)
{
	var isReallyLoading = false;
	evt = new Evt (evt);
	var targetBrowser = currentBrowser;
    var	img = evt.source.parentNode.getElementsByTagName("IMG").item (0);
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;		
	img.src = "2D/browse?browserName=" + targetBrowser + "&lastSpin=" + x + "," + y + "&" + new Date ().getTime ();
	Evt.removeEventListener (this, "mousemove", Browser2D.dragMove, false);
	Evt.removeEventListener (this, "mouseup", Browser2D.dragRelease, false);
	Evt.removeEventListerer (this, "mouseout", Browser2D.dragOut, false);
	return false;
}


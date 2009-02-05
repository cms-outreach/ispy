importClass ("Studio.Widget");

window.BrowserROOT = function (parent, id, width, height)
{
    if (arguments.length > 0)
    {
        this.init (parent, id, width, height);
    }
}

BrowserROOT = window.BrowserROOT;

BrowserROOT.inherits (Widget);

BrowserROOT.method ('init', function (parent, id, width, height) {
    this.uber ('init', parent);
    
	this.updateCallback = new Callback_1 (this, "update");

	this.img = null;
    this.initialized = false;
	var req = new HTTPRequest ();
	req.id = id;
	this.id = id;
	this.img = document.createElement ("img");
	this.img.width = width;
    this.img.height = height;
	this.div.appendChild (this.img);
	this.updateURL = "../ROOT/browse?browserName=" + id;
	
	req.parent = parent;
	req.browser = this;

	req.http.onreadystatechange = function ()
	{	
		if (parent != null)
		{
			if (req.http.readyState == 4)
			{
				req.browser.div.setAttribute ("id", id);
				Evt.addEventListener (req.browser.img, "load", req.browser.createEventLayer, false);
			    req.browser.initialized = true;
                req.browser.update ();
			}
		}
	}
	req.http.open("GET", "../ROOT/create?name=" + id, true);
	req.http.setRequestHeader("content-type", "text/xml");
	req.http.send ("");
});

BrowserROOT.method ('createEventLayer', function (evt) {
	this.removeEventListener ("load", BrowserROOT.prototype.createEventLayer, false);
	var id = this.parentNode.getAttribute ("id");
	var target = this;
//	layer = new EventLayer (target, id);
//	layer.onMouseDown (BrowserROOT.dragPress); 
//	layer.setAttribute ("onmousewheel", "BrowserROOT.mouseWheel (event)");
});


BrowserROOT.method ('dragPress', function (e) {
	var evt = new Evt(e);
	// The target img is always the previous element.
	var img = evt.source.previousSibling;
	img.targetId = this.targetId;
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;
    this.update ();
	Evt.addEventListener (evt.source, "mousemove", BrowserROOT.dragMove, false);
	Evt.addEventListener (evt.source, "mouseup", BrowserROOT.dragRelease, false);
	Evt.addEventListener (evt.source, "mouseout", BrowserROOT.dragOut, false);
	return false;
});

BrowserROOT.method ('imageUpdated', function (target) {
	//Here *this* is the img element.
	this.isLoading = false;
});

BrowserROOT.method ('dragMove', function (e) {	
	// here *this* is the event catcher div. 
	var evt = new Evt (e);
	var img = this.previousSibling;
	if (img.isLoading != true)
	{
		img.isLoading = true;
		img.addEventListener ("load", BrowserROOT.imageUpdated, false);
		var x = evt.relX / img.clientWidth;
		var y = evt.relY / img.clientHeight;
		this.update ();		
//		imgUrl = "../ROOT/browse?browserName=" + this.targetId + "&foo=" + new Date ().getTime ();
//		img.src = imgUrl;
		evt.consume ();
		return false;
	}
});

BrowserROOT.method ('dragRelease', function (evt) {
	evt = new Evt (evt);
	img = this.previousSibling; 
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;
    this.update ();
//	img.src = "../ROOT/browse?browserName=" + this.targetId + "&foo=" + new Date ().getTime ();
	Evt.removeEventListener (this, "mousemove", BrowserROOT.dragMove, false);
	Evt.removeEventListener (this, "mouseup", BrowserROOT.dragRelease, false);
	Evt.removeEventListener (this, "mouseout", BrowserROOT.dragOut, false);
	return false;
});

BrowserROOT.method ('dragOut', function (e) {
	var evt = new Evt (e);
	var img = evt.source.parentNode.getElementsByTagName("IMG").item (0);
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;		
    this.update ();
//	img.src = "../ROOT/browse?browserName=" + this.targetId + "&foo=" + new Date ().getTime ();
	Evt.removeEventListener (this, "mousemove", BrowserROOT.prototype.dragMove, false);
	Evt.removeEventListener (this, "mouseup", BrowserROOT.prototype.dragRelease, false);
	Evt.removeEventListener (this, "mouseout", BrowserROOT.prototype.dragOut, false);
	return false;
});

BrowserROOT.method ('browse', function (rpid) {
    this.updateUrl = "../ROOT/browse?" + "browserName=" + this.id + "&i=" + rpid;
    this.update ();
});

BrowserROOT.method ('update', function () {
    if (this.initialized == true)
    {
	    this.img.src = this.updateUrl+"&foo=" + new Date ().getTime ();
    }
});

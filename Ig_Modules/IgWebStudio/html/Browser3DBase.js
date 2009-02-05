window.Browser3DBase = function (parent, id, width, height, kind)
{
	if (arguments.length > 0)
	{
		this.init (parent, id, width, height, kind);
	}
};

Browser3DBase = window.Browser3DBase;

importClass ("Studio.Widget");
Browser3DBase.inherits (Widget);

Browser3DBase.method ('init', function (parent, id, width, height, kind)
{
	this.uber('init', parent);

	this.updateCallback = new Callback_1 (this, "update");

	this.img = null;
	this.id = id;
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
				req.browser.img = img;
				//var div = document.createElement ("div");
				req.browser.div.setAttribute ("id", id);
				req.browser.div.setAttribute ("isLoading", false);
				img.width = width;
				img.height = height;
				//parent.appendChild (div);
				req.browser.div.appendChild (img);
				Evt.addEventListener (img, "load", req.browser.createEventLayer, false);
				img.src = "../3D/browse?browserName=" + id + "&foo=" + new Date().getTime ();
			}
		}
	}
	req.http.open("GET", "" + kind + "/create?name=" + id, true);
	req.http.setRequestHeader("content-type", "text/xml");
	req.http.send ("");
});


Browser3DBase.method ('createEventLayer', function (evt)
{
	this.removeEventListener ("load", Browser3DBase.prototype.createEventLayer, false);
	var id = this.parentNode.getAttribute ("id");
	var target = this;
	var layer = new EventLayer (target, id);
	layer.onMouseDown (Browser3DBase.prototype.dragPress); //"Browser3DBase.dragPress(event)");
//	layer.setAttribute ("onmousewheel", "Browser3DBase.prototype.mouseWheel (event)");
});

Browser3DBase.method ('dragPress', function (evt)
{
	var evt = new Evt(evt);
	// The target img is always the previous element.
	var img = evt.source.previousSibling;
	img.targetId = this.targetId;
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;
	img.src = "../3D/browse?browserName=" + this.targetId + "&spin=" + x + "," + y + "&" + new Date ().getTime ();
	Evt.addEventListener (evt.source, "mousemove", Browser3DBase.prototype.dragMove, false);
	Evt.addEventListener (evt.source, "mouseup", Browser3DBase.prototype.dragRelease, false);
	Evt.addEventListener (evt.source, "mouseout", Browser3DBase.prototype.dragOut, false);
	return false;
});

Browser3DBase.isLoading = false;

Browser3DBase.method ('imageUpdated', function (event) {
	//Here *this* is the img element.
	this.isLoading = false;
});

Browser3DBase.method ('dragMove', function (evt) {	
	// here *this* is the event catcher div. 
	var evt = new Evt (evt);
	var img = this.previousSibling;
    
    if (!img.isLoading)
    {
        img.isLoading = false;
    }
    
	if (img.isLoading != true)
	{
		img.isLoading = true;
		img.addEventListener ("load", Browser3DBase.prototype.imageUpdated, false);
		var x = evt.relX / img.clientWidth;
		var y = evt.relY / img.clientHeight;		
		var imgUrl = "../3D/browse?browserName=" + this.targetId + "&spin=" + x + "," + y + "&foo=" + new Date ().getTime ();
		img.src = imgUrl;
		evt.stopPropagation = true;
		evt.consume ();
		return false;
	}
});

Browser3DBase.method ('dragRelease', function (evt) {
	var evt = new Evt (evt);
	var img = this.previousSibling; 
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;		
	img.src = "../3D/browse?browserName=" + this.targetId + "&lastSpin=" + x + "," + y + "&foo=" + new Date ().getTime ();
	Evt.removeEventListener (this, "mousemove", Browser3DBase.prototype.dragMove, false);
	Evt.removeEventListener (this, "mouseup", Browser3DBase.prototype.dragRelease, false);
	Evt.removeEventListener (this, "mouseout", Browser3DBase.prototype.dragOut, false);
	return false;
});

Browser3DBase.method ('dragOut',  function (evt) {
	var evt = new Evt (evt);
	var img = evt.source.parentNode.getElementsByTagName("IMG").item (0);
	var x = evt.relX / img.clientWidth;
	var y = evt.relY / img.clientHeight;		
	img.src = "../3D/browse?browserName=" + this.targetId + "&lastSpin=" + x + "," + y + "&foo=" + new Date ().getTime ();
	Evt.removeEventListener (this, "mousemove", Browser3DBase.prototype.dragMove, false);
	Evt.removeEventListener (this, "mouseup", Browser3DBase.prototype.dragRelease, false);
	Evt.removeEventListener (this, "mouseout", Browser3DBase.prototype.dragOut, false);
	return false;
});

Browser3DBase.method ('viewAll', function () {
	this.img.src = "../3D/browse?browserName=" + this.id + "&viewAll=1&" + new Date ().getTime (); 
});

Browser3DBase.method ('zoom', function (delta) {
	this.img.src = "../3D/browse?browserName=" + this.id + "&zoom=" + delta + "&foo=" + new Date ().getTime ();
});

Browser3DBase.method ('moveCameraScreen', function (dx, dy) {	
	this.img.src = "../3D/browse?browserName=" + this.id + "&moveCameraScreen=" + dx + "," + dy + "&foo=" + new Date ().getTime ();
});

Browser3DBase.method ('update', function () {
	this.img.src = "../3D/browse?browserName=" + this.id + "&foo=" + new Date ().getTime ();
});


Browser3DBase.method ('mouseWheel', function (evt) {
	if (! evt) evt = window.event;
	var delta = evt.wheelDelta;
	if (delta > 0)
		zoom (this, 0.1);
	else 
		zoom (this, -0.1);		
});

function EventLayer (img, targetId)
{
	img.removeAttribute ("onload");
	img.parentNode.style["position"] = "relative";
	var layer = document.createElement ("DIV");
	NodeUtils.insertAfter (layer, img);
	var width = img.clientWidth;
	var height = img.clientHeight;
	layer.style["position"] = "absolute";
    layer.style["top"] = "0pt"; "-" + img.clientHeight + "px";
    layer.style["left"] = "0pt";
    layer.style["color"] = "#FF0000";
	layer.style["width"] = width + "px";
	layer.style["height"] = height + "px";
	layer.targetId = targetId;
	this.targetId = targetId;
	this.layer = layer;
	this.dragMove = EventLayer.dragMove;
	this.dragRelease = EventLayer.dragRelease;
	this.onMouseDown = EventLayer.onMouseDown;
	Evt.addEventListener (this.layer, "click", EventLayer.click, true);
//	Evt.addEventListener (this.layer, "mouseout", EventLayer.shrink, false);
	layer.setAttribute ("target", targetId);
}

var currentBrowser;
var currentTarget;

EventLayer.onMouseDown = function (callback)
{
	Evt.addEventListener (this.layer, "mousedown", callback, true);
}
	
EventLayer.resize = function (layer, scale)
{
	var img = layer.previousSibling;

	var w = Math.round (img.clientWidth * scale) + "px";
	var h = Math.round (img.clientHeight * scale) + "px";
	

	img.style["width"] = w;
	img.style["height"] = h;
	layer.style["width"] = w;
	layer.style["height"] = h;	
}

var lastFocus = null;

EventLayer.click = function (evt)
{
	if (lastFocus == this)
		return;
	EventLayer.resize (this, 2);
	if (lastFocus != null)
		EventLayer.resize (lastFocus, 0.5);
	lastFocus = this;
}

EventLayer.expand = function (evt)
{
	EventLayer.resize (this, 2);
}

EventLayer.shrink = function (evt)
{
	EventLayer.resize (this, 0.5);
}

importClass ("Studio.Widget");

window.Panel = function (state, parent, id, title, onloadCallback)
{
	if (arguments.length > 0)
	{
		this.init (state, parent, id, title, onloadCallback);
	}
}

Panel = window.Panel;

Panel.inherits (Widget);

Panel.method ('init', function (state, parent, id, title, onloadCallback) {
	this.uber ('init', parent, 1);

	this.state = state;

	this.titleDiv = document.createElement ("div");
	this.contentsDiv = document.createElement ("div");

	this.div.setAttribute ("id", id);
	this.titleDiv.setAttribute ("id", id + "." + "title");
	this.titleDiv.setAttribute ("name", "Title");
	this.titleDiv.innerHTML = title;

	this.contentsDiv.setAttribute ("name", "Contents");
	this.contentsDiv.setAttribute ("id", id + "." + "contents");

	if (onloadCallback != null)
	{
		var loadImg = document.createElement ("img");
		loadImg.setAttribute ("onload", onloadCallback);
		loadImg.src = "http://eulisse.web.cern.ch/eulisse/spinner.gif";
		this.contentsDiv.appendChild (loadImg);
	}

    this.div.style["margin"] = "3px";
	this.div.style["color"] = "#3366CC";
	this.div.style["border"] = "0px";
    this.div.style["borderTop"] = "1px";
    this.div.style["borderStyle"] = "solid";

    this.titleDiv.style["backgroundColor"] = "#E5ECF9";
    this.titleDiv.style["color"] = "#06068B";
    this.titleDiv.style["fontFamily"] = "sans-serif";
    this.titleDiv.style["fontSize"] = "14px";
	
	this.contentsDiv.style["backgroundColor"] = "#FFFFFF";
    this.contentsDiv.style["fontFamily"] = "sans-serif";
    this.contentsDiv.style["fontSize"] = "12px";
    	
	this.div.appendChild (this.titleDiv);
	this.div.appendChild (this.contentsDiv);
});

Panel.method ('setupChild', function (child) {
	// Panel appends its children within contentsDiv!!
	this.contentsDiv.appendChild (child.div);
});

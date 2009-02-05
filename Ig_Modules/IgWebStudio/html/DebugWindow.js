window.DebugWindow = function ()
{
	var div = document.createElement ("div");
	div.style ["position"] = "absolute";
	div.style ["z-index"] = "1";
	div.style ["width"] = "100%";
	div.style ["height"] = "100px";
	div.style ["top"] = "0px";
	div.style ["left"] = "0";
	div.style ["background"] = "#ff0000";
	div.style ["color"] = "white";
	div.style["overflow"] = "auto";	
	document.getElementsByTagName ("BODY").item(0).appendChild (div);
	this.div = div;
}

DebugWindow = window.DebugWindow;

DebugWindow.method ("print", function (text)
{
	this.div.innerHTML += "<br/>" + text;
});

DebugWindow.method ("show", function ()
{
	this.div.style ["display"] = "block";
});

DebugWindow.method ("hide", function ()
{
	this.div.style ["display"] = "none";
});


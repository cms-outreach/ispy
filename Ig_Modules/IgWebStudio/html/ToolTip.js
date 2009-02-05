window.ToolTip = function (target, text)
{
    if (arguments.length > 0)
    {
        this.init (target, text);
    }
}

importClass ("Studio.Widget");

ToolTip.inherits (Widget);

ToolTip.method ('init', function (target, text) {
    this.uber ('init', document.documentElement);
    this.hide ();
    this.div.style["borderStyle"] = "solid";
    this.div.style["borderWidth"] = "1px";
    this.div.style["padding"] = "2px";
    this.div.style["color"] = "#000000";//3E557F
    this.div.style["backgroundColor"] = "#F3F1C8";
    this.div.style["position"] = "absolute";
    this.div.style["zIndex"] = "1000";
    
    this.contents = document.createElement ("div");
    this.contents.style["color"] = "#000000";
    this.contents.style["fontFamily"] = "sans-serif";
    this.contents.style["fontSize"] = "75%";
    
    this.contentsString = document.createTextNode (text);
    this.contents.appendChild (this.contentsString);
    this.div.appendChild (this.contents);
    target.icon.tooltip = this;
    
   	Evt.addEventListener (target.icon, 
				        "mouseover", 
				        ToolTip.prototype.showToolTip,
				        true);
    Evt.addEventListener (target.icon, 
    		            "mouseout", 
            	        ToolTip.prototype.hideToolTip,
            	        true);
});

ToolTip.method ('showToolTip', function (event) {
    this.tooltip.div.style["top"] = event.pageY + 10 + "px";
    this.tooltip.div.style["left"] = event.pageX + 10 + "px";
    this.tooltip.show ();
});

ToolTip.method ('hideToolTip', function (event) {
    this.tooltip.hide ();
});

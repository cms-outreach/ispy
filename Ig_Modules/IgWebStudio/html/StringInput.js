window.StringInput = function (parent, label, text)
{
    if (arguments.length > 0)
    {
        this.init (parent, label, text);
    }
}

StringInput = window.StringInput;

importClass ("Studio.Widget");

StringInput.inherits (Widget);

StringInput.method ("init", function (parent, label, text) {
    this.uber ("init", parent, 1);
    
    this.labelDiv = document.createElement ("div");
    this.labelDiv.style["cssFloat"] = "left";
    this.labelDiv.style["verticalAlign"] = "middle";
    this.labelString = document.createTextNode (label);
    this.labelDiv.appendChild (this.labelString);
    
    this.inputElement = document.createElement ("input");
    this.inputElement.setAttribute ("type", "text");
    this.inputElement.style["verticalAlign"] = "middle";
    
    if (text)
    {
        this.inputElement.value = text;
    }
    
    this.div.appendChild (this.labelDiv);
    this.div.appendChild (this.inputElement);    
});

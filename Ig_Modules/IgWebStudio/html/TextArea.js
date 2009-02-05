window.TextArea = function (parent, label, contents)
{
    if (arguments.length > 0)
    {
        this.init (parent, label, contents);
    }
}

TextArea = window.TextArea;
importClass ("Studio.Widget");

TextArea.inherits (Widget);

TextArea.method ("init", function (parent, label, contents) {
    this.uber ("init", parent, 1);

    if (label)
    {
        this.labelDiv = document.createElement ("div");
        this.labelString = document.createTextNode (label);
        this.labelDiv.appendChild (this.labelString);
    }
    
    this.textArea = document.createElement ("textarea");
    this.textArea.setAttribute ('rows', '25');
    this.textArea.setAttribute ('cols', '80'); 
    
    if (contents)
    {
        this.textArea.nodeValue = contents;
    }
    
    this.div.appendChild (this.labelDiv);
    this.div.appendChild (this.textArea);
});
importClass ("Studio.Widget");
   
window.PushButton = function (parent, label)
{
    if (arguments.length > 0)
    {
        this.init (parent, label);
    }
}

PushButton = window.PushButton;

PushButton.inherits (Widget);

PushButton.method ("init", function (parent, label) {
    this.uber ("init", parent, 1);
    this.button = document.createElement ("input");
    this.button.setAttribute ("type", "button");
    this.button.value = label;
    this.div.appendChild (this.button);
});


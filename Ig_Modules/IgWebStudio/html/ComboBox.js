window.ComboBox = function (parent)
{
    if (arguments.length > 0)
    {
        this.init (parent);
    }
}

ComboBox = window.ComboBox;

importClass ("Studio.Widget");

ComboBox.inherits (Widget);

ComboBox.method ("init", function (parent, items) {
    this.uber ("init", parent, 1);
    this.comboBox = document.createElement ("select");
    if (items)
    {
        for (var i = 0; i < items.length; i++)
        {
            this.appendItem (items[i][0], items[i][1]);
        }
    }
    this.div.appendChild (this.comboBox);
});

ComboBox.method ("appendItem", function (id, label) {
    var option = document.createElement ("option");
    option.value = id;
    option.text = label;
    
    this.comboBox.appendChild (option);    
});
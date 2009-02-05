window.Table = function (parent, title, columns)
{
    if (arguments.length > 0)
    {
        this.init (parent, title, columns);
    }
}

Table = window.Table;

importClass ("Studio.Widget");

Table.inherits (Widget);

Table.method ("init", function (parent, title, columns) {
    this.uber ("init", parent);
    this.table = document.createElement ("table");
    this.table.style["border"] = "1px";
    this.table.style["borderStyle"] = "solid";
    this.table.style["borderColor"] = "#336699";
    this.table.style["borderCollapse"] = "collapse";
    
    
    var tr = document.createElement ("tr");
    var htmlTitle = document.createElement ("th");
    htmlTitle.innerHTML = title;
    htmlTitle.style["color"] = "#ffffff";
    htmlTitle.style["backgroundColor"] = "#336699";
    htmlTitle.style["fontSize"] = "120%";
    
    tr.appendChild (htmlTitle);
    this.table.appendChild (tr);
    
    tr = document.createElement ("tr");

    for (var i = 0; i < columns.length; i++)
    {
        column = columns[i];
        var th = document.createElement ("th");
        th.innerHTML = column; 
        th.style["color"] = "#ffffff";
        th.style["backgroundColor"] = "#336699";
        tr.appendChild (th);
    }
    this.table.appendChild (tr);
    this.div.appendChild (this.table);
});

Table.method ("appendRow", function (cells) {
    var tr = document.createElement ("tr");
    for (var i = 0; i < cells.length; i++)
    {
        cell = cells[i];
        var td = document.createElement ("td");
        td.innerHTML = cell;
        tr.appendChild (td);
    }
    this.table.appendChild (tr);
});
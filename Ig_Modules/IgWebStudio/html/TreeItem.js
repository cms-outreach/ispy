window.TreeItem = function (parent, twigName, visible, expanded) {
    if (arguments.length > 0)
    {
        this.init (parent, twigName, visible, expanded);
    }
}

TreeItem = window.TreeItem;

TreeItem.COLLAPSED = "../Studio/getPic?name=edit_add.png";
TreeItem.EXPANDED = "../Studio/getPic?name=edit_remove.png";

TreeItem.TICK_ON = "../Studio/getPic?name=stock_calc-accept.png";
TreeItem.TICK_OFF = "../Studio/getPic?name=stock_calc-cancel.png";

TreeItem.inherits (Widget);

TreeItem.method ('init', function (parent, twigName, visible, expanded) {
    this.uber ('init', parent);
    var linkNode = document.createElement ("a");
    var textNode = document.createTextNode (twigName);

    linkNode.style["paddingLeft"] = "3%";
    linkNode.style["color"] = "#000000";
    linkNode.style["fontSize"] = "12px";
    linkNode.style["fontFamily"] = "sans-serif";

    var expandButton = document.createElement ("img");
    var visibilityButton = document.createElement ("img");

    expandButton.style ["width"] = "12px";
    expandButton.style ["height"] = "12px";
    visibilityButton.style ["width"] = "12px";
    visibilityButton.style ["height"] = "12px";

    if (visible)
    {
        visibilityButton.src = TreeItem.TICK_ON;
        visibilityButton.visible = true;
    }
    else
    {   
        visibilityButton.src = TreeItem.TICK_OFF;
        visibilityButton.visible = false;
    }
    
    if (expanded)
        expandButton.src = TreeItem.EXPANDED;
    else
        expandButton.src = TreeItem.COLLAPSED;

    visibilityButton.setAttribute ("id", idCounter);
    visibilityButton.setAttribute ("onclick","TreeItem.prototype.toggleVisibility ('" + idCounter +"')");
    idCounter += 1;

    this.childrenNode = document.createElement ("div");
    this.childrenNode.style["paddingLeft"] = "3%";
    this.childrenNode.setAttribute ("id", idCounter);
    
    this.div.setAttribute ("name", twigName);
    linkNode.setAttribute ("onclick","TreeItem.prototype.updateChildren ('" + idCounter +"')");
    expandButton.setAttribute ("onclick","TreeItem.prototype.updateChildren ('" +idCounter +"')")
    idCounter += 1;
    linkNode.appendChild (textNode);
    this.div.appendChild (expandButton);
    this.div.appendChild (linkNode);
    this.div.appendChild (visibilityButton);
    this.div.appendChild (this.childrenNode);
});

TreeItem.method ('hideChildren', function (childrenGroup) {
    childrenGroup.style["display"] = "none";
});

TreeItem.method ('showChildren', function (childrenGroup) {
    childrenGroup.style["display"] = "block";
});


TreeItem.method ('updateChildren', function (childrenGroupId) {
    var childrenGroup = document.getElementById (childrenGroupId); 
    var controllerImg = childrenGroup.parentNode.getElementsByTagName ("IMG").item (0);

    this.getTwigsFromServer (childrenGroup);
    
    if (childrenGroup.style["display"] == "block")
    {
        TreeItem.prototype.hideChildren (childrenGroup);
        controllerImg.src = TreeItem.COLLAPSED;
    }
    else
    {
        TreeItem.prototype.showChildren (childrenGroup);
        controllerImg.src = TreeItem.EXPANDED;
    }
});

TreeItem.method ('toggleVisibility', function (imgId) {
    var img = document.getElementById (imgId);
    var name = TwigOps.getTwigName (img, "");
    var state = TwigOps.findStateInParents (img);
    
    if (! state)
        alert ("State not found!");
    
    var request = new HTTPRequest ();
    request.http.onreadystatechange = function ()
    {
        if (request.http.readyState == 4)
        {
            state.lookup ("Update Service").dispatcher.broadcasts ();
        }
    }
    if (img.visible == true)
    {
        img.src = TreeItem.TICK_OFF;
        img.visible = false;
        request.http.open("GET", "../Tree/browse?setVisible=0&t=" + name, true);
    }
    else
    {
        img.src = TreeItem.TICK_ON;
        img.visible = true;
        request.http.open("GET", "../Tree/browse?setVisible=1&t=" + name, true);
    }
    request.http.send ("");
});

TreeItem.method ('setupChild', function (child)
{
    child = WidgetsUtils.extractDiv (child);
    this.childrenNode.appendChild (child);
});

TreeItem.method ('getTwigsFromServer', function (target)
{
    var request = new HTTPRequest ();
    
    request.http.onreadystatechange = function()
    {
        if(request.http.readyState == 4)
        {
            var plugin;
            plugin=target;

            var response = request.http.responseXML.documentElement;
            var rootTwig = response;
            var twigs = rootTwig.getElementsByTagName ("TWIG");

            if (twigs.length == 0)
            {
                var images = plugin.parentNode.getElementsByTagName("IMG");
                var plusImg = images.item (0);
                var visImg = images.item (1);
                plusImg.style["display"] = "none";
                TreeItem.prototype.toggleVisibility (visImg.getAttribute ("ID"));
            }

            for (var i = 0; i < twigs.length; i++)
            {
                var twig = twigs[i];
                var twigName = twig.getAttribute ("NAME");
                var twigID = twig.getAttribute ("RPID");

                var newNode = TwigOps.childWithRPID (plugin, twigID);
                var visibility = false;
                
                if (twig.getAttribute ("VIS") == "1")
                {
                    visibility = true;
                }
        
                if (! newNode)
                {
                    var item = new TreeItem (plugin, twigName, visibility, false);
                    item.setID (twigID);
                }
            }
            
            for (var i = 0; i < plugin.childNodes.length; i++)
            {
                var oldNode = plugin.childNodes.item (i); 
                var oldChildRPID = oldNode.getAttribute ("RPID");
                var newNode = TwigOps.childWithRPID (twigs, oldChildRPID);
                if (newNode != null)
                {
                    plugin.removeChild (oldNode);
                }
            }
        }
    }
    
    var id = target.parentNode.getAttribute ("RPID");
    var name = TwigOps.getTwigName (target.parentNode, "");
    request.http.open ("GET", "Tree/browse?i=" + id, true);
    request.http.setRequestHeader ("content-type", "text/xml");
    request.http.send (""); 
});

TreeItem.method ('setID', function (id) {
    this.div.setAttribute ("RPID", id);
});

TreeItem.method ('getID', function () {
    return this.div.getAttribute ("RPID");
});

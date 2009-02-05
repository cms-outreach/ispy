window.TreeBrowser = function (parent, id, rootTwig, state)
{
    if (arguments.length > 0)
    {
        this.init (parent, id, rootTwig, state);
    }
}

TreeBrowser = window.TreeBrowser;

importClass ("Studio.Widget");
importClass ("Studio.TwigOps");
importClass ("Studio.TreeItem");

TreeBrowser.inherits (Widget);

TreeBrowser.method ('init', function (parent, id, rootTwig, state)
{
    if (! rootTwig)
        rootTwig = "/";
    
    this.uber ('init', parent, 1);
    
    this.state = state;
    this.div.state = state;

    this.div.setAttribute ("ID", id);
    this.div.setAttribute ("CLASS", "treeBrowser");
    this.div["backgroundColor"] = "#ffffff";
    this.div["width"] = "250px";
    this.div.setAttribute ("NAME", "/");
    var request = new HTTPRequest ();
    var targetDiv = this.div;

    request.http.onreadystatechange = function ()
    {
        if (request.http.readyState == 4)
        {
            var response = request.http.responseXML.documentElement;
            var uid = response.childNodes[0].nodeValue;
            var item = new TreeItem (targetDiv, "/", true, false);
            item.setID (uid);
        }
    }

    request.http.open("GET", "../Tree/getRepID?t=" + rootTwig, true);
    request.http.setRequestHeader ("Content-type", "text/xml");
    request.http.send ("");
    
    
    Evt.addEventListener (this.div, 
                            "contextmenu", 
                            TreeBrowser.prototype.onContextMenu,
                            false);
});

TreeBrowser.method ('onContextMenu', function (e)
{
    var evt = new Evt (e);
    
    evt.consume ();
    
    
    var currentState = TwigOps.findStateInParents (evt.getSource ());
    
    var item = evt.getSource ();
        
    var rpid = item.getAttribute ("rpid");
    while (rpid == null && item != this)
    {
        item = item.parentNode;
        rpid = item.getAttribute ("rpid");
    }
    
    var request = new HTTPRequest ();

    var posX = evt.x;
    var posY = evt.y;
    
    request.http.onreadystatechange = function ()
    {
        if (request.http.readyState == 4)
        {
            var response = request.http.responseXML.documentElement;
            var menuName = response.getAttribute ("name");
            currentState.services["PopupMenuService"].currentMenu (menuName, 
                                                                   rpid,
                                                                   posX,
                                                                   posY);
        }
    }    

    request.http.open ("GET", "../Tree/twigMenu?i=" + rpid, true);
    request.http.setRequestHeader ("content-type", "text/xml");
    request.http.send ("");

    return false;
});
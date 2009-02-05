window.ToolbarEvent = function (state, parent)
{
    if (arguments.length > 0)
    {
        this.init (state, parent);
    }
}

ToolbarEvent = window.ToolbarEvent;

importClass ("Studio.Toolbar")
ToolbarEvent.inherits (Toolbar);

ToolbarEvent.method ('init', function (state, parent) {
    this.uber ('init', state, parent, "ToolbarEvent", "Event Toolbar" );
    this.state = state;
    this.parent = parent;

    var toolbarService = state.lookup ("Toolbar Manager");

    var parentDiv = this.contents;
    var baseUrl = "../Studio/getPic?name=";
    var button = null;

    button = new ToolbarButton (parentDiv, baseUrl + "stock_data-last.png", "Next event");
    new ToolTip (button, "Next event");
    this.addButton ("NEXT_EVENT", button);
    button.setOnClickCallback (new Callback_0_1 (this, "nextEvent", state));
});

// This should be moved to some Action class.
ToolbarEvent.method ('nextEvent', function (state) {
    var request = new HTTPRequest ();
    request.dispatcher = state.lookup ("Update Service").dispatcher;

    request.http.onreadystatechange = function()
    {
        if(request.http.readyState == 4)
        {
            request.dispatcher.broadcasts ();
            //visibilityDispatcher.broadcasts ();
        }
    }
    request.http.open ("GET", "../Framework/nextEvent", true);
    request.http.send ("");
});

window.Window = function (state, parent, id, title, onloadCallback)
{
	if (arguments.length > 0)
	{
		this.init (state, parent, id, title, onloadCallback);
	}
}

Window = window.Window;

importClass ("Studio.Panel")
Window.inherits (Panel);

Window.method ('init', function (state, parent, id, title, onloadCallback) {
	this.uber ('init', state, parent, id, title, onloadCallback);

    importClass ("Studio.State")
	this.state = new State (state);
    
	this.state.services["FocusInCallbacks"] = new Array ();
	this.state.services["FocusOutCallbacks"] = new Array ();

	var stateManager = this.state.lookup ("State Manager");
	stateManager.stateMap [id] = this.state;
    this.titleDiv.state = this.state;
    this.titleDiv.windowId = id;
    Evt.addEventListener (this.titleDiv, "click", Window.prototype.onTitleClick, false);
	this.addFocusInCallback (new Callback_0_1 (this, "focusIn", this.state));
	this.addFocusOutCallback (new Callback_0_1 (this, "focusOut", this.state));
});

Window.method ('focusIn', function (state) {
	this.titleDiv.style["color"] = "#ffffff";
	this.titleDiv.style["background"] = "#FF6B74";
});

Window.method ('focusOut', function (state) {
	this.titleDiv.style["color"] = "#06068B";
	this.titleDiv.style["background"] = "#E5ECF9";
});

Window.method ('addFocusInCallback', function (callback) {
	this.state.services["FocusInCallbacks"].push (callback);
});

Window.method ('addFocusOutCallback', function (callback) {
	this.state.services["FocusOutCallbacks"].push (callback);
});

Window.method ('onTitleClick', function (e) {
    var evt = new Evt (e);
    var source = evt.getSource ();
    var id = source.windowId;

    var stateManager = source.state.lookup ("State Manager");
	var newState = null;
	newState = stateManager.stateMap [id];
	var contextManager = newState.lookup ("Context Manager");
	var oldState = null;
	if (contextManager.currentState)
		oldState = contextManager.currentState;
	
	if (oldState != newState)
	{
		if (oldState)
		{
			var focusOutCallbacks = oldState.lookup ("FocusOutCallbacks");
			if (focusOutCallbacks)
			{
				for (id in focusOutCallbacks)
					focusOutCallbacks[id].call (oldState);
			}
 		}

		if (newState)
		{
			var focusInCallbacks = newState.lookup ("FocusInCallbacks");
			if (focusInCallbacks)
			{
				for (id in focusInCallbacks)
				{
					focusInCallbacks[id].call (newState);
				}
			}
		}
		contextManager.currentState = newState;
	}
});

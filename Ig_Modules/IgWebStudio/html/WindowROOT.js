importClass ("Studio.Window")

window.WindowROOT = function (state, parent, id, title)
{
    if (arguments.length > 0)
    {
        this.init (state, parent, id, title);
    }
}

WindowROOT = window.WindowROOT;

WindowROOT.inherits (Window);

WindowROOT.method ('init', function (state, parent, id, title) {
    this.uber ('init', state, parent, id, title, "WindowROOT.prototype.onloadCallback (this, '" + id + "')");
    importClass ("Studio.BrowserROOT")
	this.browser = 	new BrowserROOT (this, id, 156, 156);

	this.addFocusInCallback (new Callback_1 (this, "onFocusIn"));
	this.addFocusOutCallback (new Callback_1 (this, "onFocusOut"));	
	state.lookup ("Update Service").dispatcher.addObserver (this.browser.updateCallback);
//	this.onTitleClick (id);
});

WindowROOT.method ('onloadCallback', function (placeHolder, id) {
	placeHolder.setAttribute ("onload", "");
	if (placeHolder == null)
		return;
	var parent = placeHolder.parentNode;
	parent.removeChild (placeHolder);
});

WindowROOT.method ('onFocusIn', function (state) {
	var toolbar = state.lookup ("Toolbars/Toolbar3D");
});

WindowROOT.method ('onFocusOut', function (state) {
	var toolbar = state.lookup ("Toolbars/Toolbar3D");
});

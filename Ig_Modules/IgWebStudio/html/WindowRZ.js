window.WindowRZ = function (state, parent, id, title)
{
    if (arguments.length > 0)
    {
        this.init (state, parent, id, title);
    }
}

WindowRZ = window.WindowRZ;

importClass ("Studio.Window")

WindowRZ.inherits (Window);

WindowRZ.method ('init', function (state, parent, id, title) {
    this.uber ('init', state, parent, id, title, "WindowRZ.prototype.onloadCallback (this, '" + id + "')");
//	this.window = new Window (state, parent, id, title, "WindowRZ.onloadCallback (this, '" + id + "')");
    importClass ("Studio.BrowserRZ")
	this.browser = 	new BrowserRZ (this, id, 156, 156);
	this.onFocusIn = WindowRZ.prototype.onFocusIn;
	this.onFocusOut = WindowRZ.prototype.onFocusOut;

	this.addFocusInCallback (new Callback_1 (this, "onFocusIn"));
	this.addFocusOutCallback (new Callback_1 (this, "onFocusOut"));	
	state.lookup ("Update Service").dispatcher.addObserver (this.browser.updateCallback);
//	WindowRZ.prototype.onTitleClick (id);
});

WindowRZ.method ('onloadCallback', function (placeHolder, id) {
	placeHolder.setAttribute ("onload", "");
	if (placeHolder == null)
		return;
	var parent = placeHolder.parentNode;
	parent.removeChild (placeHolder);
});

WindowRZ.method ('onFocusIn', function (state) {
	var toolbar = state.lookup ("Toolbars/Toolbar3D");
	toolbar.buttons["VIEW_ALL"].setOnClickCallback (new Callback_1 (this.browser, "viewAll"));
	toolbar.buttons["ZOOM_IN"].setOnClickCallback (new Callback_0_1 (this.browser, "zoom", -0.1));
 	toolbar.buttons["ZOOM_OUT"].setOnClickCallback (new Callback_0_1 (this.browser, "zoom", 0.1));
 	toolbar.buttons["UP"].setOnClickCallback (new Callback_0_2 (this.browser, "moveCameraScreen", 0., 0.05)); 
 	toolbar.buttons["DOWN"].setOnClickCallback (new Callback_0_2 (this.browser, "moveCameraScreen", 0, -0.05)); 
 	toolbar.buttons["LEFT"].setOnClickCallback (new Callback_0_2 (this.browser, "moveCameraScreen", -0.05, 0.)); 
 	toolbar.buttons["RIGHT"].setOnClickCallback (new Callback_0_2 (this.browser, "moveCameraScreen", 0.05, 0.)); 
});

WindowRZ.method ('onFocusOut', function (state) {
	var toolbar = state.lookup ("Toolbars/Toolbar3D");
	toolbar.buttons["VIEW_ALL"].setOnClickCallback (null);			
 	toolbar.buttons["ZOOM_IN"].setOnClickCallback (null);
 	toolbar.buttons["ZOOM_OUT"].setOnClickCallback (null);
 	toolbar.buttons["UP"].setOnClickCallback (null); 
 	toolbar.buttons["DOWN"].setOnClickCallback (null); 
 	toolbar.buttons["LEFT"].setOnClickCallback (null); 
 	toolbar.buttons["RIGHT"].setOnClickCallback (null); 
});

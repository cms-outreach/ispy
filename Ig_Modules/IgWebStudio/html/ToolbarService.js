// For the time being this is just for keeping track of buttons.
window.ToolbarService = function (state)
{
	this.state = state;
	this.state.services["Toolbar Service"] = this;
	this.toolButtons = new Array (); 
	this.setButton = ToolbarService.setButton;
}

ToolbarService = window.ToolbarService;

ToolbarService.method ("setButton", function (buttonId, button)
{
	if (! this.toolButtons[buttonId])
	{
		this.toolButtons[buttonId] = button;
	}
	return this.toolButtons[buttonId];
});

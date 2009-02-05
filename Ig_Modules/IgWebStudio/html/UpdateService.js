window.UpdateService = function (state)
{
	this.state = state;
	state.services["Update Service"] = this;
	this.dispatcher = new Dispatcher ();
}

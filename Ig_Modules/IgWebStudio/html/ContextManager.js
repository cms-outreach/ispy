window.ContextManager = function (state)
{
	this.state = state;
	this.state.services ["Context Manager"] = this;
	this.currentState = null;
}


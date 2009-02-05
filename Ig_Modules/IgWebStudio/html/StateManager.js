window.StateManager = function (state)
{
	// this is the map with the association between id and states
	this.stateMap = new Function ();	
	state.services["State Manager"] = this;	
}

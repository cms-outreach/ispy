window.PlaceHolder = function (state, parent, onloadCallback)
{
	this.state = state;
	this.parent = parent;
	this.placeHolderDiv = document.createElement ("div");
	this.placeHolderImg = document.createElement ("img");
	this.placeHolderDiv.appendChild (this.placeHolderImg);
	parent.appendChild (this.placeHolderImg);
	this.placeHolderImg.setAttribute ("onload", onloadCallback);	
	this.placeHolderImg.src = "../Studio/getPic?name=stock_help-chat-16.png"	
}

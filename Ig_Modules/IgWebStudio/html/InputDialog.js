importClass ("Studio.Dialog");

window.InputDialog = function (state, id, title, onloadCallback)
{
	if (arguments.length > 0)
	{
		this.init (state, id, title, onloadCallback);
	}    
}

InputDialog = window.InputDialog;

InputDialog.inherits (Dialog);

InputDialog.method ('init', function (state, id, title, onloadCallback) {
    this.uber ('init', state, id, title, onloadCallback);
    this.id = id;
    this.div.style.top = '200px';
    this.div.style.left = '100px';
    this.div.style.align = 'center';
    this.div.style.verticalAlign = 'middle';
    this.div.style.width = '512px';
    this.div.style.height = '256px';
    
    this.label = document.createElement ("div");
    this.labelString = document.createTextNode ("Input:")
    this.label.appendChild (this.labelString);
    this.textString = document.createElement ("input");
    this.textString.setAttribute ('type', 'text')
    this.button = document.createElement ("input");
    this.button.nodeValue = 'Ok';
    this.button.setAttribute ('type', 'button');
    this.button.dialog = this;
    this.contentsDiv.appendChild (this.label);
    this.contentsDiv.appendChild (this.textString);
    this.contentsDiv.appendChild (this.button);
});

InputDialog.method ('textInput', function (labelString, defaultText, button) {
    this.labelString.nodeValue = labelString;
    this.textString.setAttribute ('value', defaultText);
    this.button.setAttribute ('value', button);
	Evt.addEventListener (this.button, "click", InputDialog.prototype.onClick, true);
    this.show ();
});

InputDialog.method ('setCallback', function (callback) {
    this.button.callback = callback;
});

InputDialog.method ('onClick', function (evt)
{
    this.callback (this.dialog.textString.getAttribute ('value'));
    this.dialog.hide ();
});

importClass ("Studio.Panel")

window.Dialog = function (state, id, title)
{
    if (arguments.length > 0)
    {
        this.init (state, id, title);
    }
}

Dialog = window.Dialog;

Dialog.inherits (Panel);

Dialog.method ('init', function (state, id, title) {
    this.uber ('init', state, window.document.documentElement, id, title, null);
    this.div.style.position = 'absolute';
    this.div.style.zIndex = '1';
    this.titleDiv.style.borderTop = '0px';
    this.titleDiv.style.borderBottom = '0px';
    this.titleDiv.style.borderLeft = '1px';
    this.titleDiv.style.borderRight = '1px';
    this.titleDiv.style.borderStyle = 'solid';
    this.contentsDiv.style.border = '1px';
    this.contentsDiv.style.borderTop = '0px';
    this.contentsDiv.style.borderStyle = 'solid';
    this.contentsDiv.style.color = '#3366CC';
    this.contentsDiv.style.backgroundColor = "#FFFFFF";
    this.hide ();
});

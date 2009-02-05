window.TopToolbar = function (state, parent)
{
    if (arguments.length > 0)
    {
        this.init (state, parent);
    }
}

TopToolbar = window.TopToolbar;

importClass ("Studio.Toolbar")
TopToolbar.inherits (Toolbar);

TopToolbar.method ('init', function (state, parent) {
    this.uber ('init', state, parent, "TopToolbar", "<div align=right><a href='../Studio/logout'>Logout</a></div>");
    this.state = state;
    this.parent = parent;
});

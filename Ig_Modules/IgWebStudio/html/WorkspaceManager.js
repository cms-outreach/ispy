window.WorkspaceManager = function (state)
{
    if (arguments.length > 0)
    {
        this.init (state);
    }
}

WorkspaceManager = window.WorkspaceManager;

WorkspaceManager.method ('init', function (state) {
    this.state = state;
    this.state.services["WorkspaceManager"] = this;
    this.workspaceMap = new Array ();
});

WorkspaceManager.method ('add', function (label, workspace) {
    this.workspaceMap[label] = workspace;
});

WorkspaceManager.method ('get', function (label){
    return this.workspaceMap [label];
});
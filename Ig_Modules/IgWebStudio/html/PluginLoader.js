var IG_loadedPlugins = new Array ();

function loadPlugin (url)
{
    if (! IG_loadedPlugins[url])
    {
        var request = new HTTPRequest ();
        request.http.open ("get", url, false);
        request.http.send ("");
        eval (request.http.responseText);
        IG_loadedPlugins[url] = "OK!";
    }
}

function importClass (classURI)
{
    var tokens = classURI.split (".");
    var packageName = tokens[0];
    var className = tokens[1];
    var url = "../" + packageName + "/getScript?name=" + className +".js";
    if (! IG_loadedPlugins[url])
    {
        var request = new HTTPRequest ();
        request.http.open ("get", url, false);
        request.http.send ("");
        eval (request.http.responseText);
        IG_loadedPlugins[url] = "OK!";
    }
}
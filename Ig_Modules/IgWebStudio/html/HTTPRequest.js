function HTTPRequest ()
{
	if (window.XMLHttpRequest)
	{
	    this.http = new XMLHttpRequest ();
	    if (this.http.overrideMimeType)
	        this.http.overrideMimeType ('text/xml');
    }
	else if (window.ActiveXObject)
	{
	    try
	    {
		    this.http = new ActiveXObject ("Microsoft.XMLHTTP");
	    }
	    catch (A)
	    {
    		alert ("This web page requires XMLHttpRequest and Javascript 1.2 support. Please use a compatible browser like MS Internet Explorer 6, Firefox or Safari 1.2 ");	       
	    }
	}
	else
	{
		alert ("This web page requires XMLHttpRequest and Javascript 1.2 support. Please use a compatible browser like MS Internet Explorer 6, Firefox or Safari 1.2 ");
    }
}


<!DOCTYPE html 
     PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<title>Foo</title>
</head>
<script type="text/javascript" src="Studio/getScript?name=Inheritance.js"></script>
<script type="text/javascript" src="Studio/getScript?name=HTTPRequest.js"></script>
<script type="text/javascript" src="Studio/getScript?name=Callback.js"></script>
<script type="text/javascript" src="Studio/getScript?name=Widget.js"></script>
<script type="text/javascript" src="Studio/getScript?name=Evt.js"></script>
<script type="text/javascript" src="Studio/getScript?name=PopupMenu.js"></script>
<script type="text/javascript" src="Studio/getScript?name=PopupMenuItem.js"></script>
<script type="text/javascript">

var menu;

function foo ()
{
	Evt.addEventListener (document.documentElement, "contextmenu", bar, false);

	menu = new PopupMenu ("Menu Title");
	new PopupMenuItem (menu, "foo", prova1);
	new PopupMenuItem (menu, "bar", prova2);
}

var bar;

function prova1 ()
{
	alert ("a");

}

function prova2 ()
{
	alert ("b");
}

function bar (e)
{
	menu.show ();
	return true;
}

function cancelMenu ()
{
	menu.hide ();
}

</script>
<body onload="foo ()" oncontextmenu="return false;">
</body>
</html>

function A (foo)
{
	if (arguments.length > 0)
	{
		this.init ();
	}
}

A.method ('init', function (foo)
{
	alert ("A");
});

function B (foo)
{
	if (arguments.length > 0)
	{
		this.init ();
	}
}

alert (B.constructor);
B.inherits (A);
alert (B.constructor);

B.method ('init', function (foo)
{
	this.uber ('init', 1, 1);
	alert ("B");
});

function C (foo)
{
	if (arguments.length > 0)
	{
		this.init ();
	}
}

C.inherits (B);

C.method ('init', function (foo)
{
	this.uber ('init', 1, 1);
	alert ("C");
});

function D (foo)
{
	if (arguments.length > 0)
	{
		this.init ();
	}
}

D.inherits (C);

D.method ('init', function (foo)
{
	this.uber ('init', 1, 1);
	alert ("D");
});

new C (3);
new D (3);

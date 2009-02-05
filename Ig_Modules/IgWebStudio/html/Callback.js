function Callback (f)
{
	this.f = f;
	this.call = Callback.call;
}

Callback.call = function ()
{
	this.f ();
}

function Callback_1 (obj, f)
{
	this.obj = obj;
	this.f = f;
	this.call = Callback_1.call;
}

Callback_1.call = function (arg)
{
	this.obj[this.f] (arg);
}

function UniversalCallback (obj, f)
{
    if (arguments.length > 0)
    {
        this.init (arguments);
    }
}

//FIXME: all the Callbacks should be converted to UniversalCallback!!

UniversalCallback.method ("init", function (args) {
    var len = args.length;
    this.obj = args[0];
    this.f = args[1];
    this.args = Array ();
    for (var i = 2; i < len; i++)
    {
        this.args = args[i];
    }
});

UniversalCallback.method ("call", function () {
    var len = this.args.length;
    
    for (var i = 0; i < arguments.length; i++)
    {
        this.args [len + i] = arguments[i];
    }

    this.obj[this.f].apply (this.obj, this.args);
});


function Callback_0_1 (obj, f, arg)
{
	this.f = f;
	this.obj = obj;
	this.call = Callback_0_1.call;
	this.arg = arg;
}

Callback_0_1.call = function ()
{
	this.obj[this.f] (this.arg);
}

function Callback_0_2 (obj, f, arg1, arg2)
{
	this.f = f;
	this.obj = obj;
	this.call = Callback_0_2.call;
	this.arg1 = arg1;
	this.arg2 = arg2;
}

Callback_0_2.call = function ()
{
	this.obj[this.f] (this.arg1, this.arg2);
}


function EmptyCallback ()
{
	this.call = EmptyCallback.call;
}

EmptyCallback.call ()
{
}

function EmptyCallback_1 ()
{
	this.call = EmptyCallback_1.call;
}

EmptyCallback_1.call = function (arg)
{
}

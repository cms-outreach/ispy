// This provides javascript inheritance functionalities.

Function.prototype.method  = function (functionName, func) 
{
	this.prototype[functionName] = func;
	return this;
};

Function.method ('inherits', function (parentClass) 
{
	var thisClass = this;
	var d = 0, p = (this.prototype = new parentClass ());
	this.prototype.constructor = thisClass;
	this.prototype.superclass = parentClass;
	
	this.method ('uber', function (functionName) {
	var f, r, t = d, v = parentClass.prototype;
	
	if (t) 
	{
		while (t) 
		{
			v = v.superclass.prototype;
			t -= 1;
		}
		f = v[functionName];
	} 
	else 
	{
		f = p[functionName];
		if (f == this[functionName]) 
		{
			f = v[functionName];
		}
	}
	d += 1;

	r = f.apply(this, Array.prototype.slice.apply (arguments, [1]));
	d -= 1;
	return r;
	});
	return this;
});

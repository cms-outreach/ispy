window.NodeUtils = function ()
{
}

NodeUtils = window.NodeUtils;

NodeUtils.insertAfter = function (newChild, refChild)
{
	var parent=refChild.parentNode;
	if(parent.lastChild==refChild) 
		return parent.appendChild(newChild);
  	else 
		return parent.insertBefore(newChild,refChild.nextSibling);
}

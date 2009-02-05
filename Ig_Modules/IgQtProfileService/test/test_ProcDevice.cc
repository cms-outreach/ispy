#include "Ig_Modules/IgQtProfileService/src/IgOProfProcDevice.h"
#include <cassert>
#include <iostream>

int main ()
{
	IgOProfProcDevice myProc ("/proc");
	assert (myProc.exists ("pci") == true);
	assert (myProc.exists ("pc") == false);
	assert (myProc.canRead ("pci") == true);
	assert (myProc.canRead ("pci") == true);
	assert (myProc.canRead ("pci") == true);
	assert (myProc.canRead ("pc") == false);
	assert (myProc.canRead ("pci") == true);
	assert (myProc.canRead ("pci") == true);
	assert (myProc.canRead ("pci") == true);
	assert (myProc.canRead ("pc") == false);

	IgOProfProcDevice myProc2 ("/proc/sys/dev/oprofile");

	if (myProc2.exists ("cpu_type")) 
	{
	    int type;	
	    assert (myProc2.readInt ("cpu_type", type) == true);	
	    assert (type == 6);
	}
	else 
	{
	    std::cout << "/proc/sys/dev/oprofile does not exist." << std::endl;
	}
}

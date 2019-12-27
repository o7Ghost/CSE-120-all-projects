#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {


		RequestCPUrate (30);				// process 2
		SlowPrintf (7, "222222222222222222");
		Exit ();
	}


	RequestCPUrate (30);					// process 1

	SlowPrintf (7, "111111111111111111");
	Printf("CPUrate 0\n");
	RequestCPUrate (0);					// process 1

	SlowPrintf (7, "111111111111111111");
	Exit ();
}

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {


		RequestCPUrate (50);				// process 2
		SlowPrintf (4, "222222222222222222");
		Exit ();
	}


	if (Fork () == 0) {


		SlowPrintf (4, "333333333333333333");
		Exit ();
	}

	RequestCPUrate (80);					// process 1
	SlowPrintf (4, "111111111111111111");

	RequestCPUrate (50);					// process 1
	SlowPrintf (4, "111111111111111111");
	Exit ();
}

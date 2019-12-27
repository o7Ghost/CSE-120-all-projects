#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {

		RequestCPUrate (50);				// process 2
		SlowPrintf (7, "22222222222222222222");
		Exit ();
	}

	if (Fork () == 0) {

		SlowPrintf (7, "33333333333333333333");
		Exit ();
	}

	RequestCPUrate (50);					// process 1
	SlowPrintf (7, "11111111111111111111");

	RequestCPUrate (0);					// process 1
	SlowPrintf (7, "11111111111111111111");
	Exit ();
}

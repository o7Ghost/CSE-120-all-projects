#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {

	if (Fork () == 0) {

		SlowPrintf (7, "444444444444444444");
		Exit ();
	}
		SlowPrintf (7, "222222222222222222");
		Exit ();
	}


	if (Fork () == 0) {

		SlowPrintf (7, "333333333333333333");
		Exit ();
	}

	RequestCPUrate (99);					// process 1
	SlowPrintf (7, "111111111111111111");
	Exit ();
}

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {

		if (Fork () == 0) {

			RequestCPUrate (10);			// process 4
			SlowPrintf (7, "4444444444444444");
			Exit ();
		}

		RequestCPUrate (30);				// process 2
		SlowPrintf (7, "2222222222222222");
		Exit ();
	}

	if (Fork () == 0) {

		RequestCPUrate (20);				// process 3
		SlowPrintf (7, "3333333333333333");
		Exit ();
	}

	RequestCPUrate (40);					// process 1
	SlowPrintf (7, "1111111111111111");
	Exit ();
}

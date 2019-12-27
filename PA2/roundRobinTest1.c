#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {


		RequestCPUrate (30);

		if (Fork() == 0) {
			SlowPrintf(7,"33");
			Exit();

		}
		SlowPrintf (7, "22");
		Exit ();
	}


	RequestCPUrate (40);					// process 1
	SlowPrintf (7, "11");
	Exit ();
}

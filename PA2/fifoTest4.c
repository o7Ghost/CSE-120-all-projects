#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{

	
	//Printf("inner\n");
	if (Fork () == 0) {

		if (Fork () == 0) {

			if (Fork () == 0) {

				SlowPrintf (0, "6");		// process 4
				Exit ();
			}

			SlowPrintf (0, "4");		// process 4
			Exit ();
		}

		SlowPrintf (0, "2222222222");			// process 2
		Exit ();
	}

	//Printf("inner2\n");
	if (Fork () == 0) {

		if (Fork () == 0) {


			if (Fork () == 0) {

				SlowPrintf (0, "7");		// process 4
				Exit ();
			}

			SlowPrintf (0, "5");		// process 4
			Exit ();
		}
		SlowPrintf (0, "3333333333");			// process 3
		Exit ();
	}


	SlowPrintf (0, "1111111111");				// process 1
	Exit ();
}

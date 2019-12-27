#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{

	
	//Printf("inner\n");
	if (Fork () == 0) {

		if (Fork () == 0) {

			if (Fork () == 0) {

				if (Fork () == 0) {


					if (Fork () == 0) {

					if (Fork () == 0) {

					if (Fork () == 0) {

					if (Fork () == 0) {

					if (Fork () == 0) {

					if (Fork () == 0) {

					if (Fork () == 0) {

						SlowPrintf (0, "12");		// process 4
						Exit ();
					}
						SlowPrintf (0, "11");		// process 4
						Exit ();
					}
						SlowPrintf (0, "10");		// process 4
						Exit ();
					}
						SlowPrintf (0, "9");		// process 4
						Exit ();
					}
						SlowPrintf (0, "8");		// process 4
						Exit ();
					}
						SlowPrintf (0, "7");		// process 4
						Exit ();
					}
						SlowPrintf (0, "6");		// process 4
						Exit ();
					}

					SlowPrintf (0, "5");		// process 4
					Exit ();
				}
				SlowPrintf (0, "4");		// process 4
				Exit ();
			}

			SlowPrintf (0, "3");		// process 4
			Exit ();
		}

		SlowPrintf (0, "2222222222");			// process 2
		Exit ();
	}


	SlowPrintf (0, "1111111111");				// process 1
	Exit ();
}

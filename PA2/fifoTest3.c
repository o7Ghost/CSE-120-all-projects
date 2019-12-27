#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{

	
	//Printf("inner\n");
	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "4444444444");		// process 4
			Exit ();
		}

		SlowPrintf (0, "2222222222");			// process 2
		Exit ();
	}

	//Printf("inner2\n");
	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "12");		// process 4
			Exit ();
		}
		SlowPrintf (0, "3333333333");			// process 3
		Exit ();
	}

	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "13");		// process 4
			Exit ();
		}
		SlowPrintf (0, "5");			// process 3
		Exit ();
	}

	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "14");		// process 4
			Exit ();
		}
		SlowPrintf (0, "6");			// process 3
		Exit ();
	}

	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "15");		// process 4
			Exit ();
		}
		SlowPrintf (0, "7");			// process 3
		Exit ();
	}

	
	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "16");		// process 4
			Exit ();
		}
		SlowPrintf (0, "8");			// process 3
		Exit ();
	}

	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "17");		// process 4
			Exit ();
		}
		SlowPrintf (0, "9");			// process 3
		Exit ();
	}

	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "18");		// process 4
			Exit ();
		}
		SlowPrintf (0, "10");			// process 3
		Exit ();
	}

	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "19");		// process 4
			Exit ();
		}
		SlowPrintf (0, "11");			// process 3
		Exit ();
	}
	
	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "21");		// process 4
			Exit ();
		}
		SlowPrintf (0, "20");			// process 3
		Exit ();
	}


	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (0, "23");		// process 4
			Exit ();
		}
		SlowPrintf (0, "22");			// process 3
		Exit ();
	}

	SlowPrintf (0, "1111111111");				// process 1
	Exit ();
}

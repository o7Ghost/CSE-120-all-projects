/* mycode2.c: your portion of the kernel
 *
 *   	Below are procedures that are called by other parts of the kernel. 
 * 	Your ability to modify the kernel is via these procedures.  You may
 *  	modify the bodies of these procedures any way you wish (however,
 *  	you cannot change the interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mycode2.h"

#define TIMERINTERVAL 1 	// in ticks (tick = 10 msec)
#define L 100000

/* 	A sample process table. You may change this any way you wish. 
 */
int head = -1;
int tail = -1;
int tracker;
int lastEx = 0;
int countProc = 0;
int unusedProc = 1000;
int min;
int pPid;
int minPos;
  
static struct {
	int valid;		// is this entry valid: 1 = yes, 0 = no
	int pid;		// process ID (as provided by kernel)
	int next;
	int prev;
	int strife;
	int pValue;
	int requested;
	int requestPrc;
	int halt;	
} proctab[MAXPROCS];



void InitSched ()
{
	int i;

	if (GetSchedPolicy () == NOSCHEDPOLICY) {	// leave as is
		SetSchedPolicy (ARBITRARY);	// set policy here
	}
		
	for (i = 0; i < MAXPROCS; i++) {
		proctab[i].valid = 0;
		proctab[i].next = -1;
		proctab[i].prev = -1;
		proctab[i].strife = 0;
		proctab[i].pValue = 0;
		proctab[i].requested = 0;
		proctab[i].requestPrc = 0;
		proctab[i].halt = 0;	
	}
	
	
	pPid = 1;
	
	SetTimer (TIMERINTERVAL);
}



int StartingProc (p)
	int p;				// process that is starting
{
	int i;
	int diviedPrc;
	
	if ( head == -1 && tail == -1) {
	  head = 0;
	  tail = 0;
	}


	for (i = 0; i < MAXPROCS; i++) {
		if (! proctab[i].valid) {
			proctab[i].valid = 1;
			proctab[i].pid = p;
			proctab[tail].next = i;
			proctab[i].prev = tail;
			proctab[i].next = head;
			tail = i;
			proctab[head].prev = tail;
			tracker = i;

			if (GetSchedPolicy() == LIFO) {
				DoSched();
			}
			
						
			if (GetSchedPolicy() == PROPORTIONAL) {
	 			countProc++;	
				for (int k = 0; k < MAXPROCS; k++) {
					if (proctab[k].valid && !proctab[k].requested) {
						int bottom = unusedProc/countProc;
						if (bottom != 0) {
						  proctab[k].strife = L / bottom;
						}
					}

					if (proctab[k].valid) {
						proctab[k].pValue = 0;
					}
 	
				}
			}
			return (1);
		}
	}

	DPrintf ("Error in StartingProc: no free table entries\n");
	return (0);
}
			



int EndingProc (p)
	int p;				// process that is ending
{
	int i;

	for (i = 0; i < MAXPROCS; i++) {
		if (proctab[i].valid && proctab[i].pid == p) {
			proctab[i].valid = 0;
			proctab[i].pid = -1;			
			proctab[proctab[i].prev].next = proctab[i].next;
			proctab[proctab[i].next].prev = proctab[i].prev;
			head = proctab[i].next;
			lastEx = proctab[i].prev;
			proctab[i].next = -1;
			proctab[i].prev = -1;
			tracker--;

			
			if (GetSchedPolicy() == PROPORTIONAL) {
				if (proctab[i].requested) { 
				unusedProc += proctab[i].requestPrc;
				}	
				else {	
				countProc--;
				}	
				proctab[i].halt = 0;
				proctab[i].requested = 0;
				proctab[i].requestPrc = 0;
				proctab[i].strife = 0;
				proctab[i].pValue = 0;
				min = L;

					
				for (int k  = 0; k < MAXPROCS; k++) {
					if (proctab[k].valid && !proctab[k].requested) {
						proctab[k].strife = L / (unusedProc / countProc);
						proctab[k].pValue = 0;
						proctab[k].halt = 0; 	
					}

					if (proctab[k].valid) {
						proctab[k].pValue = 0;
					}
				}
			}
		
			return (1);
		}
	}

	DPrintf ("Error in EndingProc: can't find process %d\n", p);
	return (0);
}



int SchedProc ()
{
	int i;

	switch (GetSchedPolicy ()) {

	case ARBITRARY:

		for (i = 0; i < MAXPROCS; i++) {
			if (proctab[i].valid) {
				return (proctab[i].pid);
			}
		}
		return 0;	
		break;

	case FIFO:
		
		if (proctab[head].valid) {	
			return (proctab[head].pid);
		}
	
		return 0;		
		

		break;

	case LIFO:

		if (proctab[tracker].valid){	
		return proctab[tracker].pid;	
		}
		else {
		  return 0;
		}	
		break;

	case ROUNDROBIN:

		  if (proctab[proctab[lastEx].prev].valid) {
			int tmp = lastEx;
			lastEx = proctab[lastEx].prev;
		  return (proctab[proctab[tmp].prev].pid);


	}	
		return 0;
		break;

	case PROPORTIONAL:


		
		for (int i = 0; i < MAXPROCS; i++) {

			if (proctab[i].valid && proctab[i].pValue < min && proctab[i].halt == 0) {
				min = proctab[i].pValue;
				pPid = proctab[i].pid;
				minPos = i;	
			}

		}
	
		proctab[minPos].pValue += proctab[minPos].strife;
		min = proctab[minPos].pValue;


		if ( proctab[minPos].valid ) {
			return proctab[minPos].pid;
		}
		else {
			return 0;
		}		

		break;

	}
	
	return (0);
}



void HandleTimerIntr ()
{
	SetTimer (TIMERINTERVAL);

	switch (GetSchedPolicy ()) {	// is policy preemptive?

	case ROUNDROBIN:		// ROUNDROBIN is preemptive
	case PROPORTIONAL:		// PROPORTIONAL is preemptive
		DoSched ();		// make scheduling decision
		break;

	default:			// if non-preemptive, do nothing
		break;
	}
}


int MyRequestCPUrate (p, n)
	int p;				// process whose rate to change
	int n;				// percent of CPU time
{

	
	if (n < 0 || n > 100 ) {

	  return (-1);
	}


	for (int i = 0; i < MAXPROCS; i++) {
		if (proctab[i].valid && proctab[i].pid == p) {

		
		
			if (n != 0 && proctab[i].requested == 0) {
				if (n > (unusedProc/10)) {
					return (-1);

				} 
				int x = proctab[i].requestPrc - (n * 10);
				unusedProc = unusedProc + x;
				proctab[i].requestPrc = (n * 10);
				proctab[i].requested = 1;
				proctab[i].strife = L / (n * 10);
				countProc--;
			}
			else if ( n != 0 && proctab[i].requested == 1) {

				int y = proctab[i].requestPrc - (n * 10);
				unusedProc = unusedProc + y;
				proctab[i].requestPrc = (n * 10);
				proctab[i].strife = L / (n * 10);
			} 	

			else {

				int y = proctab[i].requestPrc - (n * 10);
				unusedProc = unusedProc + y;
				proctab[i].requestPrc = (n * 10);
			 	proctab[i].requested = 0;
				countProc++;
			}
			
			proctab[i].pValue = 0;
		}
	}
	
	
	if ( unusedProc == 0 ) {
		for (int i = 0; i < MAXPROCS; i++) {
			if (proctab[i].valid && !proctab[i].requested) {
				proctab[i].halt = 1;
				proctab[i].strife = 0;
				proctab[i].pValue = 0;
			}
		}

	}
	

	for (int i = 0; i < MAXPROCS; i++) {
		if (proctab[i].valid && !proctab[i].requested) {
			int check = unusedProc/countProc;
			if (check != 0 ) {
				proctab[i].strife = L / (unusedProc / countProc);
				proctab[i].halt = 0;
			}		
		}

		if (proctab[i].valid) {
			
			proctab[i].pValue = 0;
		}
	}


	return (0);
}


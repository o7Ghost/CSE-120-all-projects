/* mycode3.c: your portion of the kernel
 *
 *   	Below are procedures that are called by other parts of the kernel. 
 * 	Your ability to modify the kernel is via these procedures.  You may
 *  	modify the bodies of these procedures any way you desire (however,
 *  	you cannot change the interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mycode3.h"

#define FALSE 0
#define TRUE 1

/* 	A sample semaphore table. You may change this any way you wish. 
 */

typedef struct block {
    int valid;
    int next;
    int	procs;
    int prev;	 	
}block;

static struct {
	int valid;	// Is this a valid entry (was sem allocated)?
	int value;	// value of semaphore
	int head;
	int tail;
 	block list[MAXPROCS];
	int blockList;
} semtab[MAXSEMS];



/* 	InitSem () is called when kernel starts up.  Initialize data
 *  	structures (such as semaphore table) and call any initialization
 *   	procedures here. 
 */

void InitSem ()
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {		// mark all sems free
		semtab[s].valid = FALSE;
		semtab[s].head = -1;
		semtab[s].tail = -1;
		semtab[s].blockList = 0;
		for (int i = 0; i < MAXPROCS; i++) {		// mark all sems free

			semtab[s].list[i].procs = -1;
			semtab[s].list[i].valid = 1;
			semtab[s].list[i].next = -1;
			semtab[s].list[i].prev = -1;
		}
	
	}

/*
	for (int i = 0; i < MAXPROCS; i++) {		// mark all sems free

		procsBlocked[i].proc = -1;
		procsBlocked[i].block = 0;
	}
*/
}

/* 	MySeminit (p, v) is called by the kernel whenever the system
 *  	call Seminit (v) is called.  The kernel passes the initial
 *  	value v, along with the process ID p of the process that called
 * 	Seminit.  MySeminit should allocate a semaphore (find free entry
 * 	in semtab and allocate), initialize that semaphore's value to v,
 *  	and then return the ID (i.e., index of allocated entry). 
 */

int MySeminit (int p, int v)
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {
		if (semtab[s].valid == FALSE) {
			break;
		}
	}
	if (s == MAXSEMS) {
		DPrintf ("No free semaphores.\n");
		return (-1);
	}

	semtab[s].valid = TRUE;
	semtab[s].value = v;

	return (s);
}

/*   	MyWait (p, s) is called by the kernel whenever the system call
 * 	Wait (s) is called. 
 */

void MyWait (p, s)
	int p;				// process
	int s;				// semaphore
{
	/* modify or add code any way you wish */

	semtab[s].value--;

	if (semtab[s].value < 0) {

		if (semtab[s].head == -1 && semtab[s].tail == -1) {
			semtab[s].head = 0;
			semtab[s].tail = 0;
		}

		for (int i = 0; i < MAXPROCS; i++) {
			if (semtab[s].list[i].valid) {
				semtab[s].list[i].valid = 0;
				semtab[s].list[i].procs = p;
				semtab[s].list[semtab[s].tail].next = i;
				semtab[s].tail = i;
				semtab[s].blockList++;
				break;
			} 
		} 
		Block(p);
	}
}

/*  	MySignal (p, s) is called by the kernel whenever the system call
 *  	Signal (s) is called.  
 */

void MySignal (p, s)
	int p;				// process
	int s;				// semaphore
{
	/* modify or add code any way you wish */

	semtab[s].value++;
	int ublock = -1;
	int saveNext;

	if (semtab[s].blockList != 0) {
		ublock = semtab[s].list[semtab[s].head].procs;
		
		saveNext = semtab[s].list[semtab[s].head].next;
		semtab[s].list[semtab[s].head].procs = -1;
		semtab[s].list[semtab[s].head].next = -1;
		semtab[s].list[semtab[s].head].valid = 1;
		semtab[s].blockList--;
		semtab[s].head = saveNext;
		Unblock(ublock);
	}
}


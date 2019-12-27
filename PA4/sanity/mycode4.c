/*   	Umix thread package
 *
 */

#include <setjmp.h>
#include "aux.h"
#include "umix.h"
#include "mycode4.h"
#include <string.h>

static int MyInitThreadsCalled = 0;	// 1 if MyInitThreads called, else 0
static int previous;
static int lastThreadCreated = 0;
static int currentThread = 0;
static int queue[MAXTHREADS];
static int head;
static int tail;
static int queueS = 0;
static int threadS = 0;
  
static struct thread {			// thread table
	int valid;			// 1 if entry is valid, else 0
	jmp_buf env;			// current context
	jmp_buf backup_env;			// current context
	void (*func)();			// func saves f on top of stack
	int param;			// param saves p on top of stack
} thread[MAXTHREADS];

#define STACKSIZE	65536		// maximum size of thread stack

/* 	MyInitThreads () initializes the thread package. Must be the first
 *  	function called by any user program that uses the thread package.  
 */

void MyInitThreads ()
{
	int i;

	if (MyInitThreadsCalled) {		// run only once
		Printf ("MyInitThreads: should be called only once\n");
		Exit ();
	}

	for (i = 0; i < MAXTHREADS; i++) {	// initialize thread table
		thread[i].valid = 0;
		queue[i] = -1;
	}

	thread[0].valid = 1;			// initialize thread 0
	head = 0;
	tail = 0;
	threadS++;

	for (int k = 0; k < MAXTHREADS; k++) {
		                      
       		char stack[k*STACKSIZE];

		if (((int) &stack[k*STACKSIZE-1]) - ((int) &stack[0]) + 1 != k*STACKSIZE) {

			Printf ("Stack space reservation failed\n");
			Exit ();
		}

		
		if (setjmp (thread[k].env) != 0) {
			
			thread[currentThread].func(thread[currentThread].param);		// execute func (param)
			MyExitThread ();		// thread 1 is done - exit
		}

		memcpy(&thread[k].backup_env, &thread[k].env, sizeof(jmp_buf));  

	}
	MyInitThreadsCalled = 1;

}

/*  	MyCreateThread (f, p) creates a new thread to execute
 * 	f (p), where f is a function with no return value and
 * 	p is an integer parameter.  The new thread does not begin
 *  	executing until another thread yields to it. 
 */

int MyCreateThread (f, p)
	void (*f)();			// function to be executed
	int p;				// integer parameter
{
	if (! MyInitThreadsCalled) {
		Printf ("MyCreateThread: Must call MyInitThreads first\n");
		Exit ();
	}


	if (threadS >= MAXTHREADS) {
		return -1;
	}

	while (1) {

		if (thread[(lastThreadCreated + 1)%MAXTHREADS].valid) {

			lastThreadCreated = (lastThreadCreated + 1)%MAXTHREADS;
		}

		else {
	
			lastThreadCreated = (lastThreadCreated + 1)%MAXTHREADS;
			thread[lastThreadCreated].func = f;	// func saves f on top of stack
			thread[lastThreadCreated].param = p;		// param saves p on top of stack
			queue[tail] = lastThreadCreated;
			tail = (tail + 1)%MAXTHREADS;
			thread[lastThreadCreated].valid = 1;	// mark the entry for the new thread valid
			queueS = (queueS + 1)%MAXTHREADS;
			threadS++;
			break;
		}
	}

	return (lastThreadCreated);		// done, return new thread ID
}

/*   	MyYieldThread (t) causes the running thread, call it T, to yield to
 * 	thread t. Returns the ID of the thread that yielded to the calling
 *  	thread T, or -1 if t is an invalid ID.  Example: given two threads
 *  	with IDs 1 and 2, if thread 1 calls MyYieldThread (2), then thread 2
 * 	will resume, and if thread 2 then calls MyYieldThread (1), thread 1
 * 	will resume by returning from its call to MyYieldThread (2), which
 *  	will return the value 2.
 */

int MyYieldThread (t)
	int t;				// thread being yielded to
{
	if (! MyInitThreadsCalled) {
		Printf ("MyYieldThread: Must call MyInitThreads first\n");
		Exit ();
	}

	if (t < 0 || t >= MAXTHREADS) {
		Printf ("MyYieldThread: %d is not a valid thread ID\n", t);
		return (-1);
	}
	if (! thread[t].valid) {
		Printf ("MyYieldThread: Thread %d does not exist\n", t);
		return (-1);
	}

	int tmpIndex;

	if (currentThread == t) {
		return currentThread; 
	}

	for (int i = 0; i < MAXTHREADS; i++) {
		if (queue[i] == t) {
			tmpIndex = i;
		}		
	}

	for (int j = tmpIndex; j < MAXTHREADS-1; j++) {
		queue[j] = queue[j+1];
	}

	queue[tail-1] = currentThread; 

	
        if (setjmp (thread[currentThread].env) == 0) {
		previous = currentThread;
		currentThread = t;
                longjmp (thread[t].env, 1);
        }

	return previous;
}

/*   	MyGetThread () returns ID of currently running thread. 
 */

int MyGetThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("MyGetThread: Must call MyInitThreads first\n");
		Exit ();
	}

	return currentThread;

}

/* 	MySchedThread () causes the running thread to simply give up the
 *  	CPU and allow another thread to be scheduled.  Selecting which
 *  	thread to run is determined here.  Note that the same thread may
 * 	be chosen (as will be the case if there are no other threads). 
 */

void MySchedThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("MySchedThread: Must call MyInitThreads first\n");
		Exit ();
	}

	int tmp = queue[0];

	for (int i = 0; i < MAXTHREADS-1; i++) {
		queue[i] = queue[i+1];
	}



	if (thread[currentThread].valid) {

		queue[tail-1] = currentThread;

 
        	if (setjmp (thread[currentThread].env) == 0) {
			previous = -1;
			currentThread = tmp;
                	longjmp (thread[currentThread].env, 1);
        	}

	}

	else {
		if (queueS > 0) {
			 
			currentThread = tmp;
			queueS--;
			tail--;
               		longjmp (thread[tmp].env, 1);
		}
		else {
			Exit();
		}
	}


}

/* 	MyExitThread () causes the currently running thread to exit. 
 */

void MyExitThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("MyExitThread: Must call MyInitThreads first\n");
		Exit ();
	}
	memcpy(&thread[currentThread].env, &thread[currentThread].backup_env, sizeof(jmp_buf));
	thread[currentThread].valid = 0;
	previous = -1;
	threadS--;
	MySchedThread();	
}

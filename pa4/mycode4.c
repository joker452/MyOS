/* mycode4.c: UMIX thread package
 *
 *   	Below are functions that comprise the UMIX user-level thread package. 
 * 	These functions are called by user programs that use threads.  You may
 *  	modify the bodies of these functions (and add code outside of them)
 *  	in any way you wish (however, you cannot change their interfaces).  
 */
#include <setjmp.h>
#include <string.h>
#include "aux.h"
#include "umix.h"
#include "mycode4.h"

#define STACKSIZE	65536		// maximum size of thread stack

static int MyInitThreadsCalled = 0;	// 1 if MyInitThreads called, else 0

static struct thread {			// thread table
	int valid;					// 1 if entry is valid, else 0
	void (*func)();				// function pointer
	int param;					// function param
	jmp_buf env;				// current context
	jmp_buf cacheEnv;
	int prev, next;
} thread[MAXTHREADS + 1];


static int curThread = 0;	// in the beginning, only thread 0 runs
static int nextThread = 0;	// next thread ID
static int numThread = 1; 	


/* 	MyInitThreads() initializes the thread package. Must be the first
 * 	function called by any user program that uses the thread package. 
 */

void MyInitThreads()
{
	int i, size;

	if (MyInitThreadsCalled) {		// run only once
		Printf("MyInitThreads: should be called only once\n");
		Exit();
	}

	for (i = 0; i < MAXTHREADS + 1; i++) {	// initialize thread table
		thread[i].valid = 0;
	}


	thread[0].prev = 1;
	thread[0].next = 1;
	thread[1].prev = 0;
	thread[1].next = 0;
	thread[1].valid = 1;			// initialize thread 0

	MyInitThreadsCalled = 1;

	// allocate stack space
	for (i = 1; i <= MAXTHREADS; ++i) {

		char stack[(size = i > 1? STACKSIZE + 256: 0)];
		stack[0] = 'a' + i;


		if (i > 1 && (((int) &stack[STACKSIZE - 1]) - ((int) &stack[0]) + 1 != STACKSIZE)) {
			Printf("Stack space reservation failed\n");
			Exit();
		}
		

		if (setjmp(thread[i].cacheEnv) != 0) {
			(*thread[curThread + 1].func)(thread[curThread + 1].param);
			MyExitThread();
		}
	}

}

/* 	MyCreateThread(f, p) creates a new thread to execute f(p),
 *   	where f is a function with no return value and p is an
 * 	integer parameter. The new thread does not begin executing
 *  	until another thread yields to it. 
 */

int MyCreateThread(void (*f)(), int p)
	// f: function to be executed
	// p: integer parameter
{
	int k;
	if (! MyInitThreadsCalled) {
		Printf("MyCreateThread: Must call MyInitThreads first\n");
		Exit();
	}

	if (numThread >= MAXTHREADS) {
		return -1;
	}

	if (setjmp(thread[curThread + 1].env) == 0) {	// save context of curThread

		/* The new thread will need stack space.  Here we use the
		 * following trick: the new thread simply uses the current
		 * stack.  So, there is no need to allocate space. However,
		 * to ensure that thread 0's stack may grow and (hopefully)
		 * not bump into thread 1's stack, the top of the stack is
		 * effectively extended automatically by declaring a local
		 * variable (a large "dummy" array). This array is never
		 * actually used. To prevent an optimizing compiler from
		 * removing it, it should be referenced. 
		 */

		// find nextThread's position in thread table
		for (k = 1; k <= MAXTHREADS; ++k) {
			int next = (nextThread + k) % MAXTHREADS;
			if (thread[next + 1].valid == 0) {
				nextThread = next;
				++numThread;
				break;
			}
		}

	
		// appendn nexThread to list tail
		thread[thread[0].prev].next = nextThread + 1;
		thread[nextThread + 1].prev = thread[0].prev;
		thread[0].prev = nextThread + 1;
		thread[nextThread + 1].next = 0;


		// set nextThread's function and parameter
		thread[nextThread + 1].func = f;
		thread[nextThread + 1].param = p;
		memcpy(thread[nextThread + 1].env, thread[nextThread + 1].cacheEnv, sizeof(jmp_buf));
		longjmp(thread[curThread + 1].env, 1);

	}

	thread[nextThread + 1].valid = 1;	// mark the entry for the new thread valid
	
	return(nextThread);		// done, return new thread ID
}

/*  	MyYieldThread(t) causes the running thread, call it T, to yield to
 * 	thread t.  Returns the ID of the thread that yielded to the calling
 * 	thread T, or -1 if t is an invalid ID.  Example: given two threads
 * 	with IDs 1 and 2, if thread 1 calls MyYieldThread(2), then thread 2
 *   	will resume, and if thread 2 then calls MyYieldThread(1), thread 1
 * 	will resume by returning from its call to MyYieldThread(2), which
 *  	will return the value 2.
 */

int MyYieldThread(int t)
	// t: thread being yielded to
{
	int ret, prevThread;
	if (! MyInitThreadsCalled) {
		Printf("MyYieldThread: Must call MyInitThreads first\n");
		Exit();
	}

	if (t < 0 || t >= MAXTHREADS) {
		Printf("MyYieldThread: %d is not a valid thread ID\n", t);
		return(-1);
	}
	if (!thread[t + 1].valid) {
		Printf("MyYieldThread: Thread %d does not exist\n", t);
		return(-1);
	}

	// thread yield to itself, returns immediately
	if (curThread == t) {
		return t;
	}

	// put curThread to tail
	thread[thread[curThread + 1].prev].next = thread[curThread + 1].next;
	thread[thread[curThread + 1].next].prev = thread[curThread + 1].prev;
	thread[thread[0].prev].next = curThread + 1;
	thread[curThread + 1].prev = thread[0].prev;
	thread[0].prev = curThread + 1;
	thread[curThread + 1].next = 0;
	// move thread t to head
	thread[thread[t + 1].prev].next = thread[t + 1].next;
	thread[thread[t + 1].next].prev = thread[t + 1].prev;
	thread[t + 1].next = thread[0].next;
	thread[t + 1].prev = 0;
	thread[thread[0].next].prev = t + 1;
	thread[0].next = t + 1;

    if ((ret = setjmp(thread[curThread + 1].env)) == 0) {
		prevThread = curThread;
		curThread = t;
		longjmp(thread[t + 1].env, prevThread + 1);
    }

	return ret > 0? ret - 1: ret;
}

/*  	MyGetThread() returns ID of currently running thread. 
 */

int MyGetThread()
{
	if (! MyInitThreadsCalled) {
		Printf("MyGetThread: Must call MyInitThreads first\n");
		Exit();
	}

	return curThread;
}

/* 	MySchedThread() causes the running thread to simply give up the
 * 	CPU and allow another thread to be scheduled. Selecting which
 * 	thread to run is determined here. Note that the same thread may
 *   	be chosen (as will be the case if there are no other threads). 
 */

void MySchedThread()
{
	if (! MyInitThreadsCalled) {
		Printf("MySchedThread: Must call MyInitThreads first\n");
		Exit();
	}
	// remove curThread
	thread[thread[curThread + 1].prev].next = thread[curThread + 1].next;
	thread[thread[curThread + 1].next].prev = thread[curThread + 1].prev;

	if (thread[curThread + 1].valid == 1) {
		// put curThread to tail
		thread[thread[0].prev].next = curThread + 1;
		thread[curThread + 1].prev = thread[0].prev;
		thread[0].prev = curThread + 1;
		thread[curThread + 1].next = 0;
		if (setjmp(thread[curThread + 1].env) == 0) {
			curThread = thread[0].next - 1;
			longjmp(thread[thread[0].next].env, -1);
    	}
	} else {
		curThread = thread[0].next - 1;
		longjmp(thread[thread[0].next].env, -1);
	}

}

/* 	MyExitThread() causes the currently running thread to exit.  
 */

void MyExitThread()
{
	if (! MyInitThreadsCalled) {
		Printf("MyExitThread: Must call MyInitThreads first\n");
		Exit();
	}

	if (numThread > 1) {
		--numThread;
		thread[curThread + 1].valid = 0;	
		MySchedThread();
	} else {
		--numThread;
		Exit();
	}
}

/* mycode3.c: your portion of the kernel
 *
 *   	Below are functions that are called by other parts of the kernel. 
 * 	Your ability to modify the kernel is via these functions.  You may
 *  	modify the bodies of these functions (and add code outside of them)
 *  	in any way you wish (however, you cannot change their interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mycode3.h"

#define FALSE 0
#define TRUE 1

/* 	A sample semaphore table. You may change this any way you wish. 
 */

static struct {
	int valid;	// Is this a valid entry (was sem allocated)?
	int value;	// value of semaphore
	int waitProcs[MAXPROCS + 1];	// list of wait process
	int bd_front;	
	int bd_end;
	int size;	// num of process in wait queue
} semtab[MAXSEMS];


/* 	InitSem() is called when kernel starts up. Initialize data
 * 	structures (such as the semaphore table) and call any initialization
 *   	functions here. 
 */

void InitSem()
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {		// mark all sems free
		semtab[s].valid = FALSE;
		semtab[s].bd_front = semtab[s].bd_end = 0;
		semtab[s].size = 0;
	}
}

/* 	MySeminit(v) is called by the kernel whenever the system call
 *  	Seminit(v) is called.  The kernel passes the initial value v.  
 *  	MySeminit should allocate a semaphore (find a free entry in
 * 	semtab and allocate), initialize that semaphore's value to v,
 * 	and then return the ID (i.e., index of the allocated entry). 
 */

int MySeminit(int v)
	// v: initial value of semaphore
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {
		if (semtab[s].valid == FALSE) {
			break;
		}
	}
	if (s == MAXSEMS) {
		DPrintf("No free semaphores\n");
		return(-1);
	}

	semtab[s].valid = TRUE;
	semtab[s].value = v;
	semtab[s].bd_front = semtab[s].bd_end = 0;
	semtab[s].size = 0;
	return(s);
}

/* 	MyWait(s) is called by the kernel whenever the system call
 *   	Wait(s) is called. 
 */

void MyWait(int s)
	// s: semaphore ID
{
	/* modify or add code any way you wish */
	if (-1 < s && s < MAXSEMS && semtab[s].valid == TRUE) {
		/* can value underflow? */
		semtab[s].value--;
		if (semtab[s].value < 0) {
			/* add calling process to queue */
			int p = GetCurProc();
			semtab[s].waitProcs[semtab[s].bd_end] = p;
			semtab[s].bd_end = (semtab[s].bd_end + 1) % (MAXPROCS + 1);
			semtab[s].size = semtab[s].size + 1;
			Block();
		}
	}
}

/* 	MySignal(s) is called by the kernel whenever the system call
 *  	Signal(s) is called. 
 */

void MySignal(int s)
	// s: semaphore ID
{
	/* modify or add code any way you wish */
	if (-1 < s && s < MAXSEMS && semtab[s].valid == TRUE) {
		semtab[s].value++;
		if (semtab[s].size > 0) {
			/* remove process at the head of queue */
			int p = semtab[s].waitProcs[semtab[s].bd_front];
			semtab[s].bd_front = (semtab[s].bd_front + 1) % (MAXPROCS + 1);
			semtab[s].size = semtab[s].size - 1;
			Unblock(p);
		}
	}
}

/* mycode2.c: your portion of the kernel
 *
 *   	Below are functions that are called by other parts of the kernel. 
 * 	Your ability to modify the kernel is via these functions.  You may
 *  	modify the bodies of these functions (and add code outside of them)
 *  	in any way you wish (however, you cannot change their interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mycode2.h"

#define TIMERINTERVAL 1	// in ticks (tick = 10 msec)

/* 	A sample process table. You may change this any way you wish. 
 */

static struct {
	int valid;		// is this entry valid: 1 = yes, 0 = no
	int pid;		// process ID (as provided by kernel)
} proctab[MAXPROCS + 1];


static int bd_front, bd_end;
static int lastProc;
static int size;

/* 	InitSched() is called when the kernel starts up. First, set the
 * 	scheduling policy (see sys.h). Make sure you follow the rules
 *   	below on where and how to set it.  Next, initialize all your data
 * 	structures (such as the process table).  Finally, set the timer
 *  	to interrupt after a specified number of ticks. 
 */

void InitSched()
{
	int i;

	/* First, set the scheduling policy. You should only set it
	 * from within this conditional statement. While you are working
	 * on this assignment, GetSchedPolicy() will return NOSCHEDPOLICY. 
	 * Thus, the condition will be true and you may set the scheduling
	 * policy to whatever you choose (i.e., you may replace ARBITRARY).  
	 * After the assignment is over, during the testing phase, we will
	 * have GetSchedPolicy() return the policy we wish to test (and
	 * the policy WILL NOT CHANGE during the entirety of a test).  Thus
	 * the condition will be false and SetSchedPolicy(p) will not be
	 * called, thus leaving the policy to whatever we chose to test
	 * (and so it is important that you NOT put any critical code in
	 * the body of the conditional statement, as it will not execute when
	 * we test your program). 
	 */
	if (GetSchedPolicy() == NOSCHEDPOLICY) {	// leave as is
							// no other code here
		SetSchedPolicy(ROUNDROBIN);		// set policy here
							// no other code here
	}
		
	/* Initialize all your data structures here */
	for (i = 0; i < MAXPROCS; i++) {
		proctab[i].valid = 0;
	}

	bd_front = bd_end = 0;
	lastProc = -1;
	size = 0;
	/* Set the timer last */
	SetTimer(TIMERINTERVAL);
}


/*  	StartingProc(p) is called by the kernel when the process
 * 	identified by PID p is starting. This allows you to record the
 * 	arrival of a new process in the process table, and allocate any
 * 	resources (if necessary). Returns 1 if successful, 0 otherwise. 
 */

int StartingProc(int p) 		
	// p: process that is starting
{
	int i;
	DPrintf("%d start \n", p);
	switch (GetSchedPolicy()) {
		case FIFO:
		case LIFO:
			if (size != MAXPROCS) {
				proctab[bd_end].pid = p;
				bd_end = (bd_end + 1) % (MAXPROCS + 1);
				DoSched();
				++size;
				return 1;
			}
			break;
		case ROUNDROBIN:
			if (size != MAXPROCS) {
				proctab[bd_end].pid = p;
				bd_end = (bd_end + 1) % (MAXPROCS + 1);
				++size;
				return 1;
			}
			break;			
		default:
			for (i = 0; i < MAXPROCS; i++) {
				if (! proctab[i].valid) {
					// DPrintf("Process %d start! its entry is %d\n", p, i);
					proctab[i].valid = 1;
					proctab[i].pid = p;
					return (1);
				}
			}
	}

	DPrintf("Error in StartingProc: no free table entries\n");
	return(0);
}
			

/*   	EndingProc(p) is called by the kernel when the process
 * 	identified by PID p is ending.  This allows you to update the
 *  	process table accordingly, and deallocate any resources (if
 *  	necessary).  Returns 1 if successful, 0 otherwise. 
 */


int EndingProc(int p)
	// p: process that is ending
{
	int i;
	DPrintf("%d end \n", p);
	switch (GetSchedPolicy()) {
		case FIFO:
		/* Ending should always begin from front */
			if (size > 0 && proctab[bd_front].pid == p) {
				proctab[bd_front].pid = 0;
				bd_front = (bd_front + 1) % (MAXPROCS + 1);
				--size;
				// DoSched();
				return 1;
			}
			break;
		case LIFO:
		/* Ending should always begin from end */
			if (size > 0 && proctab[(bd_end + MAXPROCS) % (MAXPROCS + 1)].pid == p) {
				proctab[(bd_end + MAXPROCS) % (MAXPROCS + 1)].pid = 0;
				bd_end = (bd_end + MAXPROCS) % (MAXPROCS + 1);
				--size;
				// DoSched();
				return 1;
			}
			break;
		case ROUNDROBIN:
			if (size > 0 && lastProc == p) {
				lastProc = -1;
				--size;
				return 1;
			}
			break;
		default:
			for (i = 0; i < MAXPROCS; i++) {
				if (proctab[i].valid && proctab[i].pid == p) {
					// DPrintf("Process %d end! free entry %d\n", p, i);
					proctab[i].valid = 0;
					return(1);
				}
			}
	}

	DPrintf("Error in EndingProc: can't find process %d\n", p);
	return(0);
}


/* 	SchedProc() is called by kernel when it needs a decision for
 * 	which process to run next. It will call the kernel function
 * 	GetSchedPolicy() which will return the current scheduling policy
 *   	which was previously set via SetSchedPolicy(policy). SchedProc()
 * 	should return a process PID, or 0 if there are no processes to run. 
 */

int SchedProc()
{
	// DPrintf("schedule, ");
	int i;
	switch(GetSchedPolicy()) {

	case ARBITRARY:

		for (i = 0; i < MAXPROCS; i++) {
			if (proctab[i].valid) {
				// DPrintf("Sched called !!!!!! choose %d\n", i);
				return(proctab[i].pid);
			}
		}
		break;

	case FIFO:
		/* schedule from font */
		if (size > 0) {
			return proctab[bd_front].pid;
		}
		break;

	case LIFO:
		/* schedule from end */
		if (size > 0) {
			return proctab[(bd_end + MAXPROCS) % (MAXPROCS + 1)].pid;
		}
		break;
	case ROUNDROBIN:
		// DPrintf("size: %d, ", size);
		if (size > 0) {
			if (lastProc > 0) {
				/* schedule from interrupt */
				// DPrintf("add %d, ", lastProc);
				proctab[bd_end].pid = lastProc;
				bd_end = (bd_end + 1) % (MAXPROCS + 1);
			}
			lastProc = proctab[bd_front].pid;
			proctab[bd_front].pid = 0;
			bd_front = (bd_front + 1) % (MAXPROCS + 1);
			DPrintf("choose %d\n", lastProc);
			return lastProc;
		}
		break;

	case PROPORTIONAL:

		/* your code here */

		break;

	}
	
	return(0);
}


/*  	HandleTimerIntr() is called by the kernel whenever a timer
 *  	interrupt occurs.  Timer interrupts should occur on a fixed
 * 	periodic basis.
 */

void HandleTimerIntr()
{
	SetTimer(TIMERINTERVAL);

	switch(GetSchedPolicy()) {	// is policy preemptive?

	case ROUNDROBIN:		// ROUNDROBIN is preemptive
	case PROPORTIONAL:		// PROPORTIONAL is preemptive

		DoSched();		// make scheduling decision
		break;

	default:			// if non-preemptive, do nothing
		break;
	}
}

/* 	MyRequestCPUrate(p, n) is called by the kernel whenever a process
 * 	identified by PID p calls RequestCPUrate(n).  This is a request for
 *   	n% of CPU time, i.e., requesting a CPU whose speed is effectively
 * 	n% of the actual CPU speed. Roughly n out of every 100 quantums
 *  	should be allocated to the calling process. n must be at least
 *  	0 and must be less than or equal to 100. MyRequestCPUrate(p, n)
 * 	should return 0 if successful, i.e., if such a request can be
 * 	satisfied, otherwise it should return -1, i.e., error (including
 * 	if n < 0 or n > 100 or over allocation). If MyRequestCPUrate(p, n) fails, it should
 *   	have no effect on the scheduling of this or any other process,
 * 	i.e., AS IF IT WERE NEVER CALLED.
 */

int MyRequestCPUrate(int p, int n)
	// p: process whose rate to change
	// n: percent of CPU time
{
	/* your code here */

	return(0);
}

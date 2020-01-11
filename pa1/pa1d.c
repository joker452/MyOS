/* Programming Assignment 1: Exercise D
 *
 * Let's continue using Yield(pid) to effect a variety of executions
 * sequences.  Using the code below, the sequence of the print statements
 * will be ABP:
 *
 *	First Child (A)
 *	Second Child (B)
 *	Parent (P)
 *
 *
 * Questions
 *
 * 1. See if you can cause a change in the sequence as specified below just by
 * ADDING Yield statements (i.e., do not remove any of the ORIGINAL Yield
 * statements, just add extra ones anywhere you wish):
 *
 * a. BAP
 * b. BPA
 * c. PAB
 * d. PBA
 * e. APB
 *
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main()
{
	int ppid, pid1, pid2;
    int rid;

	ppid = Getpid();

	if ((pid1 = Fork()) == 0) {

        /* first child executes here */
        // int lid = Yield(ppid);
        //Printf("first child get yielded from %d\n", lid);
		Printf("I am the first child, my pid is %d\n", Getpid());
		Exit();
	}

	rid = Yield(pid1);
    Printf("Parent get yielded from %d\n", rid);
	if ((pid2 = Fork()) == 0) {

		/* second child executes here */
        int rrid = Yield(ppid);
        Printf("child2 get yielded from %d\n", rrid);
		Printf("I am the second child, my pid is %d\n", Getpid());
		Exit();
	}

	rid = Yield(pid2);	// yield to second child before continuing
	Printf("Parent get yielded from %d\n", rid);
    Printf("I am the parent, my pid is %d\n", Getpid());
    //rid = Yield(pid2);
    //Printf("Parent get yielded from %d\n", rid);
    //rid = Yield(pid2);
    //Printf("Parent get yielded from %d\n", rid);

}

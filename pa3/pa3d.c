/* Programming Assignment 3: Exercise D
 *
 * Now that you have a working implementation of semaphores, you can
 * implement a more sophisticated synchronization scheme for the car
 * simulation. 
 *
 * Study the program below.  Car 1 begins driving over the road,  entering
 * from the East at 40 mph.  After 900 seconds,  both Car 3 and Car 4 try to
 * enter the road. Car 1 may or may not have exited by this time (it should
 * exit after 900 seconds, but recall that the times in the simulation are
 * approximate). If Car 1 has not exited and Car 4 enters, they will crash
 * (why?).  If Car 1 has exited,  Car 3 and Car 4 will be able to enter the
 * road, but since they enter from opposite directions,  they will eventually
 * crash. Finally,  after 1200 seconds, Car 2 enters the road from the West
 * and traveling twice as fast as Car 4.  If Car 3 was not coming from the
 * opposite direction, Car 2 would eventually reach Car 4 from the back and
 * crash.  You may wish to experiment with reducing the initial delay of
 * Car 2,  or increase the initial delay of Car 3, to cause Car 2 to crash
 * with Car 4 before Car 3 crashes with Car 4. 
 *
 *
 * Exercises
 *
 * 1) Modify the procedure driveRoad such that the following rules are obeyed:
 *
 *   	A) Avoid all collisions. 
 *
 * 	B) Multiple cars should be allowed on the road,  as long as they are
 *  	traveling in the same direction.  
 *
 *  	C) If a car arrives and there are already other cars traveling in the
 * 	SAME DIRECTION,  the arriving car should be allowed enter as soon as it
 * 	can. Two situations might prevent this car from entering immediately:
 *  	(1) there is a car immediately in front of it (going in the same
 *   	direction), and if it enters it will crash (which would break rule A);
 * 	(2) one or more cars have arrived at the other end to travel in the
 *  	opposite direction and are waiting for the current cars on the road
 *  	to exit, which is covered by the next rule.  
 *
 * 	D) If a car arrives and there are already other cars traveling in the
 * 	OPPOSITE DIRECTION,  the arriving car must wait until all these other
 *  	cars complete their course over the road and exit.  It should only wait
 *   	for the cars already on the road to exit; no new cars traveling in the
 * 	same direction as the existing ones should be allowed to enter. 
 *
 *  	E) The previous rule implies that if there are multiple cars at each
 *  	end waiting to enter the road, each side will take turns in allowing
 * 	one car to enter and exit. However,  if there are no cars waiting at
 * 	one end,  then as cars arrive at the other end, they should be allowed
 *  	to enter the road immediately.  
 *	
 *   	F) If the road is free (no cars), then any car attempting to enter
 * 	should not be prevented from doing so. 
 *
 *  	G) All starvation must be avoided.  For example,  any car that is
 *  	waiting must eventually be allowed to proceed.  
 *
 * This must be achieved ONLY by adding synchronization and making use of
 * shared memory (as described in Exercise C). You should NOT modify the
 * delays or speeds to solve the problem. In other words, the delays and
 * speeds are givens,  and your goal is to enforce the above rules by making
 * use of only semaphores and shared memory.  
 *
 * 2) Devise different tests (using different numbers of cars,  speeds
 * directions) to see whether your improved implementation of driveRoad
 * obeys the rules above. 
 *
 * IMPLEMENTATION GUIDELINES
 * 
 * 1) Avoid busy waiting.  In class one of the reasons given for using
 * semaphores was to avoid busy waiting in user code and limit it to
 * minimal use in certain parts of the kernel.  This is because busy
 * waiting uses up CPU time, but a blocked process does not. You have
 * semaphores available to implement the driveRoad function, so you
 * should not use busy waiting anywhere. 
 *
 * 2) Prevent race conditions.  One reason for using semaphores is to
 * enforce mutual exclusion on critical sections to avoid race conditions. 
 * You will be using shared memory in your driveRoad implementation.  
 * Identify the places in your code where there may be race conditions
 * (the result of a computation on shared memory depends on the order
 * that processes execute).  Prevent these race conditions from occurring
 * by using semaphores. 
 *
 * 3) Implement semaphores fully and robustly. It is possible for your
 * driveRoad function to work with an incorrect implementation of
 * semaphores,  because controlling cars does not exercise every use of
 * semaphores.  You will be penalized if your semaphores are not correctly
 * implemented, even if your driveRoad works. 
 *
 * 4) Control cars with semaphores: Semaphores should be the basic
 * mechanism for enforcing the rules on driving cars.  You should not
 * force cars to delay in other ways inside driveRoad such as by calling
 * the Delay function or changing the speed of a car.  (You can leave in
 * the delay that is already there that represents the car's speed,  just
 * don't add any additional delaying). Also,  you should not be making
 * decisions on what cars do using calculations based on car speed (since
 * there are a number of unpredictable factors that can affect the
 * actual cars' progress). 
 *
 * GRADING INFORMATION
 *
 * 1) Semaphores: We will run a number of programs that test your
 * semaphores directly (without using cars at all).  For example:
 * enforcing mututal exclusion, testing robustness of your list of
 * waiting processes, calling signal and wait many times to make sure
 * the semaphore state is consistent,  etc. 
 *
 * 2) Cars: We will run some tests of cars arriving in different ways
 * to make sure that you correctly enforce all the rules for cars given
 * in the assignment.  We will use a correct semaphore implementation for
 * these tests so that even if your semaphores are not correct you could
 * still get full credit on the driving part of the grade (you may assume
 * that in our implementation, semaphore unblocking is FIFO,  i.e.,  for
 * each semaphore, the order in which processes are unblocked is the
 * same order in which those same processes blocked).  Think about
 * how your driveRoad might handle different situations and write your
 * own tests with cars arriving in different ways to make sure you handle
 * all cases correctly. 
 *
 *
 * WHAT TO TURN IN
 *
 * You must turn in two files: mycode3.c and pa3d.c. mycode3.c should
 * contain you implementation of semaphores, and pa3d.c should contain
 * your modified version of InitRoad and driveRoad (Main will be ignored).  
 * Note that you may set up your static shared memory struct and other
 * functions as you wish. They should be accessed via InitRoad and driveRoad
 * as those are the functions that we will call to test your code.  
 *
 * Your programs will be tested with various Main programs that will exercise
 * your semaphore implementation,  AND different numbers of cars, directions
 * and speeds,  to exercise your driveRoad function.  Our Main programs will
 * first call InitRoad before calling driveRoad. Make sure you do as much
 * rigorous testing yourself to be sure your implementations are robust. 
 * Good luck.  
 */

#include <stdio.h>
#include "aux.h"
#include "sys.h"
#include "umix.h"


// west -> 0, east -> 1
#define POS_IDX(FROM)	(((FROM) == WEST) ? 0 : 1)

static struct {

	int onroads[2];		// cars on the road from different direction
	int blocked[2];		// cars blocked gate from different direction
	int gates[2];		// semaphore for gates, cars blocked on gate ARE NOT attempting entering
	int first_occupied[2]; // first position for different directions occupied or not
	int pos_lock[NUMPOS];	// mutex lock for each position 
	int shm_lock;   // mutex lock for the share memory struct

} shm;


void InitRoad(void);
void driveRoad(int from, int mph);

void Main()
{
	InitRoad();

	/* The following code is specific to this simulation,  e.g., number
	 * of cars, directions and speeds. You should experiment with
	 * different numbers of cars,  directions and speeds to test your
	 * modification of driveRoad.  When your solution is tested, we
	 * will use different Main procedures,  which will first call
	 * InitRoad before any calls to driveRoad.  So, you should do
	 * any initializations in InitRoad. 
	 */

	if (Fork() == 0) {
		Delay(1200);			// car 2
		driveRoad(WEST, 60);
		Exit();
	}

	if (Fork() == 0) {
		Delay(900);			// car 3
		driveRoad(EAST, 50);
		Exit();
	}

	if (Fork() == 0) {
		Delay(900);			// car 4
		driveRoad(WEST, 30);
		Exit();
	}

	driveRoad(EAST, 40);			// car 1

	Exit();
}

/* Our tests will call your versions of InitRoad and driveRoad, so your
 * solution to the car simulation should be limited to modifying the code
 * below. This is in addition to your implementation of semaphores
 * contained in mycode3.c.  
 */

void InitRoad()
{
	int i;
	Regshm((char *) &shm, sizeof(shm));
	shm.onroads[0] = shm.onroads[1] = 0;
	shm.blocked[0] = shm.blocked[1] = 0;
	shm.first_occupied[0] = shm.first_occupied[1] = 0;

	shm.gates[0] = Seminit(0);
	shm.gates[1] = Seminit(0);

	for (i = 0; i < NUMPOS; ++i) {
		shm.pos_lock[i] = Seminit(1);
	}

	shm.shm_lock = Seminit(1);
}

#define IPOS(FROM)	(((FROM) == WEST) ? 1 : NUMPOS)
#define TO(FROM) (((FROM) == WEST) ? NUMPOS: 1)

void driveRoad(int from, int mph)
	// from: coming from which direction
	// mph: speed of car
{
	int c;				// car ID c = process ID
	int p, np, i;			// positions

	c = Getpid();			// learn this car's ID

	int idx = POS_IDX(from);

	// condition C(1), C(2) and condition D will cause cars blocked
	Wait(shm.shm_lock);
	if ((shm.onroads[idx] > 0 && (shm.first_occupied[idx] > 0 || shm.blocked[1 - idx] > 0)) || shm.onroads[1 - idx] > 0) {
		++shm.blocked[idx];
		Signal(shm.shm_lock);
		Wait(shm.gates[idx]);
	} else {
		// attempt to enter road
		Wait(shm.pos_lock[IPOS(from) - 1]);
		// change road state
		++shm.onroads[idx];
		shm.first_occupied[idx] = 1;
		Signal(shm.shm_lock);
	}


	// DO NOT MODIFY THE NEXT 3 STATEMENTS IN ANY WAY!
	EnterRoad(from);
	PrintRoad();
	Printf("Car %d enters at %d at %d mph\n", c, IPOS(from), mph);

	for (i = 1; i < NUMPOS; i++) {
		if (from == WEST) {
			p = i;
			np = i + 1;
		} else {
			p = NUMPOS + 1 - i;
			np = p - 1;
		}

		Wait(shm.pos_lock[np - 1]);
		// DO NOT MODIFY THE NEXT 4 STATEMENTS IN ANY WAY!
		Delay(3600/mph);
		ProceedRoad();
		PrintRoad();
		Printf("Car %d moves from %d to %d\n", c, p, np);
		Signal(shm.pos_lock[p - 1]);


		if (p == IPOS(from)) {
			// unblock cars of reason C(1)
			Wait(shm.shm_lock);
			shm.first_occupied[idx] = 0;
			if (shm.blocked[1 - idx] == 0 && shm.blocked[idx] > 0) {
				--shm.blocked[idx];
				++shm.onroads[idx];
				shm.first_occupied[idx] = 1;
				Wait(shm.pos_lock[IPOS(from) - 1]);
				Signal(shm.shm_lock);
				Signal(shm.gates[idx]);
			} else {
				Signal(shm.shm_lock);
			}
		}
	}


	// DO NOT MODIFY THE NEXT 4 STATEMENTS IN ANY WAY!
	Delay(3600/mph);
	ProceedRoad();
	PrintRoad();
	Printf("Car %d exits road\n", c);
	Signal(shm.pos_lock[np - 1]);

	// change road state and unblock cars of reason D
	Wait(shm.shm_lock);
	if (--shm.onroads[idx] == 0 && shm.blocked[1 - idx] > 0) {
		--shm.blocked[1 - idx];
		++shm.onroads[1 - idx];
		shm.first_occupied[1 - idx] = 1;
		Wait(shm.pos_lock[TO(from) - 1]);
		Signal(shm.shm_lock);
		Signal(shm.gates[1 - idx]);
	} else {
		Signal(shm.shm_lock);
	}
}

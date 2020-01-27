/* Copyright 1989-2020, Joseph Pasquale, University of California, San Diego
 *
 *	mycode interface for pa3
 */

void InitSem();				// initialize semaphores
int MySeminit(int v);			// alloc sem, init to v, return semid
void MyWait(int s);			// wait using sem s */
void MySignal(int s);			// signal using sem s */

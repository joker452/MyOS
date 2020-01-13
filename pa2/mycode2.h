/* Copyright 1989-2020, Joseph Pasquale, University of California, San Diego
 *
 *	mycode interface for pa2
 */

void InitSched ();
int StartingProc (int pid);
int EndingProc (int pid);
int SchedProc ();
void HandleTimerIntr ();
int MyRequestCPUrate (int pid, int n);

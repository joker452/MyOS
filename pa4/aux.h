/* Copyright 1989-2020, Joseph Pasquale, University of California, San Diego
*
* UNIX auxiliary function interfaces
*/

void Printf(char *fmt, ...);
void DPrintf(char *fmt, ...);
void SlowPrintf(int n, char *fmt, ...);
int Delay(int t);
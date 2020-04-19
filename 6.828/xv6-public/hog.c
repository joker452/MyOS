#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {

    int pid;

    if ((pid = fork()) < 0) {
        printf(2, "fork failed\n");
    } else if (pid == 0) {
        while (1);
    } else {
        while (1);
    }

    return 0;
    
}
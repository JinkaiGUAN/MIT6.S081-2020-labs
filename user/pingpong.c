#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[]) 
{
    int p[2];
    pipe(p);
    printf("P[0] = %d, P[1]=%d \n", p[0], p[1]); // P[0] = 3, P[1]=4 , which reprents that we will not ocupise the standrize input and output.

    int childPid = fork();
    if (childPid == 0) { // child process
        // we need to read the data from pipe, 
        read(p[0], &childPid, 4);
        printf("%d: received ping\n", childPid);
    } else { // parent process
        // we need to write teh data to the pipe and then wait
        write(p[1], &childPid, 4);
        wait(0); 
        printf("%d: received pong\n", getpid());
    }   

    exit(0);
}


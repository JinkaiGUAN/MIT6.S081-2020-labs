#include "kernel/types.h"
#include "user/user.h"

#define PRIME_NUM 35

void RunChild(int* readPipe) 
{
    close(readPipe[1]); // read and write pipe
    int n, status;
    status = read(readPipe[0], &n, sizeof(int));
    if (status == 0) { 
        close(readPipe[0]); // no more number in the pipe
        exit(1);
    } else if (status == -1) {
        fprintf(2, "Parent1: Fail to read from pipe!\n");
        exit(1);
    }

    printf("prime %d\n", n); // print the first number of the prime
    
    int parentPipe[2];
    if (pipe(parentPipe) == -1) {
        fprintf(2, "Parent1: Fail to create pipe!\n");
        exit(1);
    }

    int childPid = fork();
    if (childPid < 0) {
        fprintf(2, "Parent1: Fail to create child process!\n");
        exit(-1);
    } else if (childPid == 0) {
        close(readPipe[0]);
        RunChild(parentPipe);
    } else {
        close(parentPipe[0]);
        int num;
        while ((status = read(readPipe[0], &num, sizeof(int))) > 0) {
            if (num % n != 0) {
                if (write(parentPipe[1], &num, sizeof(int)) == -1) {
                    fprintf(2, "Parent1: Fail to write to pipe!\n");
                    exit(1);
                }
            } 
        }
        if (status == -1) {
            fprintf(2, "Parent1: Fail to read from pipe!\n");
            exit(1);
        }
        close(parentPipe[1]);
        wait((int *) 0);
    }


}


int 
main(int argc, char* argv[]) 
{
    int inputPipe[2];
    if (pipe(inputPipe) == -1) {
        fprintf(2, "Failed to create the pipe!\n");
        exit(1);
    }

    int childPid = fork();
    if (childPid < 0) {
        fprintf(2, "Failed to create the child process!\n");
        exit(1);
    } else if (childPid == 0) {
        RunChild(inputPipe);
    } else {
        close(inputPipe[0]);
        for (int i = 2; i <= PRIME_NUM; i++) {
            if (write(inputPipe[1], &i, sizeof(int)) == -1) {
                fprintf(2, "Parenet: Failed to write to the pipe!\n");
                exit(1);
            }
        }
        close(inputPipe[1]);
        wait((int *) 0);
    }


    exit(0);
}
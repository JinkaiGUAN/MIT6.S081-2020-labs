#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
	// first we need to check the argument, print error msg if it does not pass an argument.
	int n;
	if (argc != 2) {
		fprintf(2, "Please enter a number after sleep command!\n");
		exit(1);
	} else {
		int tag = 1;
		char* p = argv[1];
		while (*p){
			if (*p < '0' || *p > '9') {
				tag = 0;
				break;
			}
			p++;
		}
		if (tag) {
			n = atoi(argv[1]);
			sleep(n);
			fprintf(1, "We slept for %d seconds!", n);
			exit(0);
		} else {
			fprintf(2, "We got wrong number!");
			exit(1);
		}
	}
}

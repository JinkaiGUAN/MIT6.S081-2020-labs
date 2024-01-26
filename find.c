#include "kernel/types.h"
#include "user/user.h"

void find(char* currentPath, char* target) {
}


int 
main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(2, "Please enter the file path and the target arguments!\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}

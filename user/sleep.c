#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	if(argc <= 1) {
		fprintf(2, "you need to input an interger\n");
		exit(0);
	}	

	uint n = atoi(argv[1]);
	sleep(n);
	exit(0);
}
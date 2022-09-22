#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int p[2];
	pipe(p);
	int pid = fork();
	if(pid == 0) {
		int t;
		close(p[1]);
		while(read(p[0], &t, sizeof(int)) >= 1) {
			printf("%d\n", t);
			if(t == 31) {
			close(p[0]);
			}
		}
	}
	else {
		int i = 0, status;
		close(p[0]);
		for(i = 0; i < 32; i ++ )
			write(p[1], &i, sizeof(int));
		wait(&status);
		close(p[1]);
	}


	exit(0);
}
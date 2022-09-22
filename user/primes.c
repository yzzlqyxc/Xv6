#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void check_prime(int *p, int n) {
	int p_c[2], num, pid;
	close(p[1]);
	int x = read(p[0], &num, sizeof(int));
	if(x <= 0) {
		close(p[0]);
		close(p[1]);
		return;
	}

	printf("prime %d\n", num);
	pipe(p_c);

	pid = fork();
	if(pid) {
		int t;
		// sent numbers to child 
		close(p_c[0]); // parent process no need to read
		while(read(p[0], &t, sizeof(int)) == sizeof(int)) {
			if(t % num == 0) {
				continue;
			}
			else {
				write(p_c[1], &t, sizeof(int));
			}
		}
		close(p[0]);
		close(p_c[1]);
	}
	else {
		// create an prime checker
		check_prime(p_c, n + 1);
	}
	wait(0);
	exit(0);
}

int
main(int argc, char *argv[])
{
	int p[2];
	pipe(p);
	int pid = fork();
	if(pid) {
		int status;
		close(p[0]);
		for(int i = 2; i <= 35; i ++ ) {
			write(p[1], &i, sizeof(int));
		}
		close(p[1]);
		while(wait(&status) >= 0);
	}
	else {
		check_prime(p, 0);
	}
	exit(0);
}
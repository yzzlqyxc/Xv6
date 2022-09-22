#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int p[2];
	pipe(p);
	int pid = fork();
	char line[5];

	if(pid == 0) {
		int id = getpid();
		read(p[0], line, 1);
		close(p[0]);
		printf("%d: received ping\n", id, line);

		write(p[1], "a", 1);
		close(p[1]);
	}
	else {
		int id = getpid();
		write(p[1], "b", 1);
		close(p[1]);
		read(p[0], line, 1);
		printf("%d: received pong\n", id, line);
		close(p[0]);
	}
	exit(0);
}
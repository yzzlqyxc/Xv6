#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char buf[512], ch;
char function[200] = {0}, *arg[MAXARG];
int cnt, argcnt;

int
main(int argc, char *argv[])
{

	if(argc == 1) {
		exit(0);
	}

	strcpy(function, argv[1]);
	arg[argcnt ++ ] = function;
	for(int i = 2; i < argc; i ++ ) {
		arg[argcnt ++ ] = argv[i];
	}

	while(read(0, &ch, sizeof(ch)) == sizeof(ch)) {
		buf[cnt ++ ] = ch;
		if(ch != '\n' && ch != ' ') continue;
		int pid = fork();
		if(pid) {
			// parent process wait for the child's finish
			wait(0);
		}
		else {
			cnt -= 1;
			buf[cnt] = 0;
			char *line = (char*)malloc(sizeof(buf) + 3);
			strcpy(line, buf);
			arg[argcnt ++ ] = line;
			arg[argcnt] = 0;
			// for(int i = 0; i < argcnt; i ++ )
			// 	printf("%s\n", arg[i]);
			exec(function, arg);
			exit(0);
		}

		cnt = 0;
	}

	exit(0);
}
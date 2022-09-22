#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char * cat(char *tar, char* src) {
	char *k;
	k = (char*)malloc(sizeof(tar) + sizeof(src) + 4);
	strcpy(k, tar);
	char *p = k + strlen(k);
	*p ++ = '/';	
	for(int i = 0; i < strlen(src); i ++ ) 
		*p++ = src[i];
	*p = 0;
	return k;
}

void find(char *path, char *findname, char *root) {
	int fd;
	struct stat st;
	struct dirent de;
	// open the path and check its type
	if((fd = open(path, 0)) < 0) {
		fprintf(2, "open directory or file fail %s\n", path);
		return;
	}
	if(fstat(fd, &st) < 0) {
		fprintf(2, "find : cannot stat %s\n", path);
	}

	// find files due to its type
	switch(st.type) {
		// if its a file, then just compare their name
		case T_DEVICE :
		case T_FILE : 
			if(strcmp(path, findname)) 
				printf("%s\n", path);
			break;

		// other wise find through the dir
		case T_DIR :
			while(read(fd, &de, sizeof(de)) == sizeof(de)) {
				if(de.inum == 0) continue;
				if(stat(cat(root, de.name), &st) < 0) {
					printf("file stat error %s\n", de.name);
					continue;
				}
				if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
				if(st.type == T_DIR) {
					find(cat(root, de.name), findname, cat(root, de.name));
				}

				if(strcmp(de.name, findname) == 0) {
					printf("%s\n", cat(root, de.name));
				}
			}
	}
}

int
main(int argc, char *argv[])
{
	if(argc < 3) {
		printf("find: wrong input farmat\n");
		exit(1);
	}
	for(int i = 2; i < argc; i ++ ) {
		find(argv[1], argv[i], argv[1]);
		break;
	}

	exit(0);
}
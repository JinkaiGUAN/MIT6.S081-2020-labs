#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define STDINP  0
#define STDOUT  1
#define STDERR  2

#define	DEBUG 1 


void FindPath(char* currentPath, char* filename) {
	int fd;
	struct dirent de;
	struct stat st;

//	if (DEBUG) {i
//		printf("find-debug:1 - current path %s, target filename %s\n", currentPath, filename);
//	}

	if ((fd = open(currentPath, 0)) < 0) { // we need to check the flag for open
		fprintf(STDERR, "find: cannot open the path %s\n", currentPath);
		return;	
	}

//	if (DEBUG) {
//                printf("find-debug:2 - current path %s, target filename %s\n", currentPath, filename);
//        }

	if (fstat(fd, &st) < 0) {
		fprintf(STDERR, "find: cannot stat %s\n", currentPath);
		close(fd);
		return;
	}
//	if (DEBUG) {
//                printf("find-debug:3 - current path %s, target filename %s\n", currentPath, filename);
//        }

//	if (read(fd, &de, sizeof(de)) != sizeof(de)) { //  读取失败
//		fprintf(STDERR, "We did not read well!");
//		exit(1);
//	}

//	if (DEBUG) {
//                printf("find-debug:4 - current path %s, target filename %s\n", currentPath, filename);
//        }
	
	char buf[512];  // 用来存放文件路径
	char* p;

	switch(st.type) {
	case T_FILE:
		if (strcmp(de.name, filename) == 0) {
			printf("%s/%s\n", currentPath, filename);
		} else {
			if (DEBUG) {
				fprintf(STDERR, "We got different name!");
			}
		}		
		break;

	case T_DIR: // we need to concat the path and find more.
		if (strlen(currentPath) + 1+ DIRSIZ + 1 > sizeof(buf)) {
			fprintf(STDERR, "find: path too long!");
			break;
		}

		// 开始拼接下一层路径
		strcpy(buf, currentPath);
		p = buf + strlen(buf);
	        *p++ = '/';
		
		if (DEBUG) {
			fprintf(STDERR, "we got the following path p - %s, buf - %s \n", p, buf);
		}

		while(read(fd, &de, sizeof(de)) == sizeof(de)) {
			if ((de.inum == 0) || strcmp(de.name, ".") || strcmp(de.name, "..")) { // 没有文件， 以及如果name 是. , .. 都可以去除
				continue;
			}
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			// 获取文件描述符
			if (stat(buf, &st) < 0) {
				fprintf(STDERR, "find: cannot stat %s\n", currentPath);
				continue;
			}

			if (DEBUG) {
				printf("find-debug: 6 -  we look the path %s\n", buf);
			}
			if (st.type == T_FILE) {
				if (strcmp(de.name, filename) == 0) {
					printf("%s\n", buf);
				}
			} else if (st.type == T_DIR) {
				FindPath(buf, filename);
			}
		}	
		break;
	case T_DEVICE:
		fprintf(STDERR, "find: we got a device\n");
		break;
	default:
		fprintf(STDERR, "find: we got the wrong type %d!\n", st.type);
		break;
	}
	close(fd);
}


int 
main(int argc, char* argv[]) 
{
	if (argc != 3) {
		fprintf(STDERR, "Please use the correct find command, we need the path and the filename!\n");
		exit(1);
	}
	
	FindPath(argv[1], argv[2]);
	exit(0);
}

/*********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  spipe.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/04/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "03/04/23 12:47:18"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define ARRAY_SIZE(a)			(sizeof(a) / sizeof(a[0]))
#define BUF_SIZE				512
#define PARENT					"This is parent process!\n"
#define CHILD					"This is child process!\n"

int main(int argc, char *argv[]){

	int							n;
	int							reader[2];
	int							writer[2];
	char						buffer[BUF_SIZE] = {0};
	int							wstatus, pid = -1;

	if(pipe(reader) < 0){
		printf("Fail to create a pipe for reading! [%s]\n", strerror(errno));
		return -1;
	}
	if(pipe(writer) < 0){
		printf("Fail to create a pipe for writing! [%s]\n", strerror(errno));
		return -2;
	}


	pid = fork();
	if(pid < 0){
		printf("Fail to create a new child process! [%s]\n", strerror(errno));
		return -3;
	}
	else{
		close(reader[1]);
		close(writer[0]);
	}

	for(n=0; n<10; n++){
		if(pid == 0){
			strncpy(buffer, CHILD, sizeof(buffer));
			write(writer[1], buffer, sizeof(buffer));
			memset(buffer, 0, sizeof(buffer));
		}
		else{
			read(reader[0], buffer, sizeof(buffer));
			printf("##PARENT: %s##", buffer);
			memset(buffer, 0, sizeof(buffer));
		}

		sleep(2);

		if(pid > 0){
			strncpy(buffer, PARENT, sizeof(buffer));
			write(writer[1], buffer, sizeof(buffer));
			memset(buffer, 0, sizeof(buffer));
		}
		else{
			read(reader[0], buffer, sizeof(buffer));
			printf("$$CHILD: %s$$", buffer);
			memset(buffer, 0, sizeof(buffer));
		}
	}

	waitpid(pid, &wstatus, 0);
	return 0;
}

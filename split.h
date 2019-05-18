#ifndef SPLT_H_
#define SPLT_H_

	//#define DEBUG

	#include <stddef.h>	// for size_t
	#include <stdlib.h> // for the exit system call
	#include <stdio.h> // for perror

	#define PANIC(msg, status) do{perror(msg);exit(status);}while(0)

	#define FL(fd,fs,indx)	snprintf(fd,\
					FILENAME_LEN_MAX,\
					"%s.%u",\
					fs,\
					indx)
	
	#define BUFFER_SIZE 2048
	#define FILENAME_LEN_MAX 255

	#define ERR_FILE_NAME_TOO_LONG -1

	typedef unsigned char byte;
	extern byte buffer[BUFFER_SIZE];
	
	int split(char *source, char *destination, size_t filesize, int startnum);
	int join(char *firstfile, char *destfile);

#endif // SPLT_H_

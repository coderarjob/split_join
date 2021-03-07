#include <stdlib.h> // size_t, strtol
#include <string.h> // strcasecmp
#include <limits.h> // LONG_MAX, LONG_MIN
#include <stdio.h>
#include "split.h"

byte buffer[BUFFER_SIZE];

#define USAGE(p) printf("USAGE:\n%s [-j|-s -sz <splitsize (gb|mb|kb)>] source dest\n",p)

#define MODE_UNDEFINED 0
#define MODE_SPLIT 1
#define MODE_JOIN 2

size_t strtosize(char *sizestring);

int main(int argc, char *argv[])
{
	char *source = NULL;
	char *destination = NULL;
	char *strsplitsize = NULL;

	size_t splitsize;
	int mode = MODE_UNDEFINED;

	
	// Parse the input array and retrive the values provided by the user.

	char *c;
	int i;
	for(i = 1; i < argc;i++)
	{
		c = argv[i];
		if (c[0] == '-'){
			if (c[1] == 's'){
				if (c[2] == 'z')
					strsplitsize = argv[++i];
				else
					mode = MODE_SPLIT;

			}
			else if (c[1] == 'j')
				mode = MODE_JOIN;
		}
		else{
			if (source == NULL)
				source = c;
			else
				destination = c;
		}
	}

	// Print some debug information is program build with the DEBUG defination
	// turned on.

	#ifdef DEBUG
		printf("Mode: %u\nSplit size: %s\nSource: %s\nDestination: %s\n",
				mode,
				strsplitsize,
				source,
				destination);
	#endif 

	// Validate the user input and print USAGE if found wrong.

	if (
			(mode == MODE_UNDEFINED) ||
			(mode == MODE_SPLIT && ((splitsize = strtosize(strsplitsize)) == 0)) ||
			(source == NULL) ||
			(destination == NULL)
	   ){

		USAGE(argv[0]);
		exit(3);
	}

	// Perform split or join based on user input

	int filecount;
	if (mode == MODE_SPLIT){
	  filecount = split(source,destination,splitsize,1);
	  printf("%s splitted into %u files.\n",source,filecount);
	}
	else{
	  filecount = join(source,destination);
	  printf("Joined %u files into %s.\n",filecount,destination);
	}

	return 0;
}

size_t strtosize(char *sizestring)
{
	if (sizestring == NULL)
		return 0;
	
	/* strtol: 
		If  an  underflow  occurs,  strtol() returns  LONG_MIN.   
		If an overflow occurs, strtol() returns LONG_MAX.*/

	char *rest_of_string;
	size_t size = strtol(sizestring,&rest_of_string,10);
	if (size == LONG_MIN || size == LONG_MAX)
		return 0;

	if (strcasecmp(rest_of_string,"GB") == 0)
		size *= 1024 * 1024 * 1024;
	else if (strcasecmp(rest_of_string,"MB") == 0)
		size *= 1024 * 1024;
	else if (strcasecmp(rest_of_string,"KB") == 0)
		size *= 1024;
	else if (strcmp(rest_of_string,"") == 0)
		;
	else
	{
		printf("Invalid unit '%s'\n",rest_of_string);
		return 0;
	}

	return size;
}

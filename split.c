#include "split.h"
#include <sys/stat.h> // for fstat
#include <fcntl.h> // for open
#include <unistd.h> // for read,write,close


static size_t copybytes(int sourcefd, int destinationfd, size_t splitsize);

int split(char *source, char *destination, size_t splitsize, int startnum)
{
	int sourcefd,destinationfd;
	char currentfilename[FILENAME_LEN_MAX];
	size_t readlength = 0;

	if ((sourcefd = open(source, O_RDONLY)) == -1)
		PANIC("open",1);

	do{
		// Create a new filename in the formar destination.<number>
		if (FL(currentfilename,destination,startnum++) >= FILENAME_LEN_MAX)
			return ERR_FILE_NAME_TOO_LONG;

		if ((destinationfd =
			open(currentfilename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR)) == -1)
			PANIC("open",1);

		readlength = copybytes(sourcefd,destinationfd,splitsize);
		close(destinationfd);
	}while(readlength == splitsize);

	return --startnum;
}

static size_t copybytes(int sourcefd, int destinationfd, size_t splitsize)
{
	int remaining_split_file_bytes = splitsize;
	int readlength = 1;	// so that we can enter the loop

	while (remaining_split_file_bytes > BUFFER_SIZE && readlength > 0)
	{
		if ((readlength = read(sourcefd,buffer,BUFFER_SIZE)) == -1)
			PANIC("read",1);

		if(write(destinationfd,buffer,readlength) == -1)
			PANIC("write",1);

		remaining_split_file_bytes -= readlength;	
	}

	if (readlength > 0)
	{
		if ((readlength = read(sourcefd,buffer,remaining_split_file_bytes)) == -1)
			PANIC("read",1);

		if(write(destinationfd,buffer,readlength) == -1)
			PANIC("write",1);

		remaining_split_file_bytes -= readlength;	
	}

	return splitsize - remaining_split_file_bytes;
}

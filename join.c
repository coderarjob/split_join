#include "split.h"
#include <fcntl.h> // for open
#include <unistd.h> // for write, read and close
#include <string.h> // for strlen

static void getFileNameWithoutExtension(char *filename, char *filetitle,char
		*ext);
static void substring(char *str, int start, int length, char *out);

int join(char *firstfile, char *destfile)
{
	char filename_without_number[FILENAME_LEN_MAX];
	char startnumberstr[3];
	int startnumber;
	char currentfile[FILENAME_LEN_MAX];

	getFileNameWithoutExtension(firstfile,
			filename_without_number,
			startnumberstr);
	startnumber = atoi(startnumberstr);

	int sourcefd, destinationfd;
	if ((destinationfd = open(destfile,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR)) == -1)
		PANIC("open",2);

	if (snprintf(currentfile,FILENAME_LEN_MAX,"%s",firstfile) >=
		FILENAME_LEN_MAX)
		return ERR_FILE_NAME_TOO_LONG;

	while ((sourcefd = open(currentfile,O_RDONLY)) != -1)
	{
		int readlength;
		do
		{
			if ((readlength = read(sourcefd, buffer,BUFFER_SIZE)) == -1)
				PANIC("read",2);
			if (write(destinationfd,buffer,readlength) == -1)
				PANIC("write",2);
		}while(readlength > 0);

		close(sourcefd);

		if (FL(currentfile,filename_without_number,++startnumber) >=
				FILENAME_LEN_MAX)
			return ERR_FILE_NAME_TOO_LONG;
	}

	return --startnumber;
}


static void getFileNameWithoutExtension(char *filename, char *filetitle,char *ext)
{
	int l;
	if ((l = strlen(filename)) == 0)
	{
		filetitle[0] = '\0';
		ext[0] = '\0';
	}

	int indexOfLastDot = l;
	while(indexOfLastDot >= 0 && filename[--indexOfLastDot] != '.');

	substring(filename, 0,indexOfLastDot,filetitle);
	substring(filename,indexOfLastDot+1,(l - indexOfLastDot + 1),ext);
}

static void substring(char *str, int start, int length, char *out)
{
	while(length-- > 0 && (*out++ = *(str++ + start)) != '\0');
	*out = '\0';
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <utime.h>

void mytouch (char *filename[])
{
	int fd;
	struct stat st;
	struct utimbuf buf;

	lstat(filename[1], &st);
	fd = open(filename[1],O_RDWR| O_CREAT,0664);
	close(fd);
	buf.actime = time(NULL);
	buf.modtime = time(NULL);
	utime(filename[1], &buf);
}

int main(int argc, char *argv[])
{
	mytouch(argv);
	return 0;
}

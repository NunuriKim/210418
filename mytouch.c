#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void mytouch (char *filename[])
{
	int fd;
	fd = open(filename[1],O_RDWR| O_CREAT,0664);
	close(fd);
}

int main(int argc, char *argv[])
{
	mytouch(argv);
	return 0;
}

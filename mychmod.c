#include <stdio.h>
#include <sys/stat.h>

void mychmod (char *modeNumber, char *filename)
{
	int mode;
	sscanf(modeNumber,"%o", &mode);
	chmod(filename, mode);
}

int main(int argc, char *argv[])
{
	mychmod(argv[1], argv[2]);
	return 0;
}

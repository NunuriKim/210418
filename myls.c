#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char type(mode_t);
char* perm(mode_t);
void printStat(char*, char*, struct stat*);

void exec_ls_l(DIR * dp, char *dir)
{
	struct stat st;
	struct dirent *d;
	char path[1024];

	while ((d = readdir(dp)) != NULL) { // 디렉토리의 각 파일에 대해
		sprintf(path, "%s/%s", dir, d->d_name); // 파일경로명 만들기
		if (lstat(path, &st) < 0) // 파일 상태 정보 가져오기
			perror(path);
		printStat(path, d->d_name, &st); // 상태 정보 출력
		putchar('\n');
	}
}

void exec_ls_i(DIR * dp, char *dir)
{
        struct dirent *entry = NULL;

        while((entry = readdir(dp)) != NULL)
        {
                printf("%d ", entry->d_ino);
                printf("%s\n", entry->d_name);
        }
}

void exec_ls_t(DIR * dp, char *dir)
{
        struct dirent *entry = NULL;
        struct ls_t_list {
                char *file_name;
                time_t date;
        };
        struct ls_t_list t_list[1024];
        struct ls_t_list temp;
		struct stat st;
        char cwd[1024];

        int num_of_file = 0;

        while((entry = readdir(dp)) != NULL)
        {
                t_list[num_of_file].file_name = entry->d_name;
                sprintf(cwd, "%s/%s", dir, entry->d_name);
			if (lstat(cwd, &st) < 0)
			{
				perror(cwd);
				return ;
			}
			t_list[num_of_file].date = st.st_mtime;
			num_of_file++;
        }


        for(int i = 0 ; i < num_of_file ; i ++) {
			for(int j = 0 ; j < num_of_file -i -1 ; j ++) {
					if(t_list[j].date < t_list[j+1].date) {
							temp = t_list[j];
							t_list[j] = t_list[j+1];
							t_list[j+1] = temp;
					}
			}
        }

        for(int i = 0 ; i < num_of_file ; i ++) {
                printf("%s\n",t_list[i].file_name);
        }
}

int main(int argc, char **argv) {
	DIR *dp = NULL;
	char *dir;
	struct dirent *entry = NULL;
	struct stat st;

	if (argc == 1)
		 dir = ".";
	else if (argc == 2 && argv[1][0] == '-')
		 dir = ".";
	else if (argc > 1 &&  argv[1][0] != '-' )
		dir = argv[2];

	if ((dp = opendir(dir)) == NULL) // 디렉토리 열기
	{
		lstat(dir, &st);
			if (S_ISREG(st.st_mode))
				printf("%s\n", dir);
			else
				printf("디렉토리가 아니거나 존재하지 않습니다.\n");
		return 0;
	}
	if (argc > 1)
	{
		if (strcmp(argv[1], "-t") == 0)
		{
			exec_ls_t(dp, dir);
			closedir(dp);
			return 0;
		}
		else if (strcmp(argv[1], "-l") == 0)
		{
			exec_ls_l(dp, dir);
			closedir(dp);
			return 0;
		}
		else if (strcmp(argv[1], "-i") == 0)
		{
			exec_ls_i(dp, dir);
			closedir(dp);
			return 0;
		}
	}
	while((entry = readdir(dp)) != NULL)
	{
		printf("%s\n", entry->d_name);
	}
	closedir(dp);

	return 0;
}


void printStat(char *pathname, char *file, struct stat *st) {
	printf("%c%s ", type(st->st_mode), perm(st->st_mode));
	printf("%3d ", st->st_nlink);
	printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
	printf("%9d ", st->st_size);
	printf("%.12s ", ctime(&st->st_mtime)+4);
	printf("%s\n", file);
}

char type(mode_t mode) {
	if (S_ISREG(mode))
		return('-');
	if (S_ISDIR(mode))
		return('d');
	if (S_ISCHR(mode))
		return('c');
	if (S_ISBLK(mode))
		return('b');
	if (S_ISLNK(mode))
		return('l');
	if (S_ISFIFO(mode))
		return('p');
	if (S_ISSOCK(mode))
		return('s');
}

char* perm(mode_t mode) {
	int i;
	static char perms[10] = "----------";
	for (i=0; i < 3; i++) {
		if (mode & (S_IRUSR >> i*3))
			perms[i*3] = 'r';
		if (mode & (S_IWUSR >> i*3))
			perms[i*3+1] = 'w';
		if (mode & (S_IXUSR >> i*3))
			perms[i*3+2] = 'x';
	}
	return(perms);
}

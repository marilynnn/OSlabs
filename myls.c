#include <stdio.h>
#include <dirent.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char* mon[] = {"Jan", "Feb", "Mar", "Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int total = 0;

char* get_path(int argc, char** argv){ //проверяем корректность пути и получаем его 
	char *curr_path;
	switch (argc){
		case 1:
			curr_path = (strstr(argv[0],"./ls") != NULL) ? "./" : "Invalid enter";
		break;
		case 2: 
			if (strstr(argv[1],"-l") != NULL) {
				curr_path = "./";
			} 
			else {
				curr_path = (strstr(argv[1],"-") == NULL) ? argv[1] : "Invalid enter";
			}// в блоке else проверяем, что первым аргументом указан путь, а не другой флаг
		break;
		case 3:
			 if (strstr(argv[1],"-l") != NULL) {
                                curr_path = (strstr(argv[2],"-") == NULL) ? argv[2] : "Invalid enter";
			//проверяем, что вторым аргументом не идет еще один флаг
                        }
			else {
				curr_path = (strstr(argv[1],"-") == NULL && strstr(argv[2],"-l") != NULL) ? argv[1] : "Invalid enter";
			} 
			// проверяем, что первым аргументом указан именно путь, а не еще один флаг
		break;
		default:
			curr_path = "Invalid enter";
		break;
	}
	if (strstr(curr_path,"Invalid enter")!= NULL){
		printf("%s\n", "Invalid enter");
                exit(1);
	}

	return curr_path;
} 

void file_info (char* path, struct dirent* ent){
	
	char curr_path [PATH_MAX];
	curr_path[0]='\0';
	strcat(curr_path, path);
	strcat(curr_path, "/");
	strcat(curr_path, ent->d_name);
	
	struct stat S;
	memset (&S, 0, sizeof(struct stat));
	stat(curr_path, &S);
	total+=S.st_blocks;

	printf((S.st_mode & S_IFDIR)?"d":"-");	//Is a directory.
	printf((S.st_mode & S_IRUSR)?"r":"-");	//Read permission, owner.
	printf((S.st_mode & S_IWUSR)?"w":"-");	//Write permission, owner.
	printf((S.st_mode & S_IXUSR)?"x":"-");	//Execute/search permission, owner.
	printf((S.st_mode & S_IRGRP)?"r":"-");	//Read permission, group.
	printf((S.st_mode & S_IWGRP)?"w":"-");	//Write permission, group.
	printf((S.st_mode & S_IXGRP)?"x":"-");	//Execute/search permission, group.
	printf((S.st_mode & S_IROTH)?"r":"-");	//Read permission, others.
	printf((S.st_mode & S_IWOTH)?"w":"-");	//Write permission, others.
	printf((S.st_mode & S_IXOTH)?"x":"-");	//Execute/search permission, others.

	printf("  %lu", S.st_nlink);  		//Number of hardlinks to file.

	struct passwd* user = getpwuid(S.st_uid);
	printf("  %4s", user->pw_name);		//Name of owner.	
	
	struct group* grp = getgrgid(S.st_gid);
	printf("  %6s", grp->gr_name);		//Name of group.

	printf("  %6d",(int)S.st_size);		//Size of data.

	struct tm* buff_time;
	time_t time = S.st_ctime;
	buff_time = localtime (&time);
	printf ("  %6s", mon[buff_time->tm_mon]);//Month
	printf (" %02d", buff_time->tm_mday);	 //Dayi
	printf (" %02d:%02d  ", buff_time->tm_hour, buff_time->tm_min); //Hours and minutes
	

}

void ls (DIR* dir, struct dirent* ent){
	while ((ent=readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;
		printf("%s  ", ent->d_name);	//Name of file.
			
	}
}

void maxls (DIR* dir, struct dirent* ent, char* path){
		
	struct stat buf;
        memset (&buf, 0, sizeof(struct stat));
        stat(path, &buf);
	while ((ent=readdir(dir)) != NULL) {
                if (ent->d_name[0] == '.')
                        continue;
		file_info (path, ent);	
                printf("%s\n", ent->d_name);	//Name of file.

        }
	 total/=2;
         printf("total %d\n", total);
}


int main (int argc, char** argv){
	char* path = get_path(argc, argv);	
	//printf ("%s\n", path);

	DIR* dir = opendir(path);
	struct dirent* ent = NULL;
	int opt;
	int i = 1;
	while (i < argc){
		if ((opt = getopt(argc,argv,"l")) != -1){
			switch(opt){
				case 'l':
					maxls(dir, ent, path);
					closedir(dir);
					exit (1);
				break;
				default:
				break;	
			}
		}
	++i;
	}
	ls(dir, ent);	
	closedir(dir);

		
	return 0;
}

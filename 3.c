#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main()
{
struct stat results;  
long int old,new;
stat("/home/nat/test/a.txt", &results);
old = results.st_mtime;				//simpan time modification
printf("old %ld\n",old);
int i= 1;

  while(1) {
    // main program here
	stat("/home/nat/test/a.txt", &results);
	new = results.st_mtime;

	if (old != new){ 
	printf("%d. new %ld\n",i,new);
	i++;
	old = new;
	}
}
return 0;
}

/*Ketika semua file dalam direktori Downloads diedit dan disimpan lagi, maka secara otomatis muncul folder baru dengan nama “simpanan” yang digunakan khusus untuk menyimpan file baru hasil dari yang sudah diedit dan file asli tetap ada pada tempatnya tanpa ada perubahan pada isinya. File baru hasil editan tersebut diberi nama sesuai dengan file aslinya.
*/

//http://world-just-for-you.blogspot.co.id/2014/12/tutorial-menggunakan-fusefile-system-in.html

#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/nat/Downloads";
char alamat [1000];

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	
	strcpy (alamat,path);

	int res = 0;
  	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);

	
	

	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

//File open operation
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;   
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = open(fpath, fi->flags);   
    if (res == -1)
        return -errno;
    close(res);
    return 0;
}


//Write data to an open file
static int xmp_write(const char *path, const char *buf, size_t size,off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    (void) fi;
    fd = open(fpath, O_WRONLY);

	char fpath1[1000],alamat_asli[1000];
	sprintf(fpath1,"%s%s",dirpath,alamat);
	FILE *f1,*f2;
	
	strcpy(alamat_asli,fpath1);
	strcat(alamat_asli,"-asli");

	char kata[100];

	f1 = fopen(alamat_asli,"w+");
	f2 = fopen(fpath1, "a+");
	while (fgets(kata,100,f2)!=NULL)
	{
		fprintf(f1,"%s",kata);
	}
	fclose(f1); fclose (f2);

/*

//harus punya 3 file >> 1. yg diedit 2.simpan awal mula 3.hasil edit
//nanti yg file1 diedit dihapus tinggal simpan awal mula

        char temp1[100],fpath2[1000];
	system ("mkdir -p /home/nat/Documents/simpanan");
//	char* direktori[] = "/home/nat/Downloads/simpanan";
        sprintf(fpath2,"%s%s",dirpath,alamat);   
        FILE *fd1, *fd2;
       

	
        strcpy(temp1,fpath2);
        strcat(temp1,"-copy");
   
        char kata[100];
       
        fd1 = fopen(temp1, "w+");
        fd2 = fopen(fpath2, "a+");
        while(fgets(kata,100,fd2)!=NULL)
        {
            fprintf(fd1,"%s",kata);   
        }
        fclose(fd1);
        fclose(fd2);
*/
    if (fd == -1)
        return -errno;
    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    close(fd);   
    return res;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res=mknod(fpath,mode,rdev);
    if (res==-1)
        return -errno;
    return 0;
}


static int xmp_truncate(const char *path, off_t size)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = truncate(fpath, size);
    if (res == -1)
        return -errno;
    return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.write		= xmp_write,
	.truncate	= xmp_truncate,
	.mknod		= xmp_mknod
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

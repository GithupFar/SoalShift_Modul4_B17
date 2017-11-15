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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);

    if(res == -1)
    {
        return -errno;
    }

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

static int xmp_mkdir(const char *path,mode_t mode)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = mkdir (fpath,mode);
    if(res == -1)
        return -errno;
    return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
    int res;
 char ffrom[1000];
 char fto[1000];
 sprintf(ffrom,"%s%s",dirpath,from);
 sprintf(fto,"%s%s",dirpath,to);
    res = symlink(ffrom, fto);
    if(res == -1)
        return -errno;

    return 0;
}


static int xmp_unlink(const char *path)
{
    int res;
 char fpath[1000];
 sprintf(fpath,"%s%s", dirpath, path);
    res = unlink(fpath);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;
    char fpath[1000];
    char path2[1000];
    char path3[1000];
    sprintf(fpath,"%s%s", dirpath, path);
    sprintf(path2,"%s%s","/home/nat/Downloads",path);
    int i;

    close(res);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
    int fd;
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s", dirpath, path);
    fd = open(fpath, O_RDONLY);
    if(fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
 char fpath[1000];
 sprintf(fpath,"%s%s", dirpath, path);
    fd = open(fpath, O_WRONLY);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;

    close(fd);
    return res;
}


static struct fuse_operations xmp_oper =
{
    .getattr = xmp_getattr,
    
	.readdir = xmp_readdir,
    
    .symlink = xmp_symlink,
    .unlink = xmp_unlink,
    
    
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write
    
};

int main(int argc, char *argv[])
{
    umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

//http://yaukingsyaukiaulia.blogspot.co.id/2015/

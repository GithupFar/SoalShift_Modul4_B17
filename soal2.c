/*
Pada saat membuka file dengan ekstensi *.pdf, *.doc, *.txt pada directori Documents akan muncul pesan error “Terjadi kesalahan! File berisi konten berbahaya.” dan tidak dapat membaca file tersebut. Setelah memunculkan pesan error, file tersebut diganti namanya menjadi <namafile>.<ekstensi>.ditandai
*/

//http://valdyputra.blogspot.co.id/2015/11/tentang-fuse-linux.html

// [ gcc cobafuse.c -o cobafuse `pkg-config fuse --cflags --libs` ]

#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/nat/Documents";

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

int cek (const char *nama)
{
	int x = strlen (nama);
	char file[100];
	strcpy(file, nama+x-4);	//ambil ekstensi belakang
	if (strcmp(file,".txt")==0 || strcmp(file,".pdf")==0 || strcmp(file,".doc")==0)
		return 1;
	else
		return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char fpath [1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = open(path, fi->flags);
	
	if (cek (fpath))
	{
		char source_file[1000], target_file[1000];
		sprintf(source_file,"%s",fpath);
		sprintf(target_file,"%s.ditandai",fpath);
		int ret = rename (source_file,target_file);
		char permisi[500];
		sprintf(permisi,"chmod 000 %s.ditandai",fpath);
		system(permisi);
		
		char command[500];
		sprintf(command,"zenity --error --text='Terjadi kesalahan! File berisi konten berbahaya.'");
		system (command);
		return -errno;
	}
	
	else
	return -errno;
		
}


static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.open		= xmp_open,

};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

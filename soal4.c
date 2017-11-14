/*
File hasil salinan diberi nama <namafile>.<ekstensi>.copy 
    dan jika file yang dibuka adalah file berekstensi .copy 
    (file hasil salin), maka akan muncul pesan error berupa 
    “File yang anda buka adalah file hasil salinan. 
    File tidak bisa diubah maupun disalin kembali!” 
    Dan file tersebut tidak akan terbuka dan tidak dibuat salinannya.
*/

#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/admin/Downloads"; //file awal ada di downloads

static int xmp_getattr(const char *path, struct stat *stbuf) //dapetin informasi data
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

//fungsi untuk flag
int flags(const char *namafile) //dia menandai file itu .copy atau tidak .... / masih mencari penjelasan dari sc-nya
{
    int x=strlen(namafile);
    char file[100];
    if(strcmp(file,".copy")==0)
        return 1;
    else
    strcpy(file,namafile+x-4);
        return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path); //file ditampung kedalam fpath

    if(flags(fpath)){ //begitu membuka file .copy ada notif error
        char perintah[100];
        sprintf(perintah,"zenity --error --text='File yang anda buka adalah file hasil salinan. File tidak bisa diubah maupun disalin kembali!'");
        system(perintah); //menjalankan perintah
        return 1;
    }
    else{ //dia belum berekstensi file.copy (?)
        char ch,src_file[1000],target_file[1000];
        FILE *from,*to;
        
        sprintf(src_file,"%s",fpath); //file di fpath di simpan ke src_file
        from=fopen(src_file,"r"); //diread/buka
        
        sprintf(target_file,"%s.copy",fpath); //file target=file berekstensi .copy
        
        int ada;
        ada=access(target_file,F_OK); //jika ada
        if(ada==0) //0=ada -1=tidak ada
        {
            remove(target_file);  //remove file target
        }
        
        to=fopen(target_file,"w"); //buat lagi file target
        while((ch=fgetc(from))!=EOF) //file target berisi copy-an dari file source
            fputc(ch,to);
        char perintah2[100];
        sprintf(perintah2,"chmod 444 '%s.copy'",fpath); //merubah akses tidk bisa dibuka maupun diedit
        system(perintah2);
        
        fclose(from);
        fclose(to);
    }
    
    close(res);
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

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.open		= xmp_open,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

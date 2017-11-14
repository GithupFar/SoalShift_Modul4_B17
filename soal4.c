/*
File hasil salinan diberi nama <namafile>.<ekstensi>.copy 
    dan jika file yang dibuka adalah file berekstensi .copy 
    (file hasil salin), maka akan muncul pesan error berupa 
    “File yang anda buka adalah file hasil salinan. 
    File tidak bisa diubah maupun disalin kembali!” 
    Dan file tersebut tidak akan terbuka dan tidak dibuat salinannya.
*/

static const char *dirpath = "/home/admin/Downloads/simpanan"; //pikirku dia udh di folder 'simpanan'
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

    if(flag(fpath)){ //begitu membuka file .copy ada notif error
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

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


/*referensi tambahan

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res,num=0,numb=0,flag=0,j=0,i=0,count=0;
    int len2=strlen(path),len=strlen(lastakses);
    char fpath[1000],lastakses[1000],lastaksesbackup[1000]
    ,aksesbackup[1000];
    FILE *to,*from;

    sprintf(fpath,"%s%s",dirpath,path);

    strcpy(lastakses,fpath);
    strcpy(lastaksesbackup,fpath);
    strcpy(lastaksesbackup,cbackup);
    //file .bak dibuka
    while(num<numb){ //numb;jumlah file.bak dibuat, num jumlah .bak dibuka
        if(strcmp(lastaksesbackup,aksesbackup[num])==0)flag=1;
        if(flag==1){
            res=open(fpath,fi->flags);
            if(res == -1) return -errno;
            close(res);
            return 0;
            break;
        }
        num++;
    }
    //kalo flag==0
    if(flag==0){
        strcpy(aksesbackup[numb],lastaksesbackup);
        numb++;
    }
    len=strlen(lastakses);

    if((len>=2) && strcmp(&(lastakses[len-4]),".copy")==0){
        char perintah[100];
        sprintf(perintah,"zenity --error --text='File yang anda buka adalah file hasil salinan. File tidak bisa diubah maupun disalin kembali!'");
        system(perintah);
    }

	res = open(fpath, fi->flags); //langsung dibuka filenya
	if (res == -1)
		return -errno;

    while(j<len2){
        if(path[j]='.') count=1;
        if(count==1) break;
        j++;
    }

    if(count==1 && flag=0){
        while(i<=len){
            if(lastakses[i]='.'){
                if((len >= 2) && strcmp(&(lastakses[len-4]), ".copy")!=0){
                    if(akses(lastaksesbackup,F_OK)==0) remove(lastaksesbackup);

                    from=fopen(lastakses, "r");
                    to=fopen(lastaksesbackup, "w");
                    char a;
                    while(1){
                        a=fgetc(from);
                        if(!feof(from)) fputc(a,to);
                        else break;
                    }
                    char perintah2[100];
                    sprintf(perintah2,"chmod 444 '%s' ",lastaksesbackup);
                    system(perintah2);
                }
            }
        }

    }
	close(res);
	//return 0;
}

#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
 
char dirpath[100] = "/home/moon/Downloads";
 

int cekEnkrip(char *fpath){
	char *token = strtok(fpath,"/");
	while(token != NULL){
		if(strncmp("AtoZ_",token,5) == 0){
			return 1;
		}
		if(!strncmp("RX_",token,3)){
			return 2;
		}
		token = strtok(NULL,"/");
	}
	return 0;
} 

int tipe(const char* path) {
    struct stat statbuf; 
    if (stat(path, &statbuf) == -1)
        return -1;
    return S_ISDIR(statbuf.st_mode);
}
 
// char *get_ext(char* filename) {
 
// 	if(strrchr(filename, '/')!=NULL)
// 	    filename = strrchr(filename, '/');
// 	if(strchr(filename, '.')==NULL){
// 		filename[0]=0;
// 		return filename;
// 	}
//     filename = strchr(filename, '.');
 
//     return filename;
// }
 
// char *get_name(char *filename){
//     char ok[1000];
//     strcpy(ok,filename);
 
// 	if(strrchr(ok, '/')!=NULL)
// 	    strcpy(ok,strrchr(ok, '/'));
// 	strcpy(filename,ok);
// 	if(strchr(ok, '.')!=NULL){
// 		strcpy(ok,strchr(ok, '.'));
// 	}
// 	else{
// 		ok[0]=0;
// 	}
 
//     filename[strlen(filename)-strlen(ok)] = 0;
 
//     return filename;
// }
 
char *get_path(char *filename){
	char ok[1000];
 
	strcpy(ok,strrchr(filename, '/'));
 
	filename[strlen(filename)-strlen(ok)] = 0;
 
	return filename;
}
 
// char *enkripsi_atbash(char *filename){
// 	for(int i = 0; i < strlen(filename); i++){
 
// 		if(filename[i] >= 'a' && filename[i] <= 'z'){
// 			filename[i] = 'z' + 'a' - filename[i];
// 		}	
// 		if(filename[i] >= 'A' && filename[i] <= 'Z'){
// 			filename[i] = 'Z' + 'A' - filename[i];
// 		}
 
// 	}
 
// 	return filename;
// }

char *enkripsi_atbash(char *filename){
	int length_extension = 0;
	char *extension = strrchr(filename,'.');
	if(extension){
		length_extension = strlen(extension);
	}
	for(int i = 0; i < strlen(filename)-length_extension; i++){
 
		if(filename[i] >= 'a' && filename[i] <= 'z'){
			filename[i] = 'z' + 'a' - filename[i];
		}	
		if(filename[i] >= 'A' && filename[i] <= 'Z'){
			filename[i] = 'Z' + 'A' - filename[i];
		}
 
	}

	// char *token = strtok(filename,".");
	// char temp[10000];
	// strcpy(temp,token);
	// enkripsi_atbash(temp);

	// //jika ada ekstensi
	// token = strtok(NULL,".");
	// if(token){
	// 	strcat(temp,".");
	// 	strcat(temp,token);
	// }
	
	// strcpy(filename,temp);
	return filename;
}

char *dec_vigenere(char *encryptedMsg){

    char key[] = "SISOP";
    int msgLen = strlen(encryptedMsg), keyLen = strlen(key), i, j,length_extension = 0;
    char newKey[msgLen], decryptedMsg[msgLen];

	char *extension = strrchr(encryptedMsg,'.');
	if(extension){
		length_extension = strlen(extension);
	}

    for(i = 0, j = 0; i < msgLen-length_extension; ++i, ++j){
        if(j == keyLen)
            j = 0;
 
        newKey[i] = key[j];
    }
    
    //decryption
    for(i = 0; i < msgLen-length_extension; ++i){
         if(encryptedMsg[i] >= 'a' && encryptedMsg[i] <= 'z')
            decryptedMsg[i] = (((encryptedMsg[i] - newKey[i]) + 26 - ('a' - 'A')) % 26) + 'a';
        else if(encryptedMsg[i] >= 'A' && encryptedMsg[i] <= 'Z')
            decryptedMsg[i] = (((encryptedMsg[i] - newKey[i]) + 26) % 26) + 'A';
        else 
            decryptedMsg[i] = encryptedMsg[i]; 
        // decryptedMsg[i] = (((encryptedMsg[i] - newKey[i]) + 26) % 26) + 'A';
    }
 
	for(;i<msgLen;++i){
        decryptedMsg[i] = encryptedMsg[i];
    }

    decryptedMsg[i] = '\0';

	strcpy(decryptedMsg,enkripsi_atbash(decryptedMsg));

    strcpy(encryptedMsg,decryptedMsg);
	return encryptedMsg;
}

char *vigenere(char *msg){

	strcpy(msg,enkripsi_atbash(msg));

    char key[] = "SISOP";
    int msgLen = strlen(msg), keyLen = strlen(key), i, j,length_extension = 0;

	char *extension = strrchr(msg,'.');
	if(extension){
		length_extension = strlen(extension);
	}
 
    char newKey[msgLen], encryptedMsg[msgLen], decryptedMsg[msgLen];
 
    //generating new key
    for(i = 0, j = 0; i < msgLen; ++i, ++j){
        if(j == keyLen)
            j = 0;
 
        newKey[i] = key[j];
    }

    newKey[i] = '\0';
 
    //encryption
    for(i = 0; i < msgLen - length_extension; ++i){
        if(msg[i] >= 'a' && msg[i] <= 'z')
            encryptedMsg[i] = ((msg[i] + newKey[i] - ('a' - 'A')) % 26) + 'a';
        else if(msg[i] >= 'A' && msg[i] <= 'Z')
            encryptedMsg[i] = ((msg[i] + newKey[i]) % 26) + 'A';
        else
            encryptedMsg[i] = msg[i];
    }

	for(;i<msgLen;++i){
        encryptedMsg[i] = msg[i];
    }

    encryptedMsg[i] = '\0';
    strcpy(msg,encryptedMsg);
	return msg;
}

char *rot13(char *filename){
	strcpy(filename,enkripsi_atbash(filename));

	int length_extension = 0;

	char *extension = strrchr(filename,'.');

	if(extension){
		length_extension = strlen(extension);
	}

	for(int i = 0; i < strlen(filename) - length_extension; i++){
 
		if(filename[i] >= 'a' && filename[i] <= 'z'){
			filename[i] += 13;
			if(filename[i] > 'z'){
				filename[i] -= 26;
			}
		}	
		if(filename[i] >= 'A' && filename[i] <= 'Z'){
			filename[i] += 13;
			if(filename[i] > 'Z'){
				filename[i] -= 26;
			}
		}
	}

	return filename;
}

// char *enc_one(const char *path,char *fpath,char *ret){
 
// 	if(!strncmp("/AtoZ_",path,6)){
//         char *check;
//         check = strrchr(path,'/');
//         if(strlen(check) != strlen(path)){
//             char tmp[1000],name[1000],n_name[1000],ext[1000];
 
//             strcpy(tmp,path);
//             strcpy(name,get_name(tmp));
 
// 			strcpy(tmp,fpath);
//             strcpy(n_name,get_path(tmp));
 
//             strcat(n_name,enkripsi_atbash(name));
 
// 			strcpy(tmp,path);
// 			strcpy(ext,get_ext(tmp));
 
// 			strcat(n_name,ext);
 
//             rename(fpath,n_name);
// 			ret = n_name;
 
// 			return ret;
//         }
//     }
// 	else{
// 		char hade[10];
// 		hade[0]=0;
// 		ret = hade;
// 		return ret;
// 	}
// 	return ret;
// }


char *proses_ext(const char *path,char *ret){
 
	char ok[1000];
	strcpy(ok,strrchr(path,'/')+1);
 
	if(strncmp("AtoZ_",ok,5)){
        char *check;
        check = strrchr(path,'/');
        if(strlen(check) != strlen(path)){
            char tmp[1000],name[1000],n_name[1000],ext[1000];
 
			strcpy(n_name,enkripsi_atbash(tmp));
 
			ret = n_name;
 
			return ret;
        }
    }
	else{
		ret = path;
	}
	return ret;
}
 
void rekursi(char fpath[1000],int jenis){
 
	// printf("proses %s\n",fpath);
	if(!strlen(fpath)){
		return;
	}
 
    DIR *d;
    struct dirent *dir;
 
    d = opendir(fpath);
    if (!d){
        return;
    }
    while((dir = readdir(d)) != NULL){
 
        if(!strcmp(dir->d_name,".")||!strcmp(dir->d_name,"..")){
            continue;
        }
 
		char n_name[100],tmp[100],path[100],name[100],ext[100];
		// printf("n\n");
		strcpy(tmp,dir->d_name);
		// strcpy(name,get_name(tmp));
		// // printf("h\n");
		// strcpy(tmp,dir->d_name);
		// strcpy(ext,get_ext(tmp));
 
		// printf("%s %s %s",fpath,name,ext);
		if(jenis == 1)
			sprintf(n_name,"%s/%s",fpath,enkripsi_atbash(tmp));
		else if(jenis == 2)
			sprintf(n_name,"%s/%s",fpath,vigenere(tmp));
		// else if(jenis == 3)
 
		strcpy(path,fpath);
		strcat(path,"/");
		strcat(path,dir->d_name);
 
		rename(path,n_name);
 
		// printf("renamed %s\n",n_name);
 
        if(tipe(n_name)==1){
			// printf("masuk %s\n",n_name);
            rekursi(n_name,jenis);
            // printf("MASUKK %s\n",lewat);
            continue;
        }
 
    }
    closedir(d);
}
 
 
 
// char *proses(char *filename){
// 	char *ok;
//     ok = strchr(filename,'/') + 1;
 
//     while (ok != NULL){
//         // printf("%s\n",ok);
 
// 		if(!strncmp(ok,"AtoZ_",5)){
// 			printf("OK=%s",ok);
// 			strcpy(filename,proses_ext(filename,""));
// 			break;
// 		}
 
//         if(strchr(ok,'/') == NULL)
//             break;
 
//         ok = strchr(ok,'/') + 1;
//     }
// 	return filename;
// }

char *proses(char *filename){
	// printf("TES\n");
	char *ok;
    ok = strchr(filename,'/') + 1;
 
    while (ok != NULL){
        // printf("%s\n",ok);
 
		if(!strncmp(ok,"AtoZ_",5)){
			// printf("OK NOW=%s\n",ok);
			
			if(strchr(ok,'/')){
				ok = strchr(ok,'/') + 1;
				int ind = ok - filename;
				// printf("OK=%s %d\n",ok, ok-filename);
				enkripsi_atbash(ok);
				for(int i = 0; i < strlen(ok); i++){
					filename[ind + i] = ok[i]; 
				}
				filename[ind+strlen(ok)] = '\0';
				
				// strcpy(filename,proses_ext(filename,""));
				break;
			}
		}

		if(!strncmp(ok,"RX_",3)){
			printf("Masuk");
			if(strchr(ok,'/')){
				ok = strchr(ok,'/') + 1;
				int ind = ok - filename;
				// printf("OK=%s %d\n",ok, ok-filename);
				dec_vigenere(ok);
				printf("PROSES %s\n",ok);
				for(int i = 0; i < strlen(ok); i++){
					filename[ind + i] = ok[i]; 
				}
				filename[ind+strlen(ok)] = '\0';
				
				// strcpy(filename,proses_ext(filename,""));
				break;
			}
		}
 
        if(strchr(ok,'/') == NULL)
            break;
 
        ok = strchr(ok,'/') + 1;
    }
	return filename;
}

void logSatu(char *from, char *to){
	FILE *fileout;
	fileout = fopen("/home/moon/inilog.log","a");
	fprintf(fileout, "%s -> %s\n",from,to);
	fclose(fileout);
}

void logDua(char *from, char *to,int tipe){
	FILE *fileout;
	fileout = fopen("/home/moon/logDua.log","a");

	if(tipe == 1)
		fprintf(fileout, "MKDIR: %s -> %s\n",from,to);
	if(tipe == 2)
		fprintf(fileout, "RENAME: %s -> %s\n",from,to);

	fclose(fileout);
}
 
void appendLog(char *level, char *command, char *desc){
	char time_now[100] = {0};
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	strftime(time_now,100,"%d%m%Y-%H:%M:%S",tm);
 
	FILE *fileout;
	fileout = fopen("/home/moon/SinSeiFS.log","a");
	fprintf(fileout, "%s::%s:%s::%s\n",level,time_now,command,desc);
	fclose(fileout);
}
 
void combinePath(const char* a, char* b, char* c){
    if(strcmp(a,"/")==0){
        a = b;
        sprintf(c,"%s",a);
    }else{
        sprintf(c,"%s%s",b,a);
    }
}
 
static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = lstat(proses(fpath), stbuf);
	printf("ATTR %s\n",fpath);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_access(const char *path, int mask)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
 
	res = access(proses(fpath), mask);
	// printf("aces %s\n",fpath);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = readlink(proses(fpath), buf, size - 1);
	if (res == -1)
		return -errno;
 
	buf[res] = '\0';
	return 0;
}
 
 
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	DIR *dp;
	struct dirent *de;
 
	(void) offset;
	(void) fi;
 
	dp = opendir(proses(fpath));
	// printf("PATH %s\n",fpath);
	if (dp == NULL)
		return -errno;
 
	int flag = cekEnkrip(fpath);
 
	while ((de = readdir(dp)) != NULL) {
		// printf("MASUK %s\n",de->d_name);
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		char temp[10000] = {0};
		strcpy(temp,de->d_name);
		
		if(flag==1){
			if(de->d_type == DT_REG){
				enkripsi_atbash(temp);
			}else if(de->d_type == DT_DIR && strcmp(temp,".") != 0 && strcmp(temp,"..") != 0){
				enkripsi_atbash(temp);
			}
		}
		if(flag==2){
			if(de->d_type == DT_REG){
				vigenere(temp);
				printf("DIR %s\n",temp);
			}else if(de->d_type == DT_DIR && strcmp(temp,".") != 0 && strcmp(temp,"..") != 0){
				vigenere(temp);
				printf("DIR %s\n",temp);

			}
		}
		// printf("temp = %s \n",temp);
		if (filler(buf, temp, &st, 0))
			break;
	}
 
	closedir(dp);
	return 0;
}
 
static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
	proses(fpath);
 
 
	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = mkdir(proses(fpath), mode);
	if (res == -1)
		return -errno;
 
	char desc[1000] = {0};
	strcpy(desc,fpath);
	appendLog("INFO","MKDIR",desc);
 
	char *tmp = strrchr(fpath,'/')+1;
	printf("log %s\n",tmp);
	if(!strncmp(tmp,"AtoZ_",5)){
		logSatu(dirpath,fpath);
	}
	if(!strncmp(tmp,"RX_",3)){
		logDua(dirpath,fpath,1);
	}
    // rekursi(tmp);
 
 
	return 0;
}
 
static int xmp_unlink(const char *path)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = unlink(proses(fpath));
	if (res == -1)
		return -errno;
 
	char desc[1000] = {0};
	strcpy(desc,fpath);
	appendLog("WARNING","UNLINK",desc);
	return 0;
}
 
static int xmp_rmdir(const char *path)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
 
	res = rmdir(proses(fpath));
	if (res == -1)
		return -errno;
 
	char desc[1000] = {0};
	strcpy(desc,fpath);
	appendLog("WARNING","RMDIR",desc);
 
	return 0;
}
 
static int xmp_symlink(const char *from, const char *to)
{
	int res;
 
	res = symlink(from, to);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_rename(const char *from, const char *to)
{
	int res;
    char ffrom[100]={0}, fto[100]={0};
    combinePath(from,dirpath,ffrom);
	combinePath(to,dirpath,fto);
 
	res = rename(ffrom, fto);
	if (res == -1)
		return -errno;
 
	char desc[1000] = {0};
	sprintf(desc,"%s::%s",ffrom,fto);
	appendLog("INFO","RENAME",desc);
 
	if(!strncmp("/AtoZ_",from,6) && !strncmp("/AtoZ_",to,6)){
		// return 0;
	}
 
	else if(!strncmp("/AtoZ_",from,6) || !strncmp("/AtoZ_",to,6)){
		if(!strncmp("/AtoZ_",to,6)){
			logSatu(ffrom,fto);
		}
		rekursi(fto,1);
	}
	
	if(!strncmp("/RX_",from,4) && !strncmp("/RX_",to,4)){
		// return 0;
	}

	else if(!strncmp("/RX_",to,4)){
		logDua(ffrom,fto,2);
		rekursi(fto,2);
	}

	else if(!strncmp("/RX_",from,4)){

	}

	return 0;
}
 
static int xmp_link(const char *from, const char *to)
{
	int res;
 
	res = link(from, to);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = chmod(proses(fpath), mode);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    char fpath[1000]={0};
    combinePath(fpath,dirpath,fpath);
 
	res = lchown(proses(fpath), uid, gid);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = truncate(proses(fpath), size);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, proses(fpath), ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
 
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = open(proses(fpath), fi->flags);
	if (res == -1)
		return -errno;
 
	char desc[1000] = {0};
	strcpy(desc,fpath);
	appendLog("INFO","OPEN",desc);
	close(res);
	return 0;
}
 
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	(void) fi;
	fd = open(proses(fpath), O_RDONLY);
	if (fd == -1)
		return -errno;
 
	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
 
	close(fd);
	return res;
}
 
static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000]={0};
 
    combinePath(path,dirpath,fpath);
 
	(void) fi;
	fd = open(proses(fpath), O_WRONLY);
	if (fd == -1)
		return -errno;
 
	char desc[1000] = {0};
	strcpy(desc,path);
	appendLog("INFO","WRITE",desc);
	
 
	printf("write\n");
 
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
 
	return res;
	close(fd);
}
 
static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);
 
	res = statvfs(proses(fpath), stbuf);
	if (res == -1)
		return -errno;
 
	return 0;
}
 
static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) 
{
	char fpath[1000];
	combinePath(path,dirpath,fpath);
    (void) fi;
 
    int res;
    res = creat(proses(fpath), mode);
    if(res == -1) return -errno;
 
	char desc[1000] = {0};
	strcpy(desc,fpath);
	appendLog("INFO","CREATE",desc);
    close(res);
 
	// enc_one(path,fpath);
 
    return 0;
}
 
static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */
 
	(void) path;
	(void) fi;
	return 0;
}
 
static int xmp_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */
 
	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}
 
 
 
static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.access		= xmp_access,
	.readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.link		= xmp_link,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.statfs		= xmp_statfs,
    .create     = xmp_create,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
};
 
int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

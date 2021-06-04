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

int tipe(const char* path) {
    struct stat statbuf; 
    if (stat(path, &statbuf) == -1)
        return -1;
    return S_ISDIR(statbuf.st_mode);
}

char *get_ext(char* filename) {

	if(strrchr(filename, '/')!=NULL)
	    filename = strrchr(filename, '/');
	if(strchr(filename, '.')==NULL){
		filename[0]=0;
		return filename;
	}
    filename = strchr(filename, '.');

    return filename;
}

char *get_name(char *filename){
    char ok[1000];
    strcpy(ok,filename);

	if(strrchr(ok, '/')!=NULL)
	    strcpy(ok,strrchr(ok, '/'));
	strcpy(filename,ok);
	if(strchr(ok, '.')!=NULL){
		strcpy(ok,strchr(ok, '.'));
	}
	else{
		ok[0]=0;
	}

    filename[strlen(filename)-strlen(ok)] = 0;

    return filename;
}

char *get_path(char *filename){
	char ok[1000];

	strcpy(ok,strrchr(filename, '/'));

	filename[strlen(filename)-strlen(ok)] = 0;

	return filename;
}

char *enkripsi_atbash(char *filename){
	for(int i = 0; i < strlen(filename); i++){

		if(filename[i] >= 'a' && filename[i] <= 'z'){
			filename[i] = 'z' + 'a' - filename[i];
		}	
		if(filename[i] >= 'A' && filename[i] <= 'Z'){
			filename[i] = 'Z' + 'A' - filename[i];
		}

	}

	return filename;
}

char *enc_one(const char *path,char *fpath,char *ret){

	if(!strncmp("/AtoZ_",path,6)){
        char *check;
        check = strrchr(path,'/');
        if(strlen(check) != strlen(path)){
            char tmp[1000],name[1000],n_name[1000],ext[1000];

            strcpy(tmp,path);
            strcpy(name,get_name(tmp));
            
			strcpy(tmp,fpath);
            strcpy(n_name,get_path(tmp));

            strcat(n_name,enkripsi_atbash(name));

			strcpy(tmp,path);
			strcpy(ext,get_ext(tmp));

			strcat(n_name,ext);

            rename(fpath,n_name);
			ret = n_name;

			return ret;
        }
    }
	else{
		char hade[10];
		hade[0]=0;
		ret = hade;
		return ret;
	}
	return ret;
}

void rekursi(char fpath[1000]){

	printf("proses %s\n",fpath);
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
		printf("n\n");
		strcpy(tmp,dir->d_name);
		strcpy(name,get_name(tmp));
		printf("h\n");
		strcpy(tmp,dir->d_name);
		strcpy(ext,get_ext(tmp));

		printf("%s %s %s",fpath,name,ext);
		sprintf(n_name,"%s/%s%s",fpath,enkripsi_atbash(name),get_ext(ext));

		strcpy(path,fpath);
		strcat(path,"/");
		strcat(path,dir->d_name);

		rename(path,n_name);
		
		printf("renamed %s\n",n_name);

        if(tipe(n_name)==1){
			printf("masuk %s\n",n_name);
            rekursi(n_name);
            // printf("MASUKK %s\n",lewat);
            continue;
        }

    }
    closedir(d);
}

void logSatu(char *from, char *to){
	FILE *fileout;
	fileout = fopen("/home/moon/inilog.log","a");
	fprintf(fileout, "%s -> %s\n",from,to);
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

	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);


	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);

	res = readlink(fpath, buf, size - 1);
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

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
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

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	char desc[1000] = {0};
	strcpy(desc,fpath);
	appendLog("INFO","MKDIR",desc);

	char *tmp = enc_one(path,fpath,"");
	if(!strlen(tmp) && !strncmp("/AtoZ_",path,6)){
		logSatu(dirpath,fpath);
	}
    rekursi(tmp);

	
	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);

	res = unlink(fpath);
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


	res = rmdir(fpath);
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
		return 0;
	}

	else if(!strncmp("/AtoZ_",from,6) || !strncmp("/AtoZ_",to,6)){
		if(!strncmp("/AtoZ_",to,6)){
			logSatu(ffrom,fto);
		}
		rekursi(fto);
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

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    char fpath[1000]={0};
    combinePath(fpath,dirpath,fpath);

	res = lchown(path, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);

	res = truncate(fpath, size);
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
	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);

	res = open(fpath, fi->flags);
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
	fd = open(fpath, O_RDONLY);
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
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;
	
	char desc[1000] = {0};
	strcpy(desc,path);
	appendLog("INFO","WRITE",desc);
	close(fd);

	printf("write\n");

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	
	return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	int res;
    char fpath[1000]={0};
    combinePath(path,dirpath,fpath);

	res = statvfs(fpath, stbuf);
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
    res = creat(fpath, mode);
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

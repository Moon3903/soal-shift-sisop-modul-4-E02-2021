# soal-shift-sisop-modul-4-E02-2021

## Anggota Kelompok E02
05111940000001 - Christoper Baptista

05111940000101 - Zulfiqar Fauzul Akbar

05111940000152 - Ryan Fernaldy

# Soal 1
## Penjelasan
Semua file yang berada pada direktori harus ter-encode menggunakan Atbash cipher(mirror)</br>
a) Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.</br>
b) Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.</br>
c) Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.</br>
d) Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : `/home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]`</br>
e) Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)</br>

## Penyelesaian
```c
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
	return filename;
}
```
fungsi diatas digunakan untuk enkripsi dan dekripsi atbash cipher.
```c
char *proses(char *filename){
	char *ok;
    ok = strchr(filename,'/') + 1;
 
    while (ok != NULL){ 
		if(!strncmp(ok,"AtoZ_",5)){		
			if(strchr(ok,'/')){
				ok = strchr(ok,'/') + 1;
				int ind = ok - filename;
				enkripsi_atbash(ok);
				for(int i = 0; i < strlen(ok); i++){
					filename[ind + i] = ok[i]; 
				}
				filename[ind+strlen(ok)] = '\0';
				break;
			}
		}

		if(!strncmp(ok,"RX_",3)){
			if(strchr(ok,'/')){
				ok = strchr(ok,'/') + 1;
				int ind = ok - filename;
				int mana = cekDua(filename);

				if(mana == 3){
					dec_vigenere(ok);
				}
				if(mana == 4){
					rot13(ok);
				}

				for(int i = 0; i < strlen(ok); i++){
					filename[ind + i] = ok[i]; 
				}
				filename[ind+strlen(ok)] = '\0';
				break;
			}
		}


		if(!strncmp(ok,"A_is_a_",7)){
			if(strchr(ok,'/')){
				ok = strchr(ok,'/') + 1;
				int ind = ok - filename;

				dec_special(ok);
				for(int i = 0; i < strlen(ok); i++){
					filename[ind + i] = ok[i]; 
				}
				filename[ind+strlen(ok)] = '\0';

				break;
			}
		}
        if(strchr(ok,'/') == NULL)
            break;
 
        ok = strchr(ok,'/') + 1;
    }
	return filename;
}
```
fungsi proses diatas akan di panggil di hampir setiap fungsi fuse, digunakan untuk menproses file yang dibuka/dicopy/delete dll. fungsi tersebut hanya akan menenkripsi jika ada parent directory yang memiliki nama awal AtoZ_.
```c
void rekursi(char fpath[1000],int jenis){
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
 
		char n_name[100],tmp[100],path[100];
		strcpy(tmp,dir->d_name);

		if(jenis == 1)
			sprintf(n_name,"%s/%s",fpath,enkripsi_atbash(tmp));
		else if(jenis == 2)
			sprintf(n_name,"%s/%s",fpath,vigenere(tmp));
		else if(jenis == 3)
			sprintf(n_name,"%s/%s",fpath,dec_vigenere(tmp));
		else if(jenis == 4)
			sprintf(n_name,"%s/%s",fpath,rot13(tmp));

		strcpy(path,fpath);
		strcat(path,"/");
		strcat(path,dir->d_name);
 
		rename(path,n_name);
 
        if(tipe(n_name)==1){
            rekursi(n_name,jenis);
            continue;
        }
 
    }
    closedir(d);
}
```
fungsi diatas akan dipanggil ketika rename menjadi AtoZ_ atau dari AtoZ_ untuk enkripsi dan dekripsi secara rekursive.
```
void logSatu(char *from, char *to){
	FILE *fileout;
	fileout = fopen("/home/ryan/inilog.log","a");
	fprintf(fileout, "%s -> %s\n",from,to);
	fclose(fileout);
}
```
digunakan untuk menambahkan rename atau mkdir kedalam log, log di panggil saat rename atau mkdir dengan awalan AtoZ_.
### Hasil
![Nomor_1](https://user-images.githubusercontent.com/68326540/121769514-37fc2c80-cb8e-11eb-94ed-fa7605f51efd.png)
![log_Nomor_1](https://user-images.githubusercontent.com/68326540/121769671-2b2c0880-cb8f-11eb-8d03-18b271661ecd.png)
# Soal 2
## Penjelasan
a) Jika sebuah direktori dibuat dengan awalan “RX_[Nama]” maka direktori tersebut akan menjadi direktori terencode dengan algoritma tambahan ROT13 (Atbash + ROT13).</br>
b) Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).</br>
c) Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.</br>
d) Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).</br>
e) Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal.</br>
## Penyelesaian
### Soal 2a,2b,2c,2d
```c
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
 
    char newKey[msgLen], encryptedMsg[msgLen];
 
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
```
fungsi enkripsi dan dekripsi atbash+vigenere untuk rename
```c
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
```
fungsi enkripsi dan dekripsi atbash+rot13 untuk mkdir.
```c
char *proses(char *filename){}
```
sama seperti nomer 1 fungsi ini juga menggunakan proses untuk menentukan kapan dienkripsi dan dekripsi, namun selain melihat awalah RX_ untuk encode ini juga akan membuka log untuk menentukan apakah dibuat dengan mkdir atau rename, untuk menentukan jenis enkripsi.
```c
int cekDua(char *fpath){
	
	FILE* file = fopen("/home/ryan/logDua.log", "r");
	char line[1000];
	char *token,*jenis;
	while (fgets(line, sizeof(line), file)) {
		token = strtok(line, " ");
		jenis = token;

		token = strtok(NULL, " ");
		token = strtok(NULL, " ");
		token = strtok(NULL, " ");
		token[strlen(token)-1] = 0;

		if(!strncmp(token,fpath,strlen(token))){
			if(!strncmp("RENAME",jenis,6)){
				fclose(file);
				return 3;
			}
			if(!strncmp("MKDIR",jenis,5)){
				fclose(file);
				return 4;
			}
		}
    }
	fclose(file);
	return -1;
}
```
fungsi yang akan memberi tahu jenis enkripsi yang harus di gunakan dengan membuka log
```c
void logDua(char *from, char *to,int tipe){
	FILE *fileout;
	fileout = fopen("/home/ryan/logDua.log","a");

	if(tipe == 1)
		fprintf(fileout, "MKDIR: %s -> %s\n",from,to);
	if(tipe == 2)
		fprintf(fileout, "RENAME: %s -> %s\n",from,to);

	fclose(fileout);
}
```
menambahkan mkdir dan rename kedalam log
```c
void rekursi(char fpath[1000],int jenis){}
```
juga akan menggunakan rekursi untuk rename nya.
### Hasil
![Nomor_2](https://user-images.githubusercontent.com/68326540/121769870-36cbff00-cb90-11eb-8a3c-7905ec45eb41.png)
![log_Nomor_2](https://user-images.githubusercontent.com/68326540/121769882-43e8ee00-cb90-11eb-999a-d1639e17077b.png)


# Soal 3
## Penjelasan
a) Pembuatan folder dengan awalan "A_is_a_" merupakan direktori spesial</br>
b) Jika rename folder dengan awalan "A_is_a_" maka menjadi direktori spesial</br>
c) Jika awalan "A_is_a_" dihapus maka menjadi direktori normal</br>
d) Direktori spesial akan mengembalikan encoding direktori "AtoZ_" dan "RX_" tetapi masing-masing aturan tetap berjalan di subdirektori</br>
e) Pada direktori spesial, nama file menjadi lowercase dan diberi tambahan ekstensi berupa desimal dari biner perbedaanya</br> 
## Penyelesaian
### Soal 3a), 3b), 3c), dan 3e)
```c
char *special(char *filename){
    int length_extension = 0;
    char *extension = strrchr(filename,'.');
    if(extension){
        length_extension = strlen(extension);
    }
 
    int pengkali = 1;
    int total = 0;
    for(int  i = strlen(filename) - length_extension - 1; i >= 0; i--){
        if(filename[i] >= 'A' && filename[i] <= 'Z'){
            filename[i] = 'a' + (filename[i] - 'A');
            total += pengkali;
        }
        pengkali = pengkali << 1;
    }
 
    sprintf(filename,"%s.%d",filename,total);
    return filename;
}
```
Fungsi diatas digunakan untuk menambahkan ekstensi bilangan desimal dari perbedaan biner. Pertama ambil ekstensi dengan fungsi ``strrchr``. Setelah itu dilakukan iterasi dari karakter paling kanan sebelum ekstensi. Terdapat untuk variabel ``pengkali`` yang akan di-left-shift 1 (dikali 2) setiap iterasinya. Jika ada perubahan dari uppercase ke lowercase maka nilai pengkali saat ini akan ditambahkan ke variabel ``total``. Nilai akhir dari variabel ``total`` akan ditambahkan sebagai ekstensi dengan menggunakan fungsi ``sprintf``.

```c
char *dec_special(char *filename){
    int length_decimal = 0,angka = 0;
    char *decimal = strrchr(filename,'.');
    if(decimal){
        length_decimal = strlen(decimal);
		angka = atoi(decimal+1);
    }
 
    char temp[1000] = {0};
    strncpy(temp,filename,strlen(filename)-length_decimal);
    temp[strlen(filename)-length_decimal]='\0';
 
    int length_extension = 0;
    char *extension = strrchr(temp,'.');
    if(extension){
        length_extension = strlen(extension);
    }
 
    int ctr = 0;
    for(int i = strlen(temp) - length_extension - 1; i >= 0 ; i--){
        if((1 << ctr) & angka){
            temp[i] = 'A' + temp[i] - 'a';
        }
        ctr++;
    }
    strcpy(filename,temp);
    return filename;
}
```
Fungsi di atas digunakan untuk mengetahui nama asli dari file. Pertama ambil ekstensi desimalnya dan ekstensi dari file tersebut. Setelah itu iterasi dari karakter paling belakang sebelum ekstensi. Kemudian dilakukan pengecekan seperti metode bitmask. Jika nilai 2 dipangkatkan digit ke-n (n dihitung dari kanan) di-AND-kan dengan angka desimal dari ekstensi bernilai 1 maka ubah karakter tersebut menjadi uppercase. Untuk mencari 2 pangkat dapat dilakukan dengan left shift ``1 << ctr``.

```c
char *proses(char *filename){
	char *ok;
    ok = strchr(filename,'/') + 1;
 
    while (ok != NULL){ 
    ...
		if(!strncmp(ok,"A_is_a_",7)){
			if(strchr(ok,'/')){
				ok = strchr(ok,'/') + 1;
				int ind = ok - filename;

				dec_special(ok);
				for(int i = 0; i < strlen(ok); i++){
					filename[ind + i] = ok[i]; 
				}
				filename[ind+strlen(ok)] = '\0';

				break;
			}
		}
        if(strchr(ok,'/') == NULL)
            break;
 
        ok = strchr(ok,'/') + 1;
    }
	return filename;
}
```
Potongan fungsi di atas digunakan untuk melakukan pengecekan awalan sebuah folder "A_is_a_". Fungsi ini memiliki peranan penting dalam melakukan encode direktori karena semua ``fpath`` akan dikirimkan ke fungsi tersebut untuk diproses

### Hasil
![Nomor_3](https://user-images.githubusercontent.com/68326540/121760010-b6d37400-cb52-11eb-966d-8201d9203f65.png)
# Soal 4
## Penjelasan
a) Membuat log untuk setiap pemanggilan system call. Log disimpan di ``/home/[user]/SinSeiFS.log``</br>
b) Ada dua level log INFO dan WARNING</br>
c) WARNING untuk ``rmdir`` dan ``unlink``</br>
d) Selain kedua fungsi itu, termasuk ke INFO</br>
e) Format untuk logging yaitu:``[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]`` </br>
## Penyelesaian
### Soal 4a), 4b), 4c), 4d), 4e)
```c
void appendLog(char *level, char *command, char *desc){
	char time_now[100] = {0};
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	strftime(time_now,100,"%d%m%Y-%H:%M:%S",tm);
 
	FILE *fileout;
	fileout = fopen("/home/ryan/SinSeiFS.log","a");
	fprintf(fileout, "%s::%s:%s::%s\n",level,time_now,command,desc);
	fclose(fileout);
}
```
Pertama ambil waktu saat ini dengan ``time_t``. Menggunakan ``fopen`` dengan parameter "a" untuk menambahkan (append) log ke file tujuan
Untuk xmp_rmdir dan xmp_unlink akan memanggil fungsi appendLog dengan mengirim level = "WARNING", sedangkan lainnya akan mengirimkan argumen level = "INFO"
### Hasil
![Nomor_4](https://user-images.githubusercontent.com/68326540/121760245-db7c1b80-cb53-11eb-91d1-f361a9d79f61.png)

# Kendala
- Susah melakukan debugging jika ada error.

# soal-shift-sisop-modul-4-E02-2021

## Anggota Kelompok E02
05111940000001 - Christoper Baptista

05111940000101 - Zulfiqar Fauzul Akbar

05111940000152 - Ryan Fernaldy

# Soal 1
## Penjelasan
## Penyelesaian

# Soal 2
## Penjelasan
## Penyelesaian

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
# Kendala

# SoalShift_modul3_A07

## Soal No 1
<pre>
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
    ./faktorial 5 3 4
    3! = 6
    4! = 24
    5! = 120
</pre>
## Ans
<pre>
1. Setiap input disimpan dalam array, kemudian di sort.
2. Membuat thread sejumlah input.
3. Setiap thread menjalankan fungsi faktorial.
</pre>

<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include&lt;stdio.h&gt;  
 #include&lt;string.h&gt;  
 #include&lt;pthread.h&gt;  
 #include&lt;stdlib.h&gt;  
 #include&lt;unistd.h&gt;  
 #include&lt;sys/types.h&gt;  
 pthread_t tid[1000000]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread  
 int hasil[100000];  
 int inp[1000000]; //inisialisasi jumlah untuk looping  
 int things;  
 void swap(int *xp, int *yp)   
 {   
   int temp = *xp;   
   *xp = *yp;   
   *yp = temp;   
 }   
 void bubbleSort(int arr[])   
 {   
   int i, j, n;  
   n=3;   
   for (i = 0; i &lt; n-1; i++)      
     for (j = 0; j &lt; n-i-1; j++)   
       if (arr[j] &gt; arr[j+1])   
        swap(&amp;arr[j], &amp;arr[j+1]);   
 }  
 int fact(int n){  
      int i;  
      unsigned long long ctr;  
      ctr=1;  
      for(i=1;i&lt;=n;i++){  
           ctr=ctr*i;  
      }  
      return ctr;  
 }  
 void* factor(void *arg)  
 {  
      unsigned long i=0;  
      pthread_t id=pthread_self();  
      int iter;  
      for(iter=0; iter&lt;things-1;iter++){  
           if(pthread_equal(id,tid[iter])) //thread untuk menjalankan counter  
                hasil[iter]=fact(inp[iter]);  
      }  
      return NULL;  
 }  
 int main(int argc, char* argv[] )//void)  
 {  
      int i=0,j;  
      int err;  
 //     int argc=4;  
 //     char* argv[]={"./tes", "1", "2", "3"};  
      things=argc;  
      char *ptr;  
      for(j=0;j&lt;(argc-1);j++)  
           inp[j]=strtol(argv[j+1],&amp;ptr,10);  
      bubbleSort(inp);  
      while(i&lt;argc) // loop sejumlah thread  
      {  
           err=pthread_create(&amp;(tid[i]),NULL,&amp;factor,NULL); //membuat thread  
 /*          if(err!=0) //cek error  
           {  
                printf("\n can't create thread : [%s]",strerror(err));  
           }  
           else  
           {  
                printf("\n create thread success\n");  
           } */  
           i++;  
      }  
      for(i=0;i&lt;argc-1;i++)  
           pthread_join(tid[i],NULL);  
      for(i=0; i&lt;argc-1;i++){  
           printf("%d! = %d\n", inp[i], hasil[i]);  
      }  
      exit(0);  
      return 0;  
 }  
</code></pre>

## Soal No 2
<pre>

Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
Terdapat 2 server: server penjual dan server pembeli
  1 server hanya bisa terkoneksi dengan 1 client
  Server penjual dan server pembeli memiliki stok barang yang selalu sama
  Client yang terkoneksi ke server penjual hanya bisa menambah stok
  Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
  Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
  Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
  Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
  Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
  Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
  Server penjual akan mencetak stok saat ini setiap 5 detik sekali
  Menggunakan thread, socket, shared memory
</pre>
## Ans
<pre>
1. Soal diselesaikan dengan 4 proses, yaitu Client&Server Pembeli dan Penjual.
2. Pembeli :
   2.1 Client :
       2.1.1 Client terhubung pada port yang sama dengan server pembeli.
       2.1.2 Client akan mengirimkan pesan ke server.
       2.1.3 Client mengambil pesan dari server dan diletakkan pada buffer.
       2.1.4 Client Mencetak buffer yang diterima dari server.
   2.2 Server :
       2.2.1 Server Pembeli akan memiliki shared memory dengan server penjual.
       2.2.2 
</pre>
## Soal No 3
<pre>
Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
Terdapat 2 karakter Agmal dan Iraj
Kedua karakter memiliki status yang unik
Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
Iraj memiliki Spirit_Status, di awal program memiliki status 100
Terdapat 3 Fitur utama
All Status, yaitu menampilkan status kedua sahabat
Ex: Agmal WakeUp_Status = 75 
      Iraj Spirit_Status = 30
“Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
“Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point

Terdapat Kasus yang unik dimana:
Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)

Program akan berhenti jika Salah Satu :
WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”
Syarat Menggunakan Lebih dari 1 Thread
<pre/>
## ANS
<pre>
1. Membuat sebuah loop tak hingga.
2. Merecreate thread jika thread sedang tidak disleep.
3. Proses sleep terjadi ketika flag counter=3 mengakibatkan flag sleep menjadi 1.(0 artinya tidak sleep)
4. Melakukan exit(0) ketika salah satu proses terpenuhi (Akmal Wake stat = 100 || Iraj spirit stat = 0)
</pre>
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include&lt;stdio.h&gt;  
 #include&lt;string.h&gt;  
 #include&lt;pthread.h&gt;  
 #include&lt;stdlib.h&gt;  
 #include&lt;unistd.h&gt;  
 #include&lt;sys/types.h&gt;  
 #include&lt;sys/wait.h&gt;  
 #include&lt;string.h&gt;  
 pthread_t tid[3];  
 int bangun=0, spirit=100, iterb=0, iters=0, sb=0, ss=0;  
 char cmd[100];  
 void* Iraj(void *arg){  
      if(strcmp(cmd, "Iraj Ayo Tidur")==0) {  
           spirit-=20;  
           iters++;  
      }  
      if(spirit&lt;=0){  
           printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");  
           exit(0);  
      }  
      if(iterb==3){  
           ss=1;  
           iterb=0;  
           printf("Fitur Iraj Ayo Tidur disabled 10 s\n");  
           sleep(10);  
           ss=0;  
      }  
 }  
 void* Agmal(void *arg){  
      if(strcmp(cmd, "Agmal Ayo Bangun")==0) {  
           bangun +=15;  
           iterb++;  
      }  
      if(bangun&gt;=100){  
           printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");  
           exit(0);  
      }  
      if(iters==3){  
           sb=1;  
           iters=0;  
           printf("Fitur Agmal Ayo Bangun disabled 10 s\n");  
           sleep(10);  
           sb=0;  
      }  
 }  
 void* stats(void *arg){  
    if(strcmp(cmd, "All status")==0){  
      printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", bangun, spirit);  
    }  
 }  
 int main(void)  
 {  
      while(1){  
           int i=0;pthread_t id=pthread_self();  
           fgets(cmd, sizeof(cmd), stdin);  
           cmd[strcspn(cmd,"\n")]='\0';  
           if(sb==0) pthread_create(&amp;(tid[0]),NULL,&amp;Agmal,NULL);  
           else printf("Fitur Agmal Ayo Bangun disabled 10 s\n");  
           if(ss==0) pthread_create(&amp;(tid[1]),NULL,&amp;Iraj,NULL);  
           else printf("Fitur Iraj Ayo Tidur disabled 10 s\n");  
           pthread_create(&amp;(tid[2]),NULL,&amp;stats,NULL);  
 //          printf("%d ss, %d sb, %d b, %d s\n", ss, sb, iterb, iters);  
      }  
      pthread_join(tid[0],NULL);  
      pthread_join(tid[1],NULL);  
      pthread_join(tid[2],NULL);  
      exit(0);  
      return 0;  
 }  
</code></pre>
## Soal 4
<pre>
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 
Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
Ketika Mengekstrak file .zip juga harus secara bersama-sama
Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
Wajib Menggunakan Multithreading
Boleh menggunakan system
</pre>
## ANS
<pre>
1. Membuat 2 thread (memastikan semua proses berjalan bersamaan).
2. Thread 1 : Membuat file > Mengkompres > Meremove > Mengekstrak.
3. Thread 2 bekerja seperti thread 1, dengan nama yang berbeda.
4. Semua perintah bisa dijalankan.
</pre>
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include&lt;stdio.h&gt;  
 #include&lt;string.h&gt;  
 #include&lt;pthread.h&gt;  
 #include&lt;stdlib.h&gt;  
 #include&lt;unistd.h&gt;  
 #include&lt;sys/types.h&gt;  
 #include&lt;sys/wait.h&gt;  
 pthread_t tid[3]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread  
 int length=5; //inisialisasi jumlah untuk looping  
 void* save(void *arg)  
 {  
      pthread_t id=pthread_self();  
      if(pthread_equal(id,tid[0]))  
      {  
        system("mkdir /home/carwima/Documents/FolderProses1");  
         system("ps -aux | head -n 10 &gt; /home/carwima/Documents/FolderProses1/SimpanProses1.txt");  
         system("zip /home/carwima/SoalShift_modul3_A01/KompresProses1.zip /home/carwima/Documents/FolderProses1/SimpanProses1.txt");  
        system("rm /home/carwima/Documents/FolderProses1/SimpanProses1.txt");  
         sleep(15);  
         system("unzip /home/carwima/SoalShift_modul3_A01/KompresProses1.zip");  
      }  
      else if(pthread_equal(id,tid[1]))  
      {  
        system("mkdir /home/carwima/Documents/FolderProses2");  
         system("ps -aux | head -n 10 &gt; /home/carwima/Documents/FolderProses2/SimpanProses2.txt");  
        system("zip /home/carwima/SoalShift_modul3_A01/KompresProses2.zip /home/carwima/Documents/FolderProses2/SimpanProses2.txt");  
         system("rm /home/carwima/Documents/FolderProses2/SimpanProses2.txt");  
         sleep(15);  
         system("unzip /home/carwima/SoalShift_modul3_A01/KompresProses2.zip");  
      }  
 /*     else if(pthread_equal(id,tid[2])) // thread menutup gambar  
      {  
 //          execv("/usr/bin/pkill", argv2);  
      } */  
      return NULL;  
 }  
 int main(void)  
 {  
      int i=0;  
      int err;  
      while(i&lt;2) // loop sejumlah thread  
      {  
           err=pthread_create(&amp;(tid[i]),NULL,&amp;save,NULL);  
           i++;  
      }  
      pthread_join(tid[0],NULL);  
      pthread_join(tid[1],NULL);  
 //    pthread_join(tid[2],NULL);  
      exit(0);  
      return 0;  
 }  
</code></pre>

## Soal 5
Lihat Modul 3.
## ANS

# SoalShift_modul3_A07

## Soal No 1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
    ./faktorial 5 3 4
    3! = 6
    4! = 24
    5! = 120
## Ans
1. Setiap input disimpan dalam array, kemudian di sort.
2. Membuat thread sejumlah input.
3. Setiap thread menjalankan fungsi faktorial.

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
## Ans
1. Soal diselesaikan dengan 4 proses, yaitu Client&Server Pembeli dan Penjual.
2. Pembeli :
   2.1 Client :
       2.1.1 Client terhubung pada port yang sama dengan server pembeli.
       2.1.2 Client akan mengirimkan pesan ke server.
       2.1.3 Client mengambil pesan dari server dan diletakkan pada buffer.
       2.1.4 Client Mencetak buffer yang diterima dari server.
   2.2 Server :
       2.2.1 Server Pembeli akan memiliki shared memory dengan server penjual.
       2.2.2 Untuk Server penjual menggunakan Thread dan shared memory.
       2.2.3 Antara Client dan Server menggunakan Socket.

### Server Pembeli
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include &lt;stdio.h&gt;  
 #include &lt;sys/socket.h&gt;  
 #include &lt;stdlib.h&gt;  
 #include &lt;netinet/in.h&gt;  
 #include &lt;string.h&gt;  
 #include &lt;unistd.h&gt;  
 #include &lt;sys/types.h&gt;  
 #include &lt;sys/wait.h&gt;  
 #include &lt;sys/ipc.h&gt;  
 #include &lt;sys/shm.h&gt;  
 #define PORT 8080  
 int main(int argc, char const *argv[]) {  
      int server_fd, new_socket, valread;  
      struct sockaddr_in address;  
      int opt = 1;  
      int addrlen = sizeof(address);  
      char *done = "transaksi berhasil";  
      char *fail = "transaksi gagal";  
      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {  
           perror("socket failed");  
           exit(EXIT_FAILURE);  
      }  
      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &amp;opt, sizeof(opt))) {  
           perror("setsockopt");  
           exit(EXIT_FAILURE);  
      }  
      address.sin_family = AF_INET;  
      address.sin_addr.s_addr = INADDR_ANY;  
      address.sin_port = htons( PORT );  
      if (bind(server_fd, (struct sockaddr *)&amp;address, sizeof(address))&lt;0) {  
           perror("bind failed");  
           exit(EXIT_FAILURE);  
      }  
      if (listen(server_fd, 3) &lt; 0) {  
           perror("listen");  
           exit(EXIT_FAILURE);  
      }  
      if ((new_socket = accept(server_fd, (struct sockaddr *)&amp;address, (socklen_t*)&amp;addrlen))&lt;0) {  
           perror("accept");  
           exit(EXIT_FAILURE);  
      }  
      key_t key = 1234;  
      int *stock;  
      int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);  
      stock = shmat(shmid, NULL, 0);  
      while(1){  
           char buffer[1024] = {0};  
           valread = read( new_socket , buffer, 1024);  
           if(strcmp(buffer, "beli")==0){  
                if(*stock&gt;0) {  
                     *stock=*stock-1;  
                     send(new_socket , done , strlen(done) , 0 );  
                }  
                else{  
                     send(new_socket , fail , strlen(fail) , 0 );  
                }  
           }  
      }  
      return 0;  
 }  
</code></pre>
### Server Penjual
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include &lt;stdio.h&gt;  
 #include &lt;sys/socket.h&gt;  
 #include &lt;stdlib.h&gt;  
 #include &lt;netinet/in.h&gt;  
 #include &lt;string.h&gt;  
 #include &lt;unistd.h&gt;  
 #include &lt;sys/types.h&gt;  
 #include &lt;sys/wait.h&gt;  
 #include &lt;sys/ipc.h&gt;  
 #include &lt;sys/shm.h&gt;  
 #include &lt;pthread.h&gt;  
 #define PORT 8070  
 void* printstock(void* arg){       
      key_t key = 1234;  
      int *stock;  
      int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);  
      stock = shmat(shmid, NULL, 0);  
      while(1){  
           printf("Jumlah Stock : %d\n",*stock);  
           sleep(5);  
      }  
 }  
 int main(int argc, char const *argv[]) {  
      int server_fd, new_socket, valread;  
      struct sockaddr_in address;  
      int opt = 1;  
      int addrlen = sizeof(address);  
      char buffer[1024] = {0};  
      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {  
           perror("socket failed");  
           exit(EXIT_FAILURE);  
      }  
      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &amp;opt, sizeof(opt))) {  
           perror("setsockopt");  
           exit(EXIT_FAILURE);  
      }  
      address.sin_family = AF_INET;  
      address.sin_addr.s_addr = INADDR_ANY;  
      address.sin_port = htons( PORT );  
      if (bind(server_fd, (struct sockaddr *)&amp;address, sizeof(address))&lt;0) {  
           perror("bind failed");  
           exit(EXIT_FAILURE);  
      }  
      if (listen(server_fd, 3) &lt; 0) {  
           perror("listen");  
           exit(EXIT_FAILURE);  
      }  
      if ((new_socket = accept(server_fd, (struct sockaddr *)&amp;address, (socklen_t*)&amp;addrlen))&lt;0) {  
           perror("accept");  
           exit(EXIT_FAILURE);  
      }  
      pthread_t th[2];  
      pthread_create( &amp;th[0], NULL, printstock, NULL);  
      key_t key = 1234;  
      int *stock;  
      int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);  
      stock = shmat(shmid, NULL, 0);  
      *stock=0;  
      while(1){  
           valread = read(new_socket , buffer, 1024);  
           if(strcmp(buffer, "tambah")==0){  
                *stock=*stock+1;  
           }  
      }  
      int i;  
      for(i=0; i&lt;=0;i++) pthread_join(th[i],NULL);  
 }  
</code></pre>
### Client Pembeli
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include &lt;stdio.h&gt;  
 #include &lt;sys/socket.h&gt;  
 #include &lt;stdlib.h&gt;  
 #include &lt;netinet/in.h&gt;  
 #include &lt;string.h&gt;  
 #include &lt;unistd.h&gt;  
 #include &lt;arpa/inet.h&gt;  
 #define PORT 8080  
 int main(int argc, char const *argv[]) {  
      struct sockaddr_in address;  
      int sock = 0, valread;  
      struct sockaddr_in serv_addr;  
      char hello[100];  
      fgets(hello, sizeof(hello), stdin);  
      hello[strcspn(hello,"\n")]='\0';  
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) &lt; 0) {  
           printf("\n Socket creation error \n");  
           return -1;  
      }  
      memset(&amp;serv_addr, '0', sizeof(serv_addr));  
      serv_addr.sin_family = AF_INET;  
      serv_addr.sin_port = htons(PORT);  
      if(inet_pton(AF_INET, "127.0.0.1", &amp;serv_addr.sin_addr)&lt;=0) {  
           printf("\nInvalid address/ Address not supported \n");  
           return -1;  
      }  
      if (connect(sock, (struct sockaddr *)&amp;serv_addr, sizeof(serv_addr)) &lt; 0) {  
           printf("\nConnection Failed \n");  
           return -1;  
      }  
      while(1){  
           char buffer[1024] = {0};  
           fgets(hello, sizeof(hello), stdin);  
           hello[strcspn(hello,"\n")]='\0';  
           send(sock , hello , strlen(hello) , 0 );  
           valread = read( sock , buffer, 1024);  
           printf("%s\n", buffer);  
      }  
   return 0;  
 }  
</code></pre>
### Client Penjual
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include &lt;stdio.h&gt;  
 #include &lt;sys/socket.h&gt;  
 #include &lt;stdlib.h&gt;  
 #include &lt;netinet/in.h&gt;  
 #include &lt;string.h&gt;  
 #include &lt;unistd.h&gt;  
 #include &lt;arpa/inet.h&gt;  
 #define PORT 8070  
 int main(int argc, char const *argv[]) {  
      struct sockaddr_in address;  
      int sock = 0, valread;  
      struct sockaddr_in serv_addr;  
      char hello[100];  
      char buffer[1024] = {0};  
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) &lt; 0) {  
           printf("\n Socket creation error \n");  
           return -1;  
      }  
      memset(&amp;serv_addr, '0', sizeof(serv_addr));  
      serv_addr.sin_family = AF_INET;  
      serv_addr.sin_port = htons(PORT);  
      if(inet_pton(AF_INET, "127.0.0.1", &amp;serv_addr.sin_addr)&lt;=0) {  
           printf("\nInvalid address/ Address not supported \n");  
           return -1;  
      }  
      if (connect(sock, (struct sockaddr *)&amp;serv_addr, sizeof(serv_addr)) &lt; 0) {  
           printf("\nConnection Failed \n");  
           return -1;  
      }  
      while(1){  
           fgets(hello, sizeof(hello), stdin);  
           hello[strcspn(hello,"\n")]='\0';  
           send(sock , hello , strlen(hello) , 0 );  
      }  
      return 0;  
 }  
</code></pre>
## Soal No 3
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
## ANS
1. Membuat sebuah loop tak hingga.
2. Merecreate thread jika thread sedang tidak disleep.
3. Proses sleep terjadi ketika flag counter=3 mengakibatkan flag sleep menjadi 1.(0 artinya tidak sleep)
4. Melakukan exit(0) ketika salah satu proses terpenuhi (Akmal Wake stat = 100 || Iraj spirit stat = 0)

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
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 
Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
Ketika Mengekstrak file .zip juga harus secara bersama-sama
Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
Wajib Menggunakan Multithreading
Boleh menggunakan system
## ANS
1. Membuat 2 thread (memastikan semua proses berjalan bersamaan).
2. Thread 1 : Membuat file > Mengkompres > Meremove > Mengekstrak.
3. Thread 2 bekerja seperti thread 1, dengan nama yang berbeda.
4. Semua perintah bisa dijalankan.
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
Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta. 
Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.
Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.
Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).
Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 
Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
Pembeli (terintegrasi dengan game)
Dapat mengecek stok makanan yang ada di toko.
Jika stok ada, pembeli dapat membeli makanan.
Penjual (terpisah)
Bisa mengecek stok makanan yang ada di toko
Penjual dapat menambah stok makanan.
	Spesifikasi program:
Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)
Program terdiri dari 3 scene yaitu standby, battle, dan shop.
Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :

        Standby Mode
        Health : [health status]
        Hunger : [hunger status]
        Hygiene : [hygiene status]
        Food left : [your food stock]
        Bath will be ready in [cooldown]s
        Choices
        Eat
        Bath
        Battle
        Shop
        Exit

Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :

        Battle Mode
        Monster’s Health : [health status]
        Enemy’s Health : [enemy health status]
        Choices
        Attack
        Run

Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :

		Shop Mode
		Shop food stock : [shop food stock]
		Your food stock : [your food stock]
		Choices
        Buy
        Back

Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :

        Shop
        Food stock : [shop food stock]
        Choices
        Restock
        Exit

Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))

## ANS
1. Disini ada 2 program - program untuk game utama dan untuk stock. keduanya dihubungkan dengan shared memory.
2. untuk kodingan utama menggunakan multithreading.
3. untuk progrsam stock menggunakam join thread.

### Main Program
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include &lt;stdio.h&gt;  
 #include &lt;string.h&gt;  
 #include &lt;pthread.h&gt;  
 #include &lt;stdlib.h&gt;  
 #include &lt;termios.h&gt;      //termios, TCSANOW, ECHO, ICANON  
 #include &lt;unistd.h&gt;   //STDIN_FILENO  
 #include &lt;sys/types.h&gt;  
 #include &lt;sys/wait.h&gt;  
 #include &lt;sys/ipc.h&gt;  
 #include &lt;sys/shm.h&gt;  
 int input, status, menu; //System  
 int hunger, hp, hygiene, food, cooldown, mhp;     //Game Core  
 char c,name[100];  
 void* kbhit(void *arg)  
 {  
   //source : https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar  
   static struct termios oldt, newt;  
   /*tcgetattr gets the parameters of the current terminal  
   STDIN_FILENO will tell tcgetattr that it should write the settings  
   of stdin to oldt*/  
   tcgetattr( STDIN_FILENO, &amp;oldt);  
   /*now the settings will be copied*/  
   newt = oldt;  
   /*ICANON normally takes care that one line at a time will be processed  
   that means it will return if it sees a "\n" or an EOF or an EOL*/  
   newt.c_lflag &amp;= ~(ICANON);       
   /*Those new settings will be set to STDIN  
   TCSANOW tells tcsetattr to change attributes immediately. */  
   tcsetattr( STDIN_FILENO, TCSANOW, &amp;newt);  
   /*This is your part:  
   I choose 'e' to end input. Notice that EOF is also turned off  
   in the non-canonical mode*/  
   while((c=getchar()))     
   {       
      switch(c){  
                case '1':  
                     input = 1;  
                     break;  
                case '2':  
                     input = 2;  
                     break;  
                case '3':  
                     input = 3;  
                     break;  
                case '4':  
                     input = 4;  
                     break;  
                case '5':  
                     input = 5;  
                     break;  
                default:  
                     input = 0;  
                     break;  
      }  
   /*restore the old settings*/  
   }  
   tcsetattr( STDIN_FILENO, TCSANOW, &amp;oldt);  
 }  
 void* Standby(void *arg){  
      while(1){  
           if(menu==0){  
                input=0;  
                sleep(1);  
                system("clear");  
                printf("%s's Status\nHealth : %d\nHunger : %d\nHygiene : %d\nFood left : %d\n",name, hp, hunger, hygiene, food);  
                if(cooldown!=0) printf("Bath will be ready in %ds\n", cooldown);  
                else printf("Bath is ready\n");  
                printf("Choices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");  
                printf("input : %d\n",input);  
                switch(input){  
                     case 1:  
                          if(food&gt;0&amp;&amp;hunger&lt;200){  
                               hunger+=15;  
                               food--;  
                               if(hunger&gt;200){  
                                    printf("ENOUGH!\n");  
                                    hunger=200;  
                               }  
                          }  
                     break;  
                     case 2:  
                          if(cooldown==0){  
                               hygiene+=30;  
                               cooldown=20;  
                          }  
                          else printf("Bath will be ready in %ds\n", cooldown);  
                     break;  
                     case 3:  
                          menu=1;  
                          mhp=100;  
                     break;  
                     case 4:  
                          menu=2;  
                     break;  
                     case 5:  
                          exit(0);  
                     break;  
                }  
           }  
      }  
 }  
 void* battle(void *arg){  
      while(1){  
           if(menu==1){  
                input=0;  
                sleep(1);  
                system("clear");  
                printf("Noone stands in the heat of BATTLE!\nFoe's Health : %d\n%s's Health : %d\nChoices\n1. Attack\n2. Run\n", mhp, name, hp);  
                switch(input){  
                     case 1:  
                          mhp-=20;  
                          hp-=20;  
                     break;  
                     case 2:  
                          menu=0;  
                     break;  
                }  
                if(mhp==0) menu=0;  
           }  
      }  
 }  
 void* shop(void *arg){  
      while(1){  
           if(menu==2){  
                input=0;  
                sleep(1);  
                key_t key = 1234;  
                int *stock;  
                int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);  
                stock = shmat(shmid, NULL, 0);  
                system("clear");  
                printf("Welcome to shop, What do you want?\nAvaiable Stock : %d\nInventory : %d\nMenu :\n1. Buy\n2. Main Menu\n", *stock, food);  
                switch(input){  
                case 1:  
                     if(*stock&gt;0){  
                          *stock=*stock-1;  
                          food++;  
                     }  
                break;  
                case 2:  
                     menu=0;  
                break;  
                }  
           }  
      }       
 }  
 void* soap(void *arg){  
      while(1){  
           if(cooldown&gt;0){  
                cooldown--;  
                sleep(1);  
           }  
      }  
 }  
 /*  
 void* status_hungry(void *arg){  
      while(1){  
           if(hunger&lt;=0){  
                hp-=5;  
                sleep(10);  
           }  
      }  
 }  
 */  
 void* status_regen(void *arg){  
      while(1){  
           if(menu==0&amp;&amp;hp&lt;100){  
                hp+=5;  
                sleep(10);  
           }  
      }  
 }  
 void* status_hunger(void *arg){  
      while(1){  
           if(menu==0){  
                hunger-=5;  
                sleep(10);  
           }  
      }  
 }  
 void* status_hygiene(void *arg){  
      while(1){  
           if(menu!=1){  
                hygiene-=10;  
                sleep(30);  
           }  
      }  
 }  
 void* endgame(void*arg){  
      while(1){  
           if(hp&lt;=0||hygiene&lt;=0||hunger&lt;=0){  
                menu=3;  
                system("clear");  
                printf("Master, whyyyyyy... *dead*\n");  
                exit(0);  
           }  
      }  
 }  
 int main(){  
      pthread_t th[10];  
      system("clear");  
      printf("Welcome to the MonSim\nName Your Monster : ");  
      fgets(name, sizeof(name), stdin);  
      name[strcspn(name,"\n")]='\0';  
      printf("Welcome Master, My Name is %s\n", name);  
      sleep(2);  
      hygiene=100;  
      hp=100;  
      hunger=200;  
      menu=0;   
      pthread_create( &amp;th[0], NULL, kbhit, NULL);  
      pthread_create( &amp;th[1], NULL, Standby, NULL);  
      pthread_create( &amp;th[2], NULL, battle, NULL);  
 //     pthread_create( &amp;th[9], NULL, status_hungry, NULL);  
      pthread_create( &amp;th[3], NULL, status_hygiene, NULL);  
      pthread_create( &amp;th[4], NULL, status_regen, NULL);  
      pthread_create( &amp;th[5], NULL, status_hunger, NULL);  
      pthread_create( &amp;th[6], NULL, soap, NULL);  
      pthread_create( &amp;th[7], NULL, shop, NULL);  
      pthread_create( &amp;th[8], NULL, endgame, NULL);  
      int i;  
      for(i=0; i&lt;=9;i++) pthread_join(th[0],NULL);  
 }  
</code></pre>
### Stock
<pre  style="font-family:arial;font-size:12px;border:1px dashed #CCCCCC;width:99%;height:auto;overflow:auto;background:#f0f0f0;;background-image:URL(http://2.bp.blogspot.com/_z5ltvMQPaa8/SjJXr_U2YBI/AAAAAAAAAAM/46OqEP32CJ8/s320/codebg.gif);padding:0px;color:#000000;text-align:left;line-height:20px;"><code style="color:#000000;word-wrap:normal;"> #include &lt;stdio.h&gt;  
 #include &lt;string.h&gt;  
 #include &lt;pthread.h&gt;  
 #include &lt;stdlib.h&gt;  
 #include &lt;termios.h&gt;  //termios, TCSANOW, ECHO, ICANON  
 #include &lt;unistd.h&gt;   //STDIN_FILENO  
 #include &lt;sys/types.h&gt;  
 #include &lt;sys/wait.h&gt;  
 #include &lt;sys/ipc.h&gt;  
 #include &lt;sys/shm.h&gt;  
 int input;  
 char c;  
 void* kbhit(void *arg)  
 {  
   //source : https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar  
   static struct termios oldt, newt;  
   /*tcgetattr gets the parameters of the current terminal  
   STDIN_FILENO will tell tcgetattr that it should write the settings  
   of stdin to oldt*/  
   tcgetattr( STDIN_FILENO, &amp;oldt);  
   /*now the settings will be copied*/  
   newt = oldt;  
   /*ICANON normally takes care that one line at a time will be processed  
   that means it will return if it sees a "\n" or an EOF or an EOL*/  
   newt.c_lflag &amp;= ~(ICANON);       
   /*Those new settings will be set to STDIN  
   TCSANOW tells tcsetattr to change attributes immediately. */  
   tcsetattr( STDIN_FILENO, TCSANOW, &amp;newt);  
   /*This is your part:  
   I choose 'e' to end input. Notice that EOF is also turned off  
   in the non-canonical mode*/  
   while((c=getchar()))     
   {       
      switch(c){  
                case '1':  
                     input = 1;  
                     break;  
                case '2':  
                     input = 2;  
                     break;  
                default:  
                     input = 0;  
                     break;  
      }  
   /*restore the old settings*/  
   }  
   tcsetattr( STDIN_FILENO, TCSANOW, &amp;oldt);  
 }  
 void* stocks(void*arg){  
      key_t key = 1234;  
      int *stock;  
      int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);  
      stock = shmat(shmid, NULL, 0);  
      *stock=0;       
      while(1){  
           input=0;  
           sleep(1);  
           system("clear");  
           printf("Hello shop owner, What do you want?\nAvaiable Stock : %d\nMenu :\n1. Restock\n2. Exit\ninput : %d\n", *stock,input);  
           switch(input){  
                case 1:  
                     *stock=*stock+1;  
                break;  
                case 2:  
                     exit(0);  
                break;  
           }  
      }  
 }  
 void main(){       
      pthread_t th[10];  
      system("clear");  
      pthread_create( &amp;th[0], NULL, kbhit, NULL);  
      pthread_create( &amp;th[1], NULL, stocks, NULL);  
      int i;  
      for(i=0; i&lt;=1;i++) pthread_join(th[0],NULL);  
</code></pre>

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[3]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread

int length=5; //inisialisasi jumlah untuk looping
void* save(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid[0]))
	{
             system("mkdir /home/carwima/Documents/FolderProses1");
	     system("ps -aux | head -n 10 > /home/carwima/Documents/FolderProses1/SimpanProses1.txt");
	     system("zip /home/carwima/SoalShift_modul3_A01/KompresProses1.zip /home/carwima/Documents/FolderProses1/SimpanProses1.txt");
             system("rm /home/carwima/Documents/FolderProses1/SimpanProses1.txt");
	     sleep(15);
	     system("unzip /home/carwima/SoalShift_modul3_A01/KompresProses1.zip");
	}
	else if(pthread_equal(id,tid[1]))
	{
             system("mkdir /home/carwima/Documents/FolderProses2");
	     system("ps -aux | head -n 10 > /home/carwima/Documents/FolderProses2/SimpanProses2.txt");
             system("zip /home/carwima/SoalShift_modul3_A01/KompresProses2.zip /home/carwima/Documents/FolderProses2/SimpanProses2.txt");
	     system("rm /home/carwima/Documents/FolderProses2/SimpanProses2.txt");
	     sleep(15);
	     system("unzip /home/carwima/SoalShift_modul3_A01/KompresProses2.zip");
	}
/*	else if(pthread_equal(id,tid[2])) // thread menutup gambar
	{
//		execv("/usr/bin/pkill", argv2);
	} */

	return NULL;
}

int main(void)
{
	int i=0;
	int err;
	while(i<2) // loop sejumlah thread
	{
		err=pthread_create(&(tid[i]),NULL,&save,NULL);
		i++;
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
//        pthread_join(tid[2],NULL);
	exit(0);
	return 0;
}

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

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
   for (i = 0; i < n-1; i++)        
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
}

int fact(int n){
	int i;
	unsigned long long ctr;
	ctr=1;
	for(i=1;i<=n;i++){
		ctr=ctr*i;
	}
	return ctr;
}
void* factor(void *arg)
{
	unsigned long i=0;
	pthread_t id=pthread_self();
	int iter;
	for(iter=0; iter<things-1;iter++){
		if(pthread_equal(id,tid[iter])) //thread untuk menjalankan counter
			hasil[iter]=fact(inp[iter]);
	}
	return NULL;
}

int main(int argc, char* argv[] )//void)
{
	int i=0,j;
	int err;
//	int argc=4;
//	char* argv[]={"./tes", "1", "2", "3"};
	things=argc;
	char *ptr;
	for(j=0;j<(argc-1);j++)
		inp[j]=strtol(argv[j+1],&ptr,10);
	bubbleSort(inp);
	while(i<argc) // loop sejumlah thread
	{
		err=pthread_create(&(tid[i]),NULL,&factor,NULL); //membuat thread
/*		if(err!=0) //cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success\n");
		} */
		i++;
	}
	for(i=0;i<argc-1;i++)
		pthread_join(tid[i],NULL);
	for(i=0; i<argc-1;i++){
		printf("%d! = %d\n", inp[i], hasil[i]);
	}
	exit(0);
	return 0;
}

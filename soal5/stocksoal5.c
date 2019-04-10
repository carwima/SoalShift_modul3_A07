#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int input;
char c;

void* kbhit(void *arg)
{
    //source : https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar
    static struct termios oldt, newt;
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

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
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
void* stocks(void*arg){
	while(1){
		input=0;
		sleep(1);
		key_t key = 1234;
		int *stock;
		int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
		stock = shmat(shmid, NULL, 0);
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
	pthread_create( &th[0], NULL, kbhit, NULL);
	pthread_create( &th[1], NULL, stocks, NULL);
	int i;
	for(i=0; i<=1;i++) pthread_join(th[0],NULL);
	
}

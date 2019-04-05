#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

pthread_t tid[3];
int bangun=0, spirit=100, iterb=0, iters=0, sb=0, ss=0;
char cmd[100];

void* Iraj(void *arg){
	if(strcmp(cmd, "Iraj Ayo Tidur")==0) {
		spirit-=20;
		iters++;
	}
	if(spirit<=0){
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
	if(bangun>=100){
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
		if(sb==0) pthread_create(&(tid[0]),NULL,&Agmal,NULL);
		else printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
		if(ss==0) pthread_create(&(tid[1]),NULL,&Iraj,NULL);
		else printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
		pthread_create(&(tid[2]),NULL,&stats,NULL);
//		printf("%d ss, %d sb, %d b, %d s\n", ss, sb, iterb, iters);
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);

	exit(0);
	return 0;
}

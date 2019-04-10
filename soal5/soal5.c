#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int input, status, menu; //System
int hunger, hp, hygiene, food, cooldown, mhp;	//Game Core
char c,name[100];
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
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
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
					if(food>0&&hunger<200){
						hunger+=15;
						food--;
						if(hunger>200){
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
				if(*stock>0){
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
		if(cooldown>0){
			cooldown--;
			sleep(1);
		}
	}
}
/*
void* status_hungry(void *arg){
	while(1){
		if(hunger<=0){
			hp-=5;
			sleep(10);
		}
	}
}
*/
void* status_regen(void *arg){
	while(1){
		if(menu==0&&hp<100){
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
		if(hp<=0||hygiene<=0||hunger<=0){
			menu=3;
			system("clear");
			printf("Master, whyyyyyy...  *dead*\n");
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
	pthread_create( &th[0], NULL, kbhit, NULL);
	pthread_create( &th[1], NULL, Standby, NULL);
	pthread_create( &th[2], NULL, battle, NULL);
//	pthread_create( &th[9], NULL, status_hungry, NULL);
	pthread_create( &th[3], NULL, status_hygiene, NULL);
	pthread_create( &th[4], NULL, status_regen, NULL);
	pthread_create( &th[5], NULL, status_hunger, NULL);
	pthread_create( &th[6], NULL, soap, NULL);
	pthread_create( &th[7], NULL, shop, NULL);
	pthread_create( &th[8], NULL, endgame, NULL);
	int i;
	for(i=0; i<=9;i++) pthread_join(th[0],NULL);
}

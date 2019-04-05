#include <stdio.h>
#include <string.h>
int main(){
	char buff[100];
	fgets(buff,100,stdin);
	buff[strcspn(buff,"\n")]='\0';
	if(strcmp(buff, "All Status")==0) printf("%s",buff);
}

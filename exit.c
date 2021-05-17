#include <stdio.h>
#include <stdlib.h>

int Exit();

int main(void){
	printf("Hello World");
}
	


int Exit(){
	int n;
	printf("1.C 2.E \n");
	scanf("%d", &n);
	if(n==1){
		printf("Continue\n");
	}
	else if(n==2){
		return 0;
	}
}

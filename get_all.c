#include <stdio.h>
#include <string.h>
int main(){
	unsigned char str[3];
	int i,j,k;
	for(i=127;i<=256;i++)
	{
		for(j=127;j<256;j++){
			str[0]=i,str[1]=j,str[2]=0;
			printf("\"%s\",",(char*)str);
			if(j%20==0) putchar('\n');
		}
	}
	return 0;
}

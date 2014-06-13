#include <string.h>
#include <stdio.h>
#define maxn 4096
char str[maxn];
int main(){
	int i,j,k;
	while(scanf("%s",str)!=EOF){
		for(i=0;str[i];i++){
			if(str[i]<0){
				printf("%c",str[i]);
			}
		}
	}
	return 0;
}

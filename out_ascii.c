#include <string.h>
#include <stdio.h>
int main(){
	unsigned char str[2];
	str[0]=str[1]=0;
	for(int i=256;i>-257;i--){
		str[0]=i;
		printf("%s\n",str);
	}
	return 0;
}

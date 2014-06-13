#include <iconv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
using namespace std;
//编码转换，从UTF8转换为GBK。如果原串不空，返回空，则为转换失败
string UTF8ToGBK(char src[])
{
	string ans;
	int len = strlen(src)*2+1;
	char *dst = (char *)malloc(len);
	if(dst == NULL)
	{
		return ans;
	}
	memset(dst, 0, len);
	char *in = src;
	char *out = dst;
	size_t len_in = strlen(src);
	size_t len_out = len;
	iconv_t cd = iconv_open("GBK", "UTF-8");
	if ((iconv_t)-1 == cd)
	{
		printf("init iconv_t failed\n");
		free(dst);
		return ans;
	}
	int n = iconv(cd, &in, &len_in, &out, &len_out);
	if(n<0)
	{
		printf("iconv failed\n");
	}
	else
	{
		ans = dst;
	}
	free(dst);
	iconv_close(cd);
	return ans;
}
//将str分隔成一个一个的字符，并判断是否是汉字，并输出编码，包括简体和繁体
void GetToken(const string &str)
{
	int len = str.size();
	short high, low;
	unsigned int code;
	string s;
	for(int i=0; i<len; i++)
	{
		if(str[i]>=0 || i==len-1)
		{
			printf("%c >> no\n", str[i]);   //ASCii字符
		}
		else
		{
			//计算编码
			high = (short)(str[i] + 256);
			low = (short)(str[i+1]+256);
			code = high*256 + low;
			//获取字符
			s = "";
			s += str[i];
			s += str[i+1];
			i++;
			printf("%s >> 0x%x", s.c_str(), code);
			if(code>=0xB0A1 && code<=0xF7FE || code>=0x8140 && code<=0xA0FE || code>=0xAA40 && code<=0xFEA0)
			{
				printf(" yes\n");
			}
			else
			{
				printf(" no\n");
			}
		}
	}
}
	int main(void)
	{
		//char src[] = "物畔尽啊座罪舆癣氧偷瞧張李萬事接應。？！《》【】、";
		char src[] = "abcsdfsdfsdf123253543566$#测试１２３４５６７８９０……——哇卡卡";
		string dst = UTF8ToGBK(src);
		if(dst != "")
		{
			printf("%s\n", dst.c_str());
			GetToken(dst);
		}
		return 0;
	}

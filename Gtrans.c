#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<error.h>

#include<curl/curl.h>
#include<curl/easy.h>

#include<iconv.h>


#define BUFFER 4096
#define bool int
#define false 0
#define true 1
char result[BUFFER];
char src[24]="en";
char dest[24]="zh_CN";
short  translate_flag=0;
char re_ans[100][1024];
int cmp(const void *a,const void *b){
	return strcmp((char*)a,(char*)b);
}
int get_content(char *html_content)
{
	out_ans(html_content);
	//printf("%s",html_content);
	char *ss_start = NULL;
	char *se_end = NULL;
	char *ss = "Color='#fff'\">";
	char *se = "</span>";
	ss_start = strstr(html_content, ss);
	if(ss_start != NULL&&translate_flag==0)
	{
		se_end = strstr(ss_start, se);
		ss_start += strlen(ss);
		strncat(result, ss_start, se_end - ss_start); 
		translate_flag++;
		return 0;
	}
	return -1;
}

size_t write_data(void *ptr,size_t size,size_t nmemb,void *stream)
{
	FILE *fp;
	int rs;
	get_content((char *)ptr);
	fp=fopen("/dev/null","w");
	rs=fwrite(ptr,size,nmemb,fp);
	fclose(fp);

	return rs;
}

int URLEncode(const char* str, const int strSize, char* result, const int resultSize) 
{
	int i;
	int j = 0;
	char ch;

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0)) {
		return 0;
	}

	for (i=0; (i<strSize) && (j<resultSize); i++) {
		ch = str[i];
		if ((ch >= 'A') && (ch <= 'Z')) {
			result[j++] = ch;
		} else if ((ch >= 'a') && (ch <= 'z')) {
			result[j++] = ch;
		} else if ((ch >= '0') && (ch <= '9')) {
			result[j++] = ch;
		} else if(ch == ' ' || ch == '\n' || ch == '\r'|| ch == '\t'){
			result[j++] = '+';
		} else {
			if (j + 3 < resultSize) {
				sprintf(result+j, "%%%02X", (unsigned char)ch);
				j += 3;
			} else {
				return 0;
			}
		}
	}

	result[j] = '\0';
	return j;
} 

int translate_engine(char *inputText)
{
	char urlstr[BUFFER];
	char langpair[256];
	char engineUrl[256];
	CURL *curl;

	memset(urlstr, 0, BUFFER);

	int inputTextLen = sizeof(char) * 3 * strlen(inputText);
	char *inputText_encoded = malloc(inputTextLen);

	sprintf(langpair, "%s|%s", src, dest);
	sprintf(engineUrl, "%s", "http://translate.google.cn");

	URLEncode(inputText, strlen(inputText), inputText_encoded, inputTextLen + 2);
	sprintf(urlstr, "%s?text=%s&langpair=%s", engineUrl, inputText_encoded, langpair);
	
	sprintf(urlstr,"http://translate.google.cn/translate_a/single?client=t&sl=auto&tl=zh-CN\
&hl=zh-CN&dt=bd&dt=ex&dt=ld&dt=md&dt=qc&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF\
-8&ssel=0&tsel=0&q=%s",inputText_encoded);
	//fprintf(stdout, "urlstr: %s\n", urlstr);

	curl_global_init (CURL_GLOBAL_ALL);
	curl = curl_easy_init ();
#ifdef _DEBUG
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#endif
	curl_easy_setopt (curl, CURLOPT_URL, urlstr);
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_data);	
	curl_easy_perform (curl);
	curl_easy_cleanup (curl);

	return 0;
}

bool GetToken(char *str)
{
	int len = strlen(str);
	short high, low;
	unsigned int code;
	char s[1024*10];
	int re_len=0;
	int i;
	for( i=0; i<len; i++)
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
			strcpy(s,"");
			s[re_len++]=str[i];
			s[re_len]=0;
			s[re_len++]=str[i+1];
			s[re_len]=0;
			i++;
			//printf("%s >> 0x%x", s.c_str(), code);
			if(code>=0xB0A1 && code<=0xF7FE || code>=0x8140 && code<=0xA0FE || code>=0xAA40 && code<=0xFEA0)
			{
				return true;
				//printf(" yes\n");
			}
			else
			{
				//printf(" no\n");
				return false;
			}
		}
	}
	return false;
}
char is_not[][10]={
"ǎ","ǐ","əˈ","ˈ","“","”","‘","’","“","”","…"
,"ō","冠词","副词","è","͟","名词","ò","动词"
,"ó","介词","ə","£","缩写词","�","ā","ä"
""
//	形容词 副词语 动词 
};
bool is_ok(char *str){
	int i=0;
	for(i=0;is_not[i][0]!=0;i++){
		if(strcmp(str,is_not[i])==0) 
			return false;
	}
	return true;
}

int out_ans(char *str){
	int i=0;
	int p=0,q=0;
	bool flag=true;
	for(i=0;str[i];i++){
		if(str[i]<0){
			re_ans[p][q++]=str[i];
			flag=true;
		}else{
			if(flag){
				re_ans[p][q]=0;
				p++;
			}
			q=0;
			flag=false;
		}
	}
	qsort(re_ans,p,sizeof(re_ans[0]),cmp);
	printf("%s\n",re_ans[0]);
	for(i=1;i<p;i++){
		if(strcmp(re_ans[i],re_ans[i-1])!=0 && is_ok(re_ans[i]))
		printf("%s\n",re_ans[i]);
	}
	return 0;
}
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}

int main(int argc,char *argv[])
{
	char *result_converted=malloc(BUFFER);
	switch (argc){
		case 2:  break;
		case 3:  
				 if(!strcmp(argv[1],"zh_CN"))
				 {
					 strcpy(dest,"en");
					 strcpy(src,argv[1]);
				 }
				 break;
    	default: 
                 break;
                 printf("help:  Gtrans [input language 'en'or'zh_CN'--default 'en'] ['string']\n");
				 return 0;
	}
    char bu[4096];
    memset(bu, 0, sizeof(bu));
    int i = 0;
    for(i = 1; i< argc; i++){
        strcat(bu, argv[i]);
        strcat(bu, " ");
    }
	translate_engine(bu);
	code_convert("gb2312","utf-8",result,strlen(result),result_converted,BUFFER);
	//printf("%s\n",result_converted);
	return 0;
}

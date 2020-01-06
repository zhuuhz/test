#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iconv.h"
#include <errno.h>

int code_convert(char* fromcode, char* tocode,
                 char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    iconv_t cd;
    char** pin = &inbuf;
    char** pout = &outbuf;

//    cd = iconv_open(to_charset, from_charset);
//    if ((int)cd == -1)
//    {
//        perror("iconv_open:");
//        return -1;
//    }
    cd = iconv_open(tocode, fromcode);
    if(cd == (iconv_t)(-1))
    {
        if(iconv_open("UCS-4", fromcode) == (iconv_t)(-1))
            printf(
                /* TRANSLATORS: An error message.
                   The placeholder expands to the encoding name, specified through --from-code.  */
                "conversion from %s unsupported\n",
                fromcode);
        else if(iconv_open(tocode, "UCS-4") == (iconv_t)(-1))
            printf(
                /* TRANSLATORS: An error message.
                   The placeholder expands to the encoding name, specified through --to-code.  */
                "conversion to %s unsupported\n",
                tocode);
        else
            printf(
                /* TRANSLATORS: An error message.
                   The placeholders expand to the encoding names, specified through --from-code and --to-code, respectively.  */
                "conversion from %s to %s unsupported\n",
                fromcode, tocode);
        printf(
            /* TRANSLATORS: Additional advice after an error message.
               The %s placeholder expands to the program name.  */
            "try '%s -l' to get the list of supported encodings\n",
            "iconv");
		
		perror("iconv_open:");
		
		return -1;
    }

    memset(outbuf, 0, outlen);
    if(iconv(cd, pin, &inlen, pout, &outlen) == -1)
    {
        //printf("%s: call iconv failed!\n", __FUNCTION__);
        printf("errno=%d\n", errno);
        perror("iconv failed:\n");
        iconv_close(cd);
        return -1;
    }
    iconv_close(cd);
    return 0;
}

int g2u(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    return code_convert("gbk", "utf-8", inbuf, inlen, outbuf, outlen);
}
int u2g(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    return code_convert("utf-8", "gbk", inbuf, inlen, outbuf, outlen);
}

#define ICONV_CMD	"/home/root/zms/myiconv/bin/iconv"
int u2g_ststem(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
	system(ICONV_CMD " ");
    return code_convert("utf-8", "gbk", inbuf, inlen, outbuf, outlen);
}


int main(void)
{
    char utfmsg[] = "hello 你好";
    char gbkmsg[1024] = {0};

    printf("before convert: %s\n", utfmsg);

    u2g(utfmsg, sizeof(utfmsg), gbkmsg, sizeof(gbkmsg));

    printf("after convert: %s\n", gbkmsg);

    return 0;
}

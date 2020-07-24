#include "src.h"

char* init_string(char* str, int size)
{
    str = (char*)malloc(sizeof(char) * size + 1);
    str[size] = '\0';
    return str;
}

char* concat_n(char* dest, char* src, int len)
{
 if(dest == NULL)
 {
   dest = (char*)malloc(sizeof(char) * len + 1);
   memset(dest, 0, len);
   strncat(dest, src, len);
 }
 else
 {
   dest = realloc(dest, sizeof(char) * len + 1);
   strncat(dest, src, len);
 }
 
 
 return dest;
}

int get_new_line_index(char* str)
{
 int len = strlen(str);
 for(int i = 0; i < len; i++)
 {
   if(str[i] == '\n')
   {
     return i;
   }
 }

 return -1;
}

char* cut_n(char* src, int start)
{
    int len = strlen(src) - start;
    char* str = (char*)malloc(sizeof(char) *  len + 1);
    str[len] = '\0';
    int j = 0;
    for(int i = start; src[i] != '\0';i++)
    {
        str[j] = src[i];
        j++;
    }
    return str;
} 


#include "src.h"
#define read_size 3

char* my_readline(int fd)
{
    if(fd == -1)
    {
      return NULL;
    }
   char* buffer = init_string(buffer, read_size);
   int bytes = 0;
   static char* main_buffer = NULL;
   int new_line_index = -1;
   
   while((bytes = read(fd,buffer,read_size)) > 0)
   {
     main_buffer = concat_n(main_buffer, buffer, bytes);
     new_line_index = get_new_line_index(main_buffer);
     if(new_line_index != -1)
     {
       break;
     }
   } 

  
  if(new_line_index == -1)
  {
    new_line_index = strlen(main_buffer);
  }

  if(strlen(main_buffer) == 0)
  {
    return NULL;
  }
  
  char* output = NULL;
  output = concat_n(output, main_buffer, new_line_index);
  int start = new_line_index + 1;
  main_buffer = cut_n(main_buffer, start);
   
  return output;
}


int main(void)
{
 test_0();
 test_1();
 test_2();
 test_3();
 test_4();
 test_5();
 
  int fd = open("file", O_RDONLY);
  printf("[%s]\n", my_readline(fd));

 
 return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define READLINE_READ_SIZE 1

typedef struct s_read_info
{
    int is_read;
    char* buffer;
    int buff_size;
    int new_line_index;
    int total_number_n;
} t_read_info;

int get_new_line_index(t_read_info* read_info);

void get_total_newLines(t_read_info* read_info)
{
    int n = 0;
    int len = strlen(read_info->buffer);
    for (int i = 0; i < len; i++)
    {
        if (read_info->buffer[i] == '\n')
        {
            n++;
        }
    }
    read_info->total_number_n = n;
}

void concat(t_read_info* read_info, char* buffer_value)
{
    if (read_info == NULL)
    {
       // printf("error: smth wron with pointer to structure\n");
        return;
    }

    read_info->buffer = (char*)realloc(
    read_info->buffer, sizeof(char) * (read_info->buff_size + 2));
    read_info->buffer[read_info->buff_size] = '\0';
    strcat(read_info->buffer, buffer_value);
}
char* get_to_return(t_read_info* read_info)
{
    int new_line = get_new_line_index(read_info);
    
    if(new_line == strlen(read_info->buffer))
    {
        return NULL;
    }
    else
    {
        char *str = (char*)malloc(sizeof(char) * new_line + 1);
        for(int i = 0;i<new_line;i++)
        {
            str[i] = read_info->buffer[i];
        }
        return str;
    }
}

void read_buffer(t_read_info* read_info, int fd)
{
    int bytes = 0;
    char tmp_reader[READLINE_READ_SIZE + 1];
    
    while ((bytes = read(fd, tmp_reader, sizeof(char) * READLINE_READ_SIZE)) > 0)
    {
        if(fd == 0)
        {
            if(get_to_return(read_info) != NULL)
            {
                break;
            }
        }
        read_info->buffer[read_info->buff_size] = '\0'; 
        read_info->buff_size += bytes + 1;
        tmp_reader[bytes] = '\0';
        concat(read_info, tmp_reader);
    }
}

void init_structure_values(t_read_info* read_info)
{
    read_info->new_line_index = 0;
    read_info->is_read = 1;
    read_info->buffer = (char*)malloc(sizeof(char) * (READLINE_READ_SIZE + 1));
    read_info->buff_size = 0;
}

int get_new_line_index(t_read_info* read_info)
{
    int len = strlen(read_info->buffer);
    for (int i = 0;i < len; i++)
    {
        if(read_info->buffer[i] == '\n')
        {
            return i;
        }
    }
    return strlen(read_info->buffer);
}

void set_output_value(char* str, t_read_info* read_info)
{
    int i;
    for (i = 0; i < read_info->new_line_index; i++)
    {
        str[i] = read_info->buffer[i];
    }
    str[i] = '\0';
}

void update_buffer_value(t_read_info* read_info)
{
    int j;
    int new_index = 0;
    char* new_buffer = (char*)malloc(
        sizeof(char) * (strlen(read_info->buffer) - read_info->new_line_index));
    int len = strlen(read_info->buffer);
    for (j = read_info->new_line_index + 1; j < len; j++)
    {
        new_buffer[new_index] = read_info->buffer[j];
        new_index += 1;
    }
    new_buffer[new_index] = '\0';
    strcpy(read_info->buffer, new_buffer);
    free(new_buffer);
}

char* take_line(t_read_info* read_info)
{   
    if (read_info == NULL)
    {   
        return NULL;
    }
    
    read_info->new_line_index = get_new_line_index(read_info);
    if (read_info->new_line_index == 0)
    {
        if (strlen(read_info->buffer) == 0)
        {
            //printf("\033[0;31mERROR:out of file data\n");
            return NULL;
        }
        else
        {
            update_buffer_value(read_info);
            char* empty_line = (char*)malloc(sizeof(char) * 2);
            strcpy(empty_line, "");
            return empty_line;
        }   
    }
    char* str = (char*)malloc(sizeof(char) * (read_info->new_line_index + 1));
    set_output_value(str, read_info);
    update_buffer_value(read_info);
    return str;
}
char* my_readline(int fd)
{
    if (fd == -1) // if smth goes wrong
    {
        return NULL; 
    }
    static t_read_info read_info; // init structure saving all values 
    char* buff;  // return value
    if (read_info.is_read != 1)  // if  read wasn;t used
    {
        init_structure_values(&read_info);
        read_buffer(&read_info, fd); // reads till the end     
    }
    buff = take_line(&read_info);
    if(buff == NULL)
    {
        read_info.is_read = 0;
    }
    return buff;
}
void test_0() {
  int fd;
  char* str;

  printf("<0>\n");
  system("echo 'line1' > file.txt");
    fd = open ("file.txt", O_RDONLY);

  while ((str = my_readline(fd)) != NULL) {
    printf("my_readline: [%s]\n", str);
  }
  close(fd);
  printf("Output should be line1\n");
  printf("</0>\n");
}

void test_1() {
  int fd;
  char* str;

  printf("<1>\n");
  system("echo 'line1' > file.txt");
  system("echo 'line2' >> file.txt");
  system("echo 'line3' >> file.txt");
  system("echo 'line4' >> file.txt");
  system("echo 'line5' >> file.txt");
  system("echo 'line6' >> file.txt");
  system("echo 'line7' >> file.txt");

  fd = open ("file.txt", O_RDONLY);
  while ((str = my_readline(fd)) != NULL) {
    printf("my_readline: [%s]\n", str);
  }
  close(fd);
  printf("Output should be line1 -> line7\n");
  printf("</1>\n");
}

void test_2() {
  char* str;

  printf("<2>\n");
  while ((str = my_readline(-1)) != NULL) {
    printf("my_readline: [%s]\n", str);
  }
  printf("Output should be nothing\n");
  printf("</2>\n");
}

void test_3() {
  char* str;

  printf("<3>\n");
  while ((str = my_readline(239)) != NULL) {
    printf("my_readline: [%s]\n", str);
  }
  printf("Output should be nothing\n");
  printf("</3>\n");
}

void test_4() {
  int fd;
  char* str;

  printf("<4>\n");
  system("echo 'line1' > file.txt");
  system("echo 'line2' >> file.txt");
  system("echo 'line3' >> file.txt");
  system("echo 'line4' >> file.txt");
  system("echo 'line5' >> file.txt");
  system("echo 'line6' >> file.txt");
  system("echo 'line7' >> file.txt");
  system("echo '' >> file.txt");
  system("echo '' >> file.txt");
  system("echo '' >> file.txt");
  system("echo '' >> file.txt");
  system("echo '' >> file.txt");
    fd = open ("file.txt", O_RDONLY);

  while ((str = my_readline(fd)) != NULL) {
    printf("my_readline: [%s]\n", str);
  }
  close(fd);
  printf("Output should be line1 -> line7 and 5 empty lines\n");
  printf("</4>\n");
}

void test_5() {
  char* str;

  printf("<5>\n");
  printf("enter string: \n");
  str = my_readline(0);
  printf("my_readline: [%s]\n", str);
  printf("Output should be the entered string\n");
  printf("</5>\n");
}


int main (int argc, char ** argv)
{

    test_5();
  return 0;
}

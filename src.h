#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

char* init_string(char* str, int size);
char* concat_n(char* dest, char* src, int len);
int get_new_line_index(char* str);
char* cut_n(char* src, int start);
char* my_readline(int fd);
// tests
void test_0();
void test_1();
void test_2();
void test_3();
void test_4();
void test_5();

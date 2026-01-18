#include<stdio.h>
#include<stdlib.h>
int main()
{
    #ifdef WIN32
    system("dir");
    #else 
    system("ls");
    #endif

    return 0;
}
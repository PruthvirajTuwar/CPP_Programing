#include<stdio.h>
#include<stdlib.h>
int main()
{
    #ifdef WIN32
    system("clear");
    #else 
     system("cls");
     #endif

    return 0;
}
#include<stdio.h>
int main()
{

    char str [80];
    char Command[4][20];

    int iRet =0;
    int No1 = 0, No2 = 0;
    iRet = printf("Enter 2 numbers : \n");

    iRet = scanf("%d%d",&No1,&No2);

    printf("First Number%d\n",No1); //
    printf("second Number %d\n",No2);

    printf("Return value of scanf is : %d\n",iRet);

    return 0;
}
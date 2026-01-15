/////////////////////////////////////////////////////////////////////////////////////////
//  
//          Header File Inclusion
//
/////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

/////////////////////////////////////////////////////////////////////////////////////////
//  
//          User Defined Macros
//
/////////////////////////////////////////////////////////////////////////////////////////


// Maximum file Size  that we allow in the project 

#define MAXFILESIZE 50          // 50 bytes size

#define MAXOPENFILE 20        // Maximum files that can be opened at a time

#define MAXINODE 5          // Maximum number of Inodes

#define READ 1            // Read Permission
#define WRITE 2          // Write Permission
#define EXECUTE 4       // Execute Permission

#define START 0       // Offset Starting Point
#define CURRENT 1    // Offset Current Position
#define END 2       // Offset End Position

#define EXECUTE_SUCCESS 0

/////////////////////////////////////////////////////////////////////////////////////////
////  
///        User Defined Structures
//
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
/////  
////     Structure Name : BootBlock
///      Description    : Holds the information to boot the os
//
/////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];   // Information about the file system      
}; 

/////////////////////////////////////////////////////////////////////////////////////////
/////  
////    Structure Name : SuperBlock
///     Description    : Holds the information about the file System
//
/////////////////////////////////////////////////////////////////////////////////////////
struct SuperBlock
{
    int TotalInodes;          // Total number of Inodes
    int FreeInodes;          // Total number of Free Inodes
};
/////////////////////////////////////////////////////////////////////////////////////////
/////
////   Structure Name : Inode
///    Description    : Holds the information about the file
//
/////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct Inode
{
    char FileName[20];             // Name of the file 
    int InodeNumber;              // Inode Number
    int FileSize;                // Size of the file
    int ActualFileSize;         //Auctual file Size
    int ReferenceCount;        // Number of references 
    int Permission;           // File Permission
    char *Buffer;            // Pointer to the buffer
    struct Inode *next;     // Pointer to the next Inode

};
typedef struct Inode INODE;       // Definition of Inode
typedef struct Inode* PINODE;    // Pointer to Inode
typedef struct Inode** PPINODE; // Pointer to Pointer of Inode

/////////////////////////////////////////////////////////////////////////////////////////
/////
////   Structure Name : File Table
///    Description    : Holds the information about Opened  file
//
/////////////////////////////////////////////////////////////////////////////////////////

struct Filetaable
{
    int ReadOffset;             // Read Offset
    int WriteOffset;           // Write Offset
    int Mode;                 // Mode in which file is opened
    PINODE ptrinode;         // Pointer to the Inode
}; 

typedef struct Filetaable FILETABLE;
typedef struct Filetaable* PFILETABLE;

/////////////////////////////////////////////////////////////////////////////////////////
////   Structure Name : UAREA
///    Description    : Holds the information about Process file
//
/////////////////////////////////////////////////////////////////////////////////////////

struct UAREA                        //Process  UFDT (User File Descriptor Table)
{
   char ProcessName[20];          // Name of the process
   PFILETABLE UFDT[MAXOPENFILE]; // Pointer to file table[array of pointers to FILETABLE]
};


/////////////////////////////////////////////////////////////////////////////////////////
/////
////   
///     Global Variables or objects used in the project
//
///////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;       // Global object of BootBlock
SuperBlock superobj;    // Global object of SuperBlock
UAREA uareaobj;        // Global object of UAREA

PINODE head = NULL;  // Global head pointer to maintain the linked list of Inodes



///////////////////////////////////////////////////////////////////////////////////////////////
//////                                                                                    
/////   Function Name : InitializeUAREA                                                   
////    Description   : it is used to initialize UAREA member                            
///     Author        : Pruthviraj Kashinath Tuwar                                            
//      Date          : 12/01/2026                                                     
////////////////////////////////////////////////////////////////////////////////////////////

void InitializeUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe"); // Copy process name

    int i =0; //Loop counter

    for(i=0; i< MAXOPENFILE; i++)    
    {
        uareaobj.UFDT[i] = NULL;  // Initialize all file table pointers to NULL
    }
    printf("Marvellous CVFS : UAREA gets initialized successfully\n");
}

///////////////////////////////////////////////////////////////////////////////////////////
//////
/////     Function Name : InitializeSuperBlock
////      Description   : It is used to  Initializes the Super Block Member
///       Author        : Pruthviraj Kashinath Tuwar
//        Date          : 12/01/2026 
/////////////////////////////////////////////////////////////////////////////////////////

void InitializeSuperBlock()
{
    superobj.TotalInodes = MAXINODE;  // Total Inodes
    superobj.FreeInodes = MAXINODE;  // Free Inodes

    printf("Marvellous CVFS : Super Block gets initialized successfully\n");
}


///////////////////////////////////////////////////////////////////////////////////////////
//////
/////   Function Name : CreateDILB
////    Description   : It is used  to create the Linked list of Inodes
///     Author        : Pruthviraj Kashinath Tuwar
//      Date          : 12/01/2026 
/////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i=1; i<= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE)); // Allocate the memory for inode
        
        strcpy( newn->FileName,"\0");      // Initialize file name to null    
        newn->InodeNumber = i;            // Set the inode number
        newn->FileSize = 0;              // Initialize file size to 0
        newn->ActualFileSize = 0;       // Initialize actual file size to 0
        newn->ReferenceCount = 0;      // Initialize reference count to 0
        newn->Permission = 0;         // Initialize permission to 0
        newn->Buffer = NULL;         // Initialize buffer pointer to NULL
        newn->next = NULL;          // Initialize next pointer to NULL

        if(temp == NULL)          // If linked list is empty
        {
            head = newn;        // Set head to new node
            temp = head;       // Set temp to head
        }
        else                      //LL contains atleast one node
        {
            temp->next = newn;  // Link the new node to the list
            temp = temp->next; // Move temp to the new node

        }


    }
    printf("Marvellous CVFS : DILB created successfully\n");   
}


///////////////////////////////////////////////////////////////////////////////////////////
//////
/////   Function Name : StartAuxillaryDataInitialization
////    Description   : It is used to call all such functions which are used to initialize
///                     the auxillary data 
///     Author        : Pruthviraj Kashinath Tuwar
//      Date          : 12/01/2026 
///////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialization()
{
    strcpy(bootobj.Information,"Booting process of Marvellous CVFS is done"); // Boot information
    printf("%s\n",bootobj.Information);

    InitializeSuperBlock(); // Initialize Super Block
    CreateDILB();          // Create DILB
    InitializeUAREA();    // Initialize UAREA   

    printf("Marvellous CVFS : Auxillary Data Initialization is completed successfully\n");
}
///////////////////////////////////////////////////////////////////////////////////////////
////
///         Entry Point fuction of the project
//
/////////////////////////////////////////////////////////////////////////////////////////

int main()
{

    char str[80]={"\0"};
    char command[4][20];
    int iCount =0;

    StartAuxillaryDataInitialization();

    printf("--------------------------------------------------------\n");
    printf("----------marvellous CVFS started successfully----------\n");
    printf("--------------------------------------------------------\n");


    while(1)// Infinite Leasenig  loop
    {
        fflush(stdin);     // Clear the input buffer (Keyboard)

        strcpy(str,"\0"); // Clear the str variable


        printf("\nMarvellous CVFS : > "); // Display prompt
        fgets(str,sizeof(str),stdin);    // Accept the user input 
        // ^ scanf("%s",str);

       


    }
    


    return 0;
}
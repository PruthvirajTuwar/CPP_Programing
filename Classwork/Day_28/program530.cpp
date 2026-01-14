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
//  
//          User Defined Structures
//
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//  
//     Structure Name : BootBlock
//     Description    : Holds the information to boot the os
//
/////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];   // Information about the file system      
}; 

/////////////////////////////////////////////////////////////////////////////////////////
//  
//     Structure Name : SuperBlock
//     Description    : Holds the information about the file System
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
    int ActualFileSize;
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
    PINODE ptrinode;          // Pointer to the Inode
}; 

typedef struct Filetaable FILETABLE;
typedef struct Filetaable* PFILETABLE;

/////////////////////////////////////////////////////////////////////////////////////////
////   Structure Name : UAREA
///    Description    : Holds the information about Process file
//
/////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
   char ProcessName[20];        // Name of the process
   PFILETABLE UFDT[MAXOPENFILE]; // Pointer to file table
};


/////////////////////////////////////////////////////////////////////////////////////////
/////
////   
///     Global Variables or objects used in the project
//
///////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;       // Global object of BootBlock
SuperBlock superobj;    // Global object of SuperBlock
UAREA uareaobj;         // Global object of UAREA

PINODE head = NULL; // Global head pointer to maintain the linked list of Inodes


















///////////////////////////////////////////////////////////////////////////////////////////
///         Entry Point fuction of the project
//
/////////////////////////////////////////////////////////////////////////////////////////

int main()
{


    return 0;
}
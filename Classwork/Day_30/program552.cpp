/////////////////////////////////////////////////////////////////////////////////////////
//// 
///          Header File Inclusion
//
/////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

/////////////////////////////////////////////////////////////////////////////////////////
//// 
///          User Defined Macros
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

#define REGULARFILE 1
#define SPECIALFILE 2

/////////////////////////////////////////////////////////////////////////////////////////
//// 
///          User Defined Macros for Error handling
//
/////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

/////////////////////////////////////////////////////////////////////////////////////////
///////////
//////////                   User Defined Structures
/////////                           ____
////////                            ||||
///////                          _  ||||  _
//////                           \\ |||| //
/////                             \\||||//
////                               \\ ///
///                                 \\//
//                                   \/
////////////////////////////////////////////////////////////////////////////////////////

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
    int FileType;               //
    int ActualFileSize;        //Auctual file Size
    int ReferenceCount;       // Number of references 
    int Permission;          // File Permission
    char *Buffer;           // Pointer to the buffer
    struct Inode *next;    // Pointer to the next Inode

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

struct FileTable
{
    int ReadOffset;             // Read Offset
    int WriteOffset;           // Write Offset
    int Mode;                 // Mode in which file is opened
    PINODE ptrinode;         // Pointer to the Inode
}; 

typedef struct FileTable FILETABLE;
typedef struct FileTable* PFILETABLE;

/////////////////////////////////////////////////////////////////////////////////////////
/////
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
////   
///     Global Variables or objects used in the project
//
///////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;       // Global object of BootBlock
SuperBlock superobj;    // Global object of SuperBlock
UAREA uareaobj;        // Global object of UAREA

PINODE head = NULL;  // Global head pointer to maintain the linked list of Inodes



///////////////////////////////////////////////////////////////////////////////////////////////
///////                                                                                    
//////   Function Name : InitializeUAREA                                                   
/////    Description   : it is used to initialize UAREA member                            
////     Author        : Pruthviraj Kashinath Tuwar                                            
///      Date          : 12/01/2026
//                                                     
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
///////
//////     Function Name : InitializeSuperBlock
/////      Description   : It is used to  Initializes the Super Block Member
////       Author        : Pruthviraj Kashinath Tuwar
///        Date          : 12/01/2026 
//
/////////////////////////////////////////////////////////////////////////////////////////

void InitializeSuperBlock()
{
    superobj.TotalInodes = MAXINODE;  // Total Inodes
    superobj.FreeInodes = MAXINODE;  // Free Inodes

    printf("Marvellous CVFS : Super Block gets initialized successfully\n");
}


///////////////////////////////////////////////////////////////////////////////////////////
///////
//////   Function Name : CreateDILB
/////    Description   : It is used  to create the Linked list of Inodes
////     Author        : Pruthviraj Kashinath Tuwar
///      Date          : 13/01/2026 
//
/////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB() 
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i=1; i<= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE)); // Allocate the memory for inode
        
        strcpy( newn->FileName,"\0");       // Initialize file name to null    
        newn->InodeNumber = i;             // Set the inode number
        newn->FileSize = 0;               // Initialize file size to 0
        newn->ActualFileSize = 0;        // Initialize actual file size to 0
        newn->FileType = 0 ;            //
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
///////
//////  Function Name : StartAuxillaryDataInitialization
/////   Description   : It is used to call all such functions which are used to 
////                    initialize the auxillary data 
///     Author        : Pruthviraj Kashinath Tuwar
//      Date          : 13/01/2026 
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
///////
//////   Function Name : DisplayHelp
/////    Description   : It is used to display the help page                    
////     Author        : Pruthviraj Kashinath Tuwar
///      Date          : 14/01/2026 
//
///////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    
    printf("--------------------------------------------------------\n");
    printf("--------------marvellous CVFS Help Page-----------------\n");
    printf("--------------------------------------------------------\n");

    printf("man    : it is use to Display manual page\n");
    printf("clear  : it is use to clear the terminal\n");
    printf("creat  : it is use to create new file\n");
    printf("write  : it is use to write the data into file \n");
    printf("read   : it is use to read the data from the file \n");
    printf("stat   : it is use to statistical information\n");
    printf("unlink : it is use to delete the file\n");
    printf("exit   : it is use to terminate Marvellous CVFD \n");

    printf("----------------------------------------------------------\n");


}

///////////////////////////////////////////////////////////////////////////////////////////
///////
//////   Function Name : ManpageDisplay
/////    Description   : It is used to display man page                     
////     Author        : Pruthviraj Kashinath Tuwar
///      Date          : 14/01/2026 
//
///////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("ls",Name)==0)
    {
        printf("About: it is use to list the name \n");
        printf("uses : ls\n");
    }
    else if(strcmp("man",Name) == 0)
    {
        printf("About: it is use to Display manual page \n");
        printf("uses : man Commanad_Name \n");
        printf("Command_Name : It id the Name of Command \n");

    }
    else if(strcmp("exit",Name) == 0)
    {
        printf("About: it is use to terminate the shell \n");
        printf("uses : ls\n");
    }
    else if(strcmp("clear",Name)==0)
    {
        printf("About: it is use to terminate is clear \n");
        printf("uses : clear\n");
    }
    else{
        printf("No Manual entry for ");
    }
    
}

///////////////////////////////////////////////////////////////////////////////////////////
///////
//////   Function Name : IsFileExist
/////    Description   : It is check whether file is already exist or not
////     Input: It Accepts file nam
////     Out put:           true or false         
////     Author        : Pruthviraj Kashinath Tuwar
///      Date          : 16/01/2026 
//
///////////////////////////////////////////////////////////////////////////////////////////
bool IsFileExist(
                    char *name // file name

                )
{
    PINODE  temp = head;
    bool bFlag = false;

    while (temp != NULL) 
    {
        if((strcmp(name,temp->FileName) == 0 ) && (temp-> FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    return bFlag;
    
}

///////////////////////////////////////////////////////////////////////////////////////////
////////
///////   Function Name : Createfile
//////    Description   : It is used to Create new regular file
/////     Input         : It Accepts file name and permission
////      Output        : It return the file Descripion               
////      Author        : Pruthviraj Kashinath Tuwar
///       Date          : 16/01/2026 
//
///////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                char *name,     //Name of new file
                int permission //Permission for that file
                )
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n",superobj.FreeInodes);

    if (name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    //IF THE PERMISSION value is wrong
    /// 1-> read
    //  2-> write
    //  3->read+ write
    
    if(permission < 1|| permission>3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if  
    if(superobj.FreeInodes ==0)
    {
        return ERR_NO_INODES;
    }
    if (IsFileExist(name)== true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }
    // search empty Inode
    while (( temp != NULL))
    {
        if(temp -> FileType == 0)
        {
            break;
        }
        temp = temp ->next;
    }
    if (temp == NULL)
    {
        printf("there is no Inode");
        return ERR_NO_INODES;
    }
    // search for empty UFDT enty
    // NOTE : 0,1,2,3 are reserved
    for(i= 3;i<MAXOPENFILE ;i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }

    } 
    //UFDT is full
    if(i == MAXOPENFILE)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));
    // Initialise File table 
    uareaobj.UFDT[i]->ReadOffset =0;
    uareaobj.UFDT[i]->WriteOffset =0;
    uareaobj.UFDT[i]->Mode = permission;

    // Conect File table with Inode

    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize =0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->Permission =permission;

    // Allocated emomory for file data
    uareaobj.UFDT[i]->ptrinode->Buffer= (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i; // File descriptor
 
}

///////////////////////////////////////////////////////////////////////////////////////////
////////
///////   Function Name : LsFile
//////    Description   : It is used to all files
/////     Input         : Nothing
////      Output        : Nothing              
////      Author        : Pruthviraj Kashinath Tuwar
///       Date          : 16/01/2026 
//
///////////////////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("--------------------------------------------------------\n");
    printf("------------marvellous CVFS File Information------------\n");
    printf("--------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0){
        printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        temp = temp -> next;

    }
    printf("--------------------------------------------------------\n");

}
///////////////////////////////////////////////////////////////////////////////////////////
////
///         Entry Point fuction of the project
//
/////////////////////////////////////////////////////////////////////////////////////////

int main()
{

    char str[80]={"\0"};
    char Command[4][20]={{'\0'}}; // Garbage
    int iCount = 0;
    int iRet  =0; // return value 

    StartAuxillaryDataInitialization();

    printf("--------------------------------------------------------\n");
    printf("----------marvellous CVFS started successfully----------\n");
    printf("--------------------------------------------------------\n");


    // Infinite Leasenig shell
    while(1)
    {
        fflush(stdin);   // Clear the input buffer (Keyboard)

        strcpy(str,""); // Clear the str variable


        printf("\nMarvellous CVFS : > "); // Display prompt
        fgets(str,sizeof(str),stdin);    // Accept the user input 
        // ^ scanf("%s",str);


        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3],Command[4]);

        

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp("exit",Command[0])==0)
            {
               printf ("Thank you for using Marvllous CVFS\n");
               printf("Deallocating All The Allocated \n");

               break;
            }
            //  Marvellous CVFS : >ls
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();

            }
            // Marvellous CVFS : >  Marvellous CVFS : >
            else if(strcmp("help", Command[0]) ==0)
            {
                DisplayHelp();

            }
            // Marvellous CVFS : > clear
            else if (strcmp("clear",Command[0])==0)
            {
                #ifdef _WIN32
                    system("clear");
                #else 
                    system("cls");
              #endif
            }
        }
        // Marvellous CVFS : >man ls
       else if (iCount== 2)
       {
            if(strcmp("man",Command[0])==0)
            {
                ManPageDisplay(Command[1]);
            }
       }
       //  Marvellous CVFS : > creat Ganesh.txt3
       else if(iCount == 3)
       {    
            //
             if(strcmp("creat",Command[0])==0)
            {
              iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Errror : Unable to crete the file as perameters are invalide \n");
                    printf("please refer man page\n");

                }
                if(iRet== ERR_NO_INODES)
                {
                    printf("Error : unable to create file as ther is no inode \n");
                }
                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : unable to create file because the File already present");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to Create file\n");
                    printf("Max opened File limit reached\n");
                }

                printf("File gets Succesfully created with FD %d\n");
            }
       }
       else if(iCount ==4)
       {

       }
       else
       {
        printf("Command not Found \n");
        printf("plese  refer help option to get more information");
       }

    }
    


    return 0;
}// End of main
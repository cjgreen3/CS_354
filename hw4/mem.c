/******************************************************************************
 * FILENAME: mem.c
 * AUTHOR:   cherin@cs.wisc.edu <Cherin Joseph>
 * DATE:     20 Nov 2013
 * EDITED: for CS354 UW Madison Spring 2021 - Michael Doescher
 * PROVIDES: Contains a set of library functions for memory allocation
 * *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

// fitting policy
enum POLICY policy;

/* The BLOCK_HEADER structure serves as the header for each block.
 * 
 * The heaaders we are using for this project are similar to those described 
 * in the book for the implicit free list in section 9.9.6, and in lecture
 * In the implicit free packs both the size and allocated bit in one int.
 * The size in the implicit free list includes the size of the header.
 *
 * In this project we're going to use a struct that tracks additional 
 * information in the block header.
 * 
 * The first piece of information is a 'packed_pointer' that combines the 
 * absolute location (a memory address) of the next header and the alloc bit
 * The headers must begin on an address divisible by 4. This means the last 
 * two bits must be 0.  We use the least significant bit (LSB) to indicate 
 * if the block is free: LSB = 0; or allocated LSB = 1.
 *
 * The value stored in the size variable is either the size requested by 
 * the user for allocated blocks, or the available payload size (not including 
 * the size of the header) for free blocks.
 * This will allow us to calculate the memory utilization.  Memory utilization is
 * the requested_size / (padding + header_size).  
 * The provided function Mem_Dump takes care of this calculation for us.
 *
 * The end of the list (the last header) has the packed_pointer  set to NULL,
 * and the size set to 0.
 */

typedef struct BLOCK_HEADER {
  void *packed_pointer; // address of the next header + alloc bit.
  unsigned size;
} BLOCK_HEADER;

BLOCK_HEADER *first_header; // this global variable is a pointer to the first header

// *********************************************************************************
// *********************************************************************************
// *********************************************************************************

/*  We recommend you write some helper functions to unpack the headers 
 *  and retrieve specific pieces of data. I wrote functions named:
 *  
 *  1) Is_Allocated // return 1 if allocated 0 if not
 *  2) Is_Free      // return 1 if free 0 if not
 *  3) Get_Next_Header // unpacks the header and returns a pointer to the  
 *  the next header, NULL is this is the last BLOCK_HEADER
 *  4) Get_Size 
 *  5) Get_User_Pointer // the pointer that the user can write data to
 *  6) Get_Header_From_User_Pointer // the pointer that the user writes data to - used in Mem_Free
 *  7) Set_Next_Pointer
 *  8) Set_Allocated // set the allocated bit to 1
 *  9) Set_Free // set the allocated bit to 0
 *  10) Set_Size 
 */

// *********************************************************************************
// *********************************************************************************
// *********************************************************************************

/* Function used to Initialize the memory allocator */
/* Do not change this function */
/* Written by Cherin Joseph modified by Michael Doescher */
/* Not intended to be called more than once by a program */
/* Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated 
   	      policy: indicates the policy to use eg: best fit is 0*/
/* Returns 0 on success and -1 on failure */
/* Notes we're using mmap here instead of sbrk as in the book to take advantage of caching
 * as described in the OS lectures
 *
 * Study the end of the function where the headers are initialized for hints!
 */
int Mem_Init(int sizeOfRegion, enum POLICY policy_input)
{   
    policy = policy_input;
    int pagesize;
    int padsize;
    int fd;
    int alloc_size;
    void* space_ptr;
    static int allocated_once = 0;
    if(0 != allocated_once) {
      fprintf(stderr,"Error:mem.c: Mem_Init has allocated space during a previous call\n");
      return -1;}
    if(sizeOfRegion <= 0) {
        fprintf(stderr,"Error:mem.c: Requested block size is not positive\n");
        return -1;}
  
    /* Calculate padsize as the padding required to round up sizeOfRegion to a multiple of pagesize */
    pagesize = getpagesize(); //  Get the pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;
    alloc_size = sizeOfRegion + padsize;
  
    /* Using mmap to allocate memory */
    fd = open("/dev/zero", O_RDWR);
    if(-1 == fd) {
      fprintf(stderr,"Error:mem.c: Cannot open /dev/zero\n");
      return -1;}
    space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == space_ptr) {
      fprintf(stderr,"Error:mem.c: mmap cannot allocate space\n");
      allocated_once = 0;
      return -1;}
    
    allocated_once = 1;
    
    // To begin with, there is only one big, free block.
    // Initialize the first header */
    first_header = (BLOCK_HEADER*)space_ptr;
    // free size
    // Remember that the 'size' stored for free blocks excludes the space for the headers
    first_header->size = (unsigned)alloc_size - 2*sizeof(BLOCK_HEADER);
    // address of last header
    first_header->packed_pointer = (void *)first_header + alloc_size - sizeof(BLOCK_HEADER);

    // initialize last header
    // packed_pointers are void pointer, the headers are not
    BLOCK_HEADER *last_header = (BLOCK_HEADER *)first_header->packed_pointer;
    last_header->size = 0;
    last_header->packed_pointer = NULL;
    return 0;
}


// *********************************************************************************
// *********************************************************************************
// *********************************************************************************

/* Function for allocating 'size' bytes. */
/* Return the user writeable address of allocated block 
 * - this is the first byte of the payload, not the address of the header */
/* Return NULL on failure */

/* Here is what this function should accomplish */
/* - Check for sanity of size - Return NULL when appropriate - at least 1 byte*/
/* - Traverse the list of blocks and locate a free block which can accommodate 
 * the requested size based on the policy (e.g. first fit, best fit) */
/* - The next header must be aligned with an address divisible by 4. 
 *     - Add padding to accomodate this requirement. */
/* - When allocating a block - split it into two blocks when possible 
 *     - the allocated block should go first and the free block second
 *     - the free block must have a minimum payload size of 4 bytes 
 *     - do not split if the mininmum payload size can not be reserved */

/* Tips: Be careful with pointer arithmetic */
int Is_Allocated(BLOCK_HEADER *current){
  if ((unsigned)current->packed_pointer & 1) { // allocated block
    return 1; 
  }
  return 0;
}
int Is_Free(BLOCK_HEADER *current){
  if ((unsigned)current->packed_pointer & 1) { // allocated block
    return 0;
  }
  return 1;
}
BLOCK_HEADER* Get_Next_Header(BLOCK_HEADER *current){
  return (BLOCK_HEADER *)current->packed_pointer;
}

int Get_Size(BLOCK_HEADER* current){
  return current->size;
}

//[block header | user space |block header | user space | block headeri]
void * Get_User_Pointer(BLOCK_HEADER *current){
  return (void*)current + sizeof(BLOCK_HEADER);
}

//go back to this function!!
BLOCK_HEADER* Get_Header_From_User_Pointer(void* user_space){

  return user_space - sizeof(BLOCK_HEADER);
}

//void Set_Next_Pointer(BLOCK_HEADER *current, BLOCK_HEADER *first__header ){
//    current->packed_pointer = first_header->packed_pointer;
//}

void Set_Next_Pointer(BLOCK_HEADER *current, void *bh_loc ){
    current->packed_pointer = bh_loc;
}
void Set_Allocated(BLOCK_HEADER* bh){
  bh->packed_pointer = (void*)((unsigned int)bh->packed_pointer | 1);

  return;
}
void Set_Free(BLOCK_HEADER* bh){
  bh->packed_pointer =(void*)((unsigned int) bh->packed_pointer &0xfffffffe);
  return;
}
void Set_Size(BLOCK_HEADER *curr, unsigned  size){
  curr->size = size; 
  return;
}

//[block header 80|free| block header ]
//[block header 2 |user space |new blockheader 78|free| block header ]
void* Mem_Alloc(int size) {
  /* Your code should go in here */
  BLOCK_HEADER *bh;//curr


  if(size == 0){
    return NULL;
  }


  bh = first_header;
  //void* end = first_header->packed_pointer;
  int free_space;
  int padding = 0;

  if( size % 4 != 0){
    padding = 4 - (size % 4);
  }
  while(bh->packed_pointer != NULL){//curr


    if(Is_Free(bh) == 1 && size <= Get_Size(bh) ){
      //check for padding and add if nesscessary

      //TODO check if there's any space to  add block header
      if(bh->size - size<= sizeof(BLOCK_HEADER)){
        Set_Size(bh,size);
        Set_Allocated(bh);
        return (void*)bh+sizeof(BLOCK_HEADER);
      }else{
        free_space =( Get_Size(bh) - size-sizeof(BLOCK_HEADER));//reserve free space of size  bh - size allocated
        BLOCK_HEADER* new_bh = (BLOCK_HEADER*)((void*)bh +(size+padding+sizeof(BLOCK_HEADER)));//make new bh  
        Set_Size(new_bh, free_space);//set the size of new bh to free space 
       //Set_Allocated(bh->packed_pointer);//set bh to be allocated

       //new_bh->packed_pointer =(void*)new_bh + sizeof(BLOCK_HEADER) + free_space;

        new_bh->packed_pointer = bh->packed_pointer;
        bh->packed_pointer = (void*) new_bh+1;
        Set_Size(new_bh, bh->size - size-padding- sizeof(BLOCK_HEADER));
        Set_Size(bh, size);

        return (void*)bh + sizeof(BLOCK_HEADER);
      }
    }

    bh = (BLOCK_HEADER*)( (unsigned int)Get_Next_Header(bh)& 0xFFFFFFFe) ;

   }

   return NULL;
}

// *********************************************************************************
// *********************************************************************************
// *********************************************************************************

/* Function for freeing up a previously allocated block */
/* Argument - ptr: Address of the block to be freed up i
 *     - this is the first address of the payload */
/* Returns 0 on success */
/* Returns -1 on failure */
/* Here is what this function should accomplish */
/* - Return -1 if ptr is NULL */
/*  Return -1 if ptr is not pointing to the first byte of an allocated block
 *     - hint: check all block headers, determine if the alloc bit is set */
/* - Mark the block as free */
void merge(){
    BLOCK_HEADER* curr = first_header;
    //BLOCK_HEADER* next =(void*) Get_Next_Header(curr);
    BLOCK_HEADER* next  = (BLOCK_HEADER*)( (unsigned int)Get_Next_Header(curr)& 0xFFFFFFFe);

    while(next->packed_pointer != NULL){
      if(Is_Free(curr) && Is_Free(next)){
      //update size
        Set_Size(curr, Get_Size(curr)+Get_Size(next)+ sizeof(BLOCK_HEADER));
        Set_Next_Pointer(curr, (void*) next->packed_pointer); 
      //set allocated
      //merge blocks and set next pointer
        
         if(Is_Allocated(next)==1){
             next =(void*)Get_Next_Header(next)-1;
         }
         else {
            next = Get_Next_Header(next);
         }
      }
      else{

      if(Is_Free(curr) == 1){
      curr = Get_Next_Header(curr);
      }
      else {
       curr=(void*) Get_Next_Header(curr)-1;
      }
      if(Is_Allocated(next)==1){
       next =(void*) Get_Next_Header(next)-1;
      }
      else {
        next = Get_Next_Header(next);
      }
      }
    }
}


//[bh1|payload|bh2|payload|bh3]
//[bh1|payload|bh3]
int Mem_Free(void *ptr)
{

    /* Your code should go in here */
    if(ptr == NULL ){
      return -1;
    }
    BLOCK_HEADER*iter = first_header;
    while(iter->packed_pointer !=NULL){
     
      if( ptr == Get_User_Pointer(iter)){
        break;
      }
      iter = (BLOCK_HEADER*)( (unsigned int)Get_Next_Header(iter)& 0xFFFFFFFe);
      if(iter->packed_pointer == NULL){
        return -1;
      }
    }
    BLOCK_HEADER* curr = Get_Header_From_User_Pointer(ptr);
    
    if(Is_Free(curr) == 1){
      return -1;
    }
    Set_Free(curr);
    //update the size and make allocated
        
    merge();

    return 0;
}

// *********************************************************************************
// *********************************************************************************
// *********************************************************************************

/* Function to be used for debugging */
/* Prints out a list of all the blocks along with the following information for each block */
/* No.      : Serial number of the block */
/* Status   : free/busy */
/* Begin    : Address of the first user allocated byte - i.e. start of the payload */ 
/* End      : Address of the last byte in the block (payload or padding) */
/* Payload  : Payload size of the block - the size requested by the user or free size */
/* Padding  : Padding size of the block */
/* T_Size   : Total size of the block (including the header, payload, and padding) */
/* H_Begin  : Address of the block header */
void Mem_Dump()
{
    unsigned id = 0;
    unsigned total_free_size = 0;
    unsigned total_payload_size = 0;
    unsigned total_padding_size = 0;
    unsigned total_used_size = sizeof(BLOCK_HEADER); // end of heap header not counted in loop below
    char status[5];
    unsigned payload = 0;
    unsigned padding = 0;
    BLOCK_HEADER *current = first_header;
    
    fprintf(stdout,"************************************Block list***********************************\n");
    fprintf(stdout,"%5s %7s %12s %12s %9s %9s %8s %12s\n", 
            "Id.", "Status", "Begin", "End", "Payload", "Padding", "T_Size", "H_Begin");
    fprintf(stdout,"---------------------------------------------------------------------------------\n");
            
    while (current->packed_pointer != NULL) {
        id++;
        BLOCK_HEADER *next = (BLOCK_HEADER *)((unsigned)current->packed_pointer & 0xfffffffe);
        void *begin = (void *)current + sizeof(BLOCK_HEADER);
        void *end = (void *)next - 1;
        
        if ((unsigned)current->packed_pointer & 1) { // allocated block
            strcpy(status, "Busy");
            payload = current->size;
            padding = (unsigned)((unsigned)next-(unsigned)current)-payload-sizeof(BLOCK_HEADER);
            total_payload_size += payload;
            total_padding_size += padding;
            total_used_size += payload + padding + sizeof(BLOCK_HEADER);
        }
        else { // free block 
            strcpy(status, "Free");
            payload = current->size;
            padding = 0;
            total_used_size += sizeof(BLOCK_HEADER);
            total_free_size += payload;
        }
        unsigned total_block_size = sizeof(BLOCK_HEADER) + padding + payload;
        
        fprintf(stdout,"%5d %7s %12p %12p %9u %9u %8u %12p\n", 
            id, status, begin, end, payload, padding, total_block_size, current);
        current = next;
    }
    fprintf(stdout,"---------------------------------------------------------------------------------\n");
    fprintf(stdout,"*********************************************************************************\n");
    fprintf(stdout,"Total payload size = %d\n",total_payload_size);
    fprintf(stdout,"Total padding size = %d\n",total_padding_size);
    fprintf(stdout,"Total free size = %d\n",total_free_size);
    fprintf(stdout,"Total used size = %d\n",total_used_size);
    fprintf(stdout,"*********************************************************************************\n");
    fflush(stdout);

    return;
}

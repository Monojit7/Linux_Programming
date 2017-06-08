#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cstring>

void* createSharedMemory(int numofBytes, char* nameofSharedMemory )
{

printf("create Shared Memory \n");

int shmfd = shm_open(nameofSharedMemory, O_RDWR | O_CREAT, 0777);

// shmfd file descriptor is used through out the program for other operations like nmap etc

printf("shmfd: %d \n", shmfd);

if (ftruncate( shmfd, numofBytes ) == -1 )
{
  printf("ftruncate error \n");
}

//ftruncate clears the shared memory upto the mentioned bytes. For example, if the shared memory file size is more then
// it clears the number of bytes , if the file size is less then it increases the file size and truncates it.



void* SharedMemoryAddressBase = mmap ( 0, numofBytes, PROT_READ | PROT_WRITE,  MAP_SHARED, shmfd, 0 );


// It maps the shared memory into calling process address space

if (SharedMemoryAddressBase == MAP_FAILED )
   return NULL;






return SharedMemoryAddressBase;



}



int main ()
{




char* shmadd = (char*)createSharedMemory ( 100, "/SharedMemory" );

strcpy(shmadd, "Monojit Chatterjee");

//shm_unlink("/SharedMemory");
}

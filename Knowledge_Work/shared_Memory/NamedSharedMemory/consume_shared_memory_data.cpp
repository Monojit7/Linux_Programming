#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void ReadFromSharedMemory( char* nameOfSharedMemory, int numOfBytes )
{

int shmfd = shm_open( nameOfSharedMemory, O_RDONLY | O_RDWR, 0777 );

if ( shmfd < 0 )
{
 printf("shm_open : error \n");
}

printf("shmfd: %d \n", shmfd);
char* shmadr = (char*)mmap( 0, numOfBytes,PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0 );

if( shmadr != (char*) -1)
{

char* s = shmadr;
for ( int i = 0; i < 7; i++ )
{

printf("%c", s[i]);

}
}
else
{

  printf("shmat failed \n");
}
printf("\n");

//while ( fputs ( shmadr ) >= 0 );

printf("data Reading from Shared Memory Ends \n");

}


int main()
{

ReadFromSharedMemory("/SharedMemory", 100);

return 0;
}

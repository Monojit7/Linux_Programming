#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#define SHARED_MEMORY_SIZE 100

char* create_shared_memory (const key_t& key, int size )
{


int shm_fd = shmget(key, size , IPC_CREAT|0777 );

if( shm_fd < 0 )
{

perror("shm_get \n");
return (char*)-1;
}

char* shm = (char*)shmat ( shm_fd, NULL, 0 );

if ( shm == (char*) -1 )
{

perror("shmat error \n");

return (char*)-1;
}

return shm;

}


void read_shared_memory( char* shm)
{

printf("write into the shared memory \n");
if(fputs(shm, stdout) >= 0){};

}


int main()
{

char* shm;

if ( (shm = create_shared_memory ( (key_t)5678, SHARED_MEMORY_SIZE )) == (char*) -1)
{

perror("create shared memory");
return -1;
}


read_shared_memory(shm);

}

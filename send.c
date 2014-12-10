#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 17

int main(){
	
    key_t key; // key for shm
    int shmid = 0; // this is for shm's id , use this to get mem area
    char *shm = NULL; // pointer to shared mem

    key = 2048; // pick up a key

    shmid = shmget( key, SIZE, IPC_CREAT | 0666 );
    if (shmid==(char *)-1){
    	//shmid error
    	perror("shmget error");
        return 0;
    }

    // use shmid to get shared mem address
    shm = shmat(shmid ,NULL ,0);
    if (shm==(char *)-1){
    	//shmid error
    	perror("shmat error");
        return 0;
    }

	char buffer[(SIZE - 1)];
    fgets(buffer ,SIZE ,stdin);
    
    memcpy(shm+1 ,buffer ,(SIZE-1));


}
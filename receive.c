#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 17

struct dictionary{
		char string[SIZE-1];
		int count;
};

void debug(struct dictionary *_wordList ,int _wordList_count){
	printf("total count:%d\nList:\n", _wordList_count);
	for (int i = 0; i < _wordList_count; ++i){
		printf("%d %s\n", _wordList[i].count , _wordList[i].string);
	}
}

void insert_word(struct dictionary *_wordList ,int *_wordList_count ,char *_word){
	if (*_wordList_count==0){
		_wordList[0].count=1;
		strcpy(_wordList[0].string ,_word);
		*_wordList_count=*_wordList_count+1;	
	}else{
		int finding = -1;

		for (int i = 0; i < *_wordList_count; ++i){
			if (strcmp(_wordList[i].string ,_word)==0){
				//match the string in wordList
				finding=i;
			}
		}
		if (finding!=-1){
			//match the string in wordList , count ++
			_wordList[finding].count++;
		}else{
			_wordList[*_wordList_count].count=1;
			strcpy(_wordList[*_wordList_count].string ,_word);
			*_wordList_count=*_wordList_count+1;		
		}
	}
	debug(_wordList,*_wordList_count);
}
int search_word(struct dictionary *_wordList ,int _wordList_count ,char *_word){
	//0 for no , 1 for yes
	int finding = 0;
	for (int i = 0; i < _wordList_count; ++i){
		
		if (strcmp(_wordList[i].string ,_word)==0){
			//match the string in wordList
			finding=1;
		}
	}
	return finding;
}
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
    int control_code = 1;
    // 1 = init , ready for send
    // 2 = sent
    // 3 = exit

    struct dictionary wordList[100];
    int wordList_count=0;

    while(control_code!=3){
    	memcpy(buffer ,shm ,(SIZE-1));
    	control_code = (int) buffer[0];
    	if (control_code!=1){
			memset(shm ,1 ,1); // reset control code
			char word_buffer[SIZE-1];
			memcpy(word_buffer ,buffer+1 ,SIZE-2);
			switch(word_buffer[0]){
				case '%':
					memcpy(word_buffer ,buffer+2 ,SIZE-3);
					int answer = search_word(wordList ,wordList_count ,word_buffer);
					if (answer==0){
						printf("No.\n");
					}else{
						printf("Yes.\n");
					}
					break;
				case '$':


					break;
				default:
					insert_word(wordList ,&wordList_count ,word_buffer);

			}





    	}
    	
    	sleep(0.5);
    }
}
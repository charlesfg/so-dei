#include "ipc.h"
#include "log.h"
#include "semlib.h"


int create_shm(int max_users) {

    // Create shared memory
	if ((shmid = shmget(IPC_PRIVATE, sizeof(plafond_t) * max_users, IPC_CREAT | 0766)) < 0) {
		LOG("Error in shmget with IPC_CREAT\n");
		return -1;
	}

    return shmid;
    
};

	

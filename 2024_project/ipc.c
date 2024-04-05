#include "ipc.h"
#include "log.h"

#ifndef __GLOBAL_SHMID_
#define __GLOBAL_SHMID_
int shmid = -1;
#endif


int create_shm(int max_users) {

    if (shmid != -1) {
        LOG_MSG("Shared memory already created\n");
        return shmid;
    }

    // Create shared memory
	if ((shmid = shmget(IPC_PRIVATE, sizeof(plafond_t) * max_users, IPC_CREAT | 0766)) < 0) {
		LOG_MSG("Error in shmget with IPC_CREAT\n");
		return -1;
	}

    return shmid;
    
};

plafond_t *attach_shm() {

    plafond_t *users_plafond;
    
    if ((users_plafond = (plafond_t *) shmat(shmid, NULL, 0)) == (plafond_t*)-1) {
		LOG_MSG("Shmat error!");
		return (plafond_t*)-1;
	}

    return users_plafond;
}


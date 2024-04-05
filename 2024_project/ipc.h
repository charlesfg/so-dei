#ifndef __IPC_H
#define __IPC_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "semlib.h"


typedef struct {
    int id;
    int plafond;
} plafond_t;


extern int shmid;



/*
 *  Create the shared memory based on the max number of mobile users
 *  @param Max number of  mobile users
 *  @return -1 in case of error or the shmid
 */
int create_shm(int);


/*
 *  Attach the shared memory represented by the global external variable shmid
 *  @return (void *) -1 in case of error
 */
plafond_t *attach_shm();


#endif
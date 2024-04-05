#ifndef __MOBILE_USER_H__
#define __MOBILE_USER_H__



#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include "log.h"
#include "ipc.h"

// Estrutura para armazenar os parâmetros de entrada
typedef struct {
    int plafond_inicial;
    int num_max_pedidos;
    int intervalo_video;
    int intervalo_musica;
    int intervalo_social;
    int dados_reservar;
} MobileConf;


#endif
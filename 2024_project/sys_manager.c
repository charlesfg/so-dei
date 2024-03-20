/******************************************************************************
* FILE: sys_manager.c
* DESCRIPTION:
# -> System Manager - Processo responsável por iniciar o sistema, 
Este processo lê as configurações iniciais e arranca todo o sistema. 
Em concreto tem as seguintes funcionalidades:

 [ ]  Lê e valida as informações no ficheiro de configurações 
 [ ]  Cria a Message Queue
 [ ]  Cria a Shared Memory;
 [ ]  Escreve no log file;
 [ ]  Captura o sinal SIGINT para terminar o programa, 
 [ ]  criar a Shared Memory, 
 [ ]  criar a Message Queue
 [ ]  criar os processos Authorization Requests Manager 
 [ ]  criar Monitor Engine.
 [ ]  libertando antes todos os recursos.




******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"


Logger *logger;

int main(int argc, char *argv[]){


    logger = get_logger_instance();
    log_event(logger, "Inicializando o Sistema ...");

    log_event(logger, "Finalizando o Sistema, removendo recursos!");
    close_logger(logger);
    return 0;
}
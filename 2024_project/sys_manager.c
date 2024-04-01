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
#include "config.h"
#include "ipc.h"



int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s {config-file}\n", argv[0]);
        return EXIT_FAILURE;
    }

    
    LOG("Inicializando o Sistema ...");

    Config config = read_config(argv[1]);



    LOG("Finalizando o Sistema, removendo recursos!");
    close_logger();

    return 0;
}
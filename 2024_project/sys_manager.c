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


// Função de manipulação do sinal SIGINT
void sigint_handler(int signum) {

    //Escrever no log que o programa vai acabar
    LOG_MSG("Recebido SIGINT. O programa vai acabar.");

    // TODO: Terminar as threads Receiver e Sender

    // TODO: Aguardar que todos os pedidos de autorização e comandos terminem

    // TODO: Escrever no log as tarefas nas filas que não foram executadas

    destroy_resources();
    // Encerrar o programa
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s {config-file}\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    LOG_MSG("Inicializando o Sistema ...");

    Config config ;
    if (read_config(argv[1], &config) != 0) failure("Erro ao ler o ficheiro de configurações");

    // Registra o manipulador do sinal SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        failure("Falha ao registrar o manipulador de sinal, terminando!!");
    }

    return 0;
}
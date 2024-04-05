#include "log.h"
#include "config.h"
#include "ipc.h"

void destroy_resources(){

    // Remover todos os recursos utilizados pelo sistema
    // (inclui todos os recursos, com exceção dos processos Mobile User e BackOffice User)
    LOG_MSG("Removendo recursos ...");
    close_logger();

}
    

void failure(char *msg){
    LOG_FMT("Error: %s\n", msg);
    destroy_resources();
    exit(EXIT_FAILURE);
}

#include "config.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>



// Função para ler o arquivo de configuração
int read_config(const char *filename, Config *config) {

    if (config == NULL) {
        LOG_MSG("Erro ao ler o arquivo de configuração");
        return -1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        LOG_MSG("Erro ao abrir o arquivo de configuração");
        exit(EXIT_FAILURE);
    }

    // Ler os valores do arquivo
    fscanf(file, "%d", &config->mobile_users);
    if(config->mobile_users <= 0) {
        LOG_MSG("O número de Mobile Users deve ser maior que 0");
        return -1;
    }
    fscanf(file, "%d", &config->queue_pos);
    if(config->queue_pos < 0) {
        LOG_MSG("O número de slots nas filas deve ser maior ou igual a 0");
        return -1;
    }
    fscanf(file, "%d", &config->auth_servers);
    if(config->auth_servers < 1) {
        LOG_MSG("O número de Authorization Engines deve ser maior ou igualque 1");
        return -1;
    }
    fscanf(file, "%d", &config->auth_proc_time);
    if(config->auth_proc_time < 0) {
        LOG_MSG("O tempo de processamento de pedidos deve ser maior ou igual a 0");
        return -1;
    }
    fscanf(file, "%d", &config->max_video_wait);
    if(config->max_video_wait < 1) {
        LOG_MSG("O tempo de espera de pedidos de autorização de vídeo deve ser maior ou igual a 1");
        return -1;
    }
    fscanf(file, "%d", &config->max_others_wait);
    if(config->max_others_wait < 1) {
        LOG_MSG("O tempo de espera de pedidos de autorização de outros tipos deve ser maior ou igual a 1");
        return -1;
    }

    fclose(file);
    return 0;
}

#ifdef TEST_CONFIG

int main(int argc, char *argv[]) { 

    Config config;

    if (argc != 2) { 
        fprintf(stderr, "Uso: %s <nome_arquivo_configuracao>\n", argv[0]); 
        return EXIT_FAILURE; 
    } 

    if (read_config(argv[1], &config) != 0) {
        LOG_MSG("Erro ao ler o ficheiro de configurações");
        return EXIT_FAILURE;
    }
 
    printf("Configurações lidas do arquivo:\n"); 
    printf("Mobile Users: %d\n", config.mobile_users); 
    printf("Queue Pos: %d\n", config.queue_pos); 
    printf("Auth Servers: %d\n", config.auth_servers); 
    printf("Auth Proc Time: %d\n", config.auth_proc_time ); 
    printf("Max Video Wait: %d\n", config.max_video_wait); 
    printf("Max Others Wait: %d\n", config.max_others_wait);  
    
    return EXIT_SUCCESS;
}

#endif

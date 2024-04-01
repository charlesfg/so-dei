#include "config.h"
#include <stdio.h>
#include <stdlib.h>



// Função para ler o arquivo de configuração
Config read_config(const char *filename) {
    Config config;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de configuração");
        exit(EXIT_FAILURE);
    }

    // Ler os valores do arquivo
    fscanf(file, "%d", &config.mobile_users);
    fscanf(file, "%d", &config.queue_pos);
    fscanf(file, "%d", &config.auth_servers);
    fscanf(file, "%d", &config.auth_proc_time);
    fscanf(file, "%d", &config.max_video_wait);
    fscanf(file, "%d", &config.max_others_wait);

    fclose(file);
    return config;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nome_arquivo_configuracao>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Ler as configurações do arquivo
    Config config = read_config(argv[1]);

    // Exibir as configurações lidas
    printf("Configurações lidas do arquivo:\n");
    printf("Mobile Users: %d\n", config.mobile_users);
    printf("Queue Pos: %d\n", config.queue_pos);
    printf("Auth Servers: %d\n", config.auth_servers);
    printf("Auth Proc Time: %d\n", config.auth_proc_time);
    printf("Max Video Wait: %d\n", config.max_video_wait);
    printf("Max Others Wait: %d\n", config.max_others_wait);

    return EXIT_SUCCESS;
}

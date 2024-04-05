#ifndef CONFIG_MODULE_H
#define CONFIG_MODULE_H


typedef struct {
    // MOBILE_USERS – número de Mobile Users que serão suportados pelo simulador
    int mobile_users;

    // QUEUE_POS - número de slots nas filas que são utilizadas para armazenar os pedidos de autorização
    // e os comandos dos utilizadores (>=0)
    int queue_pos;

    // AUTH_SERVERS - número de Authorization Engines que devem ser lançados inicialmente (>=1)
    int auth_servers;

    // AUTH_PROC_TIME - período (em ms) que o Authorization Engine demora para processar os pedidos
    int auth_proc_time;

    // MAX_VIDEO_WAIT - tempo máximo (em ms) que os pedidos de autorização do serviço de vídeo podem
    // aguardar para serem executados (>=1)
    int max_video_wait;

    // MAX_OTHERS_WAIT - tempo máximo (em ms) que os pedidos de autorização dos serviços de música e
    // de redes sociais, bem como os comandos podem aguardar para serem executados (>=1
    int max_others_wait;
} Config;

int read_config(const char *,  Config *);

#endif

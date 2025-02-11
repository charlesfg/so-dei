/*
    Processo que gera pedidos de autorização para cada um dos 3 serviços do simulador (streaming de
    vídeo, streaming de música e redes sociais). O Mobile User gera duas mensagens:
    1. Registo inicial: mensagem inicial para simular o registo do Mobile User na plataforma de
    autorizações de serviço. Neste pedido terá que ser indicado o plafond inicial do Mobile User.
    Este valor é registado na Shared Memory.
    2. Pedido de autorização: mensagem para simular os pedidos de autorização de serviço do
    Mobile User. Estas mensagens são enviadas em intervalos periódicos (Δt), específicos para
    cada tipo de serviço. Para cada pedido de autorização é indicada a quantidade de dados a
    reservar do plafond disponível. Este passo repete-se até o número máximo de pedidos de
    autorização estar concluído ou o plafond esgotado

    O plafond inicial, o número máximo de pedidos de autorização a enviar, os intervalos periódicos de
    renovação (Δt) por serviço e a quantidade de dados a reservar em cada pedido de renovação é
    fornecido através da linha de comandos no arranque do Mobile User.

    Cada um dos processos Mobile User envia as mensagens através do named pipe USER_PIPE. Podemos
    ter um ou mais processos destes a correr em simultâneo, cada um com os seus parâmetros.

    Sintaxe do comando de inicialização do processo Mobile User:
    $ mobile_user {plafond inicial} {número máximo de pedidos de autorização} {intervalo VIDEO} {intervalo MUSIC} {intervalo SOCIAL} {dados a reservar}

    Exemplo:
    $ mobile_user 800 50 10 20 5 40

    Todo os dados devem ser validados depois de serem lidos. E os parametros de entradas devem ser armazenados em uma estrutura de dados adequada.

    Informação a enviar para o named pipe para a mensagem de registo:
    ID_mobile_user#Plafond inicial
    Exemplo:
    345#800


    Informação a enviar para o named pipe relativamente à mensagem de pedido de autorização (para
    cada um dos 3 serviços):
    ID_mobile_user#ID serviço#Quantidade de dados a reservar
    Exemplo (para cada serviço):
    345#VIDEO#40
    345#MUSIC#40
    345#SOCIAL#40
    O identificador do Mobile_User, correspondente ao PID, será utilizado para agrupar a informação do
    utilizador na memória partilhada.
    O Mobile_User recebe alertas sobre o plafond de dados (80%, 90%, 100%) através da Message Queue Posix que é somente utilizada aqui mas tem seu identificador
    criado em outro processo, definido no cabeçalho ipc.h.

    O processo Mobile User termina quando uma das seguintes condições se verificar:
    1. Receção de um sinal SIGINT;
    2. Receção de um alerta de 100% relativo ao plafond de dados;
    3. No caso de o número máximo de pedidos de autorização ser atingido;
    4. Em caso de erro - um erro pode acontecer se algum parâmetro estiver errado ou ao tentar
    escrever para o named pipe e a escrita falhar. Nestes casos deve escrever a mensagem de erro
    no ecrã.
    Sempre que o Mobile User termina, o processo deve limpar todos os recursos.

*/

#include "mobile_user.h"

#define USER_PIPE "user_pipe"


int validar_parametros(const MobileConf *params) {
    
    if (params->plafond_inicial <= 0 || params->num_max_pedidos <= 0 ||
        params->intervalo_video <= 0 || params->intervalo_musica <= 0 ||
        params->intervalo_social <= 0 || params->dados_reservar <= 0) {
        return -1;
    }
    return 0;
}

// Função para lidar com o sinal SIGINT
void sigint_handler(int signum) {
    LOG_MSG("Recebido SIGINT. Encerrando o Mobile User.");
    exit(EXIT_SUCCESS);
}

// Função para enviar mensagens para o named pipe
void enviar_mensagem(const char *mensagem) {
    int fd = open(USER_PIPE, O_WRONLY);
    if (fd == -1) {
        LOG_MSG("Erro ao abrir o named pipe");
        exit(EXIT_FAILURE);
    }
    if (write(fd, mensagem, strlen(mensagem)) == -1) {
        perror("Erro ao escrever no named pipe");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

// Função para registrar o Mobile User na plataforma de autorizações de serviço
void registrar_mobile_user(int pid, int plafond_inicial) {
    char mensagem[100];
    snprintf(mensagem, sizeof(mensagem), "%d#%d\n", pid, plafond_inicial);
    enviar_mensagem(mensagem);
}

// Função para enviar pedido de autorização de serviço
void enviar_pedido_autorizacao(int pid, const char *servico, int quantidade_dados) {
    char mensagem[100];
    snprintf(mensagem, sizeof(mensagem), "%d#%s#%d\n", pid, servico, quantidade_dados);
    enviar_mensagem(mensagem);
}

// Função para simular o comportamento do Mobile User
void simular_mobile_user(const MobileConf *params) {
    int pid = getpid();
    int num_pedidos = 0;
    bool plafond_excedido = false;

    // Registrar o Mobile User na plataforma de autorizações de serviço
    registrar_mobile_user(pid, params->plafond_inicial);

    // Definir o handler para o sinal SIGINT
    signal(SIGINT, sigint_handler);

    // Loop para enviar pedidos de autorização de serviço
    while (num_pedidos < params->num_max_pedidos && !plafond_excedido) {
        // Esperar o intervalo adequado antes de enviar o próximo pedido
        sleep(params->intervalo_video);

        // Enviar pedido de autorização para cada tipo de serviço
        enviar_pedido_autorizacao(pid, "VIDEO", params->dados_reservar);
        enviar_pedido_autorizacao(pid, "MUSIC", params->dados_reservar);
        enviar_pedido_autorizacao(pid, "SOCIAL", params->dados_reservar);

        num_pedidos++;

        // Verificar se o plafond foi excedido
        // Supondo que o plafond foi excedido quando o número de pedidos atinge 90% do máximo
        if (num_pedidos >= params->num_max_pedidos * 0.9) {
            printf("Plafond de dados atingiu 90%% do máximo.\n");
            plafond_excedido = true;
        }
    }

    // Limpar os recursos
    // ...

    printf("Mobile User encerrado.\n");
}

int main(int argc, char *argv[]) {
    // Verificar se o número de argumentos está correto
    if (argc != 7) {
        fprintf(stderr, "Uso: %s <plafond_inicial> <num_max_pedidos> <intervalo_video> \
        <intervalo_musica> <intervalo_social> <dados_reservar>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Obter parâmetros de entrada da linha de comando
    MobileConf params;
    params.plafond_inicial = atoi(argv[1]);
    params.num_max_pedidos = atoi(argv[2]);
    params.intervalo_video = atoi(argv[3]);
    params.intervalo_musica = atoi(argv[4]);
    params.intervalo_social = atoi(argv[5]);
    params.dados_reservar = atoi(argv[6]);

    // Criar o named pipe se não existir
    mkfifo(USER_PIPE, 0666);

    // Simular o comportamento do Mobile User
    simular_mobile_user(&params);

    return 0;
}




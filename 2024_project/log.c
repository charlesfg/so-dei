#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>

// Função para obter a instância única do logger
Logger *get_logger_instance() {
    static Logger *logger = NULL;
    if (logger == NULL) {
        logger = (Logger *)malloc(sizeof(Logger));
        if (logger == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o logger.\n");
            exit(EXIT_FAILURE);
        }
        logger->file = fopen(LOG_FILE, "a");
        if (logger->file == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo de log.\n");
            exit(EXIT_FAILURE);
        }
        if (pthread_mutex_init(&(logger->lock), NULL) != 0) {
            fprintf(stderr, "Erro ao inicializar o mutex.\n");
            exit(EXIT_FAILURE);
        }
    }
    return logger;
}

// Escrever no log
void log_event(Logger *logger, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Obter a hora atual
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[9];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);

    // Bloquear o mutex
    pthread_mutex_lock(&(logger->lock));

    // Imprimir no arquivo de log
    fprintf(logger->file, "%s ", timestamp);
    vfprintf(logger->file, format, args);
    fprintf(logger->file, "\n");
    fflush(logger->file); // Limpar o buffer para garantir a escrita imediata

    // Imprimir no stdout
    printf("%s ", timestamp);
    vprintf(format, args);
    printf("\n");
    fflush(stdout);

    // Desbloquear o mutex
    pthread_mutex_unlock(&(logger->lock));

    va_end(args);
}

// Finalizar o logger
void close_logger(Logger *logger) {
    fclose(logger->file);
    pthread_mutex_destroy(&(logger->lock));
    free(logger);
}



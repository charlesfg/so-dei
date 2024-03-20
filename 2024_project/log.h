#ifndef LOG_MODULE_H
#define LOG_MODULE_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG_FILE "log.txt"

// Estrutura para manter o estado do logger
typedef struct {
    pthread_mutex_t lock;
    FILE *file;
} Logger;

// Função para obter a instância única do logger
Logger *get_logger_instance();

// Escrever no log
void log_event(Logger *logger, const char *format, ...);

// Finalizar o logger
void close_logger(Logger *logger);
#endif // LOG_MODULE_H

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
void close_logger();

static Logger *logger;

#define LOG(fmt, ...) \
    do { \
        if (!logger) { \
            logger = get_logger_instance(); \
        } \
        log_event(logger, "%s:%d: "fmt"\n",  __FILE__, __LINE__ __VA_ARGS__); \
    } while (0)


#ifdef DEBUG
#define DLOG(fmt, ...) \
    do { \
        if (!logger) { \
            logger = get_logger_instance(); \
        } \
        log_event(logger,"++DEBUG: " fmt "\n", __VA_ARGS__); \
    } while (0)
#else
#define DLOG(...) /* do nothing */
#endif

#endif // LOG_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "../claves.h"  // Asegúrate de incluir el header generado

/* Añade esta declaración externa */
extern void claves_prog_1(struct svc_req *rqstp, SVCXPRT *transp);

/* Variables globales para control del servidor */
static int server_running = 1;
static SVCXPRT *transp_tcp = NULL;

/* Handler para señales (ej. Ctrl+C) */
void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\nDeteniendo servidor...\n");
        server_running = 0;
        if (transp_tcp) {
            svc_unregister(CLAVES_PROG, CLAVES_VERS);
            svc_destroy(transp_tcp);
        }
        exit(0);
    }
}

/* Función que ejecuta cada hilo para atender peticiones */
void* server_thread(void *arg) {
    (void)arg;  // Elimina el warning de variable no usada
    svc_run();  // Bloqueante hasta que haya peticiones
    return NULL;
}

int main() {
    /* Configurar handler para Ctrl+C */
    signal(SIGINT, handle_signal);

    /* Registrar el servicio RPC sobre TCP */
    transp_tcp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (!transp_tcp) {
        fprintf(stderr, "Error: No se pudo crear el servidor TCP\n");
        exit(EXIT_FAILURE);
    }

    if (!svc_register(transp_tcp, CLAVES_PROG, CLAVES_VERS, claves_prog_1, IPPROTO_TCP)) {
        fprintf(stderr, "Error: No se pudo registrar el servicio\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor RPC iniciado (Programa: 0x%08x, Versión: %d)\n", CLAVES_PROG, CLAVES_VERS);
    printf("Usa rpcinfo -p para verificar el registro\n");

    /* Crear pool de hilos para manejar conexiones concurrentes */
    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, server_thread, (void *)transp_tcp) != 0) {
            perror("Error al crear hilo");
            exit(EXIT_FAILURE);
        }
    }

    /* Esperar a que todos los hilos terminen */
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
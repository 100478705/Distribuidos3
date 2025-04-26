#include "../claves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


typedef struct {
    int key;
    char value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord value3;
} Elemento;
struct respuesta {
    int status;
    char value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord value3;
};
typedef struct Nodo {
    Elemento dato;
    struct Nodo* sig;
} Nodo;

Nodo* cabeza = NULL;

Nodo* crearNodo(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevoNodo == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    nuevoNodo->dato.key = key;
    strncpy(nuevoNodo->dato.value1, value1, 256);
    nuevoNodo->dato.N_value2 = (N_value2 > 32) ? 32 : N_value2; // Evitar sobrepasar el límite
    memcpy(nuevoNodo->dato.V_value2, V_value2, sizeof(double) * nuevoNodo->dato.N_value2);
    nuevoNodo->dato.value3 = value3;
    nuevoNodo->sig = NULL;
    return nuevoNodo;
}

void insertarAlInicio(Nodo** cabeza, Nodo* nuevoNodo) {
    nuevoNodo->sig = *cabeza;
    *cabeza = nuevoNodo;
}
void mostrarLista(){
    Nodo* actual = cabeza;
    while (actual != NULL){
        printf("Key: %d\n", actual->dato.key);
        printf("value1: %s\n", actual->dato.value1);
        printf("n_value2: %d\n", actual->dato.N_value2);
        int i = 0;
        while(i < actual->dato.N_value2){
            printf("V_value2[%d]: %f\n", i, actual->dato.V_value2[i]);
            i += 1;
        }
        printf("value3 (x,y): %d %d\n", actual->dato.value3.x, actual->dato.value3.y);
        actual = actual->sig;
    }
}

Nodo* buscarNodo(Nodo* cabeza, int key) {
    pthread_mutex_lock(&mutex);  //Bloqueo para proteger la búsqueda
    Nodo* actual = cabeza;
    while (actual != NULL) {
        if (actual->dato.key == key) {
            printf("Key encontrada: %d\n", actual->dato.key);
            pthread_mutex_unlock(&mutex); //Desbloqueo después de buscar
            return actual;
        }
        actual = actual->sig;
    }
    pthread_mutex_unlock(&mutex);  //Desbloqueo después de buscar
    return NULL;
}

int eliminarNodo(Nodo** cabeza, int key) {
    pthread_mutex_lock(&mutex);  //Bloqueo para proteger la eliminación
    Nodo* actual = *cabeza;
    Nodo* anterior = NULL;
    while (actual != NULL) {
        if (actual->dato.key == key) {
            if (anterior == NULL) {
                *cabeza = actual->sig;
            } else {
                anterior->sig = actual->sig;
            }
            free(actual);
            pthread_mutex_unlock(&mutex);  //Desbloqueo después de eliminar
            return 0;
        }
        anterior = actual;
        actual = actual->sig;
    }
    pthread_mutex_unlock(&mutex);  //Desbloqueo si no se encontró la clave
    return -1;
}


int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {

    if (buscarNodo(cabeza, key) != NULL) {
        return -1;
    }
	printf("Valores de value1: %s\n", value1);
	printf("Valores de N_value2: %d\n", N_value2);
	printf("Valores de V_value2: ");
	for (int i = 0; i < N_value2; i++) {
		printf("%f ", V_value2[i]);
	}
	printf("\n");
	printf("Valores de value3 (x,y): %d %d\n", value3.x, value3.y);
    pthread_mutex_lock(&mutex);  //Bloqueo para proteger la inserción
    Nodo* nodo = crearNodo(key, value1, N_value2, V_value2, value3);
    insertarAlInicio(&cabeza, nodo);
    
    pthread_mutex_unlock(&mutex);  //Desbloqueo después de insertar
    return 0;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    Nodo* nodo = buscarNodo(cabeza, key);
    if (nodo == NULL) {
        return -1;  // Clave no encontrada
    }

    // Copiar los valores encontrados en los punteros proporcionados
    strncpy(value1, nodo->dato.value1, 256);
    *N_value2 = nodo->dato.N_value2;
    memcpy(V_value2, nodo->dato.V_value2, sizeof(double) * nodo->dato.N_value2);
    *value3 = nodo->dato.value3;

    return 0;  // Éxito
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    Nodo* nodo = buscarNodo(cabeza, key);
    if (nodo == NULL) {
        return -1;
    }

    pthread_mutex_lock(&mutex);  //Bloqueo antes de modificar la lista
    strncpy(nodo->dato.value1, value1, 256);
    nodo->dato.N_value2 = (N_value2 > 32) ? 32 : N_value2;
    memcpy(nodo->dato.V_value2, V_value2, sizeof(double) * nodo->dato.N_value2);
    nodo->dato.value3 = value3;

    pthread_mutex_unlock(&mutex);  //Desbloqueo después de modificar
    return 0;
}

int delete_key(int key) {
    return eliminarNodo(&cabeza, key);
}

int destroy(){
    pthread_mutex_lock(&mutex);  //Bloqueo antes de destruir la lista
    Nodo* actual = cabeza;
    Nodo* siguiente;
    while (actual != NULL) {
        siguiente = actual->sig;
        free(actual);
        actual = siguiente;
    }
    cabeza = NULL;
    pthread_mutex_unlock(&mutex);  //Desbloqueo después de destruir
    return 0;
}

int exist(int key) {
    int exists = (buscarNodo(cabeza, key) != NULL) ? 0 : -1;
    return exists;
}
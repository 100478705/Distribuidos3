// #include "../claves.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <pthread.h>


// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// typedef struct {
//     int key;
//     char value1[256];
//     int N_value2;
//     double V_value2[32];
//     struct Coord value3;
// } Elemento;
// struct respuesta {
//     int status;
//     char value1[256];
//     int N_value2;
//     double V_value2[32];
//     struct Coord value3;
// };
// typedef struct Nodo {
//     Elemento dato;
//     struct Nodo* sig;
// } Nodo;

// Nodo* cabeza = NULL;

// Nodo* crearNodo(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    
//     Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
//     if (nuevoNodo == NULL) {
//         printf("Error: No se pudo asignar memoria\n");
//         exit(1);
//     }
//     nuevoNodo->dato.key = key;
//     strncpy(nuevoNodo->dato.value1, value1, 256);
//     nuevoNodo->dato.N_value2 = (N_value2 > 32) ? 32 : N_value2; // Evitar sobrepasar el límite
//     memcpy(nuevoNodo->dato.V_value2, V_value2, sizeof(double) * nuevoNodo->dato.N_value2);
//     nuevoNodo->dato.value3 = value3;
//     nuevoNodo->sig = NULL;
//     return nuevoNodo;
// }

// void insertarAlInicio(Nodo** cabeza, Nodo* nuevoNodo) {
//     nuevoNodo->sig = *cabeza;
//     *cabeza = nuevoNodo;
// }
// void mostrarLista(){
//     Nodo* actual = cabeza;
//     while (actual != NULL){
//         printf("Key: %d\n", actual->dato.key);
//         printf("value1: %s\n", actual->dato.value1);
//         printf("n_value2: %d\n", actual->dato.N_value2);
//         int i = 0;
//         while(i < actual->dato.N_value2){
//             printf("V_value2[%d]: %f\n", i, actual->dato.V_value2[i]);
//             i += 1;
//         }
//         printf("value3 (x,y): %d %d\n", actual->dato.value3.x, actual->dato.value3.y);
//         actual = actual->sig;
//     }
// }

// Nodo* buscarNodo(Nodo* cabeza, int key) {
//     pthread_mutex_lock(&mutex);  //Bloqueo para proteger la búsqueda
//     Nodo* actual = cabeza;
//     while (actual != NULL) {
//         if (actual->dato.key == key) {
//             printf("Key encontrada: %d\n", actual->dato.key);
//             pthread_mutex_unlock(&mutex); //Desbloqueo después de buscar
//             return actual;
//         }
//         actual = actual->sig;
//     }
//     pthread_mutex_unlock(&mutex);  //Desbloqueo después de buscar
//     return NULL;
// }

// int eliminarNodo(Nodo** cabeza, int key) {
//     pthread_mutex_lock(&mutex);  //Bloqueo para proteger la eliminación
//     Nodo* actual = *cabeza;
//     Nodo* anterior = NULL;
//     while (actual != NULL) {
//         if (actual->dato.key == key) {
//             if (anterior == NULL) {
//                 *cabeza = actual->sig;
//             } else {
//                 anterior->sig = actual->sig;
//             }
//             free(actual);
//             pthread_mutex_unlock(&mutex);  //Desbloqueo después de eliminar
//             return 0;
//         }
//         anterior = actual;
//         actual = actual->sig;
//     }
//     pthread_mutex_unlock(&mutex);  //Desbloqueo si no se encontró la clave
//     return -1;
// }


// int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {

//     if (buscarNodo(cabeza, key) != NULL) {
//         return -1;
//     }

//     pthread_mutex_lock(&mutex);  //Bloqueo para proteger la inserción
//     Nodo* nodo = crearNodo(key, value1, N_value2, V_value2, value3);
//     insertarAlInicio(&cabeza, nodo);
//     printf("Guarda tupla");
//     pthread_mutex_unlock(&mutex);  //Desbloqueo después de insertar
//     return 0;
// }

// int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
//     Nodo* nodo = buscarNodo(cabeza, key);
//     if (nodo == NULL) {
//         return -1;  // Clave no encontrada
//     }

//     // Copiar los valores encontrados en los punteros proporcionados
//     strncpy(value1, nodo->dato.value1, 256);
//     *N_value2 = nodo->dato.N_value2;
//     memcpy(V_value2, nodo->dato.V_value2, sizeof(double) * nodo->dato.N_value2);
//     *value3 = nodo->dato.value3;

//     return 0;  // Éxito
// }

// int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
//     Nodo* nodo = buscarNodo(cabeza, key);
//     if (nodo == NULL) {
//         return -1;
//     }

//     pthread_mutex_lock(&mutex);  //Bloqueo antes de modificar la lista
//     strncpy(nodo->dato.value1, value1, 256);
//     nodo->dato.N_value2 = (N_value2 > 32) ? 32 : N_value2;
//     memcpy(nodo->dato.V_value2, V_value2, sizeof(double) * nodo->dato.N_value2);
//     nodo->dato.value3 = value3;

//     pthread_mutex_unlock(&mutex);  //Desbloqueo después de modificar
//     return 0;
// }

// int delete_key(int key) {
//     return eliminarNodo(&cabeza, key);
// }

// int destroy(){
//     pthread_mutex_lock(&mutex);  //Bloqueo antes de destruir la lista
//     Nodo* actual = cabeza;
//     Nodo* siguiente;
//     while (actual != NULL) {
//         siguiente = actual->sig;
//         free(actual);
//         actual = siguiente;
//     }
//     cabeza = NULL;
//     pthread_mutex_unlock(&mutex);  //Desbloqueo después de destruir
//     return 0;
// }

// int exist(int key) {
//     int exists = (buscarNodo(cabeza, key) != NULL) ? 0 : -1;
//     return exists;
// }

// servidor/claves.c
#include "claves.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Estructura interna para almacenar cada par key → valores
 */
typedef struct Elemento {
    int key;
    char value1[256];      // cadena fija de hasta 255 + '\0'
    int N_value2;          // longitud del array
    double *V_value2;      // puntero a array dinámico
    struct Coord value3;   // coordenadas
    struct Elemento *next; // siguiente en la lista
} Elemento;

/* Cabeza de la lista enlazada */
static Elemento *head = NULL;

/*
 * destroy(): libera toda la lista y sus arrays dinámicos
 */
int destroy(void) {
    Elemento *curr = head, *tmp;
    while (curr) {
        tmp = curr->next;
        free(curr->V_value2);
        free(curr);
        curr = tmp;
    }
    head = NULL;
    return 0;
}

/*
 * exist(key): devuelve 1 si existe, 0 si no
 */
int exist(int key) {
    for (Elemento *e = head; e; e = e->next)
        if (e->key == key) return 1;
    return 0;
}

/*
 * set_value: inserta o actualiza
 */
int set_value(int key,
              char *value1,
              int N_value2,
              double *V_value2,
              struct Coord value3)
{
    Elemento *e;
    // si ya existe, lo modificamos
    for (e = head; e; e = e->next) {
        if (e->key == key) {
            // actualizar value1
            strncpy(e->value1, value1, sizeof(e->value1)-1);
            e->value1[sizeof(e->value1)-1] = '\0';
            // actualizar array
            free(e->V_value2);
            e->N_value2 = N_value2;
            e->V_value2 = malloc(sizeof(double) * N_value2);
            if (!e->V_value2) return -1;
            memcpy(e->V_value2, V_value2, sizeof(double) * N_value2);
            // actualizar coord
            e->value3 = value3;
            return 0;
        }
    }
    // sino, creamos uno nuevo
    e = malloc(sizeof(*e));
    if (!e) return -1;
    e->key = key;
    strncpy(e->value1, value1, sizeof(e->value1)-1);
    e->value1[sizeof(e->value1)-1] = '\0';
    e->N_value2 = N_value2;
    e->V_value2 = malloc(sizeof(double) * N_value2);
    if (!e->V_value2) { free(e); return -1; }
    memcpy(e->V_value2, V_value2, sizeof(double) * N_value2);
    e->value3 = value3;
    // insertar al frente
    e->next = head;
    head = e;
    return 0;
}

/*
 * get_value: recupera, llenando los punteros proporcionados
 * (asume que value1 apunta a un buffer >=256,
 *  V_value2 apunta a un array >= (*N_value2) doble, y *value3 válido)
 */
int get_value(int key,
              char *value1,
              int *N_value2,
              double *V_value2,
              struct Coord *value3)
{
    for (Elemento *e = head; e; e = e->next) {
        if (e->key == key) {
            // copiar strings y arrays
            strcpy(value1, e->value1);
            *N_value2 = e->N_value2;
            memcpy(V_value2, e->V_value2, sizeof(double) * e->N_value2);
            *value3 = e->value3;
            return 0;
        }
    }
    // no encontrado
    return -1;
}

/*
 * delete_key: elimina nodo con esa clave
 */
int delete_key(int key) {
    Elemento *curr = head, *prev = NULL;
    while (curr) {
        if (curr->key == key) {
            if (prev) prev->next = curr->next;
            else       head = curr->next;
            free(curr->V_value2);
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1; // no encontrado
}

/*
 * modify_value: igual a set_value, pero error si no existía
 */
int modify_value(int key,
                 char *value1,
                 int N_value2,
                 double *V_value2,
                 struct Coord value3)
{
    Elemento *e;
    for (e = head; e; e = e->next) {
        if (e->key == key) {
            // reutiliza set-value interno para actualización
            return set_value(key, value1, N_value2, V_value2, value3);
        }
    }
    return -1; // no existía
}

#include <stdio.h>
#include <stdlib.h>
#include "../claves.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
bool noComprobado = false;

bool mostrarMenu() {
    printf("\033[1;34m===========================================\033[0m\n");
    printf("\033[1;34m             MENU PRINCIPAL              \033[0m\n");
    printf("\033[1;34m===========================================\033[0m\n");
    printf("\n");
    printf("1. Insertar un ID\n");
    printf("2. Modificar un ID\n");
    printf("3. Eliminar un ID\n");
    printf("4. Comprobar existencia de un ID\n");
    printf("5. Recuperar una tupla\n");
    printf("6. Destruir todas las tuplas\n");
    printf("7. Bateria de pruebas\n");
    printf("8. Salir\n");
    printf("\n");
    printf("\033[1;34m===========================================\033[0m\n");
    printf("Elige una opción: ");
    return true;
}

int esNumerica(const char *cadena) {
    for (int i = 0; i < strlen(cadena); i++) {
        if (!isdigit(cadena[i])) {
            printf("El valor debe ser numérico\n");
            return 0; // No es numérica
        }
    }
    return 1; // Es numérica
}
int main(int argc, char **argv) {
    
    int opcion;
    int key;


    do {
        
        if(mostrarMenu() == false){
            return 0;
        }
        scanf("%d", &opcion);
        int Nv2 = 0;
        int err;
        char v1[256];
        struct Coord v3;
        char val[256];
        bool noCumpleCondicion;
        double *v2;
        switch(opcion) {
            case 1:
                do
                {
                    printf("inserte la key: \n");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                key = atoi(val);
                noCumpleCondicion = false;
                do
                {
                    printf("inserte v1: \n");
                    scanf("%s", v1);
                    if(strlen(v1) > 256){
                        printf("El valor de v1 no puede ser mayor a 256 caracteres\n");
                        noCumpleCondicion = true;
                    }else{
                        noCumpleCondicion = false;
                    }
                } while (noCumpleCondicion);
                
                               
                printf("inserte v2: \n");
                do
                {
                    printf("¿Cuántos elementos quieres insertar? ");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                
                Nv2 = atoi(val);
                
                v2 = (double*)malloc(Nv2 * sizeof(double)); //asigno memoria

                if (v2 == NULL) {
                    printf("Error al asignar memoria.\n");
                    return 1; // Salir si no se pudo asignar memoria
                }

                // Pedir al usuario que ingrese los valores
                printf("Introduce los valores (tipo double):\n");
                noCumpleCondicion  = false;
                for (int i = 0; i < Nv2; i++) {
                    do
                    {
                        char *endptr;
                        printf("Elemento %d: ", i + 1);
                        scanf("%s", val);
                        v2[i] = strtod(val, &endptr);
                        if(*endptr != '\0'){
                            printf("El valor debe ser numérico\n");
                            noCumpleCondicion = true;
                        }else{
                            noCumpleCondicion = false;
                        }
                        
                    } while (noCumpleCondicion);
                    
                }

                do
                {
                    printf("inserte v3.x: \n");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                v3.x = atoi(val);

                do
                {
                    printf("inserte v3.y: \n");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                v3.y = atoi(val);
                err = exist(key);
                if (err == 0) {
                    printf("Error al Insertar. La tupla ya existe\n");
                    break;
                }

                err = set_value(key, v1, Nv2, v2, v3);
                if (err == -1) {
                    printf("Error al insertar la tupla.\n");
                } else {
                    printf("Tupla insertada correctamente\n");
                }

                free(v2);
                break;
            case 2:
                do
                {
                    printf("inserte la key: \n");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                key = atoi(val);
                noCumpleCondicion = false;
                do
                {
                    printf("inserte v1: \n");
                    scanf("%s", v1);
                    if(strlen(v1) > 256){
                        printf("El valor de v1 no puede ser mayor a 256 caracteres\n");
                        noCumpleCondicion = true;
                    }else{
                        noCumpleCondicion = false;
                    }
                } while (noCumpleCondicion);
                
                            
                printf("inserte v2: \n");
                do
                {
                    printf("¿Cuántos elementos quieres insertar? ");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                
                Nv2 = atoi(val);
                
                v2 = (double*)malloc(Nv2 * sizeof(double)); //asigno memoria

                if (v2 == NULL) {
                    printf("Error al asignar memoria.\n");
                    return 1; // Salir si no se pudo asignar memoria
                }

                // Pedir al usuario que ingrese los valores
                printf("Introduce los valores (tipo double):\n");
                noCumpleCondicion  = false;
                for (int i = 0; i < Nv2; i++) {
                    do
                    {
                        char *endptr;
                        printf("Elemento %d: ", i + 1);
                        scanf("%s", val);
                        v2[i] = strtod(val, &endptr);
                        if(*endptr != '\0'){
                            printf("El valor debe ser numérico\n");
                            noCumpleCondicion = true;
                        }else{
                            noCumpleCondicion = false;
                        }
                        
                    } while (noCumpleCondicion);
                    
                }

                do
                {
                    printf("inserte v3.x: \n");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                v3.x = atoi(val);

                do
                {
                    printf("inserte v3.y: \n");
                    scanf("%s", val);
                } while (esNumerica(val) == 0);
                v3.y = atoi(val);
                err = exist(key);
                if (err == -1) {
                    printf("Error al modificar. La tupla no existe\n");
                    break;
                }
                err = modify_value(key, v1, 3, v2, v3);
                if (err == -1) {
                    printf("Error al modificar la tupla.\n");
                } else {
                    printf("Tupla modificada correctamente\n");
                }
                free(v2);
                break;
            case 3:
                printf("inserte la key: \n");
                scanf("%d", &key);
                err = exist(key);
                if (err == -1) {
                    printf("Error al eliminar. La tupla no existe\n");
                    break;
                }
                err = delete_key(key);
                if (err == -1) {
                    printf("Error al eliminar la tupla\n");
                } else {
                    printf("Tupla eliminada correctamente\n");
                }
                
                break;
            case 4:
                // Prueba de exist
                printf("inserte la key: \n");
                scanf("%d", &key);
                err = exist(key);
                if (err == -1) {
                    printf("La tupla no existe\n");
                } else {
                    printf("La tupla existe\n");
                }

                break;
            case 5:
                printf("inserte la key: \n");
                scanf("%d", &key);
                // Prueba de get_value
                char value1[256];
                int N_value2;
                double V_value2[32];
                struct Coord value3;
                err = get_value(key, value1, &N_value2, V_value2, &value3);
                if (err == -1) {
                    printf("Error al obtener la tupla\n");
                } else {
                    printf("Tupla obtenida: value1=%s, N_value2=%d, value3=(%d, %d)\n", value1, N_value2, value3.x, value3.y);
                
                    printf("Valores de V_value2: ");
                    for (int i = 0; i < N_value2; i++) {
                        printf("%f ", V_value2[i]);
                    }
                    printf("\n");
                
                }
                break;
            case 6:
                // Prueba de destroy
                err = destroy();
                if (err == -1) {
                    printf("Error al destruir las tuplas\n");
                } else {
                    printf("Tupla eliminada correctamente\n");
                }
                break;
            
            case 7:
                destroy();
                printf("\033[1;34m             PRUEBA SET VALUE              \033[0m\n"); 
                key = 999;
                const char *fuente = "ejemplo de valor 1";
                strncpy(v1, fuente, sizeof(v1) - 1);
                double default_v2[] = {2.3, 0.5, 23.45};

                v3.x = 10;
                v3.y = 5;
                printf("Tupla insertada: value1=%s, N_value2=%d, value3=(%d, %d)\n", value1, 3, value3.x, value3.y);
                printf("Valores de V_value2: ");
                for (int i = 0; i < 3; i++) {
                    printf("%f ", default_v2[i]);
                }
                // Prueba de set_value
                err = set_value(key, v1, 3, default_v2, v3);
                if (err == -1) {
                    printf("Error al insertar la tupla\n");
                } else {
                    printf("Tupla insertada correctamente\n");
                }
                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();
                
                printf("\033[1;34m             PRUEBA GET VALUE              \033[0m\n");
                // Prueba de get_value
                printf("Comprobando tupla 999");
                err = get_value(key, value1, &N_value2, V_value2, &value3);
                if (err == -1) {
                    printf("Error al obtener la tupla\n");
                } else {
                    printf("Tupla obtenida: value1=%s, N_value2=%d, value3=(%d, %d)\n", value1, N_value2, value3.x, value3.y);
                    printf("Valores de V_value2: ");
                    for (int i = 0; i < N_value2; i++) {
                        printf("%f ", V_value2[i]);
                    }
                    printf("\n");
                
                }
                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();

                printf("\033[1;34m             PRUEBA MODIFY VALUE              \033[0m\n");
                // Prueba de modify_value
                char *new_v1 = "nuevo valor 1";
                double new_v2[] = {1.1, 1.1, 1.1};
                struct Coord new_v3;
                new_v3.x = 1;
                new_v3.y = 1;
                printf("Tupla insertada: value1=%s, N_value2=%d, value3=(%d, %d)\n", new_v1, 3, value3.x, value3.y);
                printf("Valores de V_value2: ");
                for (int i = 0; i < 3; i++) {
                    printf("%f ", new_v2[i]);
                }
                err = modify_value(key, new_v1, 3, new_v2, new_v3);
                if (err == -1) {
                    printf("Error al modificar la tupla\n");
                } else {
                    printf("Tupla modificada correctamente\n");
                }
                err = get_value(key, value1, &N_value2, V_value2, &value3);
                printf("Tupla obtenida: value1=%s, N_value2=%d, value3=(%d, %d)\n", value1, N_value2, value3.x, value3.y);
                printf("Valores de V_value2: ");
                for (int i = 0; i < N_value2; i++) {
                    printf("%f ", V_value2[i]);
                }
                printf("\n");
                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();

                printf("\033[1;34m             PRUEBA EXIST KEY              \033[0m\n");
                // Prueba de exist
                err = exist(key);
                if (err == -1) {
                    printf("La tupla no existe\n");
                } else {
                    printf("La tupla existe\n");
                }
                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();

                printf("\033[1;34m             PRUEBA SET VALUE A KEY EXISTENTE             \033[0m\n");
                // Prueba de insertar una tupla con una key existente
                err = set_value(key, v1, 3, default_v2, v3);
                if (err == -1) {
                    printf("Error al Insertar. La tupla ya existe\n");
                } else {
                    printf("Tupla insertada correctamente\n");
                }
                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();

                printf("\033[1;34m             PRUEBA DELETE KEY              \033[0m\n");
                // Prueba de delete_key
                err = delete_key(key);
                if (err == -1) {
                    printf("Error al eliminar la tupla\n");
                } else {
                    printf("Tupla eliminada correctamente\n");
                }

                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();

                printf("\033[1;34m             PRUEBA MODIFY VALUE A KEY AHORA VACIA              \033[0m\n");
                err = modify_value(key, new_v1, 3, new_v2, new_v3);

                if (err == -1) {
                    printf("Error al modificar la tupla. La tupla no existe\n");
                } else {
                    printf("Tupla modificada correctamente\n");
                }

                printf("\nPresiona cualquier tecla para continuar...\n");
                getchar();
                getchar();

                printf("\033[1;34m             PRUEBA DESTROY              \033[0m\n");
                err = set_value(key, v1, 3, default_v2, v3);
                err = set_value(1000, new_v1, 3, new_v2, new_v3);
                err = get_value(key, value1, &N_value2, V_value2, &value3);
                printf("Tupla obtenida: value1=%s, N_value2=%d, value3=(%d, %d)\n", value1, 3, value3.x, value3.y);
                printf("Valores de V_value2: ");
                for (int i = 0; i < 3; i++) {
                    printf("%f ", V_value2[i]);
                }
                printf("\n");
                err = get_value(1000, value1, &N_value2, V_value2, &value3);
                printf("Tupla obtenida: value1=%s, N_value2=%d, value3=(%d, %d)\n", value1, 3, value3.x, value3.y);
                printf("Valores de V_value2: ");
                for (int i = 0; i < 3; i++) {
                    printf("%f ", V_value2[i]);
                }
                printf("\n");
                printf("Se ejecuta destroy\n");
                err = destroy();
                if (err == -1) {
                    printf("Error al destruir las tuplas\n");
                } else {
                    printf("Tuplas eliminadas correctamente\n");
                }
                err = get_value(key, value1, &N_value2, V_value2, &value3);
                if (err == -1) {
                    printf("La tupla no existe\n");
                }
                err = get_value(1000, value1, &N_value2, V_value2, &value3);
                if (err == -1) {
                    printf("La tupla no existe\n");
                }
                break;
            case 8:
                printf("\nGracias por usar el sistema. ¡Hasta luego!\n");
                break;
            default:
                printf("\nOpción no válida. Por favor, elige una opción del 1 al 4.\n");
                break;
        }
        
        if (opcion != 8) {
            printf("\nPresiona cualquier tecla para continuar...\n");
            getchar(); // Para consumir el salto de línea previo a la espera de la tecla
            getchar(); // Espera una tecla para continuar
        }

    } while (opcion != 8);

    return 0;
}
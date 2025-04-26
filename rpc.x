/* claves_rpc.x  — Interfaz con arrays estáticos */

const MAX_STR  = 256;    /* longitud máxima de value1 */
const MAX_V2   = 32;     /* longitud máxima del vector V_value2 */

struct coord {
    int x;
    int y;
};

/* definimos tipos C-style de longitud fija */
typedef char   valor1[MAX_STR];
typedef double vector2[MAX_V2];

/* La tupla clave-valor */
struct tupla {
    int     key;
    valor1  value1;     /* buffer de 256 bytes */
    int     N_value2;   /* nº real de elementos en V_value2 */
    vector2 V_value2;   /* array de 32 doubles */
    coord   value3;
};

/* Respuesta del GET: código + tupla */
struct get_resp {
    int   status;  /*  0 = OK,  -1 = error */
    tupla data;
};

program CLAVES_PROG {
    version CLAVES_VERS {
        int      SET_VALUE    (tupla)    = 1;
        int      MODIFY_VALUE (tupla)    = 2;
        int      DELETE_KEY   (int)      = 3;
        int      EXIST        (int)      = 4;
        get_resp GET_VALUE    (int)      = 5;
        int      DESTROY      (void)     = 6;
    } = 1;
} = 0x31234567;

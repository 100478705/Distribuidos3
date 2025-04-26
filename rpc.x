/* rpc.x  — Interfaz del servicio de tuplas  */

const MAX_STR  = 256;   /* longitud máxima de value1            */
const MAX_V2   = 32;    /* longitud máxima del vector value2    */

struct coord {
	int x;
	int y;
};

struct tupla {
	int           key;
	string        value1<MAX_STR>;
	int           N_value2;           /* nº de elementos realmente usados */
	double        V_value2<MAX_V2>;   /* vector de doubles                */
	coord         value3;
};

/* Respuesta completa del GET (status + tupla)                    */
struct get_resp {
	int   status;   /*  0 = OK,  -1 = error (tupla indefinida) */
	tupla data;
};

program CLAVES_PROG {
	version CLAVES_VERS {
		/* mismas operaciones que en el enunciado              */
		int        SET_VALUE   (tupla)     = 1;
		int        MODIFY_VALUE(tupla)     = 2;
		int        DELETE_KEY  (int)       = 3;
		int        EXIST       (int)       = 4;
		get_resp   GET_VALUE   (int)       = 5;
		int        DESTROY     (void)      = 6;
	} = 1;
} = 0x31234567;        /* nº de programa arbitrario (no reservado) */

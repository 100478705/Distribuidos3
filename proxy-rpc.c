/* proxy-rpc.c  –  wrapper cliente para libclaves.so */

#include <rpc/rpc.h>          /* tirpc                         */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "claves.h"           /* API que ve app-cliente        */
#include "rpc.h"              /* generado por rpcgen           */

/* ------------------------------------------------------- */
/* Utilidad: abre un canal RPC usando la IP de IP_TUPLAS   */
static CLIENT *get_handle(void)
{
    char *ip = getenv("IP_TUPLAS");
    if (!ip) return NULL;

    CLIENT *cl = clnt_create(ip, CLAVES_PROG, CLAVES_VERS, "tcp");
    return cl;        /* NULL ⇒ no hay servidor                 */
}

/* ------------------------------------------------------- */
int set_value(int key, char *value1, int N,
              double *V, struct Coord v3)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;             /* sin conexión */
	printf("Llega a set_value\n");
    /* montar la tupla con la codificación XDR real */
    tupla t;
    t.key           = key;
    t.value1        = value1;       /* string ⇒ char *          */
    t.N_value2      = N;

    t.V_value2.V_value2_len = N;
    t.V_value2.V_value2_val = V;

    t.value3.x = v3.x;
    t.value3.y = v3.y;

    int result;
    enum clnt_stat s = set_value_1(&t, &result, cl);

    clnt_destroy(cl);
    return (s == RPC_SUCCESS) ? result : -1;
}

/* ------------------------------------------------------- */
int modify_value(int key, char *value1, int N,
                 double *V, struct Coord v3)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    tupla t;
    t.key      = key;
    t.value1   = value1;
    t.N_value2 = N;

    t.V_value2.V_value2_len = N;
    t.V_value2.V_value2_val = V;

    t.value3.x = v3.x;
    t.value3.y = v3.y;

    int result;
    enum clnt_stat s = modify_value_1(&t, &result, cl);
    clnt_destroy(cl);
    return (s == RPC_SUCCESS) ? result : -1;
}

/* ------------------------------------------------------- */
int delete_key(int key)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    int result;
    enum clnt_stat s = delete_key_1(&key, &result, cl);
    clnt_destroy(cl);
    return (s == RPC_SUCCESS) ? result : -1;
}

/* ------------------------------------------------------- */
int exist(int key)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    int result;
    enum clnt_stat s = exist_1(&key, &result, cl);
    clnt_destroy(cl);
    return (s == RPC_SUCCESS) ? result : -1;
}

/* ------------------------------------------------------- */
int get_value(int key, char *value1,
              int *N, double *V, struct Coord *v3)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    get_resp res;   /* estructura completa de respuesta      */
    enum clnt_stat s = get_value_1(&key, &res, cl);

    int status = -1;
    if (s == RPC_SUCCESS) {
        status = res.status;
        if (status == 0) {          /* OK, copiar los campos   */
            strncpy(value1, res.data.value1, 256);

            *N = res.data.V_value2.V_value2_len;
            memcpy(V, res.data.V_value2.V_value2_val,
                   (*N) * sizeof(double));

            v3->x = res.data.value3.x;
            v3->y = res.data.value3.y;
        }
    }

    /* liberar memoria dinámica que xdr_allocó */
    xdr_free((xdrproc_t) xdr_get_resp, (char *)&res);
    clnt_destroy(cl);
    return status;
}

/* ------------------------------------------------------- */
int destroy(void)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    int result;
    enum clnt_stat s = destroy_1(NULL, &result, cl);
    clnt_destroy(cl);
    return (s == RPC_SUCCESS) ? result : -1;
}

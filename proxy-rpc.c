
#include <rpc/rpc.h>
#include <string.h>
#include <stdlib.h>
#include "rpc.h"
#include "claves.h"

static CLIENT *get_handle(void)
{
    char *ip = getenv("IP_TUPLAS");
    if (!ip) return NULL;
    CLIENT *cl = clnt_create(ip, CLAVES_PROG, CLAVES_VERS, "tcp");
    if (!cl) clnt_pcreateerror("clnt_create");
    return cl;
}

int set_value(int key, char *v1, int N, double *v2, struct Coord c)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    tupla t = {0};
    t.key = key;
    strncpy(t.value1, v1, sizeof(t.value1)-1);
    t.N_value2 = (N > MAX_V2 ? MAX_V2 : N);
    memcpy(t.V_value2, v2, sizeof(double)*t.N_value2);
    t.value3.x = c.x;
    t.value3.y = c.y;

    int res = -1;
    if (set_value_1(&t, &res, cl) != RPC_SUCCESS) res = -1;
    clnt_destroy(cl);
    return res;
}

int modify_value(int key, char *v1, int N, double *v2, struct Coord c)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    tupla t = {0};
    t.key = key;
    strncpy(t.value1, v1, sizeof(t.value1)-1);
    t.N_value2 = (N > MAX_V2 ? MAX_V2 : N);
    memcpy(t.V_value2, v2, sizeof(double)*t.N_value2);
    t.value3.x = c.x;
    t.value3.y = c.y;

    int res = -1;
    if (modify_value_1(&t, &res, cl) != RPC_SUCCESS) res = -1;
    clnt_destroy(cl);
    return res;
}

int delete_key(int key)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;
    int res = -1;
    if (delete_key_1(&key, &res, cl) != RPC_SUCCESS) res = -1;
    clnt_destroy(cl);
    return res;
}

int exist(int key)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;
    int res = -1;
    if (exist_1(&key, &res, cl) != RPC_SUCCESS) res = -1;
    clnt_destroy(cl);
    return res;
}

int get_value(int key, char *v1, int *N, double *v2, struct Coord *c)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;

    get_resp resp;
    int st = -1;

    if (get_value_1(&key, &resp, cl) == RPC_SUCCESS) {
        st = resp.status;
        if (st == 0) {
            strncpy(v1, resp.data.value1, sizeof(resp.data.value1));
            *N = resp.data.N_value2;
            memcpy(v2, resp.data.V_value2, sizeof(double)*(*N));
            c->x = resp.data.value3.x;
            c->y = resp.data.value3.y;
        }
    }
    clnt_destroy(cl);
    return st;
}

int destroy(void)
{
    CLIENT *cl = get_handle();
    if (!cl) return -2;
    int res = -1;
    if (destroy_1(NULL, &res, cl) != RPC_SUCCESS) res = -1;
    clnt_destroy(cl);
    return res;
}

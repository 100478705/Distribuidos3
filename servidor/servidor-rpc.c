
#include <rpc/rpc.h>
#include <string.h>
#include <stdlib.h>
#include "rpc.h"     
#include "claves.h"  

extern void claves_prog_1(struct svc_req *, SVCXPRT *);

bool_t
set_value_1_svc(tupla *arg, int *res, struct svc_req *rq)
{
    struct Coord c = { arg->value3.x, arg->value3.y };
    *res = set_value(arg->key, arg->value1,
                     arg->N_value2, arg->V_value2, c);
    return TRUE;
}

bool_t
modify_value_1_svc(tupla *arg, int *res, struct svc_req *rq)
{
    struct Coord c = { arg->value3.x, arg->value3.y };
    *res = modify_value(arg->key, arg->value1,
                        arg->N_value2, arg->V_value2, c);
    return TRUE;
}

bool_t
delete_key_1_svc(int *key, int *res, struct svc_req *rq)
{
    *res = delete_key(*key);
    return TRUE;
}

bool_t
exist_1_svc(int *key, int *res, struct svc_req *rq)
{
    *res = exist(*key);
    return TRUE;
}

bool_t
get_value_1_svc(int *key, get_resp *resp, struct svc_req *rq)
{
    char tmp1[256];
    double tmpV[MAX_V2];
    int tmpN;
    struct Coord tmpC;

    resp->status = get_value(*key, tmp1, &tmpN, tmpV, &tmpC);

    if (resp->status == 0) {
        resp->data.key = *key;
        strncpy(resp->data.value1, tmp1, sizeof(resp->data.value1)-1);
        resp->data.value1[sizeof(resp->data.value1)-1] = '\0';
        resp->data.N_value2 = tmpN;
        memcpy(resp->data.V_value2, tmpV, sizeof(double)*tmpN);
        resp->data.value3.x = tmpC.x;
        resp->data.value3.y = tmpC.y;
    }
    return TRUE;
}

bool_t
destroy_1_svc(void *dummy, int *res, struct svc_req *rq)
{
    *res = destroy();
    return TRUE;
}

bool_t
claves_prog_1_freeresult(SVCXPRT *t, xdrproc_t xdr_res, caddr_t res)
{
    if (res && xdr_res) xdr_free(xdr_res, res);
    (void)t;
    return TRUE;
}

int
main(void)
{
    pmap_unset(CLAVES_PROG, CLAVES_VERS);

    SVCXPRT *tcp = svctcp_create(RPC_ANYSOCK, 0, 0);
    svc_register(tcp, CLAVES_PROG, CLAVES_VERS, claves_prog_1, IPPROTO_TCP);

    SVCXPRT *udp = svcudp_create(RPC_ANYSOCK);
    svc_register(udp, CLAVES_PROG, CLAVES_VERS, claves_prog_1, IPPROTO_UDP);

    svc_run();
    return 1;
}

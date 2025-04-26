/* servidor-rpc.c  —  servidor que envuelve claves.c  */
#include "rpc.h"            /* stubs generados por rpcgen */
#include "../claves.h"      /* implementación de la lista */
#include <string.h>         /* memcpy, strncpy           >
#include <stdlib.h>         /* malloc                     */

/* ---------- SET_VALUE ------------------------------------ */
bool_t
set_value_1_svc(tupla *argp, int *result, struct svc_req *rq)
{
	struct Coord c = { argp->value3.x, argp->value3.y };
	printf("Llega a set_value_1_svc\n");
	*result = set_value(argp->key,
	                    argp->value1,
	                    argp->N_value2,
	                    argp->V_value2.V_value2_val,
	                    c);
	return TRUE;   /* la llamada RPC en sí fue correcta      */
}

/* ---------- MODIFY_VALUE ---------------------------------- */
bool_t
modify_value_1_svc(tupla *argp, int *result, struct svc_req *rq)
{
	struct Coord c = { argp->value3.x, argp->value3.y };

	*result = modify_value(argp->key,
	                       argp->value1,
	                       argp->N_value2,
	                       argp->V_value2.V_value2_val,
	                       c);
	return TRUE;
}

/* ---------- DELETE_KEY ------------------------------------ */
bool_t
delete_key_1_svc(int *key, int *result, struct svc_req *rq)
{
	*result = delete_key(*key);
	return TRUE;
}

/* ---------- EXIST ----------------------------------------- */
bool_t
exist_1_svc(int *key, int *result, struct svc_req *rq)
{
	*result = exist(*key);     /* 0 = existe, -1 = error     */
	return TRUE;
}

/* ---------- GET_VALUE ------------------------------------- */
bool_t
get_value_1_svc(int *key, get_resp *resp, struct svc_req *rq)
{
	/* reservar espacio para el vector de salida              */
	static double tmpVec[MAX_V2];      /* vida global, segura p/ RPC MT */

	resp->data.V_value2.V_value2_val = tmpVec;

	resp->status = get_value(*key,
	                         resp->data.value1,
	                         &resp->data.N_value2,
	                         tmpVec,
	                         (struct Coord *)&resp->data.value3);  /* same layout */

	if (resp->status == 0) {
		resp->data.key = *key;
		resp->data.V_value2.V_value2_len = resp->data.N_value2;
	}
	return TRUE;
}

/* ---------- DESTROY --------------------------------------- */
bool_t
destroy_1_svc(void *dummy, int *result, struct svc_req *rq)
{
	*result = destroy();
	return TRUE;
}

bool_t
claves_prog_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	/* La mayoría de nuestras operaciones devuelven un int, cuyo
	   XDR no aloja memoria dinámica.  Sólo GET_VALUE devuelve
	   una estructura con un vector → usamos xdr_free()           */
	if (result != NULL && xdr_result != NULL)
		xdr_free(xdr_result, result);

	/* ‘transp’ no se usa, pero lo dejamos para cumplir el proto. */
	(void) transp;
	return TRUE;          /* siempre OK */
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "claves.h"

/* tus prototipos reales */
extern int set_value(int key, char *v1, int n2, double *v2, struct Coord c);
extern int get_value(int key, char *v1, int *n2, double *v2, struct Coord *c);
/* … delete_key, modify_value, exist, destroy … */

static CLIENT *cl = NULL;
static void init_rpc() {
  char *ip = getenv("IP_TUPLAS");
  if (!ip) { fprintf(stderr,"IP_TUPLAS no definida\n"); exit(1); }
  cl = clnt_create(ip, CLAVES_PROG, CLAVES_VERS, "tcp");
  if (!cl) { clnt_pcreateerror(ip); exit(1); }
}

int set_value(int key, char *v1, int n2, double *v2, struct Coord c) {
    if (!cl) init_rpc();

    printf("[PROXY] Llega al set_value del proxy\n");
    fflush(stdout);

    set_args args = {
        .key = key,
        .value1 = v1,
        .N_value2 = n2,
        .V_value2 = { (u_int)n2, v2 },
        .value3 = c
    };

    int *r = rpc_set_value_1(&args, cl);
    if (!r) {
        clnt_perror(cl, "rpc_set_value");
        return -1;
    }
    return *r;
}


int get_value(int key, char *v1, int *n2, double *v2, struct Coord *c) {
  if (!cl) init_rpc();
  get_args args = { .key = key };
  set_args *res = rpc_get_value_1(&args, cl);
  if (!res) { clnt_perror(cl,"rpc_get_value"); return -1; }
  strcpy(v1, res->value1);
  *n2 = res->N_value2;
  memcpy(v2, res->V_value2.V_value2_val, (*n2)*sizeof(double));
  *c = res->value3;
  return 0;
}

int delete_key(int key) {
  if (!cl) init_rpc();
  delete_args args = { .key = key };
  int *r = rpc_delete_key_1(&args, cl);
  return r ? *r : (clnt_perror(cl,"rpc_delete_key"), -1);
}

int modify_value(int key, char *v1, int n2, double *v2, struct Coord c) {
  if (!cl) init_rpc();
  set_args args = {
    .key = key,
    .value1 = v1,
    .N_value2 = n2,
    .V_value2 = { (u_int)n2, v2 },
    .value3 = c
  };
  int *r = rpc_modify_value_1(&args, cl);
  return r ? *r : (clnt_perror(cl,"rpc_modify_value"), -1);
}

int exist(int key) {
  if (!cl) init_rpc();
  exist_args args = { .key = key };
  int *r = rpc_exist_1(&args, cl);
  return r ? *r : (clnt_perror(cl,"rpc_exist"), -1);
}

int destroy(void) {
  if (!cl) init_rpc();
  int *r = rpc_destroy_1(NULL, cl);
  return r ? *r : (clnt_perror(cl,"rpc_destroy"), -1);
}

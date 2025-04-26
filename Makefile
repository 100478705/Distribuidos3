# ──────────────────────────────────────────────────────────────
#  Makefile – práctica RPC clave-valor
# ──────────────────────────────────────────────────────────────
CC      = gcc
RPCGEN  = rpcgen -M -C          # -M: servidor multihilo, -C: ANSI
CFLAGS  = -Wall -g -fPIC -pthread \
          -I. -I/usr/include/tirpc
LDFLAGS = -pthread -ltirpc

# distribución de carpetas
CLIENT_DIR  = cliente
SERVER_DIR  = servidor

# ---------- fuentes propias ----------
PROXY_SRC   = proxy-rpc.c
CLIENT_SRC  = $(CLIENT_DIR)/app-cliente.c
SERVER_SRC  = $(SERVER_DIR)/servidor-rpc.c $(SERVER_DIR)/claves.c

# ---------- interfaz RPC -------------
RPC_X       = rpc.x
RPC_H       = rpc.h
RPC_CLNT_C  = rpc_clnt.c
RPC_SVC_C   = rpc_svc.c
RPC_XDR_C   = rpc_xdr.c

# ---------- objetos ------------------
OBJ_PROXY    = $(PROXY_SRC:.c=.o)
OBJ_CLIENTE  = $(CLIENT_SRC:.c=.o)
OBJ_SERVER   = $(SERVER_SRC:.c=.o)
OBJ_CLNT_STB = $(RPC_CLNT_C:.c=.o)
OBJ_SVC_STB  = $(RPC_SVC_C:.c=.o)
OBJ_XDR      = $(RPC_XDR_C:.c=.o)

# ---------- metas --------------------
.PHONY: all clean
all: clienteEj servidorEj

# 1) Stubs RPC (se recrean sólo si cambia rpc.x)
$(RPC_H) $(RPC_CLNT_C) $(RPC_SVC_C) $(RPC_XDR_C): $(RPC_X)
	@echo "[rpcgen] Generando stubs…"
	$(RPCGEN) -h     -o $(RPC_H)      $<
	$(RPCGEN) -l     -o $(RPC_CLNT_C) $<
	$(RPCGEN) -s tcp -o $(RPC_SVC_C)  $<
	$(RPCGEN) -c     -o $(RPC_XDR_C)  $<

# 2) Regla genérica de compilación C → objeto
%.o: %.c $(RPC_H)
	$(CC) $(CFLAGS) -c $< -o $@

# 3) Biblioteca compartida – lado cliente
libclaves.so: $(OBJ_PROXY) $(OBJ_CLNT_STB) $(OBJ_XDR)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

# 4) Ejecutable cliente
clienteEj: libclaves.so $(OBJ_CLIENTE)
	$(CC) -o $@ $(OBJ_CLIENTE) -L. -lclaves -Wl,-rpath,. $(LDFLAGS)

# 5) Ejecutable servidor
servidorEj: $(OBJ_SERVER) $(OBJ_SVC_STB) $(OBJ_XDR)
	$(CC) -o $@ $^ $(LDFLAGS)

# 6) Limpieza
clean:
	rm -f $(CLIENT_DIR)/*.o $(SERVER_DIR)/*.o *.o \
	      rpc_*.* $(RPC_H) \
	      clienteEj servidorEj libclaves.so

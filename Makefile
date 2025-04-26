# Makefile para Distribuidos3
CC       := gcc
CFLAGS := -Wall -g -fPIC -I. -I/usr/include/tirpc
LDFLAGS  := -ltirpc -lpthread
RPCGEN   := rpcgen
XFILE    := claves.x

# Binaries
SERVER_BIN := servidorEj
CLIENT_BIN := clienteEj
LIBRARY    := libclaves.so

# Fuentes “históricas”
CLIENT_SRC := cliente/app-cliente.c
SERVER_SRC := servidor/servidor-rpc.c servidor/claves.c
PROXY_SRC  := proxy-rpc.c

# Archivos auto-generados por rpcgen
RPC_HDR    := claves.h
RPC_CLNT   := claves_clnt.c
RPC_XDR    := claves_xdr.c
RPC_SVC    := claves_svc.c

.PHONY: all clean

all: gen_rpc $(SERVER_BIN) $(LIBRARY) $(CLIENT_BIN)

# ————— Generación RPC —————
gen_rpc: $(RPC_HDR) $(RPC_CLNT) $(RPC_XDR) $(RPC_SVC)

# 1) client‐stubs + XDR + header
$(RPC_HDR) $(RPC_CLNT) $(RPC_XDR): $(XFILE)
	$(RPCGEN) -C $<

# 2) service‐stubs **sin** main()
$(RPC_SVC): $(XFILE)
	$(RPCGEN) -m $< -o $@

# ————— Ejecutable del servidor —————
$(SERVER_BIN): $(SERVER_SRC:.c=.o) $(RPC_SVC:.c=.o) $(RPC_XDR:.c=.o)
	$(CC) -o $@ $^ $(LDFLAGS)

# ————— Biblioteca proxy + client‐stubs —————
$(LIBRARY): $(PROXY_SRC:.c=.o) $(RPC_CLNT:.c=.o) $(RPC_XDR:.c=.o)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

# ————— Ejecutable del cliente —————
$(CLIENT_BIN): $(CLIENT_SRC:.c=.o) $(LIBRARY)
	$(CC) -o $@ $< -L. -lclaves $(LDFLAGS)

# ————— Reglas generales de compilación —————
%.o: %.c $(RPC_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

# ————— Limpiar todo —————
clean:
	rm -f \
	  $(SERVER_BIN) $(CLIENT_BIN) $(LIBRARY) \
	  *.o servidor/*.o cliente/*.o \
	  $(RPC_HDR) $(RPC_CLNT) $(RPC_XDR) $(RPC_SVC)

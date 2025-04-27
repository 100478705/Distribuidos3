CC = gcc
RPCGEN = rpcgen -M -C
CFLAGS = -Wall -g -fPIC -pthread -I. -I/usr/include/tirpc
LDFLAGS = -pthread -ltirpc

CLIENT_DIR = cliente
SERVER_DIR = servidor

RPC_X = rpc.x
RPC_H = rpc.h
RPC_CLNT = rpc_clnt.c
RPC_SVC  = rpc_svc.c
RPC_XDR  = rpc_xdr.c

# --- reglas -------------------------------------------------
.PHONY: all clean
all: clienteEj servidorEj

$(RPC_H) $(RPC_CLNT) $(RPC_SVC) $(RPC_XDR): $(RPC_X)
	$(RPCGEN) -h -o $(RPC_H) $<
	$(RPCGEN) -l -o $(RPC_CLNT) $<
	$(RPCGEN) -m -o $(RPC_SVC)  $<
	$(RPCGEN) -c -o $(RPC_XDR)  $<

proxy-rpc.o: proxy-rpc.c $(RPC_H)
	$(CC) $(CFLAGS) -c $<

rpc_clnt.o: $(RPC_CLNT) $(RPC_H)
	$(CC) $(CFLAGS) -c $<

rpc_xdr.o: rpc_xdr.c $(RPC_H)
	$(CC) $(CFLAGS) -Wno-unused-variable -c $< -o $@

libclaves.so: proxy-rpc.o rpc_clnt.o rpc_xdr.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

cliente/app-cliente.o: $(CLIENT_DIR)/app-cliente.c $(RPC_H)
	$(CC) $(CFLAGS) -c $< -o $@

clienteEj: cliente/app-cliente.o libclaves.so
	$(CC) -o $@ $< -L. -lclaves -Wl,-rpath,'$$ORIGIN' $(LDFLAGS)

servidor/servidor-rpc.o: $(SERVER_DIR)/servidor-rpc.c $(RPC_H)
	$(CC) $(CFLAGS) -c $< -o $@

servidor/claves.o: $(SERVER_DIR)/claves.c
	$(CC) $(CFLAGS) -c $< -o $@

rpc_svc.o: $(RPC_SVC) $(RPC_H)
	$(CC) $(CFLAGS) -c $<

servidorEj: servidor/servidor-rpc.o servidor/claves.o rpc_svc.o rpc_xdr.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f cliente/*.o servidor/*.o *.o \
	      $(RPC_H) $(RPC_CLNT) $(RPC_SVC) $(RPC_XDR) \
	      libclaves.so clienteEj servidorEj

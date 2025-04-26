CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -lrt -lpthread

# Archivos fuente
CLIENT_SRC = cliente/app-cliente.c
SERVER_SRC = servidor/servidor-rpc.c servidor/claves.c
PROXY_SRC = proxy-rpc.c

# Archivos objeto
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
PROXY_OBJ = $(PROXY_SRC:.c=.o)

# Ejecutables
CLIENT_BIN = clienteEJ
SERVER_BIN = servidorEJ

# Biblioteca dinámica
LIBRARY = libclaves.so

all: $(CLIENT_BIN) $(SERVER_BIN) $(LIBRARY)

$(CLIENT_BIN): $(CLIENT_OBJ) $(LIBRARY)
	$(CC) -o $@ $(CLIENT_OBJ) -L. -lclaves -Wl,-rpath=. $(LDFLAGS)

$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) -o $@ $(SERVER_OBJ) $(LDFLAGS)

$(LIBRARY): $(PROXY_OBJ)
	$(CC) -shared -o $@ $(PROXY_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(PROXY_OBJ) $(CLIENT_BIN) $(SERVER_BIN) $(LIBRARY)

.PHONY: all clean






Para este nuevo makefile, necesito que app-cliente se compile por un lado, servidor-rpc y claves.c por otro, proxy-rpc como librería compartida por ambos, y que metas los archivos generados por el rpc compilados con su 
correspondiente parte (si va con el servidor, que se compile con el servidor, si va con cliente, que se compile con cliente) y luego generame los Ejecutables
basate en este makefile que te voy a pasar para saber cuales eran las agrupaciones antes de implementar rpc y decide donde va cada cosa nueva que se genere para que haya comunicacion entre las partes que tocan

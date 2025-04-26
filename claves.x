/* claves.x */

struct Coord {
    int x;
    int y;
};

/* Argumentos para set_value y modify_value */
struct set_args {
    int    key;
    string value1<256>;
    int    N_value2;
    double V_value2<100>;
    Coord  value3;
};

/* Para get_value */
struct get_args {
    int key;
};

/* Para delete_key */
struct delete_args {
    int key;
};

/* Para exist */
struct exist_args {
    int key;
};

program CLAVES_PROG {
    version CLAVES_VERS {
        int        rpc_set_value(set_args)     = 1;
        set_args   rpc_get_value(get_args)     = 2;
        int        rpc_delete_key(delete_args) = 3;
        int        rpc_modify_value(set_args)  = 4;
        int        rpc_exist(exist_args)       = 5;
        int        rpc_destroy(void)           = 6;
    } = 1;
} = 0x31234567;


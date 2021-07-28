#include "inst.h"

int main(){
    
    PGconn *conn = PQconnectdb("user=admin password=admin dbname=papeleria");

    if (PQstatus(conn) == CONNECTION_BAD) {
        
        fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    
    //registrarCliente(conn);
    //registrarProveedor(conn);
    //registrarProducto(conn);
    consumir(conn);
    
    return 0;
}

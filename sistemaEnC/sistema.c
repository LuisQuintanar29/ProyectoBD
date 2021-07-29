#include "inst.h"

int main(){
    
    PGconn *conn = PQconnectdb("user=admin password=admin dbname=papeleria");

    if (PQstatus(conn) == CONNECTION_BAD) 
    {    
        fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    switch (menuInicio())
    {
    case 1:
        registrarCliente(conn);
        break;
    case 2:
        registrarProveedor(conn);
        break;
    case 3:
        registrarProducto(conn);
        break;
    case 4:
        consumir(conn);
        break;
    case 5:
        printSELECT(conn,"SELECT * FROM view_inventario");
        break;
    default:
        printSELECT(conn,"SELECT * FROM consumo");
        break;
    }
    
    return 0;
}

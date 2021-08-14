#include "inst.h"


int main(){

    PGconn *conn = PQconnectdb("user=admin password=admin dbname=papeleria");

    if (PQstatus(conn) == CONNECTION_BAD) 
    {    
        fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    while (1)
    {
        switch (menuInicio())
        {
        case 1:
            registrarCliente(conn);
            do_something(conn,"commit;");
            break;
        case 2:
            registrarProveedor(conn);
            do_something(conn,"commit;");
            break;
        case 3:
            registrarProducto(conn);
            do_something(conn,"commit;");
            break;
        case 4:
            consumir(conn);
            do_something(conn,"commit;");
            break;
        case 5:
            printSELECT(conn,"SELECT * FROM view_inventario");
            break;
        case 6:
            mostrarCompras(conn);
            break;
        case 7:
            printSELECT(conn,"SELECT * FROM view_cliente ORDER BY RFC");
            break;
        case 8:
            printSELECT(conn,"SELECT * FROM view_proveedor ORDER BY Razon_Social");
            break;
        case 9:
            return 0;
            break;
        }
    };
    
    
    return 0;
}

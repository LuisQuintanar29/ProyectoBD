#include "inst.h"

int main(){
    
    PGconn *conn = PQconnectdb("user=admin password=admin dbname=papeleria");

    if (PQstatus(conn) == CONNECTION_BAD) {
        
        fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    
    registrarCliente(conn);
    /*
    printSELECT(conn, "SELECT * FROM tipoproducto;");
    printSELECT(conn, "SELECT * FROM compañia;");
    printSELECT(conn, "SELECT * FROM preciorecarga;");
    printSELECT(conn, "SELECT * FROM tipoimpresion;");
    printSELECT(conn, "SELECT * FROM tamañohoja;");
    PQfinish(conn);
    */

    return 0;
}

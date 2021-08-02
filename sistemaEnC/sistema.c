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
    case 6:
        printSELECT(conn,"SELECT NoVenta, fechaVenta as fecha, cantidadArticulo as cantidad,"
                        "precioArticulo as precio,total,"
                        "(SELECT (Compañia,PrecioRecarga) FROM recarga as r WHERE c.idRecarga = r.idRecarga) as recarga,"
                        "(SELECT (TamañoHoja,TipoImpresion) FROM impresion as i WHERE c.idImpresion = i.idImpresion) as impresion,"
                        "(SELECT (codigoBarras,marca,descripcionprod) FROM producto as p WHERE c.codigoBarras = p.codigoBarras) as producto "
                        " FROM consumo as c ORDER BY fecha;");
        break;
    case 7:
        printSELECT(conn,"SELECT * FROM view_cliente ORDER BY RFC");
        break;
    case 8:
        printSELECT(conn,"SELECT * FROM view_proveedor ORDER BY Razon_Social");
        break;
    }
    
    return 0;
}

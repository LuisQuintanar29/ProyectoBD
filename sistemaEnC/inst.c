#include "inst.h"

// Muestra mensaje de error si se presenta y termina 
void do_exit(PGconn *conn, PGresult *res)
{
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    
    PQclear(res);
    PQfinish(conn);

    exit(1);
}

// Realiza alguna accion en la base de datos
void do_something(PGconn *conn, char * instruction)
{
    PGresult *res = PQexec(conn, instruction);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }

    PQclear(res);
}

// Muestra el resultado de realizar consultas SELECT
void printSELECT(PGconn *conn, char * string)
{
    int rows, columns;
    PGresult *res = PQexec(conn, string);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) 
    {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn,res);
    }    
    
    rows = PQntuples(res);
    columns = PQnfields(res);

    for (size_t i = 0; i < columns; i++)
    {
        printf("%24s",PQfname(res,i));
    }
    printf("\n");
    
    
    for(int i=0; i<rows; i++)
    {
        printf("|");
        for(int j = 0; j< columns;j++)
        {
            printf("%24s|", PQgetvalue(res, i, j));
        }
        printf("\n");
    }    
    printf("\n");

    PQclear(res);
}

// Limpia el buffer de entrada
void vacia_buffer()
{
    int fdflags;
    fdflags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, fdflags | O_NONBLOCK);
    while (getchar()!=EOF);
    fcntl(STDIN_FILENO, F_SETFL, fdflags);
}

// Lee una cadena y devuelve la cadena con un maximo tam
void leerCadena(char ** cadena, char * msg, int MAXSIZE)
{
    printf("%s",msg);
    fgets(*cadena,MAXSIZE+1,stdin);
    strtok(*cadena,"\n");
    vacia_buffer();
    *cadena = (char*) realloc(*cadena,strlen(*cadena)*sizeof(char));
}

// Verifica si lo ingresado en la cadena es un numero
int esNumero(char * cadena)
{
    for(size_t i = 0; i < strlen(cadena);i++)
    {
        if (cadena[i] < '0' || cadena[i] > '9')
            return 0;
    }
    return 1;
}

// Unimos la informacion del Cliente o Proveedor para la base de datos
void UnirInfo(char **instruccion,char * clave, char * nombre, char ** domicilio,char * inicio)
{
    *instruccion = (char*) realloc(*instruccion,sizeof(char)*370);

    strcpy(*instruccion,inicio);
    strcat(*instruccion,clave);
    strcat(*instruccion,"','");
    strcat(*instruccion,nombre);
    strcat(*instruccion,"','");
    strcat(*instruccion,domicilio[0]);

    strcat(*instruccion,"','");
    strcat(*instruccion,domicilio[1]);
    strcat(*instruccion,"','");
    strcat(*instruccion,domicilio[2]);
    strcat(*instruccion,"',");

    strcat(*instruccion,domicilio[3]);
    strcat(*instruccion,",");
    strcat(*instruccion,domicilio[4]);
    strcat(*instruccion,")");

    *instruccion = (char*) realloc(*instruccion,sizeof(char)*strlen(*instruccion));
}

// Almacena los datos del cliente y los inserta en la tabla
void registrarCliente(PGconn *conn)
{
    char * RFC = (char*) malloc(sizeof(char)*14);
    char * nombre = (char*) malloc(sizeof(char)*71);
    char * email = (char*) malloc(sizeof(char)*51);
    // domicilio = [Estado,Colonia,Calle,codigo Postal, Numero Oficial]
    char * domicilio[5];
    domicilio[0] = (char*) malloc(sizeof(char)*71);
    domicilio[1] = (char*) malloc(sizeof(char)*71);
    domicilio[2] = (char*) malloc(sizeof(char)*71);
    domicilio[3] = (char*) malloc(sizeof(char)*6);
    domicilio[4] = (char*) malloc(sizeof(char)*6);

    char iniEmail[] = "INSERT INTO email VALUES ('";
    char inicio[] = "INSERT INTO cliente VALUES ('";
    char * inst = (char*) malloc(sizeof(char));
    char * instEmail = (char*) malloc(sizeof(char)*98);

    do
    {
        leerCadena(&RFC,"Ingrese su RFC \n",13);
        if(strlen(RFC) != 13) printf("Ingrese un RFC valido de 13 caracteres\n");
    }while(strlen(RFC) != 13);
    do
    {
        leerCadena(&nombre,"Ingrese su Nombre \n",70);
    } while (nombre[0] == '\n');
    do
    {
        leerCadena(&domicilio[0],"Ingrese su Estado \n",70);
    } while (domicilio[0][0] == '\n');
    do
    {
        leerCadena(&domicilio[1],"Ingrese su Colonia \n",70);
    } while (domicilio[1][0] == '\n');
    do
    {
        leerCadena(&domicilio[2],"Ingrese su Calle \n",70);
    } while (domicilio[2][0] == '\n');
    do
    {
        leerCadena(&domicilio[3],"Ingrese su Codigo Postal \n",5);
    } while (!esNumero(domicilio[3]) || domicilio[3][0] == '\n');
    do
    {
        leerCadena(&domicilio[4],"Ingrese su Numero Oficial \n",5);
    } while (!esNumero(domicilio[4]) || domicilio[4][0] == '\n');
    do
    {
        leerCadena(&email,"Ingrese su email\n",50);
    } while (email[0] == '\n');

    UnirInfo(&inst,RFC,nombre,domicilio,inicio);

    strcpy(instEmail,iniEmail);
    strcat(instEmail,email);
    strcat(instEmail,"','");
    strcat(instEmail,RFC);
    strcat(instEmail,"')");
    instEmail = (char*) realloc(instEmail,sizeof(char)*strlen(instEmail)+1);
    
    printf("%s-%ld\n",inst,strlen(inst));
    printf("%s-%ld\n",instEmail,strlen(instEmail));

    do_something(conn,inst);
    do_something(conn,instEmail);
}

// Almacena los datos del proveedor y los inserta en la tabla
void registrarProveedor(PGconn *conn)
{
    char * razonSocial = (char*) malloc(sizeof(char)*31);
    char * nombre = (char*) malloc(sizeof(char)*71);
    // domicilio = [Estado,Colonia,Calle,codigo Postal, Numero Oficial]
    char * domicilio[5];
    domicilio[0] = (char*) malloc(sizeof(char)*71);
    domicilio[1] = (char*) malloc(sizeof(char)*71);
    domicilio[2] = (char*) malloc(sizeof(char)*71);
    domicilio[3] = (char*) malloc(sizeof(char)*6);
    domicilio[4] = (char*) malloc(sizeof(char)*6);
    char * telefono = (char*) malloc(sizeof(char)*11);

    char iniTelefono[] = "INSERT INTO telefono VALUES (";
    char inicio[] = "INSERT INTO proveedor VALUES ('";
    char * inst = (char*) malloc(sizeof(char));
    char * instTelefono = (char*) malloc(sizeof(char)*75);

    do
    {
        leerCadena(&razonSocial,"Ingrese su Razon Social\n",30);
    }while(razonSocial[0] == '\n');
    do
    {
        leerCadena(&nombre,"Ingrese su Nombre \n",70);
    } while (nombre[0] == '\n');
    do
    {
        leerCadena(&domicilio[0],"Ingrese su Estado \n",70);
    } while (domicilio[0][0] == '\n');
    do
    {
        leerCadena(&domicilio[1],"Ingrese su Colonia \n",70);
    } while (domicilio[1][0] == '\n');
    do
    {
        leerCadena(&domicilio[2],"Ingrese su Calle \n",70);
    } while (domicilio[2][0] == '\n');
    do
    {
        leerCadena(&domicilio[3],"Ingrese su Codigo Postal \n",5);
    } while (!esNumero(domicilio[3]) || domicilio[3][0] == '\n');
    do
    {
        leerCadena(&domicilio[4],"Ingrese su Numero Oficial \n",5);
    } while (!esNumero(domicilio[4]) || domicilio[4][0] == '\n');
    do
    {
        leerCadena(&telefono,"Ingrese su Telefono \n",10);
    } while (strlen(telefono) != 10 || !esNumero(telefono));


    UnirInfo(&inst,razonSocial,nombre,domicilio,inicio);

    strcpy(instTelefono,iniTelefono);
    strcat(instTelefono,telefono);
    strcat(instTelefono,",'");
    strcat(instTelefono,razonSocial);
    strcat(instTelefono,"')");
    instTelefono = (char*) realloc(instTelefono,sizeof(char)*strlen(instTelefono)+1);
    
    printf("%s-%ld\n",inst,strlen(inst));
    printf("%s-%ld\n",instTelefono,strlen(instTelefono));

    do_something(conn,inst);
    do_something(conn,instTelefono);
}

// Unimos la informacion del producto para insertar en la tabla Producto
void unirInfoProducto(char ** instruccion, char * codigoBarras, char * precioVenta, char * marca, char * descripcion, char * idTipoProducto, char * inicio)
{
    *instruccion = (char*) realloc(*instruccion,sizeof(char)*115);

    strcpy(*instruccion,inicio);
    strcat(*instruccion,codigoBarras);
    strcat(*instruccion,",");
    strcat(*instruccion,precioVenta);
    strcat(*instruccion,",'");
    strcat(*instruccion,marca);
    strcat(*instruccion,"','");
    strcat(*instruccion,descripcion);
    strcat(*instruccion,"',");
    strcat(*instruccion,idTipoProducto);
    strcat(*instruccion,")");

    *instruccion = (char*) realloc(*instruccion,sizeof(char)*strlen(*instruccion));
}

// Unimos la informacion del producto para insertar en la tabla Inventario
void unirInfoInventario(char ** instruccion,char * codigoBarras, char * precioCompra, char * fechaCompra, char * stock, char * inicio)
{
    *instruccion = (char*) realloc(*instruccion,sizeof(char)*75);

    strcpy(*instruccion,inicio);
    strcat(*instruccion,codigoBarras);
    strcat(*instruccion,",");
    strcat(*instruccion,precioCompra);
    strcat(*instruccion,",'");
    strcat(*instruccion,fechaCompra);
    strcat(*instruccion,"',");
    strcat(*instruccion,stock);
    strcat(*instruccion,")");

    *instruccion = (char*) realloc(*instruccion,sizeof(char)*strlen(*instruccion));
}

// Obtiene la cantidad de filas que existen
int obtenerNumeroFilas(PGconn *conn,char * cad)
{
    PGresult *res = PQexec(conn,cad);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) 
    {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn,res);
    } 

    int n = PQntuples(res);    
    PQclear(res);
    return n;
}

// Almacena los datos del cliente y los inserta en la tabla
void registrarProducto(PGconn *conn)
{
    char * codigoBarras = (char*) malloc(sizeof(char)*13);
    char * precioCompra = (char*) malloc(sizeof(char)*11);
    char * fechaCompra = (char*) malloc(sizeof(char)*11);
    char * stock = (char*) malloc(sizeof(char)*7);

    char * precioVenta = (char*) malloc(sizeof(char)*16);
    char * marca = (char*) malloc(sizeof(char)*16);
    char * descripcion = (char*) malloc(sizeof(char)*31);
    char * idTipoProducto = (char*) malloc(sizeof(char)*2);

    char iniInventario[] = "INSERT INTO inventario VALUES (";
    char iniProducto[] = "INSERT INTO producto VALUES (";
    char * instInventario;
    char * instProducto;

    do{
        leerCadena(&codigoBarras,"Ingrese el Codigo de Barras\n",12);
    }while(!esNumero(codigoBarras) || strlen(codigoBarras) != 12);
    do{
        leerCadena(&precioCompra,"Ingrese el Precio de Compra\n",10);
    }while(!esNumero(precioCompra) || precioCompra[0] == '\n' );
    do{
        leerCadena(&fechaCompra,"Ingrese la fecha de compra dd-mm-aaa\n",10);
    }while(fechaCompra[0] == '\n' );
    do{
        leerCadena(&stock,"Ingrese la Cantidad de Ejemplares en Bodega\n",6);
    }while(!esNumero(stock) || stock[0] == '\n' );

    do{
        leerCadena(&precioVenta,"Ingrese el Precio de Venta\n",15);
    }while(!esNumero(precioVenta) || precioVenta[0] == '\n');
    do{
        leerCadena(&marca,"Ingrese la Marca del producto\n",15);
    }while(marca[0] == '\n');
    do
    {
        leerCadena(&descripcion,"Ingrese la Descripcion del producto\n",30);
    } while (descripcion[0] == '\n');
    do
    {
        printf("\nid   Tipo de Producto\n");
        printSELECT(conn,"SELECT * FROM tipoproducto;");
        leerCadena(&idTipoProducto,"Ingrese el id del Tipo de Producto\n",1);
    } while (!esNumero(idTipoProducto) || idTipoProducto[0] == '\n'  || obtenerNumeroFilas(conn,"SELECT * FROM tipoproducto;") +48 < idTipoProducto[0] || idTipoProducto[0] < '1');
    
    unirInfoInventario(&instInventario,codigoBarras,precioCompra,fechaCompra,stock,iniInventario);
    unirInfoProducto(&instProducto,codigoBarras,precioVenta,marca,descripcion,idTipoProducto,iniProducto);

    printf("%s-%ld\n",instInventario,strlen(instInventario));
    printf("%s-%ld\n",instProducto,strlen(instProducto));

    do_something(conn,instInventario);
    do_something(conn,instProducto);
}

// Obtiene las cadenas de caracteres de los codigos de barras
char ** obtenElementos(PGconn *conn,char * cad)
{
    int n;
    char ** elementos;
    PGresult *res = PQexec(conn,cad);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) 
    {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn,res);
    } 

    n = PQntuples(res);
    elementos = (char**) malloc(sizeof(char*)*n);

    for (size_t i = 0; i < n; i++)
    {
        elementos[i]= (char*) malloc(sizeof(char)*strlen(PQgetvalue(res,i,0))+1);
        strcpy(elementos[i],PQgetvalue(res,i,0));
    }
    
    PQclear(res);
    return elementos;
}

// Verifica la existencia del codigo de barras
int existeCodigoBarras(PGconn *conn, char ** codigoBarras)
{
    char ** codigos = obtenElementos(conn,"SELECT codigobarras FROM inventario;");

    for (int i = 0; i < obtenerNumeroFilas(conn,"SELECT codigobarras FROM inventario;"); i++)
    {
        if( strcmp(codigos[i],*codigoBarras) == 0 )
        {
            return 1;
        }
    }

    return 0;
}

// Realizamos compras
void consumir(PGconn *conn)
{
    char * fechaVenta = (char*) malloc(sizeof(char)*11);
    char * cantidadArticulo = (char*) malloc(sizeof(char)*7);
    char * precio = (char*) malloc(sizeof(char)*16);
    char * codigoBarras = (char*) malloc(sizeof(char)*13);
    char * idImpresion = (char*) malloc(sizeof(char)*3);
    char * idRecarga = (char*) malloc(sizeof(char)*3);

    consumirProducto(conn,&codigoBarras,&precio,&cantidadArticulo);
    char inst[60] = "SELECT * FROM view_compra_prod WHERE codigo = ";
    strcat(inst,codigoBarras);

    char inicio[200] = "INSERT INTO CONSUMO (FechaVenta,CantidadArticulo,PrecioArticulo,CodigoBarras) VALUES (CURRENT_DATE,";
    strcat(inicio,cantidadArticulo);
    strcat(inicio,",");
    strcat(inicio,precio);
    strcat(inicio,",");
    strcat(inicio,codigoBarras);
    strcat(inicio,")");

    //do_something(conn,inicio);

}

// Verifica si numero1 es menor que numero2
int esNumeroMenor( char * numero1, char * numero2 )
{
    if( strlen(numero1) < strlen(numero2) ) return 1;
    if( strlen(numero1) > strlen(numero2) ) return 0;

    for (size_t i = 0; i < strlen(numero1); i++)
    {
        if( numero1[i] < numero2[i] ) return 1;
        if( numero1[i] > numero2[i] ) return 0;
    }

    return 0;
}

// 
void consumirProducto(PGconn *conn, char **codigoBarras, char ** precio, char ** cantidadArticulo )
{
    printSELECT(conn,"SELECT * FROM view_compra_prod");
    do{
        leerCadena(codigoBarras,"Ingrese el Codigo de Barras del producto en existencia\n",12);
    }while(!esNumero(*codigoBarras) || strlen(*codigoBarras) != 12 || !existeCodigoBarras(conn,codigoBarras));


    char inst[70] = "SELECT precio FROM view_compra_prod WHERE codigo = ";
    strcat(inst,*codigoBarras);
    *precio = obtenElementos(conn,inst)[0];


    strcpy(inst,"SELECT inventario FROM view_compra_prod WHERE codigo = ");
    strcat(inst,*codigoBarras);

    do
    {
        leerCadena(cantidadArticulo,"Ingrese la Cantidad de Articulos que desea\n",7);
    } while (!esNumero(*cantidadArticulo) || !esNumeroMenor(*cantidadArticulo,obtenElementos(conn,inst)[0]));
    
}

// 
void consumirRecarga(PGconn *conn, char **idRecarga, char ** precio )
{
    printSELECT(conn,"SELECT * FROM view_compra_reca");
}

// 
void consumirImpresion(PGconn *conn, char ** idImpresion, char ** precio)
{
    printSELECT(conn,"SELECT * FROM view_compra_impr");
}

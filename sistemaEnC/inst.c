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
    
    for(int i=0; i<rows; i++)
    {
        for(int j = 0; j< columns;j++)
        {
            printf("%s ", PQgetvalue(res, i, j));
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

// Unimos la informacion para la base de datos
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

    //do_something(conn,inst);
    //do_something(conn,instEmail);
}

// Almacena los datos del cliente y los inserta en la tabla
void registrarProducto(PGconn *conn)
{
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

    //do_something(conn,inst);
    //do_something(conn,instTelefono);
}

// Realizamos compras
void comprar(PGconn *conn)
{

}
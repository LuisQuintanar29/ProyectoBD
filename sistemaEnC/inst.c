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

// Unimos la informacion del cliente para la base de datos
void UnirInfoCliente(char **instruccion,char * RFC, char * nombre, char ** domicilio)
{
    char inicio[] = "INSERT INTO cliente VALUES ('";
    *instruccion = (char*) realloc(*instruccion,sizeof(char)*350);

    strcpy(*instruccion,inicio);
    strcat(*instruccion,RFC);
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
    char * domicilio[5];
    domicilio[0] = (char*) malloc(sizeof(char)*71);
    domicilio[1] = (char*) malloc(sizeof(char)*71);
    domicilio[2] = (char*) malloc(sizeof(char)*71);
    domicilio[3] = (char*) malloc(sizeof(char)*6);
    domicilio[4] = (char*) malloc(sizeof(char)*6);

    char iniEmail[] = "INSERT INTO email VALUES ('";
    char * inst = (char*) malloc(sizeof(char));
    char * instEmail = (char*) malloc(sizeof(char)*98);

    leerCadena(&RFC,"Ingrese su RFC \n",13);
    leerCadena(&nombre,"Ingrese su Nombre \n",70);
    leerCadena(&domicilio[0],"Ingrese su Estado \n",70);
    leerCadena(&domicilio[1],"Ingrese su Colonia \n",70);
    leerCadena(&domicilio[2],"Ingrese su Calle \n",70);
    leerCadena(&domicilio[3],"Ingrese su CodigoPostal \n",5);
    leerCadena(&domicilio[4],"Ingrese su Numero Oficial \n",5);
    leerCadena(&email,"Ingrese su email\n",50);

    UnirInfoCliente(&inst,RFC,nombre,domicilio);

    strcpy(instEmail,iniEmail);
    strcat(instEmail,email);
    strcat(instEmail,"','");
    strcat(instEmail,RFC);
    strcat(instEmail,"')");
    instEmail = (char*) realloc(instEmail,sizeof(char)*strlen(instEmail)+1);
    
    printf("%s-%ld\n",inst,strlen(inst));
    printf("%s-%ld\n",instEmail,strlen(instEmail)+1);

    //do_something(conn,inst);
}


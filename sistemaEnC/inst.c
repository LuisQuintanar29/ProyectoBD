#include "inst.h"

// Muestra mensaje de error si se presenta y termina 
void do_exit(PGconn *conn, PGresult *res)
{
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    
    PQclear(res);
    PQfinish(conn);
    exit(1);
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
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

// Lee una cadena y devuelve la cadena con un maximo tam
char * leerCad(int tam, char * msj)
{
    char *cad = (char*) malloc(sizeof(char));
	int i,j=0;

    printf("%s",msj);
	for(i=0;i<=j;i++)
    {
		char letra = getchar();
		cad = (char*) realloc(cad,sizeof(char)*(j+2));
		if(letra!='\n' && i < tam)
        {
			j++;
		}
		else
        {
			letra = '\0';
		}
		*(cad+i) = letra;
	}
    if (i >= tam) vacia_buffer();
	return cad;
}

char * UnirInfoCliente(char * RFC, char * nombre, char ** domicilio)
{
    char inicio[] = "INSERT INTO cliente VALUES ('";
    char * instruccion = malloc(sizeof(char)*31);
    memcpy(instruccion,inicio,strlen(inicio)+1);

    strcat(instruccion,RFC);
    strcat(instruccion,"','");
    strcat(instruccion,nombre);
    strcat(instruccion,"','");
    strcat(instruccion,domicilio[0]);
    strcat(instruccion,"','");
    strcat(instruccion,domicilio[1]);
    strcat(instruccion,"','");
    strcat(instruccion,domicilio[2]);
    strcat(instruccion,"',");
    strcat(instruccion,domicilio[3]);
    strcat(instruccion,",");
    strcat(instruccion,domicilio[4]);
    strcat(instruccion,")");

    return instruccion;
}

// Almacena los datos del cliente y los inserta en la tabla
void registrarCliente(PGconn *conn)
{
    char * RFC;
    char * nombre;
    // Domicilio[Estado,Colonia,Calle,CP,Numero]
    char * domicilio[5];
    char * email;
    char * inst;

    RFC = leerCad(13,"Ingrese su RFC \n");
    nombre = leerCad(70,"Ingrese su Nombre \n");
    domicilio[0] = leerCad(70,"Ingrese su Estado \n");
    domicilio[1] = leerCad(70,"Ingrese su Colonia \n");
    domicilio[2] = leerCad(70,"Ingrese su Calle \n");
    domicilio[3] = leerCad(5,"Ingrese su CodigoPostal \n");
    domicilio[4] = leerCad(5,"Ingrese su Numero Oficial \n");
    email = leerCad(50,"Ingrese su email\n");

    inst = UnirInfoCliente(RFC,nombre,domicilio);

    PGresult *res = PQexec(conn, inst);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }

    PQclear(res);

}


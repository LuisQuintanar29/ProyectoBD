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

// 
void registrarCliente(PGconn *conn)
{
    char * RFC;
    char * nombre;
    // Domicilio[Estado,Colonia,Calle,CP,Numero]
    char * domicilio[5];
    char * email;

    RFC = leerCad(13,"Ingrese su RFC \n");
    nombre = leerCad(70,"Ingrese su Nombre \n");
    domicilio[0] = leerCad(70,"Ingrese su Estado \n");
    domicilio[1] = leerCad(70,"Ingrese su Colonia \n");
    domicilio[2] = leerCad(70,"Ingrese su Calle \n");
    domicilio[3] = leerCad(5,"Ingrese su CodigoPostal \n");
    domicilio[4] = leerCad(5,"Ingrese su Numero Oficial \n");
    email = leerCad(50,"Ingrese su email\n");

    printf("\n\n\n%s-%ld\n",RFC,strlen(RFC));
    printf("\n%s-%ld\n",nombre,strlen(nombre));
    printf("\n%s-%ld\n",domicilio[0],strlen(domicilio[0]));
    printf("\n%s-%ld\n",domicilio[1],strlen(domicilio[1]));
    printf("\n%s-%ld\n",domicilio[2],strlen(domicilio[2]));
    printf("\n%s-%ld\n",domicilio[3],strlen(domicilio[3]));
    printf("\n%s-%ld\n",domicilio[4],strlen(domicilio[4]));
    printf("\n%s-%ld\n",email,strlen(email));

    PGresult *res = PQexec(conn, "DROP TABLE IF EXISTS Cars");
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }

    PQclear(res);
}


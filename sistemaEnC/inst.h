#ifndef INST_INC
#define INST_INC


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libpq-fe.h>


void do_exit(PGconn *conn, PGresult *res);
void printSELECT(PGconn *conn, char * string);
void registrarCliente(PGconn *conn);
void vacia_buffer();
void leerCadena(char ** cadena, char * msg, int MAXSIZE);
void do_something(PGconn *conn, char * instruction);
void UnirInfoCliente(char **instruccion,char * RFC, char * nombre, char ** domicilio);

#endif
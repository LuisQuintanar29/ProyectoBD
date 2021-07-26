#ifndef INST_INC
#define INST_INC


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>


void do_exit(PGconn *conn, PGresult *res);
void printSELECT(PGconn *conn, char * string);
void registrarCliente(PGconn *conn);
void vacia_buffer();
char * leerCad(int tam, char * msj);

#endif
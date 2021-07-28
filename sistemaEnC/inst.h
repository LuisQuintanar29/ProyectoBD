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
void UnirInfo(char **instruccion,char * clave, char * nombre, char ** domicilio,char * inicio);
void unirInfoProducto(char ** instruccion, char * codigoBarras, char * precioVenta, char * marca, char * descripcion, char * idTipoProducto, char * inicio);
void unirInfoInventario(char ** instruccion,char * codigoBarras, char * precioCompra, char * fechaCompra, char * stock, char * inicio);
void registrarProducto(PGconn *conn);
void registrarProveedor(PGconn *conn);
void consumir(PGconn *conn);
int obtenerTipoProducto(PGconn *conn);
void consumirProducto(PGconn *conn, char **codigoBarras, char ** precio );
void consumirRecarga(PGconn *conn, char **idRecarga, char ** precio );
void consumirImpresion(PGconn *conn, char ** idImpresion, char ** precio);

#endif
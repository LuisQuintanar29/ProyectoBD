# ProyectoBD
Proyecto Final de la asignatura de bases de datos

#Instalacion, configuracion y compilacion para sistemaEnC/sistema.c en LINUX
- Necesario tener instalado un compilador para el lenguaje C
- Instamos libpq-dev
	> sudo apt-get install libpq-dev
- Creamos un usuario en la base de datos, en nuestro caso usuario admin
	> sudo service postgresql start
	
	> sudo -i -u postgres psql
	
	> CREATE USER admin WITH PASSWORD 'admin';
	
	> GRANT CONNECT ON DATABASE papeleria TO admin;
	
	> GRANT USAGE ON SCHEMA public TO admin;
	
	> GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO admin;
	
	> GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO admin;
	
	
-Configuramos el pg_hba.confi
	> sudo gedit /etc/postgresql/13/main/pg_hba.conf
- Agregamos
	> local   all             admin	                                md5
- Reiniciamos 
	> sudo service postgresql restart
- Compilamos
	> gcc -Wall sistema.c -o sis.out -I/usr/include/postgresql -lpq -std=c99 inst.c
- Ejecutamos
	> ./sis.out
	



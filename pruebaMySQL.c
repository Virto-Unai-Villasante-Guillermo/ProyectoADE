#include <mysql/mysql.h>
#include <stdio.h>

int main(){
        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW row;

        char *server = "localhost";
        char *user = "unai";
        char *password = "1221";
        char *database = "BME280";

        conn = mysql_init(NULL);

        //Conectar a la base de datos
        if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
                fprintf(stderr,"%s\n",mysql_error(conn));
                return 0;
        }

        //Enviar query
        if(mysql_query(conn,"INSERT INTO Temp_Press_Hum (Temperatura,Presion,Humedad) VALUES ('123','456','789')")!=0) {
                fprintf(stderr,"Error al ejecutar la consulta: %s\n",mysql_error(conn));
                mysql_close(conn);
                return 0;
        }

        printf("Nueva fila añadida exitosamente\n");

        mysql_close(conn);

        return 0;

}



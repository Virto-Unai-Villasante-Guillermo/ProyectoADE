//********************//
//    ___   ___  ____ //
//   / _ | / _ \/ __/ //
//  / __ |/ // / _/   //
// /_/ |_/____/___/   //
//********************//
//!
//! \file: bme280server.c
//! \brief: Read from bme80 server, create local UDP server to send commands and update an online database.
//! \author: Unai Virto & Guillermo Villasante

/********************** Include Files **************************/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "bme280.h"
#include <mysql/mysql.h>


/********************* Type Definitions ************************/
/******************* Constant Definitions **********************/
#define SERVER_PORT 5000
#define BUFFSIZE 1024

/*********** Macros (Inline Functions) Definitions *************/
/******************* Variable Definitions **********************/
/******************** Function Prototypes **********************/

//
// Start of MAIN program
//

int main(void) {
    int sock, num;
    socklen_t len;
    char buffer[BUFFSIZE];
    char str[INET_ADDRSTRLEN];
    char respuesta[BUFFSIZE];
    struct sockaddr_in serveraddr, clientaddr;
    int i;
	int T, P, H; // calibrated values
    int formato_buffer;
    char comando[5];
    int tiene_mins, segs;
    int mins=0;
    
    //Para el acceso a la base de datos
    MYSQL *conn;

    char *server = "localhost";
    char *user = "unai";
    char *password = "1221";
    char *database = "BME280";
    int live = 0;
    int start = 0;
    char mensajelive[1024];
    
    conn = mysql_init(NULL);
    
    // Configure the GPIOs
    //configGPIO();

    // Create UDP socket:
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error while creating socket\n");
        return 1;
    }
    
    //Conectarse al sensor
    i = bme280Init(1, 0x76);
	if (i != 0)
	{
		return -1; // problem - quit
	}
    
    //Conectarse a la base de datos
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        fprintf(stderr,"%s\n",mysql_error(conn));
        return -1;
    }

    len = sizeof(clientaddr);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(SERVER_PORT);

    if (bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) {
        printf("Couldn't bind socket\n");
        return 1;
    }
    printf("Socket Binding OK\n");
    printf("Listening for incoming messages...\n\n");

    // Start listening:
    while (1) {;
        //Borra mensaje anterior
        memset(buffer,'\0',sizeof(buffer));
        memset(comando,'\0',sizeof(comando));
        //Recibe mensaje
        num = recvfrom(sock, buffer, BUFFSIZE, MSG_DONTWAIT,
                        (struct sockaddr *)&clientaddr, &len);
                        
        //Comprobamos si llega una palabra seguida de un número
        formato_buffer = sscanf(buffer,"%s %d",comando,&mins);
        
        if(formato_buffer==2){
            segs=mins*60;
        }
        
        //Lee sensor
        bme280ReadValues(&T, &P, &H);
        //Analiza mensaje recibido
        if (strcmp(comando,"temp")==0) {
            //Construye String con valor de temperatura calibrado
            sprintf(respuesta,"La temperatura es de %.2fºC\n",(float)T/100.0);
            //Manda valor temperatura
            sendto(sock, &respuesta, strlen(respuesta), MSG_CONFIRM,
               (struct sockaddr *)&clientaddr, len);
        }
        else if (strcmp(comando,"hum")==0) {
            sprintf(respuesta,"La humedad relativa es del %.6f%\n",(float)H/1024.0);
            sendto(sock, &respuesta, strlen(respuesta), MSG_CONFIRM,
               (struct sockaddr *)&clientaddr, len);
        }
        else if (strcmp(comando,"press")==0) {
            sprintf(respuesta,"La presión atmósferica es de %.6fhPa\n",(float)P/256.0);
            sendto(sock, &respuesta, strlen(respuesta), MSG_CONFIRM,
               (struct sockaddr *)&clientaddr, len);
        }
        else if (strcmp(comando,"stop")==0){
            //Parar el muestreo
            live=0;
            start=0;
            mysql_close(conn);
        }
        else if (strcmp(comando,"live")==0 | live==1){
            //Flag de que se ha pedido muestrear de forma continua
            start=0;
            live=1;

            //Enviar query
            sprintf(mensajelive,"INSERT INTO Temp_Press_Hum (Temperatura,Presion,Humedad) VALUES ('%.2f','%.6f','%.6f')",(float)T/100.0,(float)P/256.0,(float)H/1024.0);
            if(mysql_query(conn,mensajelive)!=0) {
                fprintf(stderr,"Error al ejecutar la consulta: %s\n",mysql_error(conn));
                mysql_close(conn);
                return 0;
            }
            
            //Delay de 1 segundo
            sleep(1);
        }
        else if (strcmp(comando,"start")==0 | start==1) {
            //Flag de que se ha pedido muestrear durante X minutos
            live=0;
            start=1;
            
            if(segs>0){
                //Enviar query
                sprintf(mensajelive,"INSERT INTO Temp_Press_Hum (Temperatura,Presion,Humedad) VALUES ('%.2f','%.6f','%.6f')",(float)T/100.0,(float)P/256.0,(float)H/1024.0);
                if(mysql_query(conn,mensajelive)!=0) {
                    fprintf(stderr,"Error al ejecutar la consulta: %s\n",mysql_error(conn));
                    mysql_close(conn);
                    return 0;
                }
                segs--;
                //Delay de 1 segundo
                sleep(1);
            }
            else{
                start=0;
            }
        }
        else if (strcmp(comando,"close")==0) {
            break;
        }

        inet_ntop(AF_INET, &(clientaddr.sin_addr), str, INET_ADDRSTRLEN);
        if(strcmp(buffer,"")!=0){
            printf("New message from %s:%d -- %s\n", str, ntohs(clientaddr.sin_port), buffer);
                // Send echo back:
            sendto(sock, &buffer, strlen(buffer), MSG_CONFIRM,
               (struct sockaddr *)&clientaddr, len);
        }
    }

    close(sock);
    mysql_close(conn);
}

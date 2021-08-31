/*
Si voglia realizzare un sistema di Client/Server con un Server e diversi Client; ogni Client invia un vettore di
numeri naturali di dimensione pari a 10, e il Server risponde inviando il valore medio di tali valori. 
Il client stampa a video il valore medio ricevuto dal Server.
Quando il Client vuole terminare, chiude la connessione.
In tal caso, il Server deve automaticamente chiudere anch'esso la connessione con il client.
Si definisca un programma Server e un programma Client che realizzi quanto detto utilizzando i Sockets e i
threads. Si deve supporre che per ogni client connesso, il server crei un thread apposito che gestisce la
comunicazione con il client. Quando il client si disconnette, il thread finisce. Per evitare che il Server debba
invocare la pthread_join, si deve cambiare lo stato di ciascun thread in modo detached. Questo è molto
importante perché in questo modo le risorse del thread vengono rilasciate dal SO alla terminazione del
thread.
Si supponga che il server non termini mai, e dunque e' necessario utilizzare il comando di kill per terminarlo
*/



#include<stdio.h> //INPUT-OUTPUT
#include <string.h> //STRINGHE
#include<stdlib.h> //PROCESSI E LIBRERIA STANDARD
#include<netinet/in.h> //BIG-Little Endian 
#include<arpa/inet.h> //CONVERSIONE IP
#include<unistd.h>  //WRITE-READ-OPEN
#include<sys/socket.h> //SOCKET


#define N 10
#define SIZE 5

typedef struct {
   unsigned int vettore[N];
   float media;
}info;

   void riempi(info* pk,int n){ //(puntatore di tipo info [l'indirizzo], dimensione pacchetto) 
   
      int i;
      
      for(i=0;i<n;i++){
         printf("inserisci un numero");
         scanf("%u",&pk->vettore[i]); // &(pk->vettore[i])____indirizzodi(accesso membro struct__Vettore[i-esimo elemento]) 
      }
      printf("Premi invio per continuare\n");
      while(getchar()!='\n');
 }

//Funzione riempi vettore Client

int main(void){
   int sockfd,result,len,running=1;
   info packet;
   char buffer[SIZE]; //crea una stringa di 5 caratteri
   //config socket
   struct sockaddr_in myAddr; //Lo struct con l' indirizzo del server con cui voglio parlare
   
   //CREO IL SOCKET------------------------------------------------
   sockfd=socket(AF_INET,SOCK_STREAM,0);
   if (sockfd==-1){
      fprintf(stderr,"Creazione socket Fallita");
      exit(EXIT_FAILURE);
   }
   printf("Socket Creato\n");
   
   //lo inizializzo
   myAddr.sin_family=AF_INET;
   myAddr.sin_port=htons(8888);
   myAddr.sin_addr.s_addr=inet_addr("127.0.0.1"); //Struct generale->struct specializzato
   len=sizeof(myAddr);
   //Handshake (connessione al server)
   
   result=connect(sockfd,(struct sockaddr *)&myAddr,len); //numero socket , indirizzo di address castato a puntatore vecchio,dimensioneMyaddr
   if (result==-1){ //0-> ok   //-1 -> fail
      fprintf(stderr,"Connessione fallita");
      exit(EXIT_FAILURE);
   }
   printf("Connesso col server\n");
   
   while (running)
   {
      printf("Vuoi continuare?");
      fgets(buffer,SIZE,stdin);//non posso usare scanf?
                               //salva in buffer dallo stream stdin SIZE (5) char
      if (!strncmp(buffer,"n",1)|| !strncmp(buffer,"N",1)){
         running=0;//POTREI USARE UN BREAK;
      }else{
         //riempie il vettore
         printf("\n");
         riempi(&packet,N); //passo l'indirizzo del pacchetto (altrimenti dovrei riprenderne la copia nel return) 
         //spedisce(scrive sul socket,con lindirizzo del messaggio,una dimensione del messaggio)
         printf("Invio del messaggio\n");
         write(sockfd,&packet,sizeof(info));
         //legge la risposta
         printf("Messaggio inviato\n");
         printf("Lettura risposta\n");
         read(sockfd,&packet,sizeof(info));
         //la stampa
         printf("Risposta Server:%f",packet.media);
         
      }                         
      
   }
   close(sockfd);
   exit(EXIT_SUCCESS);
}


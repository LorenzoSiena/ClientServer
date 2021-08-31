#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>


#define N 10
#define SIZE 5

typedef struct{
   unsigned int vettore[N];
   float media;
}info;



//FUNZIONE MEDIA
float media(info *p,int dim){
   int i;
   float m=0;
   for(i=0;i<dim;i++){
   m+=p->vettore[i];
  
   }
   return (m/dim);
}





//Il lavoro del thread è di fare la media e spedire ritorna un punt void e accetta un punt void
void* thread_job(void * client_socket){ //converto il socket in (int*)
   int c=*(int *)client_socket; //c[client socket]<---[client socket]
   int res;
   info mex;
   
   while (1) 
   {
      res=read(c,&mex,sizeof(info));//LEGGI dal socket
      if (res==0){ //se leggo niente ESCI 
         break;
      }else{
      mex.media=media(&mex,N); //fai la media del messaggio e salva nel messaggio
      write(c,&mex,sizeof(info)); //invia l'indirizzo del messaggio
      
      }
     
   }
 close(c); //chiudi il socket del client
 free(client_socket); //libera lo spazio del socket
pthread_exit(NULL); //esci
}



int main(void){

int S_socketfd ,C_socketfd;
int S_len,C_len;
int result;


int *new_sock; //puntatore a int[?]
struct sockaddr_in S_address,C_address;
pthread_t tid;


// Creo il socket del server
S_socketfd=socket(AF_INET,SOCK_STREAM,0);
if (S_socketfd==-1){
   fprintf(stderr,"Errore creazione Socket");
   exit(EXIT_FAILURE);
}
printf("Socket del server creato\n");
printf("Lo Inizializzo\n");

S_address.sin_family=AF_INET;
S_address.sin_port=htons(8888);
S_address.sin_addr.s_addr=inet_addr("127.0.0.1");
S_len=sizeof(S_address);

printf("Inizializzazione completata\n");
printf("Binding in corso\n");

result=bind(S_socketfd,(struct sockaddr *)&S_address,S_len);
         //collego il socket all'indirizzo'
         //bind(Server_socket,Server_address,sizeof(Server_address));
         
if (result==-1){
   fprintf(stderr,"Binding FALLITO");
   exit(EXIT_FAILURE);
}
printf("Binding eseguita\n");
listen(S_socketfd,SIZE);
printf("Server in ascolto con %d Slot liberi\n",SIZE);

while (1)
{
   //Accetta
   C_len=sizeof(C_address);
   
   C_socketfd=accept(S_socketfd,(struct  sockaddr *)&C_address,&C_len);
   if (C_socketfd==-1){
      fprintf(stderr,"Un client non è riuscito a collegarsi");
      exit(EXIT_FAILURE);
   }
   printf("Connessione accettata\n");
   //"Caro thread,copiati il numero nel tuo foglio""
   // Copio il valore del socket CLient a un int allocato dinamicamente (per il thread)
   new_sock=(int *)malloc(sizeof(int)); //[int*]-->[int]
   *new_sock=C_socketfd; //[int*]-->[C_sock]
   //Creo un thread e gli do il client
   result=pthread_create(&tid,NULL,thread_job,(void*)new_sock);
}
 if (res != 0) {
	  fprintf(stderr, "Creazione del Thread fallita");
	  exit(EXIT_FAILURE);
      }
      pthread_detach(tid);

}


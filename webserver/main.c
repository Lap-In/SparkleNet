#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "socket.h"

/* ============================================================

      SparkleNet:

      Authors : Kévin POULAIN / Maxime POHLE

   ============================================================  */

/* Le processus de traitement d'un client */
int traitement_client(int socket_client){
  /* Attend une seconde */
  sleep(1);

  /* Affichage du message de bienvenue */
  char *message_bienvenue = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 3027\r\n\n__________________________________________▓▓▓\n_________________________________________▓▒▒▒▓▓\n____________________▄▄▄▄▄▄▄▄▄__________▓▒▒▒▒▒▓\n___▓▓▓▓▓____▄█████▓▓▓▓▓▓░░███████▓▒▒▒▒▓▒▓\n____▓▒▓▒▓▓▓██▓█▓▓▓▓▓▓▓░░░▓▓▓▓▓▓▓▓▒▒▒▒▒▓▒▓\n______▓▒▒▒▓▒▒▓▓▓▓▓▓▓▓░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▓▒▓\n______█▓▓▒▒▓▒▒▓▒▒▓▓▓░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▒▓▒▓\n____▄█▓▓█▓▓▓▒▒▒▓▒▓▓░░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▓▒▒▓\n___█▓▓▓█▓▓█▓▓▒▓▒▓▓▓░░░░▓▓▓▓▓▓▓▓▓█▒▒▒▒▓▒▒▒▓\n__█▓▓▓█▓▓█▓▓▓▓▓▓▓▓▓░░░█████████████▒▒▒▒▒▒▓\n__█▓▓▓█▓▓█▓▓▓▓▓▓██████▒▌__▓█_____▓▓▒▒▒▒▒▒▒▓\n_▐█▓▓█▓▓▓█▓▓████▒▒▒▒▒▒▌__▓▓█▄____▓▓▒▒▒▒▒▒▓\n_▐█▓█▓▓▓▓███▒▒▒▒▒▒▒▒▒▒▌__▓▓█████▓▓▒▒▒▒▒▒▓\n__█▓█▓▓██_▅▄██▄▒▒▒▒▒▒▒▐___▓▓█▄_██▓▓▄▅▅▒▒▒▓\n__█▓▓██__▅▄▄▄▌__▀▄▒▒▒▒▒▐___▓▓▓████▓▅▅▄▒▒▒█\n__█▓█_________▓▄___▀▒▒▒▒▒▐____▓▓▓▓▓▓▅▅▄▒▒▒██\n__██___________▓▓█▀█▄▒▒▒▒▒▌________▒▒▒▒▒▒█▓█▌\n_________________▓▓███▒▒▒▒▒▐____▒▒▒██▒▒██▓██▌\n___________________▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒█▓▓██▓▓██▓▌\n____________________▓▒▒▄▒▒▌▒▒▒▒▒▒▒█▓▓▓▓██▓▓▓█\n___________________▓▒▒▒▒▒▐▒▒▒▒▒▒▒█▓███▓▓▓█▓▓█▌\n_____________________▓▓▓▄▀▒▒▒▒▓▓▓█▓▓▓▓▓▓█▓▓▓▓██\n_________________________▓▓▓▓▓▓____█▓▓██▀▀█▓▓▓▓░░█\n______________________________________▀▀__▄█▓▓▓▓▓░░▓█\n_______________________________________▄██▓▓▓▓▓▓░░▓▓█\n_____________________________________██▓▓▓▓▓▓▓▓░░▓▓█\n______________________________________█▓▓▓▓▓▓▓░░░▓▓█\n_______________________________________█▓▓▓▓▓░░░▓▓▓█\n________________________________________█▓▓▓░░░▓▓▓▓█\n__________________________________________██░░░▓▓▓▓█\n_____________________________________________█░▓▓▓█\n_______________________________________________████\n";
  char *erreur = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad Request\r\n";

  FILE *file = fdopen(socket_client,"w+");

  /* Affichage et lecture des messages envoyées par le client */
  char* buffer = malloc(5000*sizeof(char)); 
  char* buffer2 = malloc(5000*sizeof(char));
  char* premiereLigne = malloc(5000*sizeof(char));

  //Récupération première ligne
  if(fgets(buffer2,300,file)==NULL){
    perror("fprintf");
  }

  strcat(premiereLigne,buffer2);
  strcat(buffer,buffer2);

  //Fin de la récupération de la requete
  while(fgets(buffer2,300,file)){
    if(strcmp(buffer2,"\r\n")==0 || strcmp(buffer2,"\n")==0){
      break;
    }
    strcat(buffer,buffer2);
  } 

  // Début de la vérification
  while(strcmp(buffer,"\r\n")!=0 || strcmp(buffer,"\n")!=0){
    printf("debut\n\n");

    /*Vérification de la première ligne que renvoie curl */

    /* Compteur de mots */
    int tmp = 1;

    /*Vérifiaction premier mot*/
    char *premierMot = malloc(4*sizeof(char));
    strncpy(premierMot,buffer,3);
    premierMot[3] = '\0';

    if(strcmp(premierMot,"GET")==0){
      printf("Premier Mot : %s\n",premierMot);

      /* La ligne tronqué */
      char *substract = strchr(premiereLigne, ' ');

      /* Tant que le buffer contient un espace */
      while(substract != NULL) {
        tmp++;
        /* Recopie dans substact  */
        substract = substract + 1;
        substract = strchr(substract, ' ');
      }

      if(tmp != 3){
        if(fprintf(file,erreur)<0){
          perror("fprintf1");
        } 
      }

      /*Vérification du troisième mot*/
      substract = strchr(buffer, ' ');
      substract = substract + 1;
      substract = strchr(substract, ' ');
      substract = substract + 1;

      if(strncmp(substract,"HTTP/1.0",8)==0 || strncmp(substract,"HTTP/1.1",8)==0){
        printf("Troisieme mot bon\n");
      } else {
        if(fprintf(file,erreur)<0){
          perror("fprintf2");
        } 
      }

    /*Affichage du message de bienvenue si tout est bon*/
    if(fprintf(file,message_bienvenue)<0){
      perror("fprintf3");
    }
    /*Fin de la vérification*/
    } else {
      if(fprintf(file,erreur)<0){
        perror("fprintf4");
      }
    }
    printf("buffer : %s",buffer);
  }

  /* Fermeture du fichier */
  fclose(file);
  return 0;
}

int main(int argc, char ** argv)
{
  /* Verification des arguments */
  if (argc>1 && strcmp(argv[1], "-advice") == 0) {
    printf("Don ’t Panic !\n");
    return 42;
  }

  /* Crée le serveur */
  int socket_serveur = creer_serveur(8080);

  /* Variable qui contiendra le descripteur de fichier de la socket */
  int socket_client;

  /* Boucle de connexion (crée un nouveau processus par nouvelle connexion) */
  while(1) {
    /* On accepte un client */
    socket_client = accept(socket_serveur, NULL, NULL);
      /* Initialise la capture du signal d'interruption (Ctrl C) */
    initialiser_signaux();
    /* Traitement d'erreur */
    if (socket_client == -1) {
      perror ("accept");
      return -1;
    }else{
      printf("Client = %d\n", socket_client);
    }

    /* Crée un processus de traitement d'une connexion */
    int pid = fork();
    /* Si dans le processus fils */
    if(pid==0){
      traitement_client(socket_client);
      close(socket_client);
    }
  }

  /* Fermeture des connexions et du serveur */
  close(socket_serveur);

  return 0;
}

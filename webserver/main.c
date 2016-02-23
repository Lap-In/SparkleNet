#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "socket.h"


/* Le processus de traitement d'un client */
int traitement_client(int socket_client){
  /* Attend une seconde */
  sleep(1);

  /* Affichage du message de bienvenue */
  /*char *message_bienvenue = "__________________________________________▓▓▓\n_________________________________________▓▒▒▒▓▓\n____________________▄▄▄▄▄▄▄▄▄__________▓▒▒▒▒▒▓\n___▓▓▓▓▓____▄█████▓▓▓▓▓▓░░███████▓▒▒▒▒▓▒▓\n____▓▒▓▒▓▓▓██▓█▓▓▓▓▓▓▓░░░▓▓▓▓▓▓▓▓▒▒▒▒▒▓▒▓\n______▓▒▒▒▓▒▒▓▓▓▓▓▓▓▓░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▓▒▓\n______█▓▓▒▒▓▒▒▓▒▒▓▓▓░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▒▓▒▓\n____▄█▓▓█▓▓▓▒▒▒▓▒▓▓░░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▓▒▒▓\n___█▓▓▓█▓▓█▓▓▒▓▒▓▓▓░░░░▓▓▓▓▓▓▓▓▓█▒▒▒▒▓▒▒▒▓\n__█▓▓▓█▓▓█▓▓▓▓▓▓▓▓▓░░░█████████████▒▒▒▒▒▒▓\n__█▓▓▓█▓▓█▓▓▓▓▓▓██████▒▌__▓█_____▓▓▒▒▒▒▒▒▒▓\n_▐█▓▓█▓▓▓█▓▓████▒▒▒▒▒▒▌__▓▓█▄____▓▓▒▒▒▒▒▒▓\n_▐█▓█▓▓▓▓███▒▒▒▒▒▒▒▒▒▒▌__▓▓█████▓▓▒▒▒▒▒▒▓\n__█▓█▓▓██_▅▄██▄▒▒▒▒▒▒▒▐___▓▓█▄_██▓▓▄▅▅▒▒▒▓\n__█▓▓██__▅▄▄▄▌__▀▄▒▒▒▒▒▐___▓▓▓████▓▅▅▄▒▒▒█\n__█▓█_________▓▄___▀▒▒▒▒▒▐____▓▓▓▓▓▓▅▅▄▒▒▒██\n__██___________▓▓█▀█▄▒▒▒▒▒▌________▒▒▒▒▒▒█▓█▌\n_________________▓▓███▒▒▒▒▒▐____▒▒▒██▒▒██▓██▌\n___________________▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒█▓▓██▓▓██▓▌\n____________________▓▒▒▄▒▒▌▒▒▒▒▒▒▒█▓▓▓▓██▓▓▓█\n___________________▓▒▒▒▒▒▐▒▒▒▒▒▒▒█▓███▓▓▓█▓▓█▌\n_____________________▓▓▓▄▀▒▒▒▒▓▓▓█▓▓▓▓▓▓█▓▓▓▓██\n_________________________▓▓▓▓▓▓____█▓▓██▀▀█▓▓▓▓░░█\n______________________________________▀▀__▄█▓▓▓▓▓░░▓█\n_______________________________________▄██▓▓▓▓▓▓░░▓▓█\n_____________________________________██▓▓▓▓▓▓▓▓░░▓▓█\n______________________________________█▓▓▓▓▓▓▓░░░▓▓█\n_______________________________________█▓▓▓▓▓░░░▓▓▓█\n________________________________________█▓▓▓░░░▓▓▓▓█\n__________________________________________██░░░▓▓▓▓█\n_____________________________________________█░▓▓▓█\n_______________________________________________████\n";*/
  /* write(socket_client, message_bienvenue, strlen(message_bienvenue)); */
  FILE *file = fdopen(socket_client,"w+");
  /*if(fprintf(file,message_bienvenue)<0){
    perror("fprintf");
  }*/
  /* Affichage et lecture des messages envoyées par le client */
  char *buffer = malloc(300*sizeof(char));
  if(fgets(buffer,300,file)==NULL){
    perror("fgets");
  }

  /* Compteur de mots */
  int tmp = 1;

  /* La ligne tronqué */
  char *substract = strchr(buffer, ' ');

  /* Tant que le buffer contient un espace */
  while(substract != NULL) {
    printf("%s\n", substract);
    tmp++;
    /* Recopie dans substact  */
    strcpy(substract, (substract + 1));
    substract = strchr(substract, ' ');
  }

  printf("%d\n", tmp);

  fclose(file);
  return 0;
}

int main(int argc, char ** argv)
{
  /* Arnold Robbins in the LJ of February ’95 , describing RCS */

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

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

int traitement_client(int socket_client){
  /* Attend une seconde */
  sleep(1);

  /* Affichage du message de bienvenue */
  char *message_bienvenue = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 3027\r\n\n__________________________________________▓▓▓\n_________________________________________▓▒▒▒▓▓\n____________________▄▄▄▄▄▄▄▄▄__________▓▒▒▒▒▒▓\n___▓▓▓▓▓____▄█████▓▓▓▓▓▓░░███████▓▒▒▒▒▓▒▓\n____▓▒▓▒▓▓▓██▓█▓▓▓▓▓▓▓░░░▓▓▓▓▓▓▓▓▒▒▒▒▒▓▒▓\n______▓▒▒▒▓▒▒▓▓▓▓▓▓▓▓░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▓▒▓\n______█▓▓▒▒▓▒▒▓▒▒▓▓▓░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▒▓▒▓\n____▄█▓▓█▓▓▓▒▒▒▓▒▓▓░░░░▓▓▓▓▓▓▓▓█▒▒▒▒▒▒▓▒▒▓\n___█▓▓▓█▓▓█▓▓▒▓▒▓▓▓░░░░▓▓▓▓▓▓▓▓▓█▒▒▒▒▓▒▒▒▓\n__█▓▓▓█▓▓█▓▓▓▓▓▓▓▓▓░░░█████████████▒▒▒▒▒▒▓\n__█▓▓▓█▓▓█▓▓▓▓▓▓██████▒▌__▓█_____▓▓▒▒▒▒▒▒▒▓\n_▐█▓▓█▓▓▓█▓▓████▒▒▒▒▒▒▌__▓▓█▄____▓▓▒▒▒▒▒▒▓\n_▐█▓█▓▓▓▓███▒▒▒▒▒▒▒▒▒▒▌__▓▓█████▓▓▒▒▒▒▒▒▓\n__█▓█▓▓██_▅▄██▄▒▒▒▒▒▒▒▐___▓▓█▄_██▓▓▄▅▅▒▒▒▓\n__█▓▓██__▅▄▄▄▌__▀▄▒▒▒▒▒▐___▓▓▓████▓▅▅▄▒▒▒█\n__█▓█_________▓▄___▀▒▒▒▒▒▐____▓▓▓▓▓▓▅▅▄▒▒▒██\n__██___________▓▓█▀█▄▒▒▒▒▒▌________▒▒▒▒▒▒█▓█▌\n_________________▓▓███▒▒▒▒▒▐____▒▒▒██▒▒██▓██▌\n___________________▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒█▓▓██▓▓██▓▌\n____________________▓▒▒▄▒▒▌▒▒▒▒▒▒▒█▓▓▓▓██▓▓▓█\n___________________▓▒▒▒▒▒▐▒▒▒▒▒▒▒█▓███▓▓▓█▓▓█▌\n_____________________▓▓▓▄▀▒▒▒▒▓▓▓█▓▓▓▓▓▓█▓▓▓▓██\n_________________________▓▓▓▓▓▓____█▓▓██▀▀█▓▓▓▓░░█\n______________________________________▀▀__▄█▓▓▓▓▓░░▓█\n_______________________________________▄██▓▓▓▓▓▓░░▓▓█\n_____________________________________██▓▓▓▓▓▓▓▓░░▓▓█\n______________________________________█▓▓▓▓▓▓▓░░░▓▓█\n_______________________________________█▓▓▓▓▓░░░▓▓▓█\n________________________________________█▓▓▓░░░▓▓▓▓█\n__________________________________________██░░░▓▓▓▓█\n_____________________________________________█░▓▓▓█\n_______________________________________________████\n";
  char *erreur = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad Request\r\n";

  /* La socket cliente */
  FILE *file = fdopen(socket_client,"w+");

  /* Buffer contenant la requête cliente */
  char *requestBuffer = malloc(500 * sizeof(char));

  /* Trigger de validation de la socket */
  char GETRequest = 0;
  char CorrectHTMLVersion = 0;
  int wordCount = 0;
  int lineCount = 1;

  //Récupération première ligne
  if(fgets(requestBuffer, 300, file) == NULL) {
    perror("Erreur lors de la lecture de la requête client");
  }

  while (lineCount < 4) {
    // Si sur la première ligne
    if (lineCount == 1)
    {
      // Buffer contenant un mot
      char *wordsBuffer = malloc(500 * sizeof(char));
      // Récupére le premier mot
      strncpy(wordsBuffer, requestBuffer, 3);
      // Contatene la fin de chaîne par précaution
      strcat(wordsBuffer, "\0");

      // Si le premier mot est "GET"
      if (strcmp(wordsBuffer, "GET") == 0)
        GETRequest = 1;

      // Incremente wordCount
      wordCount++;

      /* La ligne tronqué */
      wordsBuffer = strchr(requestBuffer, ' ');

      /* Tant que le buffer contient un espace */
      while(wordsBuffer != NULL) {
        if (wordCount == 2)
          if(strncmp(wordsBuffer + 1,"HTTP/1.0",8)==0 || strncmp(wordsBuffer + 1,"HTTP/1.1",8)==0)
            CorrectHTMLVersion = 1;

        // Increment le wordCount
        wordCount++;
        /* Recopie dans wordsBuffer  */
        wordsBuffer = wordsBuffer + 1;
        wordsBuffer = strchr(wordsBuffer, ' ');
      }
    }

    //Récupération de le ligne suivante
    if(fgets(requestBuffer, 300, file) == NULL) {
      perror("Erreur lors de la lecture de la requête client");
    }
    // Incremente le compteur de ligne
    lineCount++;
  }

  printf("%d %d %d %d\n", lineCount, wordCount, GETRequest, CorrectHTMLVersion);

  /* Finaly, display the correcte answer */
  if (lineCount == 4 && wordCount == 3 && GETRequest && CorrectHTMLVersion)
    printf("%s\n", message_bienvenue);
  else
    printf("%s\n", erreur);

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

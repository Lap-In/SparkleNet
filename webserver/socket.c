#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#include "socket.h"

/** Crée une socket serveur qui écoute sur toute les interfaces IPv4
de la machine sur le port passé en paramètre . La socket retournée
doit pouvoir être utilisée directement par un appel à accept .
La fonction retourne -1 en cas d ’ erreur ou le descripteur de la
socket créée . */
int creer_serveur(int port)
{
	/** Creation de la socket */

	int socket_serveur;

	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_serveur == -1)
	{
		perror ( " socket_serveur " );
		return -1;
	}
	int optval = 1;
	if (setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval,
		       sizeof(int)) == -1)
	  perror("Can not set SO_REUSEADDR option");
	/* Utilisation de la socket serveur */

	/** Configuration de la socket */

	struct sockaddr_in saddr;

	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons(port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */
	
	if (bind(socket_serveur, (struct sockaddr *) &saddr , sizeof(saddr)) == -1)
	{
		perror ("bind socker_serveur");
		return -1;
	}

	/** Démarer l'attente de connexions */
	if (listen(socket_serveur,10) == -1)
	{
		perror("listen socket_serveur");
		return -1;
	}

	return socket_serveur;
}
void initialiser_signaux(void){
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
      perror ("signal");
    }
}

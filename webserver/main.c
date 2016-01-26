#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "socket.h"

int main(int argc, char ** argv)
{
	/* Arnold Robbins in the LJ of February ’95 , describing RCS */
	if (argc>1 && strcmp(argv[1], "-advice") == 0) {
		printf("Don ’t Panic !\n");
		return 42;
	}
	
	/* Crée le serveur */
	int socket_serveur = creer_serveur(8080);
	/* On accepte un client */
	int socket_client = accept(socket_serveur, NULL, NULL);

	/* Traitement d'erreur */
	if (socket_client == -1)
	{
		perror ("accept");
		return -1;
	}

	/* Attend une seconde */
	sleep(1);

	/* Affichage du message de bienvenue */
	const char *message_bienvenue = "What is a PipBuck?  A PipBuck is a device, worn on a foreleg just above the hoof, issued to every pony in a Stable when they become old enough to start work.  A blending of unicorn pony magic and science, your PipBuck will keep a constant measure of your health and even help administer healing poultices and other medicine, track and organize everything in your saddlepacks, assist in repairs, and keep all manner of notes and maps available at a hooftap.  Plus, it allows you to listen to the Stable broadcast whenever you would like as it can tune into and decrypt just about any radio frequency.  And that’s not all. A pony’s PipBuck generates an E.F.S. (Eyes-Forward Sparkle) that will indicate direction and help gauge whether the ponies or creatures around you are hostile.  And, perhaps most impressively, a PipBuck can magically aid you in a fight for brief periods of time through use of the S.A.T.S. (Stable-Tec Arcane Targeting Spell). Oh, and a feature not to be forgotten: it can keep track of the location of tagged objects or people, including the wearers of other PipBucks.  So if a pony somehow got lost -- don’t ask me how you could get lost in a Stable, but it does happen on occasion -- then anypony who knew the lost pony’s tag could find them instantly.\nIt can even be made to glow like a lamp.\n";
	write(socket_client, message_bienvenue, strlen(message_bienvenue));

	/* Affichage et lecture des messages envoyées par le client */
	while(1) {
		char *buffer = malloc(300*sizeof(char));
		read(socket_client, buffer, 300*sizeof(char));
		write(socket_client, buffer, strlen(buffer));
	}

	/* Fermeture des connexions et du serveur */
	close(socket_client);
	close(socket_serveur);

	return 0;
}

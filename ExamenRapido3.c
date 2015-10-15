#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define CANT_F 5
#define ALRM_AMNT 3



int grabar = 1;
FILE *fd;

void handler (int ids){
	grabar = 0;
	int j;
	sigset_t pending;
	for (j = 1;j<64;j++) {
		sigpending(&pending);
		if (sigismember (&pending, j)) {
 	 		fprintf(fd, "Senal %d bloqueada\n",j);
		}
	}
	fclose(fd);
}

int main(int argc, char const *argv[])
{
	sigset_t todas;
	sigfillset(&todas);
	sigdelset(&todas, SIGALRM);
	sigprocmask(SIG_BLOCK, &todas, NULL);
	if (mkdir("./datos/") == -1){
		printf("Datos existe, borrando contenido\n");
		system("rm -f datos/*");
	}

	struct sigaction action;
	action.sa_handler = handler;
	action.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &action, 0);

	int i;
	for (i=0;i<CANT_F-1;i++){
		system(strcat("touch datos/a",i + '0'));
		fd = open(strcat("datos/a",i + '0'),"w");
		alarm(ALRM_AMNT);
		while(grabar == 1) {
			fprintf(fd, "x");
		}
		grabar = 1;
	}
	system("ls -l datos/*");

	return 0;
}

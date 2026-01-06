#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
int main(void) {
    int pipe1[2];
    int pipefiltro[2];
    int pipenumero[2];
    int giocattoli[10];
    pipe(pipe1);
    pipe(pipefiltro);
    pipe(pipenumero);
    srand(time(NULL));
    if (fork() == 0) {
        close(pipenumero[1]);
        close(pipenumero[0]);
        close(pipefiltro[1]);
        close(pipefiltro[0]);
        close(pipe1[0]);
         for(int i = 0; i < 10; i++) {
             giocattoli[i] = rand() % 20 +1 ;
             write(pipe1[1], &giocattoli[i], sizeof(int));
             printf("Invio giocattolo da %d kg\n",giocattoli[i]);
         }
        close(pipe1[1]);
        exit(0);
    }
    if (fork() == 0) {
        close(pipe1[1]);
        close(pipefiltro[0]);
        close(pipenumero[0]);
        int contatore = 0;
        for (int i=0;i<10;i++) {
            read(pipe1[0], &giocattoli[i], sizeof(int));
            if (giocattoli[i] < 15) {
                write(pipefiltro[1], &giocattoli[i], sizeof(int));
                contatore = contatore + 1;
            }
            printf("Filtro giocattolo %d\n",(i+1));
        }
        write(pipenumero[1], &contatore, sizeof(int));
        close(pipefiltro[1]);
        close(pipe1[1]);
        close(pipenumero[1]);
        exit(0);
    }
    if (fork() == 0) {
        close(pipe1[1]);
        close(pipe1[0]);
        close(pipefiltro[1]);
        close(pipenumero[1]);
        int contatore;
        read(pipenumero[0], &contatore, sizeof(int));
        int totale = 0;
        for (int i=0;i<contatore;i++) {
            read(pipefiltro[0], &giocattoli[i], sizeof(int));
            totale += giocattoli[i];
        }
        printf("Peso totale della slitta %d kg con %d giocattoli",totale,contatore);
        close(pipefiltro[0]);
        close(pipenumero[0]);
        exit(0);
    }
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipefiltro[0]);
    close(pipefiltro[1]);
    close(pipenumero[0]);
    close(pipenumero[1]);
    for(int i=0;i<3;i++) {
        wait(NULL);
    }
    return 0;
}
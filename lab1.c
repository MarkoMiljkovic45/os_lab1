#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define STATUS_FILE "status.txt"
#define OBRADA_FILE "obrada.txt"

void obradi_sigusr1(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);

int broj;

int main(void) {

    struct sigaction act;

    //SIGUSR1
    act.sa_handler = obradi_sigusr1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);

    //SIGTERM
    act.sa_handler = obradi_sigterm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGTERM, &act, NULL);

    //SIGINT
    act.sa_handler = obradi_sigint;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    FILE *status = fopen(STATUS_FILE, "r");
    FILE *obrada = fopen(OBRADA_FILE, "a+");

    printf("Loading broj...\n");
    if (fscanf(status, "%d", &broj) == 0) return 1;
    if (broj == 0) {
        printf("Loading broj from %s...\n", OBRADA_FILE);
        while(fscanf(obrada, "%d", &broj) == 1)
            ;
        broj = sqrt(broj);
    }
    printf("broj: %d loaded successfully\n", broj++);
    fclose(status);

    status = fopen(STATUS_FILE, "w");
    fprintf(status, "0");
    fclose(status);

    printf("Starting process: %d\n", getpid());

    for (;;broj++) {
        fseek(obrada, 0, SEEK_END);
        fprintf(obrada, "\n%d", broj * broj);
        fflush(obrada);
        sleep(5);
    }

    return 0;
}


void obradi_sigusr1(int sig) {
    printf("Processing broj: %d\n", broj);
}

void obradi_sigterm(int sig) {
    printf("Saving broj: %d to %s...\n", broj, STATUS_FILE);
    FILE *status = fopen(STATUS_FILE, "w");
    fprintf(status, "%d", broj);
    printf("Process terminated!\n");
    exit(1);
}

void obradi_sigint(int sig) {
    printf("Process interrupted!\n");
    exit(1);
}
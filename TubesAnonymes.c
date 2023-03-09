// ajouter les directives d'inclusion nécessaires
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {

    int cmpResult;

    char *cat_args[] = {"cat", "scores", NULL};
    char *grep_args[] = {"grep", "Villanova", NULL};
    char *cut_args[] = {"cut", "-b", "1-10", NULL};

    int pipes[4];
    pipe(pipes); // sets up 1st pipe
    pipe(pipes + 2); // sets up 2nd pipe

    if( fork() == 0) {
        
        if( fork() == 0) {

            if( fork() == 0) {
                
                int fp = open("In.txt", O_WRONLY | O_RDONLY);
                dup2(fp, 0);
                dup2(pipes[1], 1);

                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);

                execl("usr/bin/rev", "rev", "In.txt", NULL);
            }
            dup2(pipes[0], 0);
            dup2(pipes[3], 1);

            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);

            execl("usr/bin/rev", "rev", "In.txt", NULL);
        }
        
        dup2(pipes[2], 0);
        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);
        execl("usr/bin/cmp", "cmp", "-","In.txt", NULL);
    }
    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);

    wait(&cmpResult);
    while(wait(NULL) > 0);
    printf("resultat de la comparaison:%d \n", WEXITSTATUS(cmpResult));
    return 0;
}

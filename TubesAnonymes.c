#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {

    int cmpResult;

   
    int pipes[4];
    pipe(pipes); // sets up 1st pipe
    pipe(pipes + 2); // sets up 2nd pipe

    if( fork() == 0) {
        
        if( fork() == 0) {

            if( fork() == 0) {
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

            execl("usr/bin/rev", "rev", NULL);
        }
        
        dup2(pipes[2], 0);

        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);
        execl("usr/bin/cmp", "cmp", "-", "In.txt", "-s" ,NULL);
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

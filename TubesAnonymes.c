// ajouter les directives d'inclusion nécessaires
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    
    if( fork() == 0) {

        if( fork() == 0) {

            if( fork() == 0) {
                int fd[2]; pipe(fd);

                int fp = open("In.txt", O_WRONLY | O_RDONLY);

                dup2(fp,0);
                //dup2(fp, 1);
                close(fp);

                //dup2(fd[0],1);
                //close(fd[0]);

                execlp("rev","rev",NULL, NULL);
                _exit(0);
            }

            //execlp("rev","rev",NULL,NULL);
            _exit(0);
        }
        
        //int fd[2];
        //pipe(fd);
        //dup2(fd[0],0);
        //dup2(fd[1],1);
        execlp("cmp","cmp","-s","-", "In.txt");
        _exit(0);
    }
    while(wait(NULL) > 0);
    printf("fin");
    return 0;
}

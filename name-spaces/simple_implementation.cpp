#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static char child_stack[10000];


static int child_function(void * arg){
    char *cmd[] = {(char *)arg, NULL};
    execvp(cmd[0],cmd);
    perror("execvp");
    return 1;
}


int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage: %s <cmd>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    int flags = SIGCHILD | CLONE_NEWPID;

    pid_t child_pid = clone(child_function,child_stack+sizeof(child_stack),flags,argv[1]);
    if(child_pid<0){
        perror("clone");
        exit(EXIT_FAILURE)
    }

    waitpid(child_pid,NULL,0);
    return 0;
}

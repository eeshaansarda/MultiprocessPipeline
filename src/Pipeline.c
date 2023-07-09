/*
 * Pipeline.c
 *
 *  Created on: 26 Mar 2021
 *      Author: jonl
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "Pipeline.h"


Pipeline* new_Pipeline() {
    Pipeline* this = malloc(sizeof(Pipeline));
    this->funcsSize = 0;
    this->funcs = malloc(sizeof(Function*));
    return this;
}


bool Pipeline_add(Pipeline* this, Function f) {
    this->funcs = realloc(
        this->funcs,
        (this->funcsSize + 1) * sizeof(Function)
    );

    if(this->funcs != NULL) {
        this->funcs[this->funcsSize++] = f;
        return true;
    }
    return false;
}


void Pipeline_execute(Pipeline* this) {
    int N = this->funcsSize;
    int prevFd[2];

    for(int i = 0; i < N; i++) {
        int fd[2];
        pipe(fd);

        int child = fork();
        if(child < 0) {
            perror("Error: Creating children\n");
            exit(-1);
        }
        else if(child > 0) {
            // Parent

            // close the ones not needed
            close(prevFd[1]);

            // Pass the ones needed
            this->funcs[i](prevFd[0], fd[1]);

            // wait for child
            int stat_loc;
            wait(&stat_loc);     //waitpid(child, &stat_loc, 0);
            // printf("Exit code (children [%i]): %i\n", i+1, stat_loc);

            if(i!=0) exit(0); // So that the parent is not exited, it will work if it was just exit(0) but the pipeline will not be freed
            else break;
        }
        // Child goes on

        // prevFd = fd
        prevFd[0] = fd[0];
        prevFd[1] = fd[1];

        if(i == N-1) exit(0); // The last child is useless, the main does its job
    }
}


void Pipeline_free(Pipeline* this) {
    free(this->funcs);
    free(this);
}

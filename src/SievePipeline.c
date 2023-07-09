/*
 * SievePipeline.c
 *
 *  Created on: 26 Mar 2021
 *      Author: jonl
 */


#include "Pipeline.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int max_ints;
static int number_of_primes;

static void generateInts(int input, int output) {
    close(input); // not used in first pipeline stage
    printf("generateInts: process %i, parent %i\n", getpid(), getppid());
    int i = 0;
    for (i = 2; i <= max_ints; i++) {
        write(output, &i, sizeof(int));
    }
    i = -1;
    write(output, &i, sizeof(int)); // write -1 to indicate end
}

/*
 * Treats the first number it gets as a prime number prints it out and then
 * sieves out multiples of that prime by only sending on numbers that are
 * not multiples of that prime to the next stage in the pipeline.
 */
static void sieveInts(int input, int output) {
    int prime, number = 0;
    read(input, &prime, sizeof(int));
    printf("sieveInts: process %i, parent %i: prime = %i\n", getpid(), getppid(), prime);
    if (prime == -1) { // this means we have already got to the end
        write(output, &prime, sizeof(int)); // pass on -1 terminator to child sieve stage
    } else {
        while (number != -1) {
            read(input, &number, sizeof(int));
            if (number % prime != 0) {
                write(output, &number, sizeof(int));
            }
        }
    }
}


static void cleanupExit(Pipeline *p) {
    if (p) {
        Pipeline_free(p);
    }
    exit(-1);
}

int main() {
    scanf("%i %i", &number_of_primes, &max_ints);
    printf("Setting up pipeline to sieve out the first %i prime numbers up to %i\n", number_of_primes, max_ints);

    Pipeline *p = new_Pipeline();
    if (p == NULL) cleanupExit(p);
    if (!Pipeline_add(p, generateInts)) cleanupExit(p);
    for (int i = 0; i < number_of_primes; i++) {
        if (!Pipeline_add(p, sieveInts)) cleanupExit(p);
    }
    Pipeline_execute(p);

    Pipeline_free(p);
    return 0;
}

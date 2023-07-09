/*
 * TestPipeline.c
 *
 * Used TestStack.c as starting point.
 *
 */

#include <stdio.h>
#include "Pipeline.h"
#include <unistd.h>
#include <stdlib.h>
#include "myassert.h"



/*
 * The stack to use during tests
 */
static Pipeline* pipeline;

/*
 * The number of tests that succeeded
 */
static int success_count = 0;

/*
 * The total number of tests run
 */
static int total_count = 0;


/*
 * Setup function to run prior to each test
 */

void setup() {
    pipeline = new_Pipeline();
}

/*
 * Teardown function to run after each test
 */
void teardown(){
    Pipeline_free(pipeline);
}

/*
 * This function is called multiple times from main for each user-defined test function
 */
void runTest(int (*testFunction)()) {
    total_count++;
    setup();

    if (testFunction()) success_count++;

    teardown();
}

/*
 * Checks that the Pipeline constructor returns a non-NULL pointer.
 */
int newPipelineIsNotNull() {
    assert(pipeline != NULL);
    return TEST_SUCCESS;
}

/*
 * Checks if can add functions to the pipeline
 */
static void someFunc(int input, int output) {
    close(input);
    close(output);
    printf("sumFunc: process %i, parent %i\n", getpid(), getppid());
}

int canAddFunctions() {
    assert(Pipeline_add(pipeline, someFunc));
    return TEST_SUCCESS;
}


/*
 * Test Functions
 */

static void sendToSecond(int input, int output) {
    close(input);

    int x;
    printf("Enter a number: ");
    scanf("%i", &x);

    write(output, &x, sizeof(int));
}

static void doubleIt(int input, int output) {
    int x;
    read(input, &x, sizeof(int));

    x *= 2;

    write(output, &x, sizeof(int));
}

static void printIt(int input, int output) {
    close(output);
    int x;
    read(input, &x, sizeof(int));
    printf("Received int: %i\n", x);
}

int testFunctions() {
    printf("This should double the number using a pipeline\n");

    Pipeline_add(pipeline, sendToSecond);
    Pipeline_add(pipeline, doubleIt);
    Pipeline_add(pipeline, printIt);
    Pipeline_execute(pipeline);

    printf("Is that the expected output (y/n): ");
    char expectedOut;
    scanf("%s", &expectedOut);
    printf("\n");

    assert(expectedOut == 'y');

    return TEST_SUCCESS;
}


/*
 * Main function for the Pipeline tests which will run each user-defined test in turn.
 */

int main() {

    // Tests
    runTest(newPipelineIsNotNull);
    runTest(canAddFunctions);
    runTest(testFunctions);

    printf("Pipeline Tests complete: %d / %d tests successful.\n----------------\n", success_count, total_count);

}

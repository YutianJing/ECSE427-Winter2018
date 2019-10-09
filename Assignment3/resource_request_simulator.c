#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

sem_t sem;
pthread_mutex_t mutex;

int *numProcess, *numResource, *available;
int **max, **hold, **need;

/*
Simulates resource requests by processes 
*/
void request_simulator(int pr_id, int* request_vector){
    
    for(int i = 0; i < numResource; i++){
        if(request_vector[i] > need[pr_id][i]){
            printf("abort--error, impossible\n");
        }
    }

}

/*
Implementation of isSafe() as described in the slides
*/
int isSafe(){

    int isSafe = 0;
    return isSafe;
}

/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int* request_vector){

    return 0;
}

/*
Simulates processes running on the system.
*/
void* process_simulator(void* pr_id){

}

/*
Simulates a fault occuring on the system.
*/
void* fault_simulator(void* pr_id){

}

/*
Checks for deadlock
*/
void* deadlock_checker(){

}

int main()
{

    //Initialize all inputs to banker's algorithm
    
    //create threads simulating processes (process_simulator)

    //create a thread that takes away resources from the available pool (fault_simulator)

    //create a thread to check for deadlock (deadlock_checker)

    return 0;
}
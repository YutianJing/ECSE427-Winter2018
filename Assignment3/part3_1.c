#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// binary semaphore for mutual exclusion
sem_t mutex;

// variables for bankers algorithms
int *avail, *max, *hold, *need;
int num_process, num_resource;

/**
 * Determines if REQ < Need
 *
 * @param request
 * @param id
 * @return
 */
bool processMatchesNeeds(int *request, int id) {
    for (int i=0; i<num_resource; i++) {
        if (request[i] > need[id*num_resource + i]) {
            return false;
        }
    }
    return true;
}

/**
 * Determines if REQ < Avail
 *
 * @param request
 * @return
 */
bool requestedAmountAvailable(int *request) {
    for (int i=0; i<num_resource; i++) {
        if (request[i] > avail[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Allocate resources
 *
 * @param request
 * @param id
 */
void allocateResources(int *request, int id) {
    // Avail = Avail - REQ
    for (int i=0; i<num_resource; i++) {
        avail[i] = avail[i] - request[i];
    }
    // Hold = Hold + REQ
    for (int i=0; i<num_resource; i++) {
        hold[id*num_resource + i] = hold[id*num_resource + i] + request[i];
    }
    // Need = Need - REQ
    for (int i=0; i<num_resource; i++) {
        need[id*num_resource + i] = need[id*num_resource + i] - request[i];
    }
}

/**
 * Deallocate resources
 *
 * @param request
 * @param id
 */
void deallocateResources(int *request, int id) {
    // Avail = Avail - REQ
    for (int i=0; i<num_resource; i++) {
        avail[i] = avail[i] + request[i];
    }
    // Hold = Hold + REQ
    for (int i=0; i<num_resource; i++) {
        hold[id*num_resource + i] = hold[id*num_resource + i] - request[i];
    }
    // Need = Need - REQ
    for (int i=0; i<num_resource; i++) {
        need[id*num_resource + i] = need[id*num_resource + i] + request[i];
    }
}

/**
 * Free resources when process completes
 *
 * @param id
 */
void freeResources(int id) {
    // Avail = Avail - REQ
    for (int i=0; i<num_resource; i++) {
        avail[i] = avail[i] + hold[id*num_resource + i];
    }
    // Hold = Hold + REQ
    for (int i=0; i<num_resource; i++) {
        hold[id*num_resource + i] = 0;
    }
}

/**
 * Checks if the current state is safe
 *
 * @return
 */
bool isSafe() {
    // initialize finished to false
    int *finished = malloc(num_process * sizeof(bool));
    // make copy of avail
    int *work = malloc(num_resource * sizeof(int));
    for (int i=0; i<num_resource; i++) {
        work[i] = avail[i];
    }
    for (int i=0; i<num_process; i++) {
        // find process where finish=false
        if (!finished[i]) {
            // find process where need < work
            bool enoughResources = true;
            for (int j=0; j<num_resource; j++) {
                if (need[i*num_resource + j] > work[j]) {
                    enoughResources = false;
                }
            }
            // if process meets need < work criteria, release resources, and set to finished
            if (enoughResources) {
                for (int j=0; j<num_resource; j++) {
                    work[j] = work[j] + hold[i*num_resource + j];
                    finished[j] = true;
                }
                // check from start again
                i=0;
            }
        }
    }
    for (int i=0; i<num_process; i++) {
        if (!finished) {
            return false;
        }
    }
    return true;
}

/**
 * Process resource request
 *
 * @param request
 * @param id
 * @return
 */
bool processRequest(int *request, int id) {
    if (!processMatchesNeeds(request, id)) {
        return false;
    }
    if (!requestedAmountAvailable(request)) {
        return false;
    }
    allocateResources(request, id);
    if (isSafe()) {
        return true;
    } else {
        deallocateResources(request, id);
    }
    return false;
}

/**
 * Determines whether process is finished or not
 *
 * @param id
 * @return
 */
bool isProcessFinished(int id) {
    for (int i=0; i<num_resource; i++) {
        if (need[id*num_resource + i] > 0) {
            return false;
        }
    }
    return true;
}

/**
 * Prints the request
 *
 * @param request
 * @param id
 */
void printRequest(int *request, int id) {
    printf("Making resource request [");
    printf("%d", request[0]);
    for (int i=1; i<num_resource; i++){
        printf(", %d", request[i]);
    }
    printf("] for process %d .... ", id);
}

/**
 * A thread that simulates a process
 *
 * @param arg
 * @return
 */
void *processSimulation(void *arg) {
    int id = (intptr_t)arg;
    // repeatedly ask for resources while process not completed
    while (!isProcessFinished(id)) {
        int *request = malloc(num_resource * sizeof(int));
        for (int i=0; i<num_resource; i++) {
            // request between 1 and number of remaining needed resource (or 0 if no more needed resource)
            int remaining_need = need[id*num_resource + i];
            if (remaining_need == 0) {
                request[i] = 0;
            } else {
                request[i] = rand() % remaining_need + 1;
            }
        }
        // make request through bankers algorithm
        sem_wait(&mutex);
        printRequest(request, id);
        bool success = processRequest(request, id);
        sem_post(&mutex);
        // if request failed, keep trying until it succeeds
        while (!success) {
            printf("Request failed. \n");
            sleep(1); // sleep for 1 second before trying again
            sem_wait(&mutex);
            printRequest(request, id);
            success = processRequest(request, id);
            sem_post(&mutex);
        }
        printf("Request successful! \n");
        sleep(3);
    }
    freeResources(id);
    return NULL;
}

int main() {

    // Grab user inputs for number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &num_process);

    // Grab user inputs for number of resources
    printf("Enter the number of resources: ");
    scanf("%d", &num_resource);

    // Allocate the right number of memory
    // Processes = Rows (i), Resources = Columns (j)
    avail = malloc(num_resource * sizeof(int));
    max = malloc(num_process * num_resource * sizeof(int));
    hold = malloc(num_process * num_resource * sizeof(int));
    need = malloc(num_process * num_resource * sizeof(int));

    // Grab user inputs for maximum number of each resource type
    for (int i=0; i<num_resource; i++) {
        printf("Enter the maximum number of resource type %d: ", i);
        scanf("%d", &avail[i]);
    }

    // Grab user inputs for maximum number of each resource that can be claimed by each process
    for (int i=0; i<num_process; i++) {
        for (int j=0; j<num_resource; j++) {
            int n;
            printf("Enter the maximum number of resource %d that can be claimed by process %d: ", j, i);
            scanf("%d", &n);
            bool valid_num = false;
            while (!valid_num) {
                if (n > avail[j]) {
                    printf("This exceeds the maximum available number of resource %d. Please enter a new number: ", j);
                    scanf("%d", &n);
                } else {
                    valid_num = true;
                }
            }
            max[i*num_resource + j] = n;
            need[i*num_resource + j] = n;
        }
    }

    // create semphore
    sem_init(&mutex, 0, 1);

    // instantiate process threads
    pthread_t process_thread;
    for (int i=0; i<num_process; i++)
    {
        printf("Creating thread for process %d ... \n", i);
        pthread_create(&process_thread, NULL, processSimulation, (void*)(intptr_t)i);
    }

    pthread_exit(NULL);

}
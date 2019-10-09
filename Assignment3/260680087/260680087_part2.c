#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 101

//qsort(request, numRequest, sizeof(int), cmpfunc);
//realloc();

//compare function for qsort
//you might have to sort the request array
//use the qsort function 
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
//qsort in decreasing order
int icmpfunc (const void * a, const void * b) {
    return ( *(int*)b - *(int*)a );
}

//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b)
    {
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++)
    {
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    //write your logic here
    int i, j;
    for(j = 0; j < numRequest; j++){
        int diff = HIGH - LOW + 1;
        for(i = j; i < numRequest; i++){
            //for the first element, comparing with START
            if(j == 0){
                if(abs(request[i] - START) < diff){
                    diff = abs(request[i] - START);
                    swap(&request[i], &request[0]);
                }
            }
            //for the rest element, comparing with the first element in the current sorting array
            else{
                if(abs(request[i] - request[j - 1]) < diff){
                    diff = abs(request[i] - request[j - 1]);
                    swap(&request[i], &request[j]);
                }
            }
        }
    }

    printf("\n----------------\n");
    printf("SSTF :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{
	//write your logic here
    int i, j;
    int counterL = 0;
    int m, n;
    int counterR = 0;
    int switchDir = 1;
    int *result;
    result = malloc((numRequest + 1) * sizeof(int));

    //sort the request array
    qsort(request, numRequest, sizeof(int), cmpfunc);

    //going left, decreasing
    if(START <= (int)(HIGH/2)){
        //for elements less than START
        for(i = numRequest - 1; i >= 0; i--){
            //for the first i
            if(i == numRequest - 1){
                //if the largest request is still less than START
                if(request[i] < START){
                    result[counterL] = request[i];
                    counterL++;
                    switchDir = 0;
                }
                //if the largest request is larger than START, add 1 slot to the request array for request on LOW
                else{
                    request = realloc(request, (numRequest + 1) * sizeof(int));
                }
            }
            //for the rest i
            else{
                if(request[i] < START){
                    result[counterL] = request[i];
                    counterL++;
                }
            }
        }
        if(switchDir != 0){
            result[counterL] = LOW;
            counterL++;
        }
        //for elements larger than START
        for(j = counterL - 1; j < numRequest; j++){
                result[counterL] = request[j];
                counterL++;
        }
    }

    //going right, increasing
    else{
        //for elements larger than START
        for(m = 0; m < numRequest; m++){
            //for the first m
            if(m == 0){
                //if the smallest request is still larger than START
                if(request[m] > START){
                    result[counterR] = request[m];
                    counterR++;
                    switchDir = 0;
                }
                //if the smallest request is less than START, add 1 slot to the request array for request on HIGH
                else{
                    request = realloc(request, (numRequest + 1) * sizeof(int));
                }
            }
            //for the rest m
            else{
                if(request[m] > START){
                    result[counterR] = request[m];
                    counterR++;
                }
            }
        }
        if(switchDir != 0){
            result[counterR] = HIGH;
            counterR++;
        }
        //for elements less than START
        for(n = numRequest - counterR; n >= 0; n--){
            result[counterR] = request[n];
            counterR++;
        }
    }

    if(switchDir != 0){
        numRequest++;
    }

    printf("\n----------------\n");
    printf("SCAN :");
    printSeqNPerformance(result, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
    //write your logic here
    int i, j;
    int counter = 0;
    int switchDir = 1;
    int *result;
    result = malloc((numRequest + 2) * sizeof(int));

    //sort the request array
    qsort(request, numRequest, sizeof(int), cmpfunc);

    //going left, decreasing
    if(START <= (int)(HIGH/2)){
        //if everything is less than START
        if(request[numRequest - 1] < START){
            switchDir = 0;
            for(i = 0; i < numRequest; i++){
                result[i] = request[numRequest - 1 - i];
            }
        }
        //if something is larger than start
        else{
            for(i = 0; i < numRequest; i++){
                if(request[i] < START){
                    if(counter == 0){
                        result[counter] = request[i];
                        counter++;
                    }else{
                        result[counter] = result[counter - i];
                        result[counter - i] = request[i];
                        counter++;
                    }
                }
            }
            result[counter] = LOW;
            counter++;
            result[counter] = HIGH;
            counter++;
            for(j = numRequest - 1; j >= 0; j--){
                if(request[j] > START){
                    result[counter] = request[j];
                    counter++;
                }
            }
        }
    }

    //going right, increasing
    else{
        //if everything is larger than START
        if(request[0] > START){
            switchDir = 0;
            for(i = 0; i < numRequest; i++){
                result[i] = request[i];
            }
        }
        //if something is less than START
        else{
            for(i = 0; i < numRequest; i++){
                if(request[i] > START){
                    result[counter] = request[i];
                    counter++;
                }
            }
            result[counter] = HIGH;
            counter++;
            result[counter] = LOW;
            counter++;
            for(j = 0; j < numRequest; j++){
                if(request[j] < START){
                    result[counter] = request[j];
                    counter++;
                }
            }
        }
    }

    if(switchDir != 0){
        numRequest = numRequest + 2;
    }

    printf("\n----------------\n");
    printf("CSCAN :");
    printSeqNPerformance(result, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
    //write your logic here
    int i, j;
    int counter = 0;
    int *result;
    result = malloc((numRequest) * sizeof(int));

    //sort the request array
    qsort(request, numRequest, sizeof(int), cmpfunc);
    
    //going left, decreasing
    if(START <= (int)(HIGH/2)){
        //for elements less than START
        for(i = numRequest - 1; i >= 0; i--){
            if(request[i] < START){
                result[counter] = request[i];
                counter++;
            }
        }
        //then for elements larger than START
        for(j = 0; j < numRequest; j++){
            if(request[j] > START){
                result[counter] = request[j];
                counter++;
            }
        }
    }

    //going right, increasing
    else{
        //for elements larger than START
        for(i = 0; i < numRequest; i++){
            if(request[i] > START){
                result[counter] = request[i];
                counter++;
            }
        }
        //then for elements less than START
        for(j = numRequest - 1; j >= 0; j--){
            if(request[j] < START){
                result[counter] = request[j];
                counter++;
            }
        }
    }

    printf("\n----------------\n");
    printf("LOOK :");
    printSeqNPerformance(result, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
    //write your logic here
    int i, j;
    int counter = 0;
    int switchDir = 1;
    int *result;
    result = malloc((numRequest + 1) * sizeof(int));

    //sort the request array
    qsort(request, numRequest, sizeof(int), cmpfunc);

    //going left, decreasing
    if(START <= (int)(HIGH/2)){
        //for elements less than START
        for(i = numRequest - 1; i >= 0; i--){
            if(request[i] < START){
                result[counter] = request[i];
                counter++;
            }
        }
        if(request[numRequest - 1] > START){
            result[counter] = HIGH;
            counter++;
            numRequest++;
        }
        //for elements larger than START
        for(j = numRequest - 1; j >= 0; j--){
            if(request[j] > START){
                result[counter] = request[j];
                counter++;
            }
        }
    }

    //going right, increasing
    else{
        //for elements larger than START
        for(i = 0; i < numRequest; i++){
            if(request[i] > START){
                result[counter] = request[i];
                counter++;
            }
        }
        if(request[0] < START){
            result[counter] = LOW;
            counter++;
            numRequest++;
        }
        //for elements less than START
        for(j = 0; j < numRequest; j++){
            if(request[j] < START){
                result[counter] = request[j];
                counter++;
            }
        }
    }

    printf("\n----------------\n");
    printf("CLOOK :");
    printSeqNPerformance(result,numRequest);
    printf("----------------\n");
    return;
}

int main()
{
    int *request, numRequest, i, ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d",&ans);

    switch (ans)
    {
    //access the disk location in FCFS
    case 1: accessFCFS(request, numRequest);
        break;

    //access the disk location in SSTF
    case 2: accessSSTF(request, numRequest);
        break;

    //access the disk location in SCAN
     case 3: accessSCAN(request, numRequest);
        break;

    //access the disk location in CSCAN
    case 4: accessCSCAN(request,numRequest);
        break;

    //access the disk location in LOOK
    case 5: accessLOOK(request,numRequest);
        break;

    //access the disk location in CLOOK
    case 6: accessCLOOK(request,numRequest);
        break;

    default:
        break;

    }
    return 0;
}
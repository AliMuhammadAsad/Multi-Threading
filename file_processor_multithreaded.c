#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<limits.h>

#define ITERS 100
#define DEFAULT_NUM_THREADS 4

typedef long long int ll;

// Global Shared Variables
ll global_sum = 0, global_min = LLONG_MAX, global_max = LLONG_MIN;

// Mutex for synchronization
pthread_mutex_t mutex_sum, mutex_minmax;

// Thread Data Structure
typedef struct{
    ll* data;           // Pointer to the data array
    ll start;           // Start index of the chunk
    ll end;             // End index of the chunk
} threadData;

void* compSum(void* arg){
    threadData* tdata = (threadData*) arg;
    ll* data = tdata->data; // Pointer to the data array
    ll sum = 0;
    for(size_t i = tdata->start; i < tdata->end; i++) sum += data[i];
    // Update global variables
    pthread_mutex_lock(&mutex_sum);
    global_sum += sum;
    pthread_mutex_unlock(&mutex_sum);
    pthread_exit(NULL);
}

void* compMinMax(void* arg){
    threadData* tdata = (threadData*) arg;
    ll* data = tdata->data; // Pointer to the data array
    ll min = LLONG_MAX, max = LLONG_MIN;
    for(size_t i = tdata->start; i < tdata->end; i++){
        if(data[i] > max) max = data[i];
        if(data[i] < min) min = data[i];
    }
    // Update global variables
    pthread_mutex_lock(&mutex_minmax);
    if(min < global_min) global_min = min;
    if(max > global_max) global_max = max;
    pthread_mutex_unlock(&mutex_minmax);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    // Check if the number of arguments is correct
    if(argc < 2 || argc > 3){
        fprintf(stderr, "Usage: %s <input_file> [num_threads] where num_threads is optional, if not provided, the default is 4\n", argv[0]); return 1;
    }
    int num_threads = DEFAULT_NUM_THREADS; // Default number of threads
    if(argc == 3){ // If number of threads is provided, check if it is valid
        num_threads = atoi(argv[2]);
        if(num_threads < 1){
            fprintf(stderr, "Error: Invalid number of threads\n"); return 1;
        }
    }
    clock_t file_start = clock(); // Start timer for reading file
    FILE* datafile = fopen(argv[1], "r");
    if(datafile == NULL){ // Check if file exists
        fprintf(stderr, "Error: Could not open file %s. Please check if the file with the given name exists\n", argv[1]);
        return 1;
    }

    // Allocate memory for data
    ll* data, *temp; ll size = 0, cap = 100;
    data = (ll*)malloc(cap * sizeof(ll));
    if(data == NULL){ // Check if memory allocation was successful
        fprintf(stderr, "Error: Memory allocation failed!\n");
        fclose(datafile); return 1;
    }
    
    // While there is data in the file, add it to the integer array
    while(fscanf(datafile, "%lld", &data[size]) == 1){
        size++; // Increment size
        if(size == cap){ // If size is equal to capacity, double the capacity and dynamically reallocate memory
            cap *= 10;
            temp = (ll*)realloc(data, cap * sizeof(ll));
            if(temp == NULL){ // Check if memory reallocation was successful
                fprintf(stderr, "Error: Memory allocation failed!\n");
                free(data); fclose(datafile); return 1;
            } data = temp; // If successful, assign the new memory location to data
        }
    } fclose(datafile);
    // End timer for reading file and print the time taken
    clock_t file_end = clock();
    double file_read = (double)(file_end - file_start) / CLOCKS_PER_SEC;
    printf("Time taken to read file: %lf\n", file_read);

    // Initialize mutexes
    pthread_mutex_init(&mutex_sum, NULL); pthread_mutex_init(&mutex_minmax, NULL);
    double sum_time = 0, minmax_time = 0; // Time taken to calculate sum, min and max
    // Run the function ITERS times and calculate the average time taken for sum, and, minimum and maximum
    pthread_t threads[num_threads]; // Array of threads
    threadData thread_data[num_threads]; // Array of thread data
    // pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    // threadData* thread_data = (threadData*)malloc(num_threads * sizeof(threadData));

    ll chunk_size = size / num_threads; // Size of each chunk
    for(int i = 0; i < num_threads; i++){
        thread_data[i].data = data;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
    }
    struct timespec sum_start, sum_end, minmax_start, minmax_end;
    clock_gettime(CLOCK_MONOTONIC, &sum_start);
    for(int j = 0; j < ITERS; j++){
        global_sum = 0; 
        // Create and lau   nch threads
        for(int i = 0; i < num_threads; i++) pthread_create(&threads[i], NULL, compSum, &thread_data[i]);

        // Wait for all threads to finish
        for(int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &sum_end);
    sum_time = (double)(sum_end.tv_sec - sum_start.tv_sec) + (double)(sum_end.tv_nsec - sum_start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &minmax_start);
    for(int j = 0; j < ITERS; j++){
        global_min = LLONG_MAX; global_max = LLONG_MIN;
        // Create and launch threads
        for(int i = 0; i < num_threads; i++) pthread_create(&threads[i], NULL, compMinMax, &thread_data[i]);
        
        // Wait for all threads to finish
        for(int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &minmax_end);
    minmax_time = (double)(minmax_end.tv_sec - minmax_start.tv_sec) + (double)(minmax_end.tv_nsec - minmax_start.tv_nsec) / 1e9;

    // Print the sum, min and max values, and the average time taken by each thread and the average elapsed time
    printf("Sum: %lld \t Min: %lld \t Max: %lld\n", global_sum, global_min, global_max);
    printf("Average time taken to calculate sum: %lf\n", sum_time / ITERS);
    printf("Average time taken to calculate min and max: %lf\n", minmax_time / ITERS);

    // Destroy the mutex and free the memory allocated to data
    pthread_mutex_destroy(&mutex_sum); pthread_mutex_destroy(&mutex_minmax);
    free(data);
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>

#define ITERS 100
typedef long long int ll; 

int main(int argc, char *argv[]){
    if (argc != 2){ // Check if the number of arguments is correct
        printf("Usage: %s <input_file>\n", argv[0]); return 1;
    }
    clock_t start = clock(); // Start timer for reading file
    FILE *datafile = fopen(argv[1], "r"); // Open file in read mode
    if(datafile == NULL){ // Check if file exists
        fprintf(stderr, "Error: Could not open file %s. Please check if the file with the given name exists\n", argv[1]); return 1;
    }

    // Allocate memory for data
    ll *data, *temp; ll size = 0, cap = 1000;
    data = (ll *)malloc(cap * sizeof(ll));
    if(data == NULL){ // Check if memory allocation was successful
        fprintf(stderr, "Error: Memory allocation failed!\n");
        fclose(datafile); return 1;
    }

    // While there is data in the file, add it to the integer array
    while(fscanf(datafile, "%lld", &data[size]) == 1){
        size++; // Increment size
        if(size == cap){ // If size is equal to capacity, double the capacity and dynamically reallocate memory
            cap *= 10;
            temp = (ll *)realloc(data, cap * sizeof(ll));
            if(temp == NULL){ // Check if memory reallocation was successful
                fprintf(stderr, "Error: Memory allocation failed!\n");
                free(data); fclose(datafile); return 1;
            } data = temp; // If successful, assign the new memory location to data
        }
    } fclose(datafile); 
    // End timer for reading file and print the time taken
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to read file: %lf\n", elapsed_time);

    // Calculating sum, min and max values of the data set
    double sum_time = 0, minmax_time = 0; ll sum, min, max;
    // Run the function ITERS times and calculate the average time taken for sum, and, minimum and maximum
    for(size_t j = 0; j < ITERS; j++){
        clock_t start_sum = clock(); 
        sum = 0;
        for(int i = 0; i < size; i++) sum += data[i];
        clock_t end_sum = clock();
        sum_time += (double)(end_sum - start_sum) / CLOCKS_PER_SEC;
    }
    for(int j = 0; j < ITERS; j++){
        clock_t start_minmax = clock();
        min = LLONG_MAX; max = LLONG_MIN;
        for(int i = 0; i < size; i++){
            if(data[i] > max) max = data[i];
            if(data[i] < min) min = data[i];
        }
        clock_t end_minmax = clock();
        minmax_time += (double)(end_minmax - start_minmax) / CLOCKS_PER_SEC;
    }

    printf("Sum: %lld \t Min: %lld \t Max: %lld\n", sum, min, max);
    printf("Average time taken to calculate sum: %lf\n", sum_time / ITERS);
    printf("Average time taken to calculate min and max: %lf\n", minmax_time / ITERS);
    
    // Free the memory allocated to data
    free(data); 
    return 0;
}
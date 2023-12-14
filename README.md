# Multi-Threading

The program was made on receiving the homework assignment for the course ```CS232 Operating Systems```. The assignment was to use the knowledge of multi-threading using POSIX Threads API to use and implement a multi-threaded file processor. The goal was to develop a multi-threaded C program that processes a large dataset concurrently. Therefore, synchronization mechanisms would be needed to ensure thread safety and efficient resource utilization.

### Running the Program

The accompanying ```Makefile``` has commands for compiling, and running both the single threaded, and multithreaded program. 

To compile: ```make build```
To run: ```make run <file> <Arguments>```

###### Note*

The [data](data) folder contains the datasets, however, the large dataset couldn't be uploaded to Github due to its large size. The rest of the details can be seen in the accompanying report [here](Report.pdf)